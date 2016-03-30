/*
 * Geo.h
 * 
 * Geometric primitives and utilities.
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
 
#ifndef _GEO_H_
#define _GEO_H_

#define METERS_TO_NAUTICAL_MILES 0.000539957

/*
 * Latitude/Longitude point.
 */
class Point {
public:
  //Geographic coordinates in decimal degrees
  float latitude; 
  float longitude;

  Point();
  Point(float lat, float lon);

  void set(float lat, float lon);
  void set(const Point& point);
  
  //Computes great-circle bearing from this to the specified point
  float bearing(const Point& point) const;
  
  //Computes great-circle distance from this to the specified point in meters (accuracy %0.5)
  float distance(const Point& point) const;
};

#endif /* _GEO_H_ */
