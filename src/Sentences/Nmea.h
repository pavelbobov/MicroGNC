/*
 * Nmea.h
 * 
 * Parsing and construction of some NMEA 0183 sentences.
 * 
 * The specification of NMEA sentences is based on publicly available sources,
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
 
#ifndef _SENTENCES_H_
#define _SENTENCES_H_

#include <stdlib.h>
#include "Chrono.h"
#include "Geo.h"
#include "Sentence.h"

//NMEA sentences tags
#define NMEA_RMC  "RMC" //Recommended Minimum Navigation Information
#define NMEA_WPL  "WPL" //Waypoint Location
#define NMEA_BWC  "BWC" //Bearing & Distance to Waypoint - Great Circle
#define NMEA_MWV  "MWV" //Wind Speed and Angle
#define NMEA_HDG  "HDG" //Heading - Deviation & Variation
//Proprietary tags
#define NMEA_PWM  "PWM" //Remote Control Pulse Width Modulation

//Code strings
#define NORTH "N"
#define SOUTH "S"
#define EAST  "E"
#define WEST  "W"

#define NMEA_MAX_LENGTH  83
#define NMEA_MAX_WAYPOINT_NAME_LENGTH 10

/*
 * RMC - Recommended Minimum Navigation Information
 *
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
class RMCSentence : public Sentence {   
public:
  tm       datetime;
  int      milliseconds;
  bool     fix;
  Point    position;
  float    speed;
  float    course;
  float    variation;
  
  RMCSentence();
  ~RMCSentence();

  char*  get(char buffer[], size_t buflen) const;
  bool   set(const char str[]);
};

/*
 * WPL - Waypoint Location
 *
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
class WPLSentence : public Sentence {
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
 *
 *        1         2       3 4        5 6   7 8   9 10 11 12  13 14
 *        |         |       | |        | |   | |   | |   | |    |  |
 * $--BWC,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x.x,T,x.x,M,x.x,N,c--c,m*hh<CR><LF>
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
class BWCSentence : public Sentence {   
public:
  tm       datetime;
  int      milliseconds;
  Point    waypoint;
  float    bearingTrue;
  float    bearingMagnetic;
  float    distance;
  char     waypointId[NMEA_MAX_WAYPOINT_NAME_LENGTH + 1];
  
  BWCSentence();
  ~BWCSentence();

  char*  get(char buffer[], size_t buflen) const;
  bool   set(const char str[]);
};

/*
 * MWV - Wind Speed and Angle
          1   2 3   4 5 6
          |   | |   | | |
 * $--MWV,x.x,a,x.x,a,a*hh<CR><LF>
 * Field Number:
 * 1. Wind Angle, 0 to 360 degrees
 * 2. Reference, R = Relative, T = True
 * 3. Wind Speed
 * 4. Wind Speed Units, K/M/N
 * 5. Status, A = Data Valid
 * 6. Checksum
 *
 * Example: $WIMWV,214.8,R,0.1,K,A*28
 */
class MWVSentence : public Sentence {
public:
  float windAngle;
  char  reference;
  float windSpeed;
  char  windSpeedUnits;

  MWVSentence();
  ~MWVSentence();

  char*  get(char buffer[], size_t buflen) const;
  bool   set(const char str[]);
};

/*
 * HDG - Heading - Deviation & Variation
 *        1   2   3 4   5 6
 *        |   |   | |   | |
 * $--HDG,x.x,x.x,a,x.x,a*hh<CR><LF>
 *
 * Field Number:
 * 1. Magnetic Sensor heading in degrees
 * 2. Magnetic Deviation, degrees
 * 3. Magnetic Deviation direction, E = Easterly, W = Westerly
 * 4. Magnetic Variation degrees
 * 5. Magnetic Variation direction, E = Easterly, W = Westerly
 * 6. Checksum
 *
 * Example: $HCHDG,98.3,0.0,E,12.6,W*57
 */
class HDGSentence : public Sentence {
public:
  float magneticHeading;
  float magneticDeviation;
  float magneticVariation;

  HDGSentence();
  ~HDGSentence();

  char*  get(char buffer[], size_t buflen) const;
  bool   set(const char str[]);
};

/*
 * PWM - Remote Control Pulse Width Modulation
 *        1  3
 *        |  |
 * $RCPWM,x,xxxx*hh<CR><LF>
 * Field Number:
 * 1. Channel number
 * 2. Channel value
 * 3. Checksum
 *
 * Example: $RCPWM,1,1500*6E
 */
class PWMSentence : public Sentence {
public:
  int  channel;
  int  value;

  PWMSentence();
  ~PWMSentence();

  char*  get(char buffer[], size_t buflen) const;
  bool   set(const char str[]);
};

/*
 * Parses latitude/longitude string to Point
 *
 * latitide,<N|S>,longitude,<E|W>
 */
const char* parsePoint(const char str[], Point& point);
char* pointToString(const Point& point, char str[]);

/*
 * Where a numeric latitude or longitude is given, the two digits immediately to the left
 * of the decimal point are whole minutes, to the right are decimals of minutes,
 * and the remaining digits to the left of the whole minutes are whole degrees.
 * Eg. 4533.35 is 45 degrees and 33.35 minutes. ".35" of a minute is exactly 21 seconds.
 * Eg. 16708.033 is 167 degrees and 8.033 minutes. ".033" of a minute is about 2 seconds.
 */
char* decimalDegreesToString(float dd, char str[], size_t len);
float stringToDecimalDegrees(const char str[]);

#endif /* _SENTENCES_H_ */

