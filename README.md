# Train Journey Manager

A multithreaded train journey simulation system developed by David Brugnara for Operating Systems (A.A. 2023/2024).

## Overview

This project simulates a train system where multiple trains navigate through a railway network consisting of segments and stations. The system utilizes POSIX threads to manage concurrent train movements while preventing collisions and ensuring proper synchronization.

Key features:
- Multithreaded design with POSIX threads
- Configurable railway maps via CSV files
- Thread synchronization using mutexes and condition variables
- Collision detection and reporting
- Detailed train journey logging

## Compilation

To compile the project, navigate to the TrainJourneyManager directory and run:

```bash
gcc -o TrainManager main.c controller.c journey.c train.c complementaryFunctions.c shared.h -pthread
```

## Execution

To execute the program, run:

```bash
./TrainManager MAP-CODE
```

Where MAP-CODE is the name of the map file (without the .csv extension). Currently, only MAP1 and MAP2 are implemented.

## Project Structure

The project is divided into 6 files:

1. **main.c**: Main program body that initializes shared variables, synchronization tools, and launches controller and journey processes.

2. **shared.h**: Header file that removes clutter from main, imports libraries, and groups functions and variables used across the program.

3. **controller.c**: Initializes track segments, launches train processes, and waits for them to finish.

4. **train.c**: Contains the main body of the train thread and functions to move trains between segments while checking segment availability.

5. **journey.c**: Delivers itineraries to each train by reading the map file and communicating with trains.

6. **complementaryFunctions.c**: Contains utility functions for reading map files and generating timestamps for logs.

## System Design

### Synchronization Mechanisms

#### Journey-Train Communication
Trains communicate with the Journey process to receive their itineraries through:
- A shared itinerary variable
- A communication mutex (`comm_mutex`)
- Two condition variables for queuing (`journeyQueue` and `trainQueue`)
- A ready flag to check conditions
- A progress counter (`served`)

#### Segment Availability
Each track segment has a binary status (0/1) and is protected by its own mutex:
- An array of locks (`segmentMutex`) for individual segment protection
- Each train must acquire the lock for the next segment before releasing its current segment
- This prevents deadlocks and ensures data consistency

### Train Movement Logic

Trains follow a simple but effective state machine:
1. **Station Departure**: Check if the next segment is available
2. **Segment Movement**: Check if the next segment is available, then release the current segment
3. **Station Arrival**: Release the current segment
4. **Final Destination**: End journey

### Collision Handling

The system can encounter three types of potential collisions:
1. **Type 1**: Two trains heading in the same direction - resolved by the system
2. **Type 2**: Path intersections - avoided through proper map planning
3. **Type 3**: Fatal collision where trains block each other - detected and reported

A collision detection mechanism terminates trains after 10 unsuccessful attempts to acquire a segment.

## Map System

Maps are specified in CSV files, where each line represents a train's journey through segments and stations. The system supports:
- Multiple trains (default: 5)
- Multiple segments (default: 16)
- Multiple stations (default: 8)

## Logs

Each train generates a log file (T*.txt) containing:
- Timestamps of movements
- Current and next positions
- Collision information if applicable

## Limitations and Potential Improvements

As described in the report, the current system has limitations:
- No priority ranking for trains
- No ability to request multiple segments at once
- No "safe zones" for trains to wait without occupying segments
- No ability for trains to move backward
- No segment reservation system

These limitations are deliberate design choices to adhere to the project specifications.

## Testing Environments

The project has been tested on:
- Windows 11 (AMD-based, 16 cores, 32GB RAM)
- Ubuntu 22.04.3 in VirtualBox (8 virtual cores, 12GB RAM)
