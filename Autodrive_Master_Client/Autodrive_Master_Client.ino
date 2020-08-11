//*************************************************
//*************************************************
//***************** AUTODRIVE *********************
//**************** MASTER_DEMO ********************
//*************************************************
//*************************************************

// include libraries
#include "SoftwareSerial.h"
#include "FloatMapping.h"

SoftwareSerial soft(2, 3); // RX, TX

float ANGLE;
float ACCEL;           
int BRAKE = 0;         // variable to store the read value (HIGH or LOW)

int inputBrake = 7;    // pushbutton connected to digital pin 7 (with pulldown resistor)
int analogPin = A2;    // potentiometer connected to simulate the angle output to the Slave

char str_ANGLE[7];
char str_ACCEL[7];

char ANGLE_Serial[12];
char ACCEL_Serial[12];
char BRAKE_Serial[12];


void setup() {
  Serial.begin(9600);  // Setup serial monitor
  soft.begin(115200);
  Serial.println("**Master Demo**");
  
  pinMode(inputBrake, INPUT);    // sets the digital pin 7 as input (with pulldown resistor)
  pinMode(analogPin, INPUT);     // potentiometer connected to simulate the angle output to the Slave

}

void loop() {

  ANGLE = mapfloat(analogRead(analogPin), 0, 1023, -33, 33); //Mapping the pot value to maximum angle degree (-33 , +33) | ONLY FOR TESTING
  BRAKE = digitalRead(inputBrake);   // Read the button state to simulate ON/OFF of the break | ONLY FOR TESTING
  delay(2);

  //***************** This sequence prepares the data to be sent as a float ***************
  
  dtostrf(ANGLE, 1, 2, str_ANGLE);
  dtostrf(ACCEL, 1, 2, str_ACCEL);

  sprintf(ANGLE_Serial, "ANGLE %s;", str_ANGLE);
  sprintf(ACCEL_Serial, "ANGLE %s;", str_ACCEL);
  sprintf(BRAKE_Serial, "BRAKE %hd;", BRAKE);
  
  //***************************************************************************************
  
  Serial.println("Sending Data to Slave");
  
  // Write data to the Slave
  soft.write(BRAKE_Serial);  //Output will look like: BRAKE 1; or Brake 0;
  soft.write(ANGLE_Serial);  //Output will look like: ANGLE 25; (Value can varry between -33 and +33)
  soft.write(ANGLE_Serial);  //Output will look like: ACCEL 0; (Value can varry between 0 and 1023)
  
  Serial.println(BRAKE_Serial);
  Serial.println(ACCEL_Serial);
  Serial.println(ANGLE_Serial);

delay(20);
}
