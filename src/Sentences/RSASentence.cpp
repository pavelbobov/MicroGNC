/*
 * RSASentence.cpp
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

#include "Sentences.h"
#include "StrUtils.h"

RSASentence::RSASentence() :
  Sentence(TALKER_ER, TAG_RSA), starboardRudderAngle(0), portRudderAngle(0)  {
}

RSASentence::~RSASentence() {
}

char* RSASentence::get(char str[], size_t buflen) const {
  if (str == NULL || buflen < MAX_SENTENCE_LENGTH)
    return NULL;

  addHead(str);

  strcat(str, ",");

  char* p = strend(str);

  ftoa(starboardRudderAngle, p, 1);

  strcat(str, ",A,");

  p = strend(str);

  ftoa(portRudderAngle, p, 1);

  strcat(str, ",A");

  return addChecksum(str);
}

bool  RSASentence::set(const char str[]) {
  if (!valid(str))
    return false;

  if (!matches(str))
    return false;

  const char *p = str;

  // get time
  p = nextToken(p);

  if (',' != *p)
    starboardRudderAngle = atof(p);
  else
    starboardRudderAngle = 0;

  p = nextToken(p);
  p = nextToken(p);

  if (',' != *p)
    portRudderAngle = atof(p);
  else
    portRudderAngle = 0;

  return true;
}
