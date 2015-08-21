//Sumo-Motor-Test
//Code to test the fully built sumo robots driving ability

#include <QTRSensors.h>   //Pololu's Reflectance Senosor Library
#include <Sumo.h>         //Library for running the Custom Sumo Robot

Sumo sumo;    //Create a Sumo robot object called sumo

//setup code that runs when the robot is turned on
void setup() {  
    bool run = false;      //create a boolean that tells us if the button has been pressed to run the sumo program
                            //Set it to false because button has not been pressed                    

  //wait for button1 to be pressed
  //Once button1 is pressed, the robot will wait 5 seconds, and the main code will run
  while(run == false)
  {
    run = sumo.readButton1();   //check to see if button 1 was pressed    
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

//Main Motor-Test Code which repeats forever
void loop() {
  sumo.Drive(100, 100);
  //digitalWrite(9, LOW);
  //analogWrite(10, 255);
  delay(500);
  sumo.Drive(0, 0);
  delay(1000);
  digitalWrite(9, LOW);
  analogWrite(10, 255);
  digitalWrite(12, LOW);
  analogWrite(11, 255);
  delay(500);
  sumo.Drive(0, 0);
  delay(1000);
 /* sumo.Drive(-100, -100);
  delay(500);
  sumo.Drive(0, 0);
  delay(1000);
  sumo.Drive(-100, 0);
  delay(500);
  sumo.Drive(0, 0);
  delay(1000);
  sumo.Drive(0, -100);
  delay(500);
  sumo.Drive(0, 0);
  delay(1000);*/
  
}
