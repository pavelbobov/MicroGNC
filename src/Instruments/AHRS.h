/*
 * AHRS.h
 *
 *  Created on: Jun 10, 2016
 *      Author: pavel
 */

#ifndef AHRS_H_
#define AHRS_H_

#include <Arduino.h>
#include "Instrument.h"
#include "MPU9250.h"

/*
 * Attitude Heading Reference System (AHRS) based on InvenSense nine-axis
 * (gyroscope + accelerometer + compass) MEMS MotionTracking device MPU-9250
 */
class AHRS : public Instrument {
  MPU9250 mpu;

public:
  /*
   * Constructor
   */
  AHRS();

  /*
   * Destructor
   */
  virtual ~AHRS();

  /*
   * Initializes the device.
   */
  void init();

  /*
   * Reads:
   *  - raw megnetometer reading as MAG sentence
   *  - magnetic heading from the sensor as HDG sentence.
   *
   * @param  sentence buffer
   * @param  maxSize buffer size. Recommended buffer size is MAX_MESSAGE_LENGTH (83 characters)
   * @return message string or NULL if no sentences are available
   */
  virtual char* getSentence(char sentence[], size_t maxSize);

private:
  void getAccelData(float* ax, float* ay, float* az);
  void getGyroData(float* gx, float* gy, float* gz);
  void getMagData(float* mx, float* my, float* mz);
};

#endif /* AHRS_H_ */
