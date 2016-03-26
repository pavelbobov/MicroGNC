/*
 * CourseComputer.h
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

#ifndef COURSECOMPUTER_H_
#define COURSECOMPUTER_H_

#include "MessageBus.h"
#include "Nmea/Nmea.h"

/*
 * Computes direct course from current location to a waypoint. 
 * 
 * Input:
 * Location specified by RMC sentence
 * Waypoint specified by WPL sentence
 * 
 * Output:
 * direct course as BWC 
 */
class CourseComputer : public Instrument {
	RMCSentence rmc;
	WPLSentence wpl;
	BWCSentence bwc;
  
public:
  /*
   * Constructor
   */
	CourseComputer();

  /* 
   *  Destructor
   */
	~CourseComputer();

  /*
   * Gets BWC sentence with direct course from current location to a waypoint.
   * 
   * @param message message buffer 
   * @param buflen buffer size
   * @return BWC sentence if the operation was successful and NULL otherwise
   */
  char* getMessage(char message[], size_t buflen);

  /*
   * Accepts sentences:
   * RMC - Recommended Minimum Navigation Information
   * WPL - Waypoint Location
   */
  bool  putMessage(const char message[]);

private:
  void  updateBWC();
};

#endif /* COURSECOMPUTER_H_ */
