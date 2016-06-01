/*
 * TX23UWindSensor.h
 *
 * Created on: May 27, 2016
 *     Author: pavel
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
