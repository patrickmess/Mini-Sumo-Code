//This is a modification of the Pololu Accelerometer Serial Program example
//It reads the raw accelerometer value from the LSM303D accelerometer while
//the robot is moving forward, backward, and staying still
//It then averages a certain number (depending on the motion state)
//of readings and the outputs the average reading through Serial

/* This example reads the raw values from the L3GD20H gyro and
LSM303D accelerometer and magnetometer on the Zumo 32U4, and
prints those raw values to the serial monitor.

The accelerometer readings can be converted to units of g using
the conversion factors specified in the "Sensor characteristics"
table in the LSM303 datasheet.  The default full-scale (FS)
setting is +/- 2 g, so the conversion factor is 0.61 mg/LSB
(least-significant bit).  A raw reading of 16384 would correspond
to 1 g.

The gyro readings can be converted to degrees per second (dps)
using the "Mechanical characteristics" table in the L3GD20H
datasheet.  The default sensitivity is 8.75 mdps/LSB
(least-significant bit).  A raw reading of 10285 would correspond
to 90 dps.

The magnetometer readings are more difficult to interpret and
will usually require calibration. */

#include <Wire.h>
#include <LSM303.h>

#include <QTRSensors.h>   //Pololu's Reflectance Senosor Library
#include <Sumo.h>         //Library for running the Custom Sumo Robot  

Sumo sumo;    //Create a Sumo robot object called sumo

LSM303 compass;

char report[120];

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  compass.init();
  compass.enableDefault();

}

void loop()
{
  //1     Test when robot is driving forward at 50% power
  delay(5000);
  sumo.Drive(50, 50);
  int array[100];
  for(int i = 0; i < 100; i++)
  {
    digitalWrite(13, HIGH);    // turn on Yellow LED to indicate
    compass.read();
/*
    Serial.print(i);
    Serial.print("     ");
  snprintf(report, sizeof(report), "A: %6d %6d %6d    ",
    compass.a.x, compass.a.y, compass.a.z);
    
    Serial.println(report);*/

    delay(10);

    array[i] = compass.a.x;
  }

  Serial.println();
  
  long total = 0;
  int ii = 0;
  while (ii < 100)
  {
    total = total + array[ii];
    ii++;
  }
  int average;
  average = total / ii;
  Serial.print("The average of the accelerometer values going forward at 50% power is: ");
  Serial.print(average);

  Serial.println();
  sumo.Drive(0, 0);

  //2                     Test when robot is driving backward at 50% power
  delay(5000);
  sumo.Drive(-50, -50);
  array[100];
  for(int i = 0; i < 100; i++)
  {
    digitalWrite(13, HIGH);    // turn on Yellow LED to indicate
    compass.read();
/*
    Serial.print(i);
    Serial.print("     ");
  snprintf(report, sizeof(report), "A: %6d %6d %6d    ",
    compass.a.x, compass.a.y, compass.a.z);
    
    Serial.println(report);*/

    delay(10);

    array[i] = compass.a.x;
  }
  
  Serial.println();
  
  total = 0;
  ii = 0;
  while (ii < 100)
  {
    total = total + array[ii];
    ii++;
  }
  average;
  average = total / ii;
  Serial.print("The average of the accelerometer values going backward at 50% power is: ");
  Serial.print(average);

  Serial.println();

  sumo.Drive(0, 0);

  //3               Test when robot is stationary
  delay(5000);
  sumo.Drive(0, 0);
  for(int i = 0; i < 100; i++)
  {
    digitalWrite(13, HIGH);    // turn on Yellow LED to indicate
    compass.read();
/*
    Serial.print(i);
    Serial.print("     ");
  snprintf(report, sizeof(report), "A: %6d %6d %6d    ",
    compass.a.x, compass.a.y, compass.a.z);
    
    Serial.println(report);*/

    delay(10);

    array[i] = compass.a.x;
  }
  
  Serial.println();
  
  total = 0;
  ii = 0;
  while (ii < 100)
  {
    total = total + array[ii];
    ii++;
  }
  average;
  average = total / ii;
  Serial.println(total);
  Serial.print("The average of the accelerometer values staying still is: ");
  Serial.print(average);

  Serial.println();
  Serial.println();
  sumo.Drive(0, 0);

  //4                               Test when robot is driving forward at 100% power
  delay(5000);
  sumo.Drive(100, 100);
  int array2[50];
  for(int i = 0; i < 50; i++)
  {
    digitalWrite(13, HIGH);    // turn on Yellow LED to indicate
    compass.read();
/*
    Serial.print(i);
    Serial.print("     ");
  snprintf(report, sizeof(report), "A: %6d %6d %6d    ",
    compass.a.x, compass.a.y, compass.a.z);
    
    Serial.println(report);*/

    delay(10);

    array2[i] = compass.a.x;
  }
  
  Serial.println();
  
  total = 0;
  ii = 0;
  while (ii < 50)
  {
    total = total + array2[ii];
    ii++;
  }
  average;
  average = total / ii;
  Serial.println(total);
  Serial.print("The average of the accelerometer values going forward at 100% power is: ");
  Serial.print(average);

  Serial.println();
  sumo.Drive(0, 0);

  //5                            Test when robot is driving backward at 100% power
  delay(5000);
  sumo.Drive(-100, -100);
  for(int i = 0; i < 50; i++)
  {
    digitalWrite(13, HIGH);    // turn on Yellow LED to indicate
    compass.read();

    /*
    Serial.print(i);
    Serial.print("     ");
  snprintf(report, sizeof(report), "A: %6d %6d %6d    ",
    compass.a.x, compass.a.y, compass.a.z);
    
    Serial.println(report);*/

    delay(10);

    array2[i] = compass.a.x;
  }
  
  Serial.println();
  
  total = 0;
  ii = 0;
  while (ii < 50)
  {
    total = total + array2[ii];
    ii++;
  }
  average;
  average = total / ii;
  Serial.println(total);
  Serial.print("The average of the accelerometer values going backward at 100% power is: ");
  Serial.print(average);

  Serial.println();

  sumo.Drive(0, 0);

  //6                                   Test when robot is stationary
  delay(5000);
  sumo.Drive(0, 0);
  for(int i = 0; i < 100; i++)
  {
    digitalWrite(13, HIGH);    // turn on Yellow LED to indicate
    compass.read();
/*
    Serial.print(i);
    Serial.print("     ");
  snprintf(report, sizeof(report), "A: %6d %6d %6d    ",
    compass.a.x, compass.a.y, compass.a.z);
    
    Serial.println(report);*/

    delay(10);

    array[i] = compass.a.x;
  }
  
  Serial.println();
  
  total = 0;
  ii = 0;
  while (ii < 100)
  {
    total = total + array[ii];
    ii++;
  }
  average;
  average = total / ii;
  Serial.println(total);
  Serial.print("The average of the accelerometer values staying still is: ");
  Serial.print(average);

  Serial.println();
  Serial.println();
  sumo.Drive(0, 0);

}


