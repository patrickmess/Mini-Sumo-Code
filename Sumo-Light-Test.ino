//Sumo-Light-Test
//This program is to test the ability of the sumo robot to stay inside the sumo ring on its own
//It does not use the proximity or inertial sensors
//Created in the Arduio IDE

#include <QTRSensors.h>   //Pololu's Reflectance Senosor Library
#include <Sumo.h>         //Library for running the Custom Sumo Robot    

Sumo sumo;    //Create a Sumo robot object called sumo
//bool lightSensors[4];   //create array indicating reflectance sensor readings
//bool proxSensors[4];    //create array indicating which proximity sensors are detecting objects


//The different States that the robot can be in
enum State
{
  Scanning,   //Searching for the other robot using the prximity sensors
  DrivingF,   //Driving forward to a scanning position
  DrivingB,   //Driving Backward to a scanning position
  ChargingF,  //Charging forward at the opposing robot to push it out of the ring 
  ChargingB,  //Charging backwards at the opposing robot to push it out of the ring
};
State state = DrivingF;   //Set Starting State to Scanning

//The different directions that the robot can begin turning at the beginning of the Scanning State
enum Direction
{
  Left,
  Right,
};
Direction direction = Left;  //Set Left as Starting Direction

// The time, in milliseconds, that we entered the current top-level state.
uint16_t stateStartTime;

// The time, in milliseconds, to spend at max in a certain state
int maxTime;

// This gets set to true whenever we change to a new state.
// A state can read and write this variable this in order to
// perform actions just once at the beginning of the state.
bool justChangedState;


//setup code that runs when the robot is turned on
void setup() {  
     bool run = false;      //create a boolean that tells us if the button has been pressed to run the sumo program
                            //Set it to false because button has not been pressed                    

  //wait for button1 to be pressed
  //Once button1 is pressed, the robot will wait 5 seconds, and the main code will run
  while(run == false)
  {
      run = sumo.readButton1();   //check to see if button 1 was pressed    
      
      //if in the meantime button2 is pressed
      //calibrate the light sensors
      if(sumo.readButton2() == true)  //Button 2 was pressed and releaased
      {
          sumo.CalibrateLight();  //Run the Reflectance Sensor Calibration routine
      }

      if(run == true)   //If the button was pressed
      {
          delay(5000);   //wait 5 seconds before running main code
      }
  }
  //We waited until button 1 was pressed and then waited the mandatory 5 seconds
  //End Setup
  //Run Main Code

}
//End of Setup Code


int counter = 0;



//Main Sumo Code which repeats forever
void loop() {
  //No matter what state the robot is in, read the sensors
  sumo.LightSensorRead();   //Read light Sensors 
    //Driving Forward State
  if(state == DrivingF)   //The robot is driving forward to a new scanning position 
                          //from a border or from a previous scanning position
  {

      sumo.Drive(85, 85);   //Drive Forward

      //Run the function that checks to see if the Sumo Ring border was detected 
      //The function also automatically gets the robot away from the border    
      borderDetected();
  
  }
  //End DrivingF State (Driving Forward State) Code

  //Driving Backwards State
  if(state == DrivingB)   //The robot is driving backward to a new scanning position 
                          //from a border or from a previous scanning position
     {
      sumo.Drive(-85, -85);   //Drive Backward

      //Run the function that checks to see if the Sumo Ring border was detected 
      //The function also automatically gets the robot away from the border    
      borderDetected();
  }
  //End DrivingB State (Driving Backward State) Code      
}
//End of the Main Sumo Code





//Function that checks to see if the Sumo Ring border was detected 
//The function also automatically gets the robot away from the border
//by changing the robot state to either DrivingF (Driving Forward)or DrivingB (Driving Backward)   
void borderDetected()
{
  if(sumo.lightSensors[0] == true)    //The front-right light Sensor detects the border 
  {
      //Forward-Pivot on the right side (Just the left wheels are moving forwards) 
      //until the front-left sensor detects the border
      //Then drive away from the border
      while(sumo.lightSensors[3] == false)  //Check to see if the front-left sensor detects the border
      {
          sumo.LightSensorRead();   //Read the light Sensors
          sumo.Drive(80, 0);        //Forward-Pivot on the right side 
                                    //(Just the left wheels are moving forwards)    
      }
      sumo.Drive(-85, -85);   //Drive Backward
      delay(250);
      changeState(DrivingB);   //Drive forward away from the border
  }
  
  else if(sumo.lightSensors[1] == true)   //The back-right light Sensor detects the border
  {
      //Backward-Pivot on the right side (Just the left wheels are moving backwards) 
      //until the back-left sensor detects the border
      //Then drive away from the border
      while(sumo.lightSensors[2] == false)  //Check to see if the back-left sensor detects the border
      {
          sumo.LightSensorRead();   //Read the light Sensors
          sumo.Drive(-80, 0);       //Backward-Pivot on the right side
                                    //(Just the left wheels are moving backwards)
      }
      sumo.Drive(85, 85);   //Drive Backward
      delay(250);
      changeState(DrivingF);   //Drive backward away from the border
  }
  else if(sumo.lightSensors[2] == true)   //The back-left light Sensor detects the border
  {
      //Backward-Pivot on the left side (Just the right wheels are moving backwards) 
      //until the back-right sensor detects the border
      //Then drive away from the border
      while(sumo.lightSensors[1] == false)    //Check to see if the back-right sensor detects the border
      {
          sumo.LightSensorRead();   //Read the light Sensors
          sumo.Drive(0, -80);       //Backward-Pivot on the left side
                                    //(Just the right wheels are moving backwards)
      }
      sumo.Drive(85, 85);   //Drive Backward
      delay(250);
      changeState(DrivingF);   //Drive backward away from the border
  }
  else if(sumo.lightSensors[3] == true)   //The front-left light Sensor detects the border 
  {
      //Forward-Pivot on the left side (Just the right wheels are moving forwards) 
      //until the front-right sensor detects the border
      //Then drive away from the border
      while(sumo.lightSensors[0] == false)    //Check to see if the front-right sensor detects the border
      {
          sumo.LightSensorRead();     //Read the light Sensors
          sumo.Drive(0, 80);          //Forward-Pivot on the left side 
                                      //(Just the right wheels are moving forwards)  
      }
      sumo.Drive(-85, -85);   //Drive Backward
      delay(250);
      changeState(DrivingB);     //Drive backward away from the border
  }
}
//End of the borderDetected() function code

// Changes to a new state.  It also clears the LCD and turns off
// the LEDs so that the things the previous state were doing do
// not affect the feedback the user sees in the new state.
void changeState(uint8_t newState)
{
  state = (State)newState;
  justChangedState = true;
  stateStartTime = millis();
}
 
// Gets the amount of time we have been in this state, in
// milliseconds.  After 65535 milliseconds (65 seconds), this
// overflows to 0.
uint16_t timeInThisState()
{
  return (uint16_t)(millis() - stateStartTime);
}
