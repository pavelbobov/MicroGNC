/*
 * MAGSentence.cpp
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

MAGSentence::MAGSentence() :
  Sentence(TALKER_HC, TAG_MAG), mx(0.0), my(0.0), mz(0.0) {
}

MAGSentence::~MAGSentence() {
}

char* MAGSentence::get(char str[], size_t buflen) const {
  if (str == NULL || buflen < MAX_SENTENCE_LENGTH)
    return NULL;

  addHead(str);

  strcat(str, ",");

  char* p = strend(str);

  if (!isnan(mx)) {
    ftoa(mx, p, 3);
  }

  strcat(str, ",");

  if (!isnan(my)) {
    p = strend(p);
    ftoa(my, p, 3);
  }

  strcat(str, ",");

  if (!isnan(mz)) {
    p = strend(p);
    ftoa(mz, p, 3);
  }

  return addChecksum(str);
}

bool   MAGSentence::set(const char str[]) {
  if (!valid(str))
    return false;

  if (!matches(str))
    return false;

  const char *p = str;

  // get time
  p = nextToken(p);

  if (',' != *p)
    mx = atof(p);
  else
    mx = NAN;

  p = nextToken(p);

  if (',' != *p)
    my = atof(p);
  else
    my = NAN;

  p = nextToken(p);

  if (',' != *p)
    mz = atof(p);
  else
    mz = NAN;

  return true;
}

