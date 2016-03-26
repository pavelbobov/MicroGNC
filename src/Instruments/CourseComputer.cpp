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

CourseComputer::CourseComputer() : Instrument("IN") {
}

CourseComputer::~CourseComputer() {
}

char* CourseComputer::getMessage(char message[], size_t buflen) {
  return bwc.get(message, buflen);
}

bool CourseComputer::putMessage(const char message[]) {
  if (rmc.set(message) || wpl.set(message)) {
    updateBWC();
    return true;
  }
  
  return false;
}

void CourseComputer::CourseComputer::updateBWC() {
  if (rmc.fix) {
    bwc.datetime.Hour = rmc.datetime.Hour;
    bwc.datetime.Minute = rmc.datetime.Minute;
    bwc.datetime.Second = rmc.datetime.Second;
    bwc.milliseconds = rmc.milliseconds;
    
    bwc.waypoint.set(wpl.waypoint);
    
    strcpy(bwc.waypointId, wpl.name);
    
    bwc.bearingTrue = rmc.position.bearing(wpl.waypoint);
    
    if (rmc.variation >= 0)
      bwc.bearingMagnetic = fmod(bwc.bearingTrue + rmc.variation + 360.0, 360.0);
    
    bwc.distance = rmc.position.distance(wpl.waypoint) * METERS_TO_NAUTICAL_MILES;
  }
}

