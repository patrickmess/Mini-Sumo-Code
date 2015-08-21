//Prox-Sensor-Function-Tes2t-Code
//This code is to used to test the ProxSensorRead
//function in the Sumo.h library

#include <QTRSensors.h>   //Pololu's Reflectance Senosor Library
#include <Sumo.h>         //Library for running the Custom Sumo Robot    

Sumo sumo;    //Create a Sumo robot object called sumo

const int sensor = 3; //Designates which proximity sensor in the proxSensors array we will be reading

void setup() {
  // put your setup code here, to run once:
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

//Main Test Code wich repeats forever 
void loop() {
    sumo.ProxSensorRead();   //Read Proximity Sensors
    //Serial.print("Return for Prox Sensor 1: ");
    //Serial.println(sumo.proxSensors[1]);

    if(sumo.proxSensors[sensor] == true) //If an object is detected
      digitalWrite(13, HIGH);    // turn on Yellow LED to indicate the object is detected
    
    else  //If an object is not detected
       digitalWrite(13, LOW);    // turn off the Yellow LED

       //delay(5000);
}
