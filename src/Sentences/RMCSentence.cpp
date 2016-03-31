/*
 * RMCSentence.cpp
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

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <Time.h>

#include "Nmea.h"
#include "StrUtils.h"

const float DEGREES_IN_MINUTE = 1.0/60.0;
const float MINUTES_IN_DEGREE = 60.0;


RMCSentence::RMCSentence() : Sentence("GP", NMEA_RMC) {
}

RMCSentence::~RMCSentence() {
}

char* RMCSentence::get(char str[], size_t buflen) const {
  if (str == NULL || buflen < NMEA_MAX_LENGTH)
    return NULL;
    
  str[0] = '$';
  str[1] = '\0';
  strcat(str, talker);
  strcat(str, tag);

  addComma(str);

  timeToString(datetime, milliseconds, strchr(str, '\0'));

  addComma(str);
  
  strcat(str, fix ? "A" : "V");

  addComma(str);

  pointToString(position, strchr(str, '\0'));

  addComma(str);
  
  ftoa(speed, strchr(str, '\0'), 2);

  addComma(str);
    
  ftoa(course, strchr(str, '\0'), 2);

  addComma(str);
    
  long fulldate = datetime.Day * 10000L + datetime.Month * 100L + (datetime.Year - 30);
  zeropad(ltoa(fulldate, strchr(str, '\0'), 10), 6);

  addComma(str);

  ftoa(fabs(variation), strchr(str, '\0'), 1);

  addComma(str);

  if (variation > 0)
    strcat(str, EAST);
  else
    strcat(str, WEST);

  addComma(str);
  
  strcat(str, "D");
  
  return addChecksum(str);
}

bool RMCSentence::set(const char nmea[]) {
  if (!valid(nmea))
    return false;

  if (!matches(nmea))
    return false;
  
  const char *p = nmea;

  // get time
  p = nextToken(p);

  p = parseTime(p, datetime, milliseconds);
  
  if (*p == 'A') 
    fix = true;
  else if (*p == 'V')
    fix = false;
  else
    return false;

  p = nextToken(p);

  p = parsePoint(p, position);

  if (',' != *p)
    speed = atof(p);
  
  p = nextToken(p);
  
  if (',' != *p)
    course = atof(p);
  
  p = nextToken(p);
  
  if (',' != *p) {
    uint32_t fulldate = atol(p);
    datetime.Day = fulldate / 10000L;
    datetime.Month = (fulldate % 10000L) / 100L;
    datetime.Year = 30 + (fulldate % 100L);
  }

  p = nextToken(p);

  if (',' != *p)
    variation = atof(p);

  p = nextToken(p);

  if (*p == *WEST) 
    variation = -variation;
  
  return true;
}

time_t RMCSentence::makeTime() {
  return ::makeTime(datetime);
}

const char* parsePoint(const char* str, Point& point) {
  const char* p = str;

  if (',' != *p)
    point.latitude = stringToDecimalDegrees(p);

  p = nextToken(p);

  if (*p == *SOUTH)
    point.latitude = -point.latitude;

  // parse out longitude
  p = nextToken(p);

  if (',' != *p)
    point.longitude = stringToDecimalDegrees(p);

  p = nextToken(p);

  if (*p == *WEST)
    point.longitude = -point.longitude;

  return nextToken(p);
}

char* pointToString(const Point& point, char str[]) {
  decimalDegreesToString(fabs(point.latitude), str, 9);

  addComma(str);

  if (point.latitude > 0)
    strcat(str, NORTH);
  else
    strcat(str, SOUTH);

  addComma(str);

  decimalDegreesToString(fabs(point.longitude), strchr(str, '\0'), 10);

  addComma(str);

  if (point.longitude > 0)
    strcat(str, EAST);
  else
    strcat(str, WEST);

  return strchr(str, '\0');
}

const char* parseTime(const char str[], tmElements_t& datetime, uint16_t& milliseconds) {
  float timef = atof(str);

  uint32_t time = timef;
  datetime.Hour = time / 10000L;
  datetime.Minute = (time % 10000L) / 100L;
  datetime.Second = (time % 100L);

  milliseconds = fmod(timef, 1.0) * 1000.0;

  return nextToken(str);
}

char* timeToString(const tmElements_t& datetime, uint16_t milliseconds, char str[]) {
  float timef = datetime.Hour * 10000.0  + datetime.Minute * 100.0 + datetime.Second + milliseconds * 0.001;

  zeropad(ftoa(timef, str, 3), 10);

  return strchr(str, '\0');
}

char* decimalDegreesToString(float dd, char str[], size_t len) {
  float f = floor(dd) * 100.0 + fmod(dd, 1.0) * MINUTES_IN_DEGREE;

  return zeropad(ftoa(f, str, 4), len);
}

float stringToDecimalDegrees(const char str[]) {
  float f = atof(str);

  return floor(f * 0.01) + fmod(f, 100.0) * DEGREES_IN_MINUTE;
}

