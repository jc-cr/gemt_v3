#pragma once
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "gemt_interface.h"

// Test will display readings for evaluation

GEMTtest mpuDisplay;
Adafruit_MPU6050 mpu;

void mpuTest(void);



void runMPUtest(void)
{
  mpuDisplay.setFirstLine("MPU Info:");
  mpuDisplay.setInfoMsgLine("+ -> 5V"); 
  mpuDisplay.setInfoMsgLine("- -> GND"); 
  mpuDisplay.setInfoMsgLine("SDA-> 20");
  mpuDisplay.setInfoMsgLine("SCL -> 21");

  if(mpuDisplay.showInfoScreen())
  {
    // set accelerometer range to +-8G
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    // set gyro range to +- 500 deg/s
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    // set filter bandwidth to 21 Hz
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    mpuDisplay.setFirstLine("MPU Test:");
    mpuDisplay.showStaticTestScreen(mpuTest);
  }

}

void mpuTest()
{
  if (!mpu.begin()) 
  {
		mpuDisplay.setStaticTestFeedbackLine("Failed to find MPU6050 chip");
    mpuDisplay.showStaticTestFeedback(10000);
	}

 /* Get new sensor events with the readings */
	sensors_event_t a, g, temp;
	mpu.getEvent(&a, &g, &temp);

  String accelMsgs[3] = {
      "Accel X: " + String(a.acceleration.x),
      "Y: " + String(a.acceleration.y),
      "Z: " + String(a.acceleration.z)
    };

    String gyroMsgs[3] = {
      "Rotate X: " + String(g.gyro.x),
      "Y: " + String(g.gyro.y),
      "Z: " + String(g.gyro.z)
    };

    for(int i=0; i<3; i++) {
      mpuDisplay.setStaticTestFeedbackLine(accelMsgs[i]); 
    }
    
    for(int i=0; i<3; i++) {
      mpuDisplay.setStaticTestFeedbackLine(gyroMsgs[i]);
    }
  
    mpuDisplay.showStaticTestFeedback(2000);
}