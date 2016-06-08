/*
 * CourseComputer.cpp
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
#include <math.h>
#include "CourseComputer.h"

CourseComputer::CourseComputer() : Instrument(TALKER_IN) {
}

CourseComputer::~CourseComputer() {
}

char* CourseComputer::getSentence(char sentence[], size_t buflen) {
  return bwc.get(sentence, buflen);
}

bool CourseComputer::putSentence(const char sentence[]) {
  if (rmc.set(sentence) || wpl.set(sentence)) {
    updateBWC();
    return true;
  }
  
  return false;
}

void CourseComputer::CourseComputer::updateBWC() {
  if (rmc.fix) {
    bwc.datetime.tm_hour = rmc.datetime.tm_hour;
    bwc.datetime.tm_min = rmc.datetime.tm_min;
    bwc.datetime.tm_sec = rmc.datetime.tm_sec;
    bwc.milliseconds = rmc.milliseconds;
    
    bwc.waypoint.set(wpl.waypoint);
    
    strcpy(bwc.waypointId, wpl.name);
    
    Arc course(rmc.position, wpl.waypoint);

    bwc.bearingTrue = course.bearing();
    
    if (rmc.variation >= 0)
      bwc.bearingMagnetic = fmod(bwc.bearingTrue + rmc.variation + 360.0, 360.0);
    
    bwc.distance = course.length() * METERS_TO_NAUTICAL_MILES;
  }
}

