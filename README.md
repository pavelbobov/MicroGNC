[![Build Status](https://travis-ci.org/pavelbobov/MicroGNC.svg?branch=master)](https://travis-ci.org/pavelbobov/MicroGNC)

# MicroGNC
Guidance, Navigation and Control Library for Microcontrollers

Autopilot components for model boats, cars, planes, rockets, and everything else that moves, can be steered, and has a mission. This library is intended to be light, easy to use, and extensible. It brings big industry standards and software development practices to small-scale projects.     

* *Guidance* refers to the determination of the path of travel from the vehicle's current location to a designated the target.
* *Navigation* refers to the determination of the vehicle's location and velocity as well as its attitude (orientation).
* *Control* refers to steering controls needed to track guidance commands while maintaining vehicle stability.

Currently Supported Controllers:
* Arduino

The library currently includes:
* Latitude/Longitude point with methods for computing bearing and distance on great-circle.  
* Classes for parsing and construction of some NMEA 0183 and proprietary NMEA sentences. (NMEA 0183 is a proprietary protocol issued by the National Marine Electronics Association for use in boat navigation and control systems.)
* Base class for instruments, such as sensors, sensor filter, controllers, and actuators.  
* Message bus for exchanging messages among instruments. (Conceptually similar to NMEA 'backbone')
* StremTalker class for reading/writing NMEA sentences from/to serial interface.
* CourseComputer class for computing direct course from current location to a waypoint.

 