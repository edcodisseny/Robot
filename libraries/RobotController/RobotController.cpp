/*
  RobotController.cpp - Library for robot code.
  Created by EDCO, January 31, 2019.
  Released into the public domain.
*/

#include "RobotController.h"

//See http://www.ti.com/lit/ds/symlink/pcf8574a.pdf
PCF8574 IRSensors(I2C_INFRA);  // add switches to lines  	(used as input)
PCF8574 MotorController(I2C_MOTORS);  // add LEDs to lines  (used as output)
uint8_t _motorStatus = 0;


/**
 * Divides a given PWM pin frequency by a divisor.
 * 
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 * 
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0 	// Modify millis function
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 * 
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 * 
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   http://forum.arduino.cc/index.php?topic=16612#msg121031
 */
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

void RobotBegin(){
	pinMode(BUTTON_START, INPUT_PULLUP);   	
	pinMode(PWM_CONTROL_FRONT_LEFT, OUTPUT);   	// Sets the (analog) pin as output
	pinMode(PWM_CONTROL_BACK_LEFT, OUTPUT);   	// Sets the (analog) pin as output
	pinMode(PWM_CONTROL_FRONT_RIGHT, OUTPUT);   // Sets the (analog) pin as output
	pinMode(PWM_CONTROL_BACK_RIGHT, OUTPUT);   	// Sets the (analog) pin as output
#ifdef ROBOT20
	setPwmFrequency(5, 64);	// 976.5625 Hz to pin 5 & 6 timer
	setPwmFrequency(9, 64);	// 488.28125 Hz to pin 9 & 10
#else
	setPwmFrequency(5, 256);	// 244.140625 Hz to pin 5 & 6
	setPwmFrequency(9, 256);	// 122.0703125 Hz to pin 9 & 10
	setPwmFrequency(3, 256);	// 244.140625 Hz to pin 3 & 11
#endif
	MotorController.begin(0);										// Set the initial status of the motors (off)
	_motorStatus = 0;
	MotorController.write8(_motorStatus);				// 
}

uint8_t statusMotor(){
	return _motorStatus;
}

void frontRight(int value, int mode){
  switch(mode){
    case FORWARD:
			analogWrite(PWM_CONTROL_FRONT_RIGHT, value);
			_motorStatus = (_motorStatus & (0b00111111) ) | M_RIGHT_FRONT_1;
			MotorController.write8(_motorStatus);
      break;
    case BACKWARD:
			analogWrite(PWM_CONTROL_FRONT_RIGHT, value);
			_motorStatus = (_motorStatus & (0b00111111) ) | M_RIGHT_FRONT_2;
			MotorController.write8(_motorStatus);
      break;
  }
}

void frontLeft(int value, int mode){
  switch(mode){
    case FORWARD:
			analogWrite(PWM_CONTROL_FRONT_LEFT, value);
			_motorStatus = (_motorStatus & (0b11001111) ) | M_LEFT_FRONT_1;
			MotorController.write8(_motorStatus);
      break;
    case BACKWARD:
			analogWrite(PWM_CONTROL_FRONT_LEFT, value);
			_motorStatus = (_motorStatus & (0b11001111) ) | M_LEFT_FRONT_2;
			MotorController.write8(_motorStatus);
      break;
  }
}


void backRight(int value, int mode){
  switch(mode){
    case FORWARD:
			analogWrite(PWM_CONTROL_BACK_RIGHT, value);
			_motorStatus = (_motorStatus & (0b11111100) ) | M_RIGHT_BACK_1;
			MotorController.write8(_motorStatus);
      break;
    case BACKWARD:
			analogWrite(PWM_CONTROL_BACK_RIGHT, value);
			_motorStatus = (_motorStatus & (0b11111100) ) | M_RIGHT_BACK_2;
			MotorController.write8(_motorStatus);
      break;
  }
}

