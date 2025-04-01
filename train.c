#include "shared.h"

void * train(void * arg) {
  int * segmentStatus = (int * ) arg; //COPYING THE SEGMENT ADRESSES IN A LOCAL VARIABLE FOR CLEARER HANDELING
  itinerary local_itinerary; //CREATING VARIABLE TO STORE ITINERARY WE'LL RECIVE BY JOURNEY
  
  //START COMMUNICATION PROCESS, has been explained in detail in the pdf
  pthread_mutex_lock( & comm_mutex);
    while(ready==false){
    	pthread_cond_wait( & trainQueue, & comm_mutex);
	}
  local_itinerary = shared;
  ready = false;
  pthread_cond_signal( & journeyQueue);
  pthread_mutex_unlock( & comm_mutex);
  //END COMMUNICATION
  moveTrain(local_itinerary, segmentStatus); //LET'S USE A DIFFERENT FUNCTION FOR THE REST OF THE TRAIN LIFESPAN
  pthread_exit(NULL);
}


//SINCE ACCORDING TO THE RULES STATIONS CAN HOUSE AN UNLIMITED AMOUNT OF TRAINS, THERE'S NO NEED TO ASK CONFIRMATION

void moveTrain(itinerary local, int * segmentStatus) {
  int collisionInd=0;
  int counter = 0;
  int current;
  int next;
  char fileName[20];
  char* tmpTime;
  
  //CREATING LOG FILE
  sprintf(fileName, "T%i.txt", local.trainID);
  FILE * log = fopen(fileName, "w+");
  if (log==NULL){
  	{fprintf(stderr,"failed to open trainID:%i file\n",local.trainID); exit(1);}
  }
  //EARLY EXIT FOR UNVALID PATHS
  if (local.numStops <= 2) {
	tmpTime=getTime();
	fprintf(log, "This train dosen't have an assigned path or it's below 2 stops long (station-to-station not possible) %s",tmpTime);
    fclose(log);
    free(tmpTime);
    pthread_exit(NULL);
  }
  
  
  //THIS CYCLE CONTAINS THE "HANDLERS" FOR THE 4 POSSIBLE BEHAVORS 
  //IT WILL CONTINUE UNTIL IT REACES ITS LAST ELEMENT, IN WHICH HANDLER WE HAVE A break;
  
  while (1) { 
	if (counter==0){//THIS IS THE FIRST CASE, WE'RE LEAVING FROM THE STARTING STATION, AND ONLY HAVE TO CHECK THE NEXT SEGMENT
	current = local.path[counter]-1;
    next = local.path[counter+1]-1;
    pthread_mutex_lock( & segmentMutex[next]);
    if (segmentStatus[next] == 0) {
		segmentStatus[next] = 1;
		pthread_mutex_unlock( & segmentMutex[next]);
		tmpTime=getTime();
        fprintf(log, "[Current: S%i] [Next: MA%i], %s\n", current+1,next+1,tmpTime);
        free(tmpTime);
        counter++;
		current = local.path[counter]-1;
        next = local.path[counter+1]-1;
        sleep(2); 
		continue;
    } else { pthread_mutex_unlock( & segmentMutex[next]);
    
    sleep(2);
    continue;
	}}
	
	//THIS IS THE SECOND CASE, WE'RE MOVING IN-BETWEEN SEGMENTS, SO WE NEED TO LOCK THE NEXT AND UNLOCK THE CURRENT
	if(local.numStops-counter>2){ 
	  pthread_mutex_lock( & segmentMutex[next]);
	  if (segmentStatus[next] == 0) {
		segmentStatus[next] = 1;
	    pthread_mutex_unlock( & segmentMutex[next]);
		//UNLOCKING PREVIOUS
		pthread_mutex_lock( & segmentMutex[current]);
		segmentStatus[current]=0;
		pthread_mutex_unlock(& segmentMutex[current]);
		//DONE UNLOCKING
		
		tmpTime=getTime();
        fprintf(log, "[Current: MA%i] [Next: MA%i], %s\n", current+1,next+1,tmpTime);
        free(tmpTime);
        counter++;
		current = local.path[counter]-1;
        next = local.path[counter+1]-1;
        sleep(2); 
		continue;
    } else { pthread_mutex_unlock( & segmentMutex[next]);
	 collisionInd++;
    if (collisionInd>10) {
    	collision=true;
    	fprintf(log, "Current [MA%i], collision on [MA%i]\n",current+1,next+1);
    	break;
	}
	sleep(2); continue;}}
	
	//THIS IS THE THIRD CASE, NEXT STOP IS THE END STATION, WE ONLY HAVE TO UNLOCK THE CURRENTLY HELD SEGMENT
    if(local.numStops-counter==2){
    	//non ho bisogno di controllare prox frame
    	pthread_mutex_lock( & segmentMutex[current]);
		segmentStatus[current]=0;
		pthread_mutex_unlock(& segmentMutex[current]);
		
		tmpTime=getTime();
        fprintf(log, "[Current: MA%i] [Next: S%i], %s\n", current+1,next+1,tmpTime);
        free(tmpTime);
        counter++;
		current = local.path[counter]-1;
        next = local.path[counter+1]-1;
        sleep(2); 
		continue;
	}
	//WE ARE ARRIVING TO THE FINAL STATION, TIME TO BREAK THE CYCLE
	if(local.numStops-counter==1){
	    
	    tmpTime=getTime();
        fprintf(log, "[Current: S%i] [Next: --], %s\n", current+1, tmpTime);
        free(tmpTime);
	    break;
	}}
	//CLOSE THE LOG
	if (fclose(log)!=0){
    {fprintf(stderr,"failed to close trainID:%i file\n",local.trainID); exit(1);}
  }
}
