# MicroGNC
Guidance, Navigation and Control Library for Microcontrollers

* *Guidance* refers to the determination of the desired path of travel (the "trajectory") from the vehicle's current location to a designated the target.
* *Navigation* refers to the determination of the vehicle's location and velocity as well as its attitude (orientation).
* *Control* refers to steering controls, thrusters, etc., needed to track guidance commands while maintaining vehicle stability.

The library currently includes:
* Latitude/Longitude point with methods for computing bearing and distance on great-circle.  
* Classes for parsing and construction of some NMEA 0183 and proprietary NMEA sentences. (NMEA 0183 is a proprietary protocol issued by the National Marine Electronics Association for use in boat navigation and control systems.)
* Base class for instruments, such as sensors, sensor filter, controllers, and actualtors.  
* Messase bus for exchanging messages among instruments. (Conceptually similar to NMEA 'backbone')
* StremTalker class for reading/writing NMEA sentences from/to serial interface.
* CourseComputer class for computing direct course from current location to a waypoint.

 