//Sumo-Program-1
//This is the main code for the custom mini sumo robot
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
State state = Scanning;   //Set Starting State to Scanning

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
  randomSeed(analogRead(sumo.unconnectedPin));
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






//Main Sumo Code which repeats forever
void loop() {
  //No matter what state the robot is in, read the sensors
  sumo.LightSensorRead();   //Read light Sensors 
  sumo.ProxSensorRead();   //Read Proximity Sensors

  //Scanning State
  if(state == Scanning)   //The robot is in a scanning state and looking for the opponet
  {
      if(justChangedState == true)    //If we just entered this state, calculate the max time to spend in this state
      {
            maxTime = random(1500, 4000); //Limit the amount of time to spend in the scanning state in this position
                                              //to a random interval between 1.5 seconds and 4 seconds
            justChangedState = false;    //Keeps max time from being recalculated
      }
      
      uint16_t time = timeInThisState();    //Update how long the robot has been scanning
    
      //Depending on what direction is selected, spin in order to search for the opponet
      if(direction == Left)   //The direction selected is Left        
          sumo.Drive(-100, 100);  //Spin Left
      else                    //The direction selected is Right
          sumo.Drive(100, -100);  //Spin Right

      //If the forward or rear proximity Sensors detect the opponet, 
      //change to a charging state- driving strati forward at them
      if(sumo.proxSensors[0] == true)   //The front proximity Sensor is detecting a robot
          changeState(ChargingF);       //Charge forward
      else if(sumo.proxSensors[2] == true)  //The rear proximity Sensor is detecting a robot
          changeState(ChargingB);      //Charge backward

      //Run the function that checks to see if the Sumo Ring border was detected 
      //The function also automatically gets the robot away from the border    
      borderDetected();
   
      //If we have spent more than the calculated max time scanning
      //Then randomy chose to drive forwards or backwards to a new scanning position
      if(time <= maxTime)
      {
          int driveDirection = random(1);   //Randomly choose 0 or 1
          if(driveDirection == 0)     //if 0 is chosen
            changeState(DrivingF);      //Drive Forwards to new scanning position
          else                      //if 1 is chosen
            changeState(DrivingB);      //Drive Backwards to new scanning position
      }
  }
  //End of Scanning State Code

  //Driving Forward State
  if(state == DrivingF)   //The robot is driving forward to a new scanning position 
                          //from a border or from a previous scanning position
  {
      if(justChangedState == true)    //If we just entered this state, calculate the max time to spend in this state
      {
            maxTime = random(500, 2000); //Limit the amount of time to spend driving
                                              //to a random interval between 0.5 seconds and 2 seconds
            justChangedState = false;    //Keeps max time from being recalculated
      }

      uint16_t time = timeInThisState();    //Update how long the robot has been driving


      sumo.Drive(75, 75);   //Drive Forward


      //If the forward or rear proximity Sensors detect the opponet, 
      //change to a charging state- driving strait at them (forward or backwards)
      if(sumo.proxSensors[0] == true)   //The front proximity Sensor is detecting a robot
          changeState(ChargingF);       //Charge forward
      else if(sumo.proxSensors[2] == true)  //The rear proximity Sensor is detecting a robot
          changeState(ChargingB);      //Charge backward
      //If one of the side proximity Sensors detect the opponet,
      //change to a scanning state with the turn direction set accordingly
      else if(sumo.proxSensors[1])  //The right proximity Sensor is detecting a robot
      {
           direction = Left;        //Set the turn direction for when the robot starts scanning to left
            /*Reason for setting the scann direction left when the right sensor detects the other robot is that
            * it is unlikely that the opposing robot and our robot were moving in the same direction, because our
            * previous scan would have detected them, so it is more likely that thee robots are traveling on roughly
            * parrallel paths in opposite directions. Also note that the scann code keeps scanning until either
            * the front or rear proximity sensors detect an object. Keeping those two things in mind, if the other 
            * robot is passing us in the opposite direction of our travell on our right side, if we turn right, then we need
            * to wait for our robot to turn 90 degrees to see where the robot was detected, then contnue turning to track
            * down where the other robot is because the other robot is no longer in the same position; it is still moving.
            * We are playing catch up, but we do not want to chase, we want to engage as soon ass possible. The best way to
            * do this is to set our robot up where the scan is likely to intercept the other robots path rather than try to
            * catch up to them. If we turn left for our scan when we detect the other robot, then it is likely that the rear  
            * proximity sensor will intercept the other robot. This idea will also be applied to when a robot is detected
            * with the left sensor. In this situation, we will set the scann direction to Right. 
            * The alternative situation is that a robot is aproaching our side on a roughly perpedicular path. If this is the
            * case, then it is irrelevent which direction we turn. Turning left 90 degrees will take just as long as turning
            * right 90 degrees.
            */
            changeState(Scanning);    //Change to scanning state so we can face the 
                                      //opponet with the blade of the robot and engage.
      }
      else if(sumo.proxSensors[3])  //The left proximity Sensor is detecting a robot
      {
           direction = Right;     //Set the turn direction for when the robot starts scanning to right
           changeState(Scanning);    //Change to scanning state so we can face the 
                                      //opponet with the blade of the robot and engage.
      }
      //Run the function that checks to see if the Sumo Ring border was detected 
      //The function also automatically gets the robot away from the border    
      borderDetected();
   
      //If we have spent more than the calculated max time driving forward
      //Then randomy chose to which direction to scann
      if(time <= maxTime)
      {
          int driveDirection = random(1);   //Randomly choose 0 or 1
          if(direction == 0)    //if 0 is chosen
            direction = Left;     //turn left when scanning begins
          else                  //if 1 is chosen
            direction = Right;    //turn right when scanning begins
          //We have a direction chosen
          changeState(Scanning);   //Begin Scanning to find opponet 
      }
  
  }
  //End DrivingF State (Driving Forward State) Code

  //Driving Backwards State
  if(state == DrivingB)   //The robot is driving backward to a new scanning position 
                          //from a border or from a previous scanning position
  {
      if(justChangedState == true)    //If we just entered this state, calculate the max time to spend in this state
      {
            maxTime = random(500, 2000); //Limit the amount of time to spend driving
                                              //to a random interval between 0.5 seconds and 2 seconds
            justChangedState = false;    //Keeps max time from being recalculated
      }

      uint16_t time = timeInThisState();    //Update how long the robot has been driving


      sumo.Drive(-75, -75);   //Drive Backward


      //If the forward or rear proximity Sensors detect the opponet, 
      //change to a charging state- driving strait at them (forward or backwards)
      if(sumo.proxSensors[0] == true)   //The front proximity Sensor is detecting a robot
          changeState(ChargingF);       //Charge forward
      else if(sumo.proxSensors[2] == true)  //The rear proximity Sensor is detecting a robot
          changeState(ChargingB);      //Charge backward
      //If one of the side proximity Sensors detect the opponet,
      //change to a scanning state with the turn direction set accordingly
      else if(sumo.proxSensors[1])  //The right proximity Sensor is detecting a robot
      {
           direction = Right;        //Set the turn direction for when the robot starts scanning to right
          /*Reason for setting the scann direction being different here from when we are in the driving forward state is 
            * that since we are going the opposite direction, we need to turn the opposite direction to intercept them
            */
            changeState(Scanning);    //Change to scanning state so we can face the 
                                      //opponet with the blade of the robot and engage.
      }
      else if(sumo.proxSensors[3])  //The left proximity Sensor is detecting a robot
      {
           direction = Left;     //Set the turn direction for when the robot starts scanning to left
           changeState(Scanning);    //Change to scanning state so we can face the 
                                      //opponet with the blade of the robot and engage.
      }
      //Run the function that checks to see if the Sumo Ring border was detected 
      //The function also automatically gets the robot away from the border    
      borderDetected();
   
      //If we have spent more than the calculated max time driving backward
      //Then randomy chose to which direction to scann
      if(time <= maxTime)
      {
          int driveDirection = random(1);   //Randomly choose 0 or 1
          if(direction == 0)    //if 0 is chosen
            direction = Left;     //turn left when scanning begins
          else                  //if 1 is chosen
            direction = Right;    //turn right when scanning begins
          //We have a direction chosen
          changeState(Scanning);   //Begin Scanning to find opponet 
      }  
  }
  //End DrivingB State (Driving Backward State) Code

  //Charging forward to push other robot state
  if(state == ChargingF)  //We are trying to push the other robot out of the ring
  {
      sumo.Drive(100, 100);   //Drive Forward Full Speed

        //The rear proximity Sensor is detecting a robot and the front sensor is not
       if(sumo.proxSensors[2] == true && sumo.proxSensors[0] != true)   
          changeState(ChargingB);       //Charge backward
       //The right proximity Sensor is detecting a robot and the front sensor is not   
       else if(sumo.proxSensors[1] == true && sumo.proxSensors[0] != true)
       {
           direction = Left;        //Set the turn direction for when the robot starts scanning to left
           changeState(Scanning);    //Change to scanning state so we can face the 
                                      //opponet with the blade of the robot and engage.
       }
       //The left proximity Sensor is detecting a robot and the front sensor is not 
       else if(sumo.proxSensors[3] == true && sumo.proxSensors[0] != true)
       {
           direction = Right;        //Set the turn direction for when the robot starts scanning to right
           changeState(Scanning);    //Change to scanning state so we can face the 
                                      //opponet with the blade of the robot and engage.
       }
       //The front proximity sensor is not detecting the opponet anymore, but no other sensor is either
       else if(sumo.proxSensors[0] != true)
       {
          //Randomly choose a scan direction and begin scanning
          int driveDirection = random(1);   //Randomly choose 0 or 1
          if(direction == 0)    //if 0 is chosen
            direction = Left;     //turn left when scanning begins
          else                  //if 1 is chosen
            direction = Right;    //turn right when scanning begins
          changeState(Scanning);
       }
      //Run the function that checks to see if the Sumo Ring border was detected 
      //The function also automatically gets the robot away from the border    
      borderDetected();  
  }
  //end ChargingF State(Charging Forward State) Code

  //Charging backwards to push other robot state
  if(state == ChargingB)  //We are trying to push the other robot out of the ring
  {
      sumo.Drive(-100, -100);   //Drive Backwards Full Speed

        //The front proximity Sensor is detecting a robot and the rear sensor is not
       if(sumo.proxSensors[0] == true && sumo.proxSensors[2] != true)   
          changeState(ChargingB);       //Charge backward
       //The right proximity Sensor is detecting a robot and the rear sensor is not   
       else if(sumo.proxSensors[1] == true && sumo.proxSensors[2] != true)
       {
           direction = Right;        //Set the turn direction for when the robot starts scanning to right
           changeState(Scanning);    //Change to scanning state so we can face the 
                                      //opponet with the blade of the robot and engage.
       }
       //The left proximity Sensor is detecting a robot and the rear sensor is not 
       else if(sumo.proxSensors[3] == true && sumo.proxSensors[2] != true)
       {
           direction = Left;        //Set the turn direction for when the robot starts scanning to left
           changeState(Scanning);    //Change to scanning state so we can face the 
                                      //opponet with the blade of the robot and engage.
       }
       //The rear proximity sensor is not detecting the opponet anymore, but no other sensor is either
       else if(sumo.proxSensors[0] != true)
       {
          //Randomly choose a scan direction and begin scanning
          int driveDirection = random(1);   //Randomly choose 0 or 1
          if(direction == 0)    //if 0 is chosen
            direction = Left;     //turn left when scanning begins
          else                  //if 1 is chosen
            direction = Right;    //turn right when scanning begins
          changeState(Scanning);
       }
      //Run the function that checks to see if the Sumo Ring border was detected 
      //The function also automatically gets the robot away from the border    
      borderDetected();  
  }
  //end ChargingB State(Charging Backward State) Code
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
