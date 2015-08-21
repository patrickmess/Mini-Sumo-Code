#include <QTRSensors.h>

#define NUM_SENSORS   3     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   QTR_NO_EMITTER_PIN     // emitter is controlled by digital pin 2



//Sensor to Test
//by Pin Number
//Input either 0, 1, 20, or 21
const int SensorNum = 21;





// sensors 0 through 3 are connected to digital pins 0 through 3, respectively
QTRSensorsRC qtrrc((unsigned char[]) {SensorNum},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

void setup() {
    delay(500);
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
    for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
    {
     qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
    }
    digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

    delay(1000);
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
    digitalWrite(13, HIGH);
    delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value;
  qtrrc.read(sensorValues);
  value = sensorValues[0];
  if (value < 500)
    digitalWrite(13, HIGH);
  else
    digitalWrite(13, LOW);
}
