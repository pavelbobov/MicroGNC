/*
 * MPU9250AHRS.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: pavel
 */

#include "AHRS.h"
#include "../Sentences/Sentences.h"

AHRS::AHRS() : Instrument(TALKER_HC) {
}

AHRS::~AHRS() {
}

void AHRS::init() {
  Wire.begin();
  delay(1000);
  mpu.MPU9250SelfTest(mpu.SelfTest);
  delay(1000);
  mpu.calibrateMPU9250(mpu.gyroBias, mpu.accelBias);
  delay(1000);
  mpu.initMPU9250();
  delay(1000);
   // Get magnetometer calibration from AK8963 ROM
  mpu.initAK8963(mpu.magCalibration);
  delay(1000);
}

char* AHRS::getSentence(char sentence[], size_t maxSize) {
  float mx, my, mz;

  getMagData(&mx, &my, &mz);

  //HDGSentence hdg;

  //hdg.magneticHeading = fmod(540.0 - atan2(my, mx) * RAD_TO_DEG, 360.0);

  //return hdg.get(sentence, maxSize);

  MAGSentence mag;
  mag.mx = mx;
  mag.my = my;
  mag.mz = mz;

  return mag.get(sentence, maxSize);
}

void AHRS::getAccelData(float* ax, float* ay, float* az)
{
  int16_t accelCount[3];
  mpu.readAccelData(accelCount);  // Read the x/y/z adc values
  mpu.getAres();

  // Now we'll calculate the accleration value into actual g's
  *ax = (float)accelCount[0]*mpu.aRes; // - accelBias[0];  // get actual g value, this depends on scale being set
  *ay = (float)accelCount[1]*mpu.aRes; // - accelBias[1];
  *az = (float)accelCount[2]*mpu.aRes; // - accelBias[2];
}

void AHRS::getGyroData(float* gx, float* gy, float* gz)
{
  int16_t gyroCount[3];
  mpu.readGyroData(gyroCount);  // Read the x/y/z adc values
  mpu.getGres();

  // Calculate the gyro value into actual degrees per second
  *gx = (float)gyroCount[0]*mpu.gRes;  // get actual gyro value, this depends on scale being set
  *gy = (float)gyroCount[1]*mpu.gRes;
  *gz = (float)gyroCount[2]*mpu.gRes;
}

void AHRS::getMagData(float* mx, float* my, float* mz)
{
  int16_t magCount[3];
  mpu.readMagData(magCount);  // Read the x/y/z adc values
  mpu.getMres();

  // Calculate the magnetometer values in milliGauss
  // Include factory calibration per data sheet and user environmental corrections
  *mx = (float)magCount[0] * mpu.mRes * mpu.magCalibration[0] - mpu.magbias[0];  // get actual magnetometer value, this depends on scale being set
  *my = (float)magCount[1] * mpu.mRes * mpu.magCalibration[1] - mpu.magbias[1];
  *mz = (float)magCount[2] * mpu.mRes * mpu.magCalibration[2] - mpu.magbias[2];
}


