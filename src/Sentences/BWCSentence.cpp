 /*
 * BWCSentence.cpp
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

BWCSentence::BWCSentence() : Sentence(TALKER_GP, NMEA_BWC),
                             milliseconds(0), bearingTrue(-1),
                             bearingMagnetic(-1), distance(-1) {
  waypointId[0] = '\0';
}

BWCSentence::~BWCSentence() {
}

char* BWCSentence::get(char str[], size_t buflen) const {
  if (str == NULL || buflen < NMEA_MAX_LENGTH)
    return NULL;
    
  addHead(str);

  strcat(str, ",");

  char* p = strend(str);

  timeToString(datetime, milliseconds, p);

  strcat(p, ",");

  p = strend(p);
  
  pointToString(waypoint, p);

  strcat(p, ",");

  if (bearingTrue >= 0) {
    p = strend(p);
    ftoa(bearingTrue, p, 2);
  }

  strcat(p, ",T,");

  if (bearingMagnetic >= 0) {
    p = strend(p);
    ftoa(bearingMagnetic, p, 2);
  }

  strcat(p, ",M,");

  if (distance >= 0) {
    p = strend(p);
    ftoa(distance, p, 2);
  }
  
  strcat(p, ",N,");

  strcat(p, waypointId);
  
  return addChecksum(str);
}

bool BWCSentence::set(const char nmea[]) {
  if (!valid(nmea))
    return false;

  if (!matches(nmea))
    return false;
  
  const char *p = nmea;

  // get time
  p = nextToken(p);

  p = parseTime(p, datetime, milliseconds);
  
  p = parsePoint(p, waypoint);

  if (',' != *p)
    bearingTrue = atof(p);
  else 
    bearingTrue = -1;
  
  p = nextToken(p);
  p = nextToken(p);

  if (',' != *p)
    bearingMagnetic = atof(p);
  else 
    bearingMagnetic = -1;
    
  p = nextToken(p);
  p = nextToken(p);

  if (',' != *p)
    distance = atof(p);
  else 
    distance = -1;

  p = nextToken(p);
  p = nextToken(p);
  
  if (',' != *p)
    strncpy(waypointId, p, strchr(p, '*') - p);
  else 
    waypointId[0] = '\0';
  
  return true;
}

