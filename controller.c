#include "shared.h"

void * controller(void * arg) {
  pthread_t trainsTH[NUM_TRAINS]; //SINCE CONTROLLER LAUNCHES THE TRAINS, IT NEEDS TO MEMORIZE THEIR ID
  
  //DECLARING AND INITIALIZING SEGMENT STATUSES IN THE HEAP FOR EASIER THREAD COMMUNICATION
  int * segmentStatus = (int * ) malloc(NUM_SEGMENTS * sizeof(int));
  for (int i = 0; i < NUM_SEGMENTS; i++) {
    segmentStatus[i] = 0;
  }
  printf("ON BOARD, TRAINS ARE ABOUT TO DEPART\n");
  //LAUNCHING THREADS
  for (int i = 0; i < NUM_TRAINS; i++) {
    if (pthread_create( & trainsTH[i], NULL, train, segmentStatus)!=0)
    {fprintf(stderr,"failed to launch train nr:%i\n",i+1); exit(1);}
  }
  //WAITING FOR THREADS TO BE DONE
  for (int i = 0; i < NUM_TRAINS; i++) {
   if (pthread_join(trainsTH[i], NULL)!=0)
    {fprintf(stderr,"failed to join train nr:%i\n",i+1); exit(1);}
  }
  if (collision==true){
  	printf("A collision HAS OCCURED\n");
  }
  else printf("ALL TRAINS ARRIVED (SAFELY AND ON TIME) TO THEIR RESPECTIVE DESTINATION\n");
  free(segmentStatus); //UNLOCATING WHAT WE MANUALLY LOCATED IN THE HEAP
}