void backLeft(int value, int mode){
  switch(mode){
    case FORWARD:
			analogWrite(PWM_CONTROL_BACK_LEFT, value);
			_motorStatus = (_motorStatus & (0b11110011) ) | M_LEFT_BACK_1;
			MotorController.write8(_motorStatus);
      break;
    case BACKWARD:
			analogWrite(PWM_CONTROL_BACK_LEFT, value);
			_motorStatus = (_motorStatus & (0b11110011) ) | M_LEFT_BACK_2;
			MotorController.write8(_motorStatus);
      break;
  }
}

void vMotion(int nLeft,int nRight){ // -255 <= nLeft <= 255      -255 <= nRight <= 255
	if(nRight >= 0){
		analogWrite(PWM_CONTROL_FRONT_RIGHT, nRight);
		_motorStatus = (_motorStatus & (0b00111111) ) | ((nRight != 0) ? M_RIGHT_FRONT_1 : 0x00);
		analogWrite(PWM_CONTROL_BACK_RIGHT, nRight);
		_motorStatus = (_motorStatus & (0b11111100) ) | ((nRight != 0) ? M_RIGHT_BACK_1 : 0x00);
	}else{
		analogWrite(PWM_CONTROL_FRONT_RIGHT, -nRight);
		_motorStatus = (_motorStatus & (0b00111111) ) | M_RIGHT_FRONT_2;
		analogWrite(PWM_CONTROL_BACK_RIGHT, -nRight);
		_motorStatus = (_motorStatus & (0b11111100) ) | M_RIGHT_BACK_2;
	}
	if(nLeft >= 0){
		analogWrite(PWM_CONTROL_FRONT_LEFT, nLeft);
		_motorStatus = (_motorStatus & (0b11001111) ) | ((nLeft != 0) ? M_LEFT_FRONT_1 : 0x00);
		analogWrite(PWM_CONTROL_BACK_LEFT, nLeft);
		_motorStatus = (_motorStatus & (0b11110011) ) | ((nLeft != 0) ? M_LEFT_BACK_1 : 0x00);
	}else{
		analogWrite(PWM_CONTROL_FRONT_LEFT, -nLeft);
		_motorStatus = (_motorStatus & (0b11001111) ) | M_LEFT_FRONT_2;
		analogWrite(PWM_CONTROL_BACK_LEFT, -nLeft);
		_motorStatus = (_motorStatus & (0b11110011) ) | M_LEFT_BACK_2;
	}
	MotorController.write8(_motorStatus);
}

void front(int value, int mode){
  frontRight(value, mode);
  frontLeft(value, mode);
}

void back(int value, int mode){
  backRight(value, mode);
  backLeft(value, mode);
}

void right(int value, int mode){
  frontRight(value, mode);
  backRight(value, mode);
}

void left(int value, int mode){
  frontLeft(value, mode);
  backLeft(value, mode);
}


//* CONTROL DELS SENSORS 
int getFrontSideRight(){//Lateral dret davanter
  return (IRSensors.read8() >> 6) & 0x01;
}

int getFrontSideLeft(){//Lateral esquerra davanter
  return (IRSensors.read8() >> 2) & 0x01;
}

int getFrontRight(){
  return (IRSensors.read8() >> 5) & 0x01;
}

int getFrontLeft(){
  return (IRSensors.read8() >> 3) & 0x01;
}

int getFrontCenter(){
  return (IRSensors.read8() >> 4) & 0x01;
}

int getBackLeft(){
  return (IRSensors.read8() >> 1) & 0x01;
}

int getBackRight(){
  return (IRSensors.read8()) & 0x01;
}

byte byIR(){
	return IRSensors.read8();
}

void vIR(bool *bBR,bool *bBL,bool *bFSL,bool *bFL,bool *bFC,bool *bFR,bool *bFSR){
	byte byIr = byIR();
	
	*bBR = byIr & 0x01;
	*bBL = (byIr >> 1) & 0x01;
	*bFSL = (byIr >> 2) & 0x01;
	*bFL = (byIr >> 3) & 0x01;
	*bFC = (byIr >> 4) & 0x01;
	*bFR = (byIr >> 5) & 0x01;
	*bFSR = (byIr >> 6) & 0x01;
}

