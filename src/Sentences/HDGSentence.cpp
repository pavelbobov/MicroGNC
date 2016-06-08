/*
 * HDGSentence.cpp
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

HDGSentence::HDGSentence() :
   magneticHeading(0), magneticDeviation(NAN), magneticVariation(NAN),
   Sentence(TALKER_HC, NMEA_HDG) {
}

HDGSentence::~HDGSentence() {
}

char* HDGSentence::get(char str[], size_t buflen) const {
  if (str == NULL || buflen < NMEA_MAX_LENGTH)
    return NULL;

  addHead(str);

  strcat(str, ",");

  char* p = strend(str);

  if (magneticHeading >= 0) {
    ftoa(magneticHeading, p, 1);
  }

  strcat(str, ",");

  if (!isnan(magneticDeviation)) {
    p = strend(p);
    ftoa(fabs(magneticDeviation), p, 1);
  }

  strcat(str, ",");

  if (magneticDeviation > 0)
    strcat(str, "W,");
  else
    strcat(str, "E,");

  if (!isnan(magneticVariation)) {
    p = strend(p);
    ftoa(fabs(magneticVariation), p, 1);
  }

  strcat(str, ",");

  if (magneticVariation > 0)
    strcat(str, "W");
  else
    strcat(str, "E");

  return addChecksum(str);
}

bool HDGSentence::set(const char nmea[]) {
  if (!valid(nmea))
    return false;

  if (!matches(nmea))
    return false;

  const char *p = nmea;

  // get time
  p = nextToken(p);

  if (',' != *p)
    magneticHeading = atof(p);
  else
    magneticHeading = -1;

  p = nextToken(p);

  if (',' != *p)
    magneticDeviation = atof(p);
  else
    magneticDeviation = NAN;

  p = nextToken(p);

  if (p[0] == 'E')
    magneticDeviation = -magneticDeviation;

  p = nextToken(p);

  if (',' != *p)
    magneticVariation = atof(p);
  else
    magneticVariation = NAN;

  p = nextToken(p);

  if (p[0] == 'E')
    magneticVariation = -magneticVariation;

  return true;
}
