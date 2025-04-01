#include "shared.h"

void * journey(void * arg) {
  char* fileName=(char*)arg; //I NEED TO PROVIDE THE SUPPORT FUNCTION WITH THE FILE NAME GIVEN BY COMMAND LINE
  itinerary collection[NUM_TRAINS];//THIS IS WHERE DATA FROM FILE WILL BE STORED
  readFile(collection,fileName); //USING THE SUPPORT FUNCTION TO FILL MY ITINERARY ARRAY

  //START COMMUNICATION PROCESS, has been explained in detail in the pdf
  while (served < NUM_TRAINS) {
    pthread_mutex_lock( & comm_mutex);
    shared = collection[served];
    ready = true;
    served++;
    pthread_cond_signal( & trainQueue);
    while (ready == true) {
	  pthread_cond_wait( & journeyQueue, & comm_mutex);
    }
    pthread_mutex_unlock( & comm_mutex);
  }
}
