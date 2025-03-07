/******************** Ultrasonic Sensors ********************/
/*
 *
 */
float getUltraSonicBack()
{
  delay(25); // Short delay to prevent excessive readings
  return backSensor.ping_cm();
  // digitalWrite(trigBack, LOW);
  // delayMicroseconds(2);
  // digitalWrite(trigBack, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(trigBack, LOW);
  // float duration = pulseIn(echoBack, HIGH);
  // float distance = (duration * 0.0343) / 2; // Convert to cm
  // return distance;

}

/*
 *
 */
float getUltraSonicFront()
{
  delay(25); // Short delay to prevent excessive readings
  return frontSensor.ping_cm();
  // digitalWrite(trigFront, LOW);
  // delayMicroseconds(2);
  // digitalWrite(trigFront, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(trigFront, LOW);
  // float duration = pulseIn(echoFront, HIGH);
  // float distance = (duration * 0.0343) / 2; // Convert to cm
  // return distance;

}



/*
 *
 */
float getUltraSonicLeft()
{
  delay(25); // Short delay to prevent excessive readings
  return leftSensor.ping_cm();
  // digitalWrite(trigLeft, LOW);
  // delayMicroseconds(2);
  // digitalWrite(trigLeft, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(trigLeft, LOW);
  // float duration = pulseIn(echoLeft, HIGH);
  // float distance = (duration * 0.0343) / 2; // Convert to cm
  // return distance;
}

float getUltraSonicRight()
{
  delay(25); // Short delay to prevent excessive readings
  return rightSensor.ping_cm();
  // digitalWrite(trigFront, LOW);
  // delayMicroseconds(2);
  // digitalWrite(trigFront, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(trigFront, LOW);
  // float duration = pulseIn(echoFront, HIGH);
  // float distance = (duration * 0.0343) / 2; // Convert to cm
  // return distance;

}











