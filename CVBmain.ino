// Computer Vision Robot 
// by Jiunn Siow and Antonius Panggabean
// UCR CS122A (Jeffrey McDaniels)

#include <SPI.h>

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

Pixy2 pixy;


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
  }
}

void lineFunction() 
{
  pixy.line.getMainFeatures();
}

void setup() 
{
  Serial.begin(115200);
  Serial.println("CVBot Start..\n");
  
  pixy.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  cccFunction();
}
