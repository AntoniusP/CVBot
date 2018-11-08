#include <Wire.h>
#include <ZumoShield.h>

#define LED_PIN 13

//global variable to interface with motors
ZumoMotors motors;

void move_left_motor_speed(int speed ,int time){
  //Speed goes from 400 to -400
  motors.setLeftSpeed(speed);
  delay(time); 
}
void move_right_motor_speed(int speed,int time){
  //speed goes from 400 to -400
  motors.setRightSpeed(speed);
  delay(time);  
}
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN,HIGH);
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);
}

void loop() {
  // put your main code here, to run repeatedly:
   move_left_motor_speed(200,3000);
   move_right_motor_speed(200,3000);
   digitalWrite(LED_PIN,HIGH);
}
