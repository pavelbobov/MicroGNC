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

RMCSentence::RMCSentence() : NMEASentence("GP", NMEA_RMC) {
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