void vShowIR(bool bBR,bool bBL,bool bFSL,bool bFL,bool bFC,bool bFR,bool bFSR){
  char sz[12];int i;

  for(i = 0; i < 5; i++)
    Serial.println();
  Serial.println(" /----/^\\----\\");
  sprintf(sz,    " |%c  %c %c %c  %c|",bFSL?'W':'B',bFL?'W':'B',bFC?'W':'B',bFR?'W':'B',bFSR?'W':'B');Serial.println(sz);
  Serial.println("[ ]    ^    [ ]");
  sprintf(sz,    "[ ]    %c    [ ]",digitalRead(BUTTON_START) ? 'N':'P');Serial.println(sz);
  Serial.println(" |     ^     |");
  Serial.println("[ ]    ^    [ ]");
  Serial.println("[ ]    ^    [ ]");
  sprintf(sz," |%c    ^    %c|",bBL?'W':'B',bBR?'W':'B');Serial.println(sz);
  Serial.println(" +-----------+");
  for(i = 0; i < 5; i++)
    Serial.println();
}

void vShowRobot(bool bBR,bool bBL,bool bFSL,bool bFL,bool bFC,bool bFR,bool bFSR,int nUsDist){
  char sz[12];int i;
  char cFR = (_motorStatus & 0x40) ? '^' : (_motorStatus & 0x80) ? 'v' : ' ';
  char cFL = (_motorStatus & 0x10) ? '^' : (_motorStatus & 0x20) ? 'v' : ' ';
  char cBR = (_motorStatus & 0x01) ? '^' : (_motorStatus & 0x02) ? 'v' : ' ';
  char cBL = (_motorStatus & 0x08) ? '^' : (_motorStatus & 0x04) ? 'v' : ' ';
  
  for(i = 0; i < 5; i++)
    Serial.println();
  Serial.print("    { "); Serial.print(nUsDist,DEC);Serial.println(" cm }");
  Serial.println(" /----/^\\----\\");
  sprintf(sz,    " |%c  %c %c %c  %c|",bFSL?'W':'B',bFL?'W':'B',bFC?'W':'B',bFR?'W':'B',bFSR?'W':'B');Serial.println(sz);
  Serial.println("[ ]         [ ]");
  sprintf(sz,    "[%c]    %c    [%c]",cFL,digitalRead(BUTTON_START) ? 'N':'P',cFR);Serial.println(sz);
  sprintf(sz,    "[ ]  +---+  [ ]");Serial.println(sz);
  Serial.println(" |   | | |   |");
  Serial.println("[ ]  | | |  [ ]");
  sprintf(sz,    "[%c]  +-|-+  [%c]",cBL,cBR);Serial.println(sz);
  Serial.println("[ ]    -    [ ]");
  sprintf(sz," |%c    -    %c|",bBL?'W':'B',bBR?'W':'B');Serial.println(sz);
  Serial.println(" +-----------+");
  for(i = 0; i < 5; i++)
    Serial.println();
}

void stopMotors(){
  front(0, FORWARD);
  back(0, FORWARD);
}

int nUltrasonicSensorDistanceCm(int trigPin, int echoPin){
  long int duration;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  duration = duration / 59;
  return (int)duration;
}

int getDistance(){
	return nUltrasonicSensorDistanceCm(TRIGGER_PIN,ECHO_PIN);
}

int nUltrasonicCm(){
	int nGd = getDistance();
	
	if(nGd < 2)
		return 2;
	if(nGd > 450)
		return 450;
		
	return nGd;
}

unsigned long getMillis(){
#ifdef ROBOT20
	return millis();
#else
	return millis()<<2; // Multiply by 4
#endif	
}

void doDelay(unsigned long msec){
	unsigned long initialMillis = getMillis();
	while((getMillis()-initialMillis) < msec);//{Serial.print(millis());Serial.print(" vs ");Serial.println(getMillis());}
}

