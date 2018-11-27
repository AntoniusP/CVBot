// Computer Vision Robot 
// UCR CS122A Final Project
// Partners: Antonius Panggabean, Jiunn Siow
// Lab Section: 021
 
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
#define MAX_VELOCITY 250

// global variables to interface motors and camera
ZumoMotors motors;
Pixy2 pixy;

// parameters: proportional, integral, derivative, isServo
PIDLoop pan(400, 0, 400, true);
PIDLoop tilt(500, 0, 500, true);
PIDLoop turn(300, 500, 300, false);
PIDLoop translate(400, 750, 300, true);


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



// follow camera with pan/tilt motors
void track()
{
    
}


// chase object with zumo shield motors
void chase()
{
    
}


// use pan/tilt servos to scan for blocks
int scan_inc = (PIXY_RCS_MAX_POS - PIXY_RCS_MIN_POS) / 150;
int last_move = 0;
void findObjects()
{
    if(millis() - last_move > 20) 
    {
        last_move = millis();
        pan.m_command += scan_inc;
        if((pan.m_command >= PIXY_RCS_MAX_POS) || (pan.m_command <= PIXY_RCS_MIN_POS))
        {
            tilt.m_command = random(PIXY_RCS_MAX_POS * 0.5, PIXY_RCS_MAX_POS);
            scan_inc = -scan_inc;
            if(scan_inc < 0)
            {
                move_left_motor_speed(-250, 2);
                move_right_motor_speed(250, 2);
            }
            else
            {
                move_left_motor_speed(-250, 2);
                move_right_motor_speed(250, 2);
            }
            delay(random(250, 500));
        }
        pixy.setServos(pan.m_command, tilt.m_command);
    }
}


// main colored object tracking function
// signature 1-7: 1-red, 2-orange, 3-yellow, 4-green, 5-light blue, 6-blue, 7-violet
void cccFunction() 
{
    int panError, tiltError, translateError, turnError;
    Block tracked_block = NULL;
    
    pixy.ccc.getBlocks();

    // track and follow appropriate blocks, else scan for new objects
    if(pixy.ccc.numBlocks > 0 ) 
    {
        Serial.print("Blocks Detected: ");
        for(int i=0; i < pixy.ccc.numBlocks; i++)
        {
            Serial.print("block "); Serial.print(i); Serial.print(": ");
            pixy.ccc.blocks[i].print();
        }

        // track();
        if(pixy.ccc.blocks[0].m_age > 25)
        {
            for(i=0; i<pixy.ccc.numBlocks[i].m_index)
            {
                if(pixy.ccc.blocks[0].index 
            }
        }
        
        // chase();
        
        move_left_motor_speed(100, 2);
        move_right_motor_speed(100,2);
        digitalWrite(LED_PIN,HIGH);
    }
    else
    {
        // findObjects();
        
        move_left_motor_speed(0, 2);
        move_right_motor_speed(0,2); 
        digitalWrite(LED_PIN,LOW);
    }
}



// ======================= main loop =======================
void setup() 
{
    Serial.begin(115200);
    Serial.println("CVBot Start..\n");
    pixy.init();
    motors.setLeftSpeed(0);
    motors.setRightSpeed(0);
    pinMode(LED_PIN,HIGH);
}


void loop() 
{
    cccFunction();
}


//motors.flipLeftMotor(true);
//motors.flipRightMotor(true);
