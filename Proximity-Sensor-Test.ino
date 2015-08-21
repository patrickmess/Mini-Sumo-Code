//Proximity Sensor to Test
//by Pin Number
//Input either 4, 5, 6, or 7
const int SensorNum = 6;


void setup() {
    pinMode(13, OUTPUT); //Yellow LED
    pinMode(SensorNum, INPUT);
}

void loop() {
  if (digitalRead(SensorNum) == LOW)  //Object Detected
    digitalWrite(13, HIGH);
  else                                //No Object Detected
    digitalWrite(13, LOW); 
}
