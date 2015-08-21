//Left Side Motor Test
// Motor Drivers 3 and 4

void setup() {
  // put your setup code here, to run once:

    pinMode(12, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(12, LOW);
  for(int i = 0; i < 255; i++)
  {
    digitalWrite(13, HIGH);
    analogWrite(11, i);
    delay(10);
  }
  delay(1000);
  for(int i = 255; i > 0; i--)
  {
    digitalWrite(13, LOW);
    analogWrite(11, i);
    delay(10);
  }
  delay(1000);
  digitalWrite(12, HIGH);
  for(int i = 0; i < 255; i++)
  {
    digitalWrite(13, HIGH);
    analogWrite(11, i);
    delay(10);
  }
  delay(1000);
  for(int i = 255; i > 0; i--)
  {
    digitalWrite(13, LOW);
    analogWrite(11, i);
    delay(10);
  }
  delay(1000);
  
}
