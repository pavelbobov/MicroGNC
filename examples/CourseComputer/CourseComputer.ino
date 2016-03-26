/*
 * CourseComputer.ino
 * 
 * Example Arduino sketch for MicroGNC library
 *
 * Exchanges NMEA sentences among GPS, Serial, and CourseComputer. 
 * Send $GPWPL sentence with waypoint location to the serial. 
 * CourseComputer will compute direct course to the waypoint 
 * from the current location obtained from GPS, and will send it to 
 * the Serial as $GPBWC sentence.  
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
 
#include <SoftwareSerial.h>
#include "MicroGNC.h"

#define GPS_RX           4
#define GPS_TX           5

//Output
#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
#define PMTK_SET_NMEA_OUTPUT_RMCGGA  "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
#define PMTK_SET_NMEA_OUTPUT_OFF     "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

//Update rates
#define PMTK_SET_NMEA_UPDATE_1HZ     "$PMTK220,1000*1F"
#define PMTK_SET_NMEA_UPDATE_5HZ     "$PMTK220,200*2C"
#define PMTK_SET_NMEA_UPDATE_10HZ    "$PMTK220,100*2F"

#define GPRMC_SAMPLE                 "$GPRMC,052420.600,A,3404.3640,N,11708.7536,W,0.38,341.13,120316,,,D*7C"
#define GPWPL_SAMPLE                 "$GPWPL,3404.3640,N,11708.7529,W,TEST*4B"
#define GPBWC_SAMPLE                 "$GPBWC,220516,5130.02,N,00046.34,W,213.8,T,218.0,M,0004.6,N,EGLM*21"
//#define GPBWC_SAMPLE                 "$GPBWC,081837,,,,,,T,,M,,N,*13"


SoftwareSerial gpsSerial(GPS_TX, GPS_RX);
StreamTalker gps("GP", &gpsSerial);
StreamTalker serial("UP", &Serial);
CourseComputer courseComputer;

MessageBus bus;

char buffer[NMEA_MAX_LENGTH];

RMCSentence rmc;
WPLSentence wpl;
BWCSentence bwc;


void setup() 
{
  //start serial connection
  Serial.begin(9600);
  Serial.setTimeout(100);

  gpsSerial.setTimeout(100);
  gpsSerial.begin(9600);

  gps.putMessage(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  gps.putMessage(PMTK_SET_NMEA_UPDATE_5HZ);
  
  gps.addFilter("$PMTK");

  courseComputer.putMessage(GPWPL_SAMPLE);

  bus.subscribe(&gps);
  bus.subscribe(&courseComputer);
  bus.subscribe(&serial);
}

void loop() 
{
  unsigned long t = millis();
  
  bus.exchange();

  unsigned long elapsed = millis() - t;
  Serial.print("t = "); Serial.println(elapsed);
}

