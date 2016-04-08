/*
 * Chrono.cpp
 *
 *  Created on: Apr 7, 2016
 *      Author: pavel
 */

#include <math.h>
#include <string.h>
#include "StrUtils.h"
#include "Chrono.h"

const char* parseTime(const char str[], tm& datetime, int& milliseconds) {
  float timef = atof(str);

  unsigned long time = timef;
  datetime.tm_hour = time / 10000L;
  datetime.tm_min = (time % 10000L) / 100L;
  datetime.tm_sec = (time % 100L);

  milliseconds = fmod(timef, 1.0) * 1000.0;

  return nextToken(str);
}

char* timeToString(const tm& datetime, int milliseconds, char str[]) {
  float timef = datetime.tm_hour * 10000.0  + datetime.tm_min * 100.0 + datetime.tm_sec + milliseconds * 0.001;

  zeropad(ftoa(timef, str, 3), 10);

  return strchr(str, '\0');
}

const char* parseDate(const char str[], tm& datetime) {
  unsigned long fulldate = atol(str);
  datetime.tm_mday = fulldate / 10000L;
  datetime.tm_mon = (fulldate % 10000L) / 100L;
  datetime.tm_year = 100 + (fulldate % 100L);

  return nextToken(str);
}

char* dateToString(const tm& datetime, char str[]) {
  long fulldate = datetime.tm_mday * 10000L + datetime.tm_mon * 100L + (datetime.tm_year - 100);
  zeropad(ltoa2(fulldate, strchr(str, '\0'), 10), 6);
  return str;
}

