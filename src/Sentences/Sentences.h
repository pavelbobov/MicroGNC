/*
 * Sentences.h
 * 
 * Parsing and construction of sentences.
 * 
 * Format of some sentences is similar to NMEA sentences described on
 * publicly available sources, such as http://www.catb.org/gpsd/NMEA.html.
 * An exact match to NMEA specifications is not guaranteed.
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

//Sentences tags
#define TAG_RMC   "RMC" //Recommended Minimum Navigation Information
#define TAG_WPL   "WPL" //Waypoint Location
#define TAG_BWC   "BWC" //Bearing & Distance to Waypoint - Great Circle
#define TAG_MWV   "MWV" //Wind Speed and Angle
#define TAG_HDG   "HDG" //Heading - Deviation & Variation
#define TAG_MAG   "MAG" //Raw 3-axis magnetometer data
#define TAG_PWM   "PWM" //Remote Control Pulse Width Modulation
#define TAG_MCS   "MCS" //Motor Current Sensing
#define TAG_RSA   "RSA" //Rudder Sensor Angle

//Talker IDs
#define TALKER_WI "WI"  //Weather Instrument
#define TALKER_GP "GP"  //GPS
#define TALKER_RC "RC"  //Radio Control
#define TALKER_HC "HC"  //Heading - Magnetic Compass
#define TALKER_IN "IN"  //Integrated Navigation
#define TALKER_ER "ER"  //Engine Room

//Code strings
#define NORTH     "N"
#define SOUTH     "S"
#define EAST      "E"
#define WEST      "W"

#define MAX_SENTENCE_LENGTH  83
#define MAX_WAYPOINT_NAME_LENGTH 10

/*
 * Base class for sentences
 *
 */
class Sentence {
protected:
  /*
   * Constructor
   *
   * @param talker talker ID
   * @param tag tag
   */
  Sentence(const char talker[], const char tag[]);

public:
  /*
   * Destructor
   */
  virtual ~Sentence();

  /*
   * Implementations of this method must copy sentence to the specified buffer
   *
   * @param buffer buffer
   * @param buflen buffer size.
   * @return sentence or NULL if the buffer size is insufficient.
   */
  virtual char* get(char buffer[], size_t buflen) const = 0;

  /*
   * Implementations of this method must change the sentence to the specified string
   * if the string is a valid sentence and the sentence tag matches the tag of the string.
   *
   * @param str sentence
   * @return true if the sentence was changed
   */
  virtual bool  set(const char str[]) = 0;

protected:
  /*
   * Talker ID - a two-character prefix that identifies
   * the type of the transmitting unit.
   */
  const char* talker;

  /*
   * Sentence tag - a three character string that
   * identifies the type of sentence.
   */
  const char* tag;

  /*
   * Returns true if tag of the sentence matches tag of the member variable
   */
  bool  matches(const char str[]);

  /*
   * Adds $<talker><tag> to the end of the string
   */
  char* addHead(char str[]) const;

  /*
   * returns true if checksum at the end of the sentence is valid
   */
  static bool  valid(const char str[]);

  /*
   * Calculates sentence checksum and adds it to the end of the string
   */
  static char* addChecksum(char str[]);
};

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
  char   name[MAX_WAYPOINT_NAME_LENGTH + 1];

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
  char     waypointId[MAX_WAYPOINT_NAME_LENGTH + 1];
  
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
 * MAG - Raw 3-axis magnetometer data
 *          1      2    3   4
 *          |      |    |   |
 * $HCMAG,x.xxx,x.xxx,x.xxx*hh<CR><LF>
 *
 * Field Number:
 * 1. x-value
 * 2. y-value
 * 3. z-value
 * 4. Checksum
 *
 * Example: $HCMAG,0.123,12.345,0.234*76
 */
class MAGSentence : public Sentence {
public:
  float mx;
  float my;
  float mz;

  MAGSentence();
  ~MAGSentence();

  char*  get(char buffer[], size_t buflen) const;
  bool   set(const char str[]);
};

/*
 * PWM - Remote Control Pulse Width Modulation
 *        1  3
 *        |  |
 * $RCPWM,x,xxxx*hh<CR><LF>
 *
 * Field Number:
 * 1. Channel number
 * 2. Channel PWM value
 * 3. Checksum
 *
 * Example: $RCPWM,1,1500*6E
 */
class PWMSentence : public Sentence {
public:
  int  channel;
  int  pulse;

  PWMSentence();
  ~PWMSentence();

  char*  get(char buffer[], size_t buflen) const;
  bool   set(const char str[]);
};

/*
 * MSC - Motor Current Sensing
 *
 * $ERMCS,x,x.xxx*hh<CR><LF>
 *
 * Field Number:
 * 1. Motor number
 * 2. Current in Ampers
 * 3. Checksum
 *
 * Example: $ERMCS,1,0.080*5D
 */
class MCSSentence : public Sentence {
public:
  int motor;
  float current;

  MCSSentence();
  ~MCSSentence();

  char*  get(char buffer[], size_t buflen) const;
  bool   set(const char str[]);
};

/*
 * RSA - Rudder Sensor Angle
 *        1   2 3   4 5
 *        |   | |   | |
 * $--RSA,x.x,A,x.x,A*hh<CR><LF>
 * Field Number:
 * 1. Starboard (or single) rudder sensor, "-" means Turn To Port
 * 2. Status, A means data is valid
 * 3. Port rudder sensor
 * 4. Status, A means data is valid
 * 5. Checksum
 */
class RSASentence : public Sentence {
public:
  float starboardRudderAngle;
  float portRudderAngle;

  RSASentence();
  ~RSASentence();

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

