/*
 * Nmea.h
 * 
 * Parsing and construction of some NMEA 0183 and proprietary NMEA sentences .
 * 
 * NMEA 0183 is a proprietary protocol issued by the 
 * National Marine Electronics Association 
 * for use in boat navigation and control systems. 
 * 
 * The implementation is based on publicly available sources, 
 * such as http://www.catb.org/gpsd/NMEA.html. 
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
 
#ifndef _NMEA_H_
#define _NMEA_H_

#include <stdlib.h>
#include "Time.h"
#include "Geo.h"

//NMEA sentence tags
#define NMEA_RMC "RMC"
#define NMEA_WPL "WPL"
#define NMEA_BWC "BWC"

//NMEA code strings
#define NORTH "N"
#define SOUTH "S"
#define EAST  "E"
#define WEST  "W"

#define NMEA_MAX_LENGTH  83
#define NMEA_MAX_WAYPOINT_NAME_LENGTH 6


/*
 * Base class for NMEA sentences
 * 
 * See http://www.catb.org/gpsd/NMEA.html for details.
 */
class NMEASentence {
public:
  /*
   * Constructor
   * 
   * @param talker talker ID
   * @param tag tag
   */

  NMEASentence(const char talker[], const char tag[]);
  /*
   * Destructor
   */
  virtual ~NMEASentence();

  /*
   * Implementations of this method must copy NMEA sentence to the specified buffer
   * 
   * @param buffer buffer
   * @param buflen buffer size. Recommended size is NMEA_MAX_LENGTH.
   * @return NMEA sentence or NULL if the buffer size is insufficient. 
   */
  virtual char* get(char buffer[], size_t buflen) const = 0;

  /*
   * Implementations of this method must change the sentence to the specified string
   * if the string is a valid NMEA sentence and the sentence tag matches the tag of the string.
   * 
   * @param str NMEA string
   * @return true if the sentence was changed
   */
  virtual bool  set(const char str[]) = 0;

protected:
  /*
   * Talker ID - a two-character prefix that identifies 
   * the type of the transmitting unit. 
   * 
   * "P" for proprietaty NMEA tags
   */
  const char* talker;

  /*
   * NMEA sentence tag - a three character string that
   * identifies the type of sentence.
   */
  const char* tag;

  static bool  valid(const char str[]);
  bool         matches(const char str[]);

  /*
   * Calculates NMEA sentence checksum and adds it to the end of the string
   */
  static char* addChecksum(char str[]);

  /*
   * Parses latitude/longitude string to Point
   * 
   * latitide,<N|S>,longitude,<E|W> 
   */
  static const char* parsePoint(const char str[], Point& point);
  static char* pointToString(const Point& point, char str[]);   

  /*
   * Parses hhmmss.ss string to time elements of tmElements_t structure
   */
  static const char* parseTime(const char str[], tmElements_t& datetime, uint16_t& milliseconds);
  static char* timeToString(const tmElements_t& datetime, uint16_t milliseconds, char str[]);       
  
  /*
   * Where a numeric latitude or longitude is given, the two digits immediately to the left 
   * of the decimal point are whole minutes, to the right are decimals of minutes, 
   * and the remaining digits to the left of the whole minutes are whole degrees.
   * Eg. 4533.35 is 45 degrees and 33.35 minutes. ".35" of a minute is exactly 21 seconds.
   * Eg. 16708.033 is 167 degrees and 8.033 minutes. ".033" of a minute is about 2 seconds.
   */
  static char* decimalDegreesToString(float dd, char str[], size_t len);
  static float stringToDecimalDegrees(const char str[]);

};

/*
 * RMC - Recommended Minimum Navigation Information
 * This is one of the sentences commonly emitted by GPS units.
 *                                                           12
 *        1         2 3       4 5        6  7   8   9    10 11|  13
 *        |         | |       | |        |  |   |   |    |  | |   |
 * $--RMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,xxxx,x.x,a,m,*hh<CR><LF>
 * Field Number:
 * 1. UTC Time
 * 2. Status, V=Navigation receiver warning A=Valid
 * 3. Latitude
 * 4. N or S
 * 5. Longitude
 * 6. E or W
 * 7. Speed over ground, knots
 * 8. Track made good, degrees true
 * 9. Date, ddmmyy
 * 10. Magnetic Variation, degrees
 * 11. E or W
 * 12. FAA mode indicator (NMEA 2.3 and later)
 * 13. Checksum
 *
 * A status of V means the GPS has a valid fix that is below an internal quality threshold, 
 * e.g. because the dilution of precision is too high or an elevation mask test failed.
 *
 * Example: $GPRMC,194509.000,A,4042.6142,N,07400.4168,W,2.03,221.11,160412,,,A*77
 */
class RMCSentence : public NMEASentence {   
public:
  tmElements_t  datetime;
  uint16_t      milliseconds  = 0;
  bool          fix = false;
  Point         position;
  float         speed = 0;
  float         course = 0;
  float         variation = 0;
  
  RMCSentence();
  ~RMCSentence();

  char*  get(char buffer[], size_t buflen) const;
  bool   set(const char str[]);

  time_t makeTime();
};

/*
 * WPL - Waypoint Location
 *       1       2 3        4 5    6
 *       |       | |        | |    |
 * $--WPL,llll.ll,a,yyyyy.yy,a,c--c*hh<CR><LF>
 * 
 * Field Number:
 * 1. Latitude
 * 2. N or S (North or South)
 * 3. Longitude
 * 4. E or W (East or West)
 * 5. Waypoint name
 * 6. Checksum
 * 
 * Example: $GPWPL,3404.3640,N,11708.7529,W,TEST*4B
 */
class WPLSentence : public NMEASentence {
public:  
  Point  waypoint;
  char   name[NMEA_MAX_WAYPOINT_NAME_LENGTH + 1];

  WPLSentence();
  ~WPLSentence();

  char*  get(char buffer[], size_t buflen) const;
  bool   set(const char str[]);
};

/*
 * BWC - Bearing & Distance to Waypoint - Great Circle
 *                                                        12
 *        1         2       3 4        5 6   7 8   9 10  11|    13 14
 *        |         |       | |        | |   | |   | |   | |    |   |
 * $--BWC,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x.x,T,x.x,M,x.x,N,c--c,m,*hh<CR><LF>
 * 
 * Field Number:
 * 1. UTC Time
 * 2. Waypoint Latitude
 * 3. N = North, S = South
 * 4. Waypoint Longitude
 * 5. E = East, W = West
 * 6. Bearing, True
 * 7. T = True
 * 8. Bearing, Magnetic
 * 9. M = Magnetic
 * 10. Nautical Miles
 * 11. N = Nautical Miles
 * 12. Waypoint ID
 * 13. FAA mode indicator (NMEA 2.3 and later, optional)
 * 14. Checksum
 * 
 * Example 1: $GPBWC,081837,,,,,,T,,M,,N,*13
 * Example 2: $GPBWC,220516,5130.02,N,00046.34,W,213.8,T,218.0,M,0004.6,N,EGLM*11
 */
class BWCSentence : public NMEASentence {   
public:
  tmElements_t  datetime;
  uint16_t      milliseconds  = 0;
  Point         waypoint;
  float         bearingTrue = -1;
  float         bearingMagnetic = -1;
  float         distance = -1;
  char          waypointId[NMEA_MAX_WAYPOINT_NAME_LENGTH + 1];  
  
  BWCSentence();
  ~BWCSentence();

  char*  get(char buffer[], size_t buflen) const;
  bool   set(const char str[]);
};

#endif /* _NMEA_H_ */ 

