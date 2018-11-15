// Computer Vision Robot 
// by Jiunn Siow and Antonius Panggabean
// UCR CS122A (Jeffrey McDaniels)

#include <PIDLoop.h>
#include <Pixy2.h>
#include <Pixy2CCC.h>
#include <Pixy2I2C.h>
#include <Pixy2Line.h>
#include <Pixy2SPI_SS.h>
#include <Pixy2UART.h>
#include <Pixy2Video.h>
#include <TPixy2.h>
#include <ZumoBuzzer.h>
#include <ZumoMotors.h>

#include <Wire.h>
#include <SPI.h>


#define LED_PIN 13

//global variable to interface with motors
ZumoMotors motors;
Pixy2 pixy;

// signature 1: red object
// signature 5: blue object
void cccFunction() 
{
    pixy.ccc.getBlocks();
    
    if(pixy.ccc.numBlocks > 0 )
    {
        Serial.print("Blocks Detected: ");
        for(int i=0; i < pixy.ccc.numBlocks; i++)
        {
            Serial.print("block ");
            Serial.print(i);
            Serial.print(": ");
            pixy.ccc.blocks[i].print();
        }
  
         move_left_motor_speed(100, 2);
         move_right_motor_speed(100,2);
         digitalWrite(LED_PIN,HIGH);
    }
    else
    {
         move_left_motor_speed(0, 2);
         move_right_motor_speed(0,2); 
    }
}

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

void setup() 
{
    Serial.begin(115200);
    Serial.println("CVBot Start..\n");
  
    pixy.init();
    
    pinMode(LED_PIN,HIGH);
    //motors.flipLeftMotor(true);
    //motors.flipRightMotor(true);
}

void loop() 
{
    cccFunction();
}
