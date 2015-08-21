//Right Side Motor Test
// Motor Drivers 1 and 2

void setup() {
  // put your setup code here, to run once:

    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(9, LOW);
  for(int i = 0; i < 255; i++)
  {
    digitalWrite(13, HIGH);
    analogWrite(10, i);
    delay(10);
  }
  delay(1000);
  for(int i = 255; i > 0; i--)
  {
    digitalWrite(13, LOW);
    analogWrite(10, i);
    delay(10);
  }
  delay(1000);
  digitalWrite(9, HIGH);
  for(int i = 0; i < 255; i++)
  {
    digitalWrite(13, HIGH);
    analogWrite(10, i);
    delay(10);
  }
  delay(1000);
  for(int i = 255; i > 0; i--)
  {
    digitalWrite(13, LOW);
    analogWrite(10, i);
    delay(10);
  }
  delay(1000);
  
}
