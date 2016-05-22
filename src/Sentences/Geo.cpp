/*
 * Geo.cpp
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
#include <math.h>
#include "Geo.h"

#ifndef M_PI
#define M_PI 3.14159265359
#endif

const float earthR = 6371009;
const float degToRad = 0.01745329251994;//M_PI / 180;
const float radToDeg = 57.29577951308233;//180/M_PI

float sqr(float v) {
  return v * v;
}

float haversine(float f) {
  //haversine(φ) = sin²(φ/2) = (1 - cos(φ)) / 2
  return sqr(sin(f * 0.5));
}

float angle(const Point& s, const Point& e) {
  //Haversine formula from http://www.movable-type.co.uk/scripts/latlong.html
  //a = sin²(Δφ/2) + cos φ1 ⋅ cos φ2 ⋅ sin²(Δλ/2)
  //c = 2 ⋅ atan2( √a, √(1−a) )
  float f1 = s.latitude * degToRad;
  float f2 = e.latitude * degToRad;
  float dl = (e.longitude - s.longitude) * degToRad;
  float df = f2 - f1;
  float a = haversine(df) + cos(f1) * cos(f2) * haversine(dl);
  return 2 * atan2(sqrt(a), sqrt(1.0 - a));
}

/********************************* Point **************************************/

Point::Point() : latitude(0), longitude(0) {
}

Point::Point(float lat, float lon) : latitude(lat), longitude(lon) {
}

void Point::set(float lat, float lon) {
  this->latitude = lat;
  this->longitude = lon;
}

void Point::set(const Point& point) {
  set(point.latitude, point.longitude);
}

/********************************** Arc ***************************************/

Arc::Arc() {
}

Arc::Arc(const Point& s, const Point& e) : start(s), end(e) {
}

void Arc::set(const Point& s, const Point& e) {
  start.set(s);
  end.set(e);
}

//Great-circle initial bearing in degrees from start to end point
float Arc::bearing() const {
  /*
  http://www.movable-type.co.uk/scripts/latlong.html
  var φ1 = lat1.toRadians();
  var φ2 = lat2.toRadians();
  var Δφ = (lat2-lat1).toRadians();
  var Δλ = (lon2-lon1).toRadians();
  θ = atan2( sin Δλ ⋅ cos φ2 , cos φ1 ⋅ sin φ2 − sin φ1 ⋅ cos φ2 ⋅ cos Δλ )
  */
  float f1 = start.latitude * degToRad;
  float f2 = end.latitude * degToRad;
  float dl = (end.longitude - start.longitude) * degToRad;
  float y = sin(dl) * cos(f2);
  float x = cos(f1) * sin(f2) - sin(f1) * cos(f2) * cos(dl);
  float rad = atan2(y,x);
  return (rad > 0 ? rad : (2 * M_PI + rad)) * radToDeg;
}

//Great-circle distance in meters from start to end point (accuracy %0.5)
float Arc::length() const {
  return angle(start, end) * earthR;
}

//Great-circle distance from the given point to the arc, a.k.a. cross-track error (XTE)
float Arc::distance(const Point& p) const {
  /*
  Formula from http://www.movable-type.co.uk/scripts/latlong.html
  xte = asin( sin(δ13) ⋅ sin(θ13−θ12) ) ⋅ R
  where δ13 is (angular) distance from start point to third point
  θ13 is (initial) bearing from start point to third point
  θ12 is (initial) bearing from start point to end point
  R is the earth’s radius
  */
  Arc arc;
  arc.set(start, p);
  return asin(sin(angle(start, p)) * sin((arc.bearing() - bearing()) * degToRad)) * earthR;
}

