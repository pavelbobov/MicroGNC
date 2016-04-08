/*
 * Chrono.h
 *
 *  Created on: Apr 7, 2016
 *      Author: pavel
 */

#ifndef CHRONO_H_
#define CHRONO_H_

#ifdef ARDUINO

struct tm
{
  unsigned char tm_sec;     /* Seconds. [0-60] (1 leap second) */
  unsigned char tm_min;     /* Minutes. [0-59] */
  unsigned char tm_hour;    /* Hours. [0-23] */
  unsigned char tm_mday;    /* Day.   [1-31] */
  unsigned char tm_mon;     /* Month. [0-11] */
  unsigned int  tm_year;    /* Year - 1900.  */
};

#else

#include <time.h>

#endif

/*
 * Parses hhmmss.ss string to time elements of tm structure
 */
const char* parseTime(const char str[], tm& datetime, int& milliseconds);
char* timeToString(const tm& datetime, int milliseconds, char str[]);

/*
 * Parses ddmmyy string to date elements of tm structure
 */
const char* parseDate(const char str[], tm& datetime);
char* dateToString(const tm& datetime, char str[]);

#endif /* CHRONO_H_ */
