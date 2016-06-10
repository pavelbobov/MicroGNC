/*
 * MCSSentence.cpp
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

MCSSentence::MCSSentence() :
  Sentence(TALKER_ER, TAG_MCS), motor(1), current(0)  {
}

MCSSentence::~MCSSentence() {
}

char* MCSSentence::get(char str[], size_t buflen) const {
  if (str == NULL || buflen < MAX_SENTENCE_LENGTH)
    return NULL;

  addHead(str);

  strcat(str, ",");

  char* p = strend(str);

  if (motor >= 0) {
    ltoa2(motor, p, 10);
  }

  strcat(str, ",");

  p = strend(str);

  if (current >= 0) {
    ftoa(current, p, 3);
  }

  return addChecksum(str);
}

bool  MCSSentence::set(const char str[]) {
  if (!valid(str))
    return false;

  if (!matches(str))
    return false;

  const char *p = str;

  // get time
  p = nextToken(p);

  if (',' != *p)
    motor = atoi(p);
  else
    motor = -1;

  p = nextToken(p);

  if (',' != *p)
    current = atof(p);
  else
    current = -1;

  return true;
}
