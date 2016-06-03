/*
 * TX23UWindSensor.h
 *
 * Driver for LaCrosse TX-23U wind sensor
 *
 * (C) Copyright 2016 Pavel Bobov.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TX23UWINDSENSOR_H_
#define TX23UWINDSENSOR_H_

#include "MicroGNC.h"

/*
 * Driver for LaCrosse TX-23U wind sensor
 */
class TX23UWindSensor : public Instrument {

  int           TxD;
  unsigned long start;
  bool          requestData;

public:
  /*
   * Constructor
   *
   * @param pin digital pin connected to TxD pin of the wind sensor
   */
  TX23UWindSensor(int pin);

  /*
   * Destructor
   */
  virtual ~TX23UWindSensor();

  /*
   * Reads wind speed and angle from the sensor formatted as MWV NMEA sentences.
   *
   * @param  sentence buffer
   * @param  maxSize buffer size. Recommended buffer size is MAX_MESSAGE_LENGTH (83 characters)
   * @return message string or NULL if no sentences are available
   */
  virtual char* getSentence(char sentence[], size_t maxSize);
};

#endif /* TX23UWINDSENSOR_H_ */
