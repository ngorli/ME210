
/******************** Ultrasonic Sensors ********************/
/*
 *
 */
float getUltraSonicBack()
{
  digitalWrite(trigBack, LOW);
  delayMicroseconds(2);
  digitalWrite(trigBack, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigBack, LOW);
  float duration = pulseIn(echoBack, HIGH);
  float distance = (duration * 0.0343) / 2; // Convert to cm
  return distance;

}

/*
 *
 */
float getUltraSonicFront()
{
 digitalWrite(trigFront, LOW);
  delayMicroseconds(2);
  digitalWrite(trigFront, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigFront, LOW);
  float duration = pulseIn(echoFront, HIGH);
  float distance = (duration * 0.0343) / 2; // Convert to cm
  return distance;

}

/*
 *
 */
float getUltraSonicRight()
{
  digitalWrite(trigLeft, LOW);
  delayMicroseconds(2);
  digitalWrite(trigLeft, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigLeft, LOW);
  float duration = pulseIn(echoLeft, HIGH);
  float distance = (duration * 0.0343) / 2; // Convert to cm
  return distance;

}

/*
 *
 */
float getUltraSonicLeft()
{

}

