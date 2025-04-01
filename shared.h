#ifndef __SHARED_H__
#define __SHARED_H__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>

//SHARED DECLARATIONS 
#define NUM_SEGMENTS 16
#define NUM_TRAINS 5
#define NUM_STATIONS 8

typedef struct itinerary {
  int trainID;
  int numStops;
  int path[NUM_SEGMENTS];
}itinerary;

extern pthread_mutex_t segmentMutex[NUM_SEGMENTS];
extern pthread_mutex_t comm_mutex;
extern pthread_cond_t journeyQueue;
extern pthread_cond_t trainQueue;


//SHARED VARIABLES FOR COMMUNICATION
extern itinerary shared;
extern bool ready;
extern int served;

extern bool collision;

//FUNCTION DECLARATIONS, IMPLEMENTATION IN RISPECTIVE FILES
void * readFile(itinerary[],char*);
void * controller(void * arg);
void * journey(void * arg);
void * train(void * arg);
void moveTrain(itinerary local, int * segments);
char* getTime();

#endif
