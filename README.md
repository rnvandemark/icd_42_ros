# icd_42_ros

## Project Description

This project serves as an interface to the 42 spacecraft simulator, such that
it can be dynamically configured to output telemetry onto a ROS network, and so
the simulation can accept commands via ROS utilities (messages, services, and
maybe actions?).

## Current State

The initial system as is can be brought up with the main launch file, which is
configurable via arguments to start 1 or more instances of 42, and for now, a
simple ROS node which will listen over a socket for outbound data (at least
one 42 program must act as a server and output to a socket for this node).
roslaunch brings up each 42 program as a 'node' with no pubs, subs, etc., this
is done for the benefit of being able to contain the entire launched system in
a single roslaunch file, where the types of 42 programs created are specified
as arguments to that launch file.

The package _demo_42_ros_ shows how additional 42 simulations/configurations
can be added without modifying the 42 project or this package.

For now, this ROS node just receives the data over its port, nothing is done
to it yet.

## Todo

- Plan and implement a plugin design pattern to be able to interpret input data
from 42, pack that into the correct message type, and output onto the ROS
network
- Plan and implement the ability to send commands back to 42

## Example

- A version of 42 like the one that is forked on my GitHub account (will
guess Linux and assume Glut is installed when compiling) is required for now
- A catkin workspace with at least this package and my demo_42_ros is required
- Only tested (very basically) on Ubuntu 20.04 and with ROS Noetic thus far
- Example:
```
roslaunch icd_42_ros main.launch sim_names:=RosTx,VizRx port:=10002 delay:=8.0
```
