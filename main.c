#include "shared.h"

//SHARED MUTEX AND CONDITIONS FOR CONCURRENT ACCESS
pthread_mutex_t segmentMutex[NUM_SEGMENTS];
pthread_mutex_t comm_mutex;
pthread_cond_t journeyQueue;
pthread_cond_t trainQueue;

//SHARED VARIABLES FOR COMMUNICATION
itinerary shared;
bool ready;
int served;
bool collision=false;

int main(int argc, char ** argv) {
  if (argc!=2){printf("MISSING MAP NAME\n"); exit(1);}
  pthread_t controllerTh, journeyTh;//I NEED TWO POINTERS FOR CONTROLLER AND JOURNEY THR.

  //INITIALIZING LOCKS, CONDITIONS AND VARIABLES
  for (int i = 0; i < NUM_SEGMENTS; i++) {
    pthread_mutex_init( & segmentMutex[i], NULL);
  }
  pthread_mutex_init( & comm_mutex, NULL);
  pthread_cond_init( & journeyQueue, NULL);
  pthread_cond_init( & trainQueue, NULL);
  ready = false;
  served = 0;
   
  //LAUNCHING THE 2 THREADS
  if (pthread_create( & controllerTh, NULL, controller, NULL)!=0) 
    {fprintf(stderr,"failed to launch controller\n"); exit(1);}
  if (pthread_create( & journeyTh, NULL, journey, argv[1])!=0)
    {fprintf(stderr,"failed to launch journey\n"); exit(1);}
  //JOINING THE 2 THREADS
  if (pthread_join(controllerTh, NULL)!=0)                        
    {fprintf(stderr,"failed to join controller\n"); exit(1);}
  if (pthread_join(journeyTh, NULL)!=0)
    {fprintf(stderr,"failed to join journey\n"); exit(1);}
    
  //CLEAN UP ALL THREAD ASSOCIATED TOOLS
  for (int i = 0; i < NUM_SEGMENTS; i++) {      
    pthread_mutex_destroy( & segmentMutex[i]);
  }
  pthread_mutex_destroy( & comm_mutex);
  pthread_cond_destroy( & journeyQueue);
  pthread_cond_destroy( & trainQueue);

  return 0;
}










