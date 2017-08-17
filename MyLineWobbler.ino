
#include <I2Cdev.h>
#include <MPU6050.h>
#include <FastLED.h>

//global LED params
const uint8_t dataPin = 12;
const uint8_t clockPin = 13;
const uint16_t ledNum = 270;
CRGB colors[ledNum];



//MPU6050 settings
#if I2CDev_IMPLEMENTATION == I2CDEDV_ARDUINO_WIRE
  #include "Wire.h"
#endif

MPU6050 mpu6050;

int16_t ax,ay,az;
int16_t gx,gy,gz;
int16_t ay_offset;

//player settings

int32_t playerPos = 0;
const int16_t offsetAccelToPos = 1600;
const int16_t offsetPosToLedPos = 10;
const CRGB playerColor = CRGB(30,0,0);
const uint8_t atkThreshold = 12;
const uint8_t atkRange = 4;
const CRGB atkColor = CRGB(0,10,10);
const uint8_t offset = 0;

int8_t direction = 1;


void clearLED(CRGB colors[],CRGB color = CRGB(0,0,0))
{
  for(uint16_t i = 0;i<ledNum-5;i+=1)
  {
    colors[i] = color;
  }
  for(uint16_t i=ledNum-5;i<ledNum;i+=1)
  {
    colors[i] = CRGB(20,10,5);
  }
}

void setup() {

  //LED initialize
  FastLED.addLeds<APA102,dataPin,clockPin>(colors,ledNum);
  //clear LED
  clearLED(colors);
  FastLED.show();
  
  //initialize MPU6050
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400,true);
  #endif
  mpu6050.initialize();
  if(!mpu6050.testConnection())
  {
    clearLED(colors,CRGB(10,0,0));
    FastLED.show();
    return 1;
  }
  //start serial
  Serial.begin(9600);
  Serial.println("START");
  ay_offset = mpu6050.getAccelerationY();
  pinMode(2,INPUT_PULLUP);
}

void loop() {
  //reset led state
  clearLED(colors);
  if(digitalRead(2)==LOW)
    direction*=-1;
  

  //get player move 
  ay = (mpu6050.getAccelerationY()-ay_offset)*direction;
  int16_t moveDistance = ay/offsetAccelToPos;
  if(moveDistance>atkThreshold||moveDistance<-atkThreshold)
  {
    uint16_t ledPos = playerPos/offsetPosToLedPos;
    for(int8_t i = -atkRange;i<atkRange+1;i+=1)
    {
      if(ledPos+i<0||ledPos+i>ledNum-1)
        continue;
      colors[ledPos+i] = atkColor*(abs(i)+1);
    }
  }
  else
  {
    playerPos += moveDistance+offset;
    if(playerPos<0)playerPos = 0;
    if(playerPos>(ledNum-1)*offsetPosToLedPos)playerPos = (ledNum-1)*offsetPosToLedPos;
    uint16_t ledPos = playerPos/offsetPosToLedPos;
    colors[ledPos] = playerColor;
  }
  
  //display
  FastLED.show();
//  Serial.println(ay/offsetAccelToPos);

}
