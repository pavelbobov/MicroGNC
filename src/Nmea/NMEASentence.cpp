/*
 * NMEASentence.cpp
 * 
 * NMEA sentences parsing and construction.
 * 
 * http://www.catb.org/gpsd/NMEA.html 
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
#include "Nmea.h"
#include "StrUtils.h"

const float DEGREES_IN_MINUTE = 1.0/60.0;
const float MINUTES_IN_DEGREE = 60.0;

NMEASentence::NMEASentence(const char talker[], const char tag[]) : 
  talker(talker), tag(tag) {
}

NMEASentence::~NMEASentence() {
}

bool NMEASentence::matches(const char str[]) {
  return strncmp(str + 1 + strlen(talker), tag, strlen(tag)) == 0;
}

const char* NMEASentence::parsePoint(const char* str, Point& point) {
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

char* NMEASentence::pointToString(const Point& point, char str[]) {
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

const char* NMEASentence::parseTime(const char str[], tmElements_t& datetime, uint16_t& milliseconds) {
  float timef = atof(str);
  
  uint32_t time = timef;
  datetime.Hour = time / 10000L;
  datetime.Minute = (time % 10000L) / 100L;
  datetime.Second = (time % 100L);

  milliseconds = fmod(timef, 1.0) * 1000.0;

  return nextToken(str);
}

char* NMEASentence::timeToString(const tmElements_t& datetime, uint16_t milliseconds, char str[]) {
  float timef = datetime.Hour * 10000.0  + datetime.Minute * 100.0 + datetime.Second + milliseconds * 0.001;
  
  zeropad(ftoa(timef, str, 3), 10);

  return strchr(str, '\0');
}

bool NMEASentence::valid(const char nmea[]) {
  if (!nmea) 
    return false;
    
  unsigned char sum = 0;
  const char* p;
  
  for (p = nmea + 1; *p != '*' && *p != '\0'; p++) 
    sum ^= *p;

  if (*p == '\0') return false;

  sum ^= (parseHex(p[1]) << 4) + parseHex(p[2]);

  return !sum;
}

char* NMEASentence::addChecksum(char str[]) {
  unsigned char parity = 0;

  char* p = str + 1;
  
  for(;*p != 0; p++)
    parity ^= *p; 

  p[0] = '*';
  p[1] = toHex(parity / 16);
  p[2] = toHex(parity % 16);
  p[3] = 0;
  
  return str;
}

char* NMEASentence::decimalDegreesToString(float dd, char str[], size_t len) {
  float f = floor(dd) * 100.0 + fmod(dd, 1.0) * MINUTES_IN_DEGREE;

  return zeropad(ftoa(f, str, 4), len);
}

float NMEASentence::stringToDecimalDegrees(const char str[]) {
  float f = atof(str);

  return floor(f * 0.01) + fmod(f, 100.0) * DEGREES_IN_MINUTE;
}
