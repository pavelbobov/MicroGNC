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

#include "Sentences.h"
#include "StrUtils.h"

const float DEGREES_IN_MINUTE = 1.0/60.0;
const float MINUTES_IN_DEGREE = 60.0;

RMCSentence::RMCSentence() :
  Sentence(TALKER_GP, TAG_RMC),
  milliseconds(0), fix(false),
  speed(0), course(0), variation(0) {
}

RMCSentence::~RMCSentence() {
}

char* RMCSentence::get(char str[], size_t buflen) const {
  if (str == NULL || buflen < MAX_SENTENCE_LENGTH)
    return NULL;
    
  addHead(str);

  strcat(str, ",");

  char* p = strend(str);

  timeToString(datetime, milliseconds, p);

  strcat(p, fix ? ",A," : ",V,");

  p = strend(p);

  pointToString(position, p);

  strcat(p, ",");

  p = strend(p);
  
  ftoa(speed, p, 2);

  strcat(p, ",");
    
  p = strend(p);

  ftoa(course, p, 2);

  strcat(p, ",");
    
  p = strend(p);

  dateToString(datetime, p);

  strcat(p, ",");

  p = strend(p);

  ftoa(fabs(variation), p, 1);

  if (variation > 0)
    strcat(p, ",E,D");
  else
    strcat(p, ",W,D");

  return addChecksum(str);
}

bool RMCSentence::set(const char str[]) {
  if (!valid(str))
    return false;

  if (!matches(str))
    return false;
  
  const char *p = str;

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
  
  p = parseDate(p, datetime);

  if (',' != *p)
    variation = atof(p);

  p = nextToken(p);

  if (*p == *WEST) 
    variation = -variation;
  
  return true;
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

  strcat(str, ",");

  if (point.latitude > 0)
    strcat(str, NORTH);
  else
    strcat(str, SOUTH);

  strcat(str, ",");

  decimalDegreesToString(fabs(point.longitude), strchr(str, '\0'), 10);

  strcat(str, ",");

  if (point.longitude > 0)
    strcat(str, EAST);
  else
    strcat(str, WEST);

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


