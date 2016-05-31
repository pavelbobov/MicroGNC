/*
 * MWVSentence.cpp
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
#include "Nmea.h"
#include "StrUtils.h"

MWVSentence::MWVSentence() :
    windAngle(0), reference('R'), windSpeed(0), windSpeedUnits('N'),
    Sentence("WI", NMEA_MWV) {
}

MWVSentence::~MWVSentence() {
}

char* MWVSentence::get(char str[], size_t buflen) const {
  if (str == NULL || buflen < NMEA_MAX_LENGTH)
    return NULL;

  addHead(str);

  strcat(str, ",");

  char* p = strend(str);

  if (windAngle >= 0) {
    ftoa(windAngle, p, 1);
  }

  char ref[] = {',', reference, ',', '\0'};

  strcat(p, ref);

  if (windSpeed >= 0) {
    p = strend(p);
    ftoa(windSpeed, p, 1);
  }

  char units[] = {',', windSpeedUnits, ',', 'A', '\0'};

  strcat(p, units);

  return addChecksum(str);
}

bool MWVSentence::set(const char nmea[]) {
  if (!valid(nmea))
    return false;

  if (!matches(nmea))
    return false;

  const char *p = nmea;

  // get time
  p = nextToken(p);

  if (',' != *p)
    windAngle = atof(p);
  else
    windAngle = -1;

  p = nextToken(p);

  reference = p[0];

  p = nextToken(p);

  if (',' != *p)
    windSpeed = atof(p);
  else
    windSpeed = -1;

  p = nextToken(p);

  windSpeedUnits = p[0];

  return true;
}
