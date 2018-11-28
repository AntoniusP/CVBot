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
#define MAX_TRANSLATE_VELOCITY 250

// global variables to interface motors and camera
ZumoMotors motors;
Pixy2 pixy;

// parameters: proportional, integral, derivative, isServo
PIDLoop pan(500, 200, 500, true);
PIDLoop tilt(500, 0, 500, true);
PIDLoop turn(300, 200, 300, false);
PIDLoop translate(5, 0, 11, false);


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

// get largest block on camera frame that has existed for at least 25ms and return it to follow.

int acquireBlock(){
    if (pixy.ccc.numBlocks)
    {
        if(pixy.ccc.blocks[0].m_age>25)
        {
            return pixy.ccc.blocks[0].m_index;       
        }  
    }
    return -1;
}

Block* trackBlock(int index)
{
    int i;
    for (i=0; i<pixy.ccc.numBlocks; i++)
    {
        if (index==pixy.ccc.blocks[i].m_index)
        {
            return &pixy.ccc.blocks[i];
        }
    }
    return NULL;
}


// use pan/tilt servos to scan for blocks
int scan_inc = (PIXY_RCS_MAX_POS - PIXY_RCS_MIN_POS) / 120;
int last_move = 0;
void findObjects()
{
    if(millis() - last_move > 20) 
    {
        last_move = millis();
        pan.m_command += scan_inc;
        
        if((pan.m_command >= PIXY_RCS_MAX_POS) || (pan.m_command <= PIXY_RCS_MIN_POS))
        {
            tilt.m_command = random(PIXY_RCS_MIN_POS * 0.7, PIXY_RCS_MIN_POS);
            scan_inc = -scan_inc;
            if(scan_inc < 0)
            {
                move_left_motor_speed(-200, 2);
                move_right_motor_speed(200, 2);
            }
            else
            {
                move_left_motor_speed(-200, 2);
                move_right_motor_speed(200, 2);
            }
            delay(random(250, 500));
        }
        pixy.setServos(pan.m_command, tilt.m_command);
    }
}


// main colored object tracking function
// signature 1-7: 1-red, 2-orange, 3-yellow, 4-green, 5-light blue, 6-blue, 7-violet
enum States {searching = 1, tracking = 0};
States CVB = searching;
static int index = -1;
void cccFunction() 
{
    int block_width = 0;
    int block_height = 0;
    int block_size = 8000; // running average for distance of block
    int panError, tiltError, translateError, turnError,left,right;
    Block *tracked_block = NULL;
    
    pixy.ccc.getBlocks();
    
    if(index == -1)
    {
        CVB = searching;
        index = acquireBlock();
        if(index >= 0)
        {
            CVB = tracking;
        }
    }
    
    if(index >= 0)
    {
        tracked_block = trackBlock(index);
    }
    
    if(tracked_block){
        //calculate the for the pan and tilt offset
        panError = (int)pixy.frameWidth/2 - (int)tracked_block->m_x;
        tiltError = (int)tracked_block->m_y - (int)pixy.frameHeight/2;

        //update PID's "m_command"
        pan.update(panError);
        tilt.update(tiltError);

        //set pan/tilt servos
        pixy.setServos(pan.m_command, tilt.m_command);
        
        
        //use pan error for turning 
        panError += pan.m_command - PIXY_RCS_CENTER_POS;
        tiltError += tilt.m_command - PIXY_RCS_CENTER_POS - PIXY_RCS_CENTER_POS/2 + PIXY_RCS_CENTER_POS/8;    

        
        block_size += block_width * block_height;
        block_size -= block_size / 8;
        translateError = (14000 - block_size) ;
        
        
        turn.update(panError);
        translate.update(translateError);
        
        if (translate.m_command > 20)
        {
            translate.m_command *= 3.4;
        }
        else if (translate.m_command < -30) 
        {
            translate.m_command *= 2;
        }
        else 
        {
            translate.m_command = 0;
        }
        
        left = -turn.m_command + translate.m_command;
        right = turn.m_command + translate.m_command;
  
        move_left_motor_speed(left, 2);
        move_right_motor_speed(right, 2);
    }
    else 
    {
        findObjects();
        turn.reset();
        translate.reset(); 
        move_left_motor_speed(0, 2);
        move_right_motor_speed(0, 2);
        index = -1;
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


