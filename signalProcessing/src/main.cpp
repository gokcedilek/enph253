#include <Arduino.h>
#include "ultrasonic.h"
#include "SideDoors.h"

#define TRIG PB6
#define ECHO PB7
ultrasonic ultra = ultrasonic(TRIG, ECHO);
//ultrasonic::location loc;
ultrasonic::points point;

#define LEFT_DOOR_SERVO PB13
#define RIGHT_DOOR_SERVO PB12
SideDoors side_doors = SideDoors(LEFT_DOOR_SERVO, RIGHT_DOOR_SERVO);

#define TX3 PB10
#define RX3 PB11
HardwareSerial Serial3 = HardwareSerial(RX3, TX3);

#define THANOS 0
#define METHANOS 1
#define ROLE METHANOS

#define RAMP_TIME 12
#define COLLECT_TIME 24

//enum majorState { upRamp, collectPlushie, depositPlushie, /*stones, shutDown*/ } currentMajorState;
int currentMajorState;

//based on the currentState we receive from the driving MCU, we will close/open the side doors, as well give instructions to the driving MCU when we need to change our path to avoid collision
//switch case for the state we receive from the driving MCU
//check the state sent from the driving MCU with a timer interrupt? or at the beginning of each loop?
//we need to also check what the ultrasonic sensor is "seeing"
//a bit like we will have "sub state machines" in plushie collection and deposit states, to both check the MCU state and check the ultrasonic sensor's output
float initialTime;
float timeElapsed;

void serialComm();
// void ultrasonicStateMachine_new();

void setup() {
  Serial.begin(115200);
  Serial3.begin(115200);
  serialComm();
  //currentMajorState = upRamp;
  //initialTime = millis();
}

void loop() {  
  // ---- hard-coding the time like the slave MCU -----

  // timeElapsed = (millis() - initialTime)/1000; // in seconds
  // if (timeElapsed < RAMP_TIME)
  //   currentMajorState = upRamp;
  // else if (timeElapsed < COLLECT_TIME)
  //   currentMajorState = collectPlushie;
  // else {
  //     currentMajorState = depositPlushie;
  // }


  // ---- receiving the state from the slave MCU ----
  currentMajorState = Serial3.read(); //get the state that the driver MCU is in
  switch (currentMajorState) {
    case 0: //up-ramp
      side_doors.doorsClose();
      break;

    case 1: //collection
      #if (ROLE == THANOS)
        side_doors.doorsOpenT();
      #elif (ROLE == METHANOS)
        side_doors.doorsOpenM();
      #endif
      //after this position, we need the ultrasonic:
      //ultrasonicStateMachine_new();
      break;
    
    case 2: //deposit
      side_doors.doorsTogether();
      break;

    case -1:
      break; //if it reads -1, this means no data was available, so keep reading
  }
}

void serialComm(){
      int check_available = Serial3.available();
      while (!check_available)
            check_available = Serial3.available(); 
      return;      
}

/*
void ultrasonicStateMachine(){
      loc = ultra.loc_of_obj(RANGE);
      switch(loc){
       case ultrasonic::left:
            side_doors.leftDoorWrite(90); //left 90, right same as before
            break;
        case ultrasonic::left_center:
            //need to turn right until we read black tape again
            //Serial3.write(loc);
            side_doors.leftDoorWrite(45); //left 90, right same as before
            break;
        case ultrasonic::center:
            //need to turn left (METHANOS) or right (THANOS) until we read black tape again
            //Serial3.write(loc);
            //?
            break;
        case ultrasonic::center_right:
            //need to turn left until we read black tape again
            //Serial3.write(loc);
            side_doors.rightDoorWrite(45);
            break;
        case ultrasonic::right:
            side_doors.rightDoorWrite(90); //right 90, left same as before
            break;
        case ultrasonic::left_right:
            side_doors.doorsWrite(90);  //left 90, right 90
            break;
        case ultrasonic::all:
            //need to turn left (METHANOS) or right (THANOS) until we read black tape again
            //Serial3.write(loc);
            //?
            break;
        case ultrasonic::none:
            side_doors.doorsWrite(120); //left 120, right 60 (normal plushie collection position!)
            break;        
      }
}
*/

/*void ultrasonicStateMachine_new(){
  point = ultra.checkLocation(RANGE);
  switch(point){
    case ultrasonic::c80:
      side_doors.doorsWrite(24);
      break;
    case ultrasonic::l60:
      side_doors.leftDoorWrite(48); //left is 180-angle
      break;
    case ultrasonic::r60:
      side_doors.rightDoorWrite(48);
      break;
    case ultrasonic::lr60:
      side_doors.doorsWrite(48);
      break;
    case ultrasonic::l40:
      side_doors.leftDoorWrite(72);
      break;
    case ultrasonic::r40:
      side_doors.rightDoorWrite(72);
      break;
    case ultrasonic::lr40:
      side_doors.doorsWrite(72);
      break;
    case ultrasonic::l20: 
      side_doors.leftDoorWrite(24);
      break;
    case ultrasonic::r20:
      side_doors.rightDoorWrite(24);
      break;
    case ultrasonic::lr20:
      side_doors.doorsWrite(24);
      break;
    case ultrasonic::lr0:
      #if (ROLE == THANOS)
        side_doors.doorsOpenT();
      #elif (ROLE == METHANOS)
        side_doors.doorsOpenM();
      #endif
      break;
  }
}*/