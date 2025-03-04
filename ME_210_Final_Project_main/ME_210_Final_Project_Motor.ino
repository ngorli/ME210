/**************************** RIGHT MOTOR CONTROL **********************/
/*
*
*/
void rightMotorForward()
{
  analogWrite(ENA_R, SPEED_R);
  digitalWrite(INPUT1_R, HIGH);
  digitalWrite(INPUT2_R, LOW);  
}


/*
*
*/
void rightMotorBackward()
{
  analogWrite(ENA_R,SPEED_R);
  digitalWrite(INPUT1_R, LOW);
  digitalWrite(INPUT2_R, HIGH);  
}


/*
*
*/
void rightMotorOff()
{
  analogWrite(ENA_L, 0);
  digitalWrite(INPUT1_R, LOW);
  digitalWrite(INPUT2_R, LOW);  
}


/**************************** LEFT MOTOR CONTROL **********************/
/*
*
*/
void leftMotorForward()
{
  analogWrite(ENA_L, SPEED_L);
  digitalWrite(INPUT1_L, HIGH);
  digitalWrite(INPUT2_L, LOW);  
}


/*
*
*/
void leftMotorBackward()
{
  analogWrite(ENA_L, SPEED_L);
  digitalWrite(INPUT1_L, LOW);
  digitalWrite(INPUT2_L, HIGH);  
}


/*
*
*/
void leftMotorOff()
{
  analogWrite(ENA_L, 0);
  digitalWrite(INPUT1_L, LOW);
  digitalWrite(INPUT2_L, LOW);
}
