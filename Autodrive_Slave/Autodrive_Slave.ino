//*************************************************
//*************************************************
//***************** AUTODRIVE *********************
//***************** SLAVE_SOFT ********************
//*************************************************
//*************************************************


// include libraries
#include <AccelStepper.h>
#include <Adafruit_MCP4725.h>
#include "FloatMapping.h"

Adafruit_MCP4725 dac;

uint16_t Analog_Accel;  // Output to the invertor from the DAC MCP module
#define Digital_Brake 4 // Outputs 5V / 0V

char inChar;
String inString = "";            // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


float ANGLE;
float ACCEL;
int BRAKE;


//***********************************Driver Settings***********************************
AccelStepper stepper(AccelStepper::DRIVER, 6, 5);
//AccelStepper stepper; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

uint16_t rpm;
int Neutral_Pos;
int Maxspeed = 1000;
long Destination;
long Max_Left = 80000;
long Max_Right = -80000;
int Acceleration = 2000;

//********************************Fetch_Data Interval**************************************
unsigned long interval = 50; // the time we need to wait
unsigned long previousMillis = 0; // millis() returns an unsigned long.



void setup() {

  // Start serial communication
  Serial.begin(115200);

  //Initialize I2C communication as Slave with the DAC
  dac.begin(0x62);

  // Configure output pins

  pinMode(Analog_Accel, OUTPUT);
  pinMode(Digital_Brake, OUTPUT);

  digitalWrite(Digital_Brake, LOW);

  stepper.setMaxSpeed(Maxspeed);
  stepper.setAcceleration(Acceleration);
  Neutral_Pos = stepper.currentPosition();


}

//****************************MAIN LOOP****************************
void loop() {

  filterData();
  
  serialReadEvent();

  stepper.moveTo(Destination);
  stepper.run();

}


//****************************FETCH_DATA****************************
void filterData() {

  if (stringComplete) {

    //****************************DIRECTION****************************

    if (inString.substring(0, 5) == "ANGLE") {
      ANGLE = inString.substring(6, 12).toFloat();

      Destination = mapfloat(ANGLE, -33, 33, Max_Left, Max_Right);

      //Destination = map(ANGLE, -33, 33, Max_Left, Max_Right);

      Serial.print("Steering Angle:");
      Serial.println(ANGLE, 2);
      Serial.print("Destination: ");
      Serial.println(Destination);

    }

    //****************************ACCELERATION****************************

    else if (inString.substring(0, 5) == "ACCEL") {
      ACCEL = inString.substring(6, 12).toFloat();
      Serial.print("Steering Angle: ");
      Serial.println(ACCEL);

      Analog_Accel = map(ACCEL, 0, 1023, 0, 4095);

      dac.setVoltage(Analog_Accel, false);

      Serial.print("Analog_Accel: ");
      Serial.println(Analog_Accel);
    }

    //****************************BRAKE****************************

    else if (inString.substring(0, 5) == "BRAKE") {
      BRAKE = inString.substring(6, 7).toInt();

      Serial.print("BRAKE: ");
      Serial.println(BRAKE);

      digitalWrite(Digital_Brake, BRAKE);

    }

    inString = "";
    stringComplete = false;
  }
}


//********************************Void SerialReadEvent*********************************//

void serialReadEvent() {

  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inString += inChar;
    if (inChar == ';') {
      stringComplete = true;
    }
  }
}
