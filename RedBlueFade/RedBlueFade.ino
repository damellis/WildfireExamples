int redpin = 8, greenpin = 9, bluepin = 10;
int val, temp, redval, blueval;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  val = analogRead(A0);
  Serial.print(val); Serial.print("\t");
  
  temp = map(val, 0, 1023, 0, 100);
  Serial.print(temp); Serial.print("\t");
  
  redval = map(temp, 45, 100, 0, 255);
  redval = constrain(redval, 0, 255);
  analogWrite(redpin, 255 - redval);
  Serial.print(redval); Serial.print("\t");
  
  blueval = map(temp, 0, 55, 255, 0);
  blueval = constrain(blueval, 0, 255);
  analogWrite(bluepin, 255 - blueval);
  Serial.print(blueval); Serial.print("\t");
  
  Serial.println();
  
  delay(10);
}
