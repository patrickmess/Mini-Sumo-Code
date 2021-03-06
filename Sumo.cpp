//The source file for the Sumo library
//Created in Notepad++

#include <Arduino.h>
#include <QTRSensors.h>
#include <Sumo.h>
//#include <Wire.h>
//#include <LSM303.h>

#define NUM_SENSORS   4     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   QTR_NO_EMITTER_PIN     // emitter is controlled by digital pin 2


QTRSensorsRC qtrrc((unsigned char[]) {0,1,20,21}, NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

//Initialization Function
//Sets pin modes
Sumo::Sumo()
{
	
	//Set LED as an output pin
	pinMode(led, OUTPUT);

	//Set button pins as inputs
	pinMode(button1, INPUT);
	pinMode(button2, INPUT);
	
	//Set Proximity Sensors as inputs
	pinMode(p1, INPUT);			
	pinMode(p2, INPUT);
	pinMode(p3, INPUT);
	pinMode(p4, INPUT);
	
	//Set Motor direction and power pins as outputs
	pinMode(rmd, OUTPUT);
	pinMode(rmp, OUTPUT);
	pinMode(lmd, OUTPUT);
	pinMode(lmp, OUTPUT);
	
	//Setup Accelerometer
	Wire.begin();
}


//Function for reading the reflectance sensors
//Edits lightSensors (the array of booleans indicating which sensors read BLACK and which read WHITE)
void Sumo::LightSensorRead()
{
	//"lightSensors" is an Array of Booleans that indicates which Light Sensors detected the color white
	//False is for Black detected (High Value)
	//True is for White detected (Low Value)
	
	//set default value to false for all values in array
	for(int i = 0; i <= 4; i++)
	{
			lightSensors[i] = false;				//Set value to false
	}
														
	
	const int BLACK = 500;			//The threshold for the difference between white and black
	
	qtrrc.read(sensorValues);		//Reads each sensor and stores the reading in a public array
									//called sensorValues
									//Each value is between 0 and 1000
									//1000 is least reflective (BLACK)
									//0 is most reflective (WHITE)
	
	//Read each value in sensorvValue array and determine whether or not an white 
	//has been detected. 
	//Then change the corresponding array position to indicate that
	for(int i = 0; i <= 4; i++)
	{
		if(sensorValues[i] < BLACK)		//White was detected by the i Sensor
			lightSensors[i] = true;				//The corresponding array position changed to true
		//Otherwise nothing happens
		//The corresponding array position returns false
	}
}


//Function for reading the proximity sensors
//Edits proxSensors (the array of booleans indicating which sensors detect an object and which do not)
void Sumo::ProxSensorRead()
{	
	//"proxSensors" is an Array of Booleans that indicates which Proximity Sensors detected an object
	//False is for no object detected
	//True is for object detected
	
	const int diff = p1;	//"diff" for difference	
							//Variable that stores the difference between 0 and the pin number
							//of the first proximity sensor
							//Used to relate the sensor pin number to the corresponding array position
	
	//set default value to true for all values in array
	for(int i = 0; i < 4; i++)
	{
		proxSensors[i] = true;		//Set the corresponding array position to true
											//If all the readings of the sensor are LOW
											//Then we know that we are detecting an object
											//If any are HIGH, then we are getting an intermittent
											//which means there is no object in range of the sensor
											//so we will change the array position to false
		//Serial.print(i);				//For Debugging
		//Serial.println(" = True");	//For Debugging
	}
	
	//Read each Sensor Value 10 times and determine whether or not an object has been detected
	//Then change the corresponding array position to indicate that
	for(int i = p1; i <= p4; i++)
	{
		 //Serial.println();				//For Debugging
		 //Serial.println(i);				//For Debugging
		 //Serial.println(i - diff);		//For Debugging
		//Read the sensor 20 times
		for(int ii = 0; ii < 30; ii++)
		{
			//Serial.print("-");		//For Debugging
			//Serial.print(ii);			//For Debugging
			if (digitalRead(i) == HIGH)		//Sensor reads that there is not an object		
			{			
				proxSensors[i - diff] = false;		//The corresponding array position changed to false
				//Serial.println("false");	//For Debugging
			}
			else
			{
				//Do Nothing
				//Serial.println("true");		//For Debugging
			}
				
			delay(1);	//wait before reading the sensor again
		}
		//If all the readings were LOW, indicating that an object was detected
		//then the array position will remain with a true value
		//Otherwise, the array will array position will read false
	}
}

//Function that returns true if button1 is pressed 
bool Sumo::readButton1()
{
	int buttonState = 0;         // variable for reading the pushbutton status
	
	buttonState = digitalRead(button1);		//Read the button state
	if (buttonState == HIGH)				//If the button is pressed
	{
		/*while(buttonState == LOW)				//wait until the button is released
		{
			//check if button is released
			buttonState = digitalRead(button1);					
		}	*/
		return true;	//return true: indicating that the button has been pressed and released
	}
	
	else	//If the button is not pressed
	{
		//do nothing
		return false; 	//return false: indicating that the button has not been pressed
	}
}

//Function that returns true if button2 is pressed 
bool Sumo::readButton2()
{
	int buttonState = 0;         // variable for reading the pushbutton status
	
	buttonState = digitalRead(button2);		//Read the button state
	if (buttonState == HIGH)				//If the button is pressed
	{
		/*while(buttonState == HIGH)				//wait until the button is released
		{
			//check if button is released
			buttonState = digitalRead(button2);					
		}
		delay(500);		//wait 1/2 second		*/
		return true;	//return true: indicating that the button has been pressed
	}
	
	else	//If the button is not pressed
	{
		//do nothing
		return false; 	//return false: indicating that the button has not been pressed
	}
}

//Function that runs the drive motors
//Left and Right should be inputs on the range of -100 (full reverse) to 100 (full forward)
//0 is for no motor movement
void Sumo::Drive(int left, int right)
{
	const double pwmMAX = 255; 	//The pwm pin sends a value on the range of 0 to 255
	const double factor = pwmMAX / 100; 	//The factor that the input will need to be multiplied by
										//in order to convert a base 100 value to a base 255 value
										//equal to 2.55 when pwm range is 0 to 255
	
	left = -1 * left;	//Reverse one side so that motors drive in the same direction
	
	//Set Left Motor direction and speed	
	if(left > 0)		//If the left input value is positive
	{
		digitalWrite(lmd, HIGH);		//set left direction to forwards	
		double powerSignal = left * factor;	//Calculate value to send across pwm to
											 // motor driver for power													 
		analogWrite(lmp, powerSignal);	//Set motor speed to calculated value		
	}
	else				//If the left input value is negative 
	{
		digitalWrite(lmd, LOW);		//set left direction to backwards
		double powerSignal = -1 * left * factor;	//Calculate value to send across pwm to
											 // motor driver for power
												/*////////////IMPORTANT/////////////////
												left is multiplied by -1 in this equation 
												to ensure that the result is positive
												//////////////////////////////////////*/					
		analogWrite(lmp, powerSignal);	//Set motor speed to calculated value
	}
	
	//Set Right Motor direction and speed
	if(right > 0)		//If the right input value is positive
	{
		digitalWrite(rmd, HIGH);		//set right direction to forwards
		double powerSignal = right * factor;	//Calculate value to send across pwm to
											 // motor driver for power											 
		analogWrite(rmp, powerSignal);	//Set motor speed to calculated value		
	}
	else				//If the right input value is negative 
	{
		digitalWrite(rmd, LOW);		//set right direction to backwards
		double powerSignal = -1 * right * factor;	//Calculate value to send across pwm to
											 // motor driver for power
												/*////////////IMPORTANT/////////////////
												right is multiplied by -1 in this equation 
												to ensure that the result is positive
												//////////////////////////////////////*/
		analogWrite(rmp, powerSignal);	//Set motor speed to calculated value
	}
	//void function, nothing to return
}

//Function to Calibrate the reflectance sensors
//Comes from the QTRRC Example code (lines 45-50)
void Sumo::CalibrateLight()
{
	digitalWrite(led, HIGH);    // turn on Yellow LED to indicate we are in calibration mode
	for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
	{
		qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
	}
	digitalWrite(led, LOW);     // turn off Yellow LED to indicate we are done with the calibration
}

//Function to read the Accelerometer
void Sumo::AccelRead()
{
	//Set Accel to 0
	int Accel = 0;		//Set Accel to 0
	int array[100];		//Create an array with 100 integers
	
	//read the accelerometer 100 times, storing values in array
	for(int i = 0; i < 50; i++)
	{
		compass.read();
		array[i] = compass.a.x;
		//delay(1);
	}
	
	//Average those values
	long total = 0;
	int ii = 0;
	while (ii < 100)
	{
		total = total + array[ii];
		ii++;
	}	
	
	Accel = total / ii;		//set Accel to that averaged value
}
