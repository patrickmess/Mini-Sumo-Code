//The header file for the Sumo library
//Created in Notepad++

#ifndef Sumo_h
#define Sumo_h

#include <QTRSensors.h>
#include <Wire.h>
#include <LSM303.h>


class Sumo
{
	public:
		Sumo();
			
		bool readButton1();		//Function that returns true if button1 is pressed 
		bool readButton2();		//Function that returns true if button2 is pressed 
		
		bool lightSensors[4];	//the array of booleans indicating which light sensors 
									//detect BLACK and which detect WHITE
		void LightSensorRead();	//Function for reading the reflectance sensors
									//Edits an array of booleans indicating which 
									//sensors read BLACK and which read WHITE
		
		bool proxSensors[4];	//the an array of booleans indicating which sensors 
									//detect an object and which do not
		void ProxSensorRead();	//Function for reading the proximity sensors
									//Edits an array of booleans indicating which 
									//sensors detect an object and which do not						
		
		LSM303 compass;		//Accelerometer object 
		int Accel;			//The Accelerometer Sensor reading
		void AccelRead();	//The function to read the Accelerometer
		
		void Drive(int left, int right);	//Function that runs the drive motors
												//Left and Right should be inputs on the range 
												//of -100 (full reverse) to 100 (full forward)
												//0 is for no motor movement	
												
		void CalibrateLight();				//Function to Calibrate the reflectance sensors
												//Comes from the QTRRC Example code (lines 45-50)
												

		const int led = 13;		//led pin
		const int unconnectedPin = 23;		//Empty pin for initialize a random number generator
	
	private:
		//Sensors are labelled clockwise with the mini USB-port from the A-Star Mini facing 12:00
	
		const int L1 = 0;	//Light Sensor 1
		const int L2 = 1;	//Light Sensor 2
		const int L3 = 20;	//Light Sensor 3
		const int L4 = 21;	//Light Sensor 4
		
		const int p1 = 4;	//Proximity Sensor 1
		const int p2 = 5;	//Proximity Sensor 2
		const int p3 = 6;	//Proximity Sensor 3
		const int p4 = 7;	//Proximity Sensor 4
		
		const int rmd = 12; 		//Right motor direction
		const int rmp = 11;		//Right motor power
		const int lmp = 10;		//Left motor power
		const int lmd = 9;		//Left motor direction
		
		const int button1 = 18;	//Button to start Main Sumo Program
		const int button2 = 19; //Button to start Sensor Calibration

		
};

#endif
