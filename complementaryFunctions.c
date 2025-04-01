//TWO FUNCTIONS USED TO STANDARDIZE THE PROCESS
#include <time.h>
#include "shared.h"

//THE LIBRARY USED TO GET THE DATE ONLY PROVIDES THE MONTH A NUMERIC CODE, WE USE THIS TO TRANSLATE IT TO STRING
const char * monthNames[] = {
  "January",
  "February",
  "March",
  "April",
  "May",
  "June",
  "July",
  "August",
  "September",
  "October",
  "November",
  "December"
};

//THIS FUNCTION WILL ADD A TIME STAMP TO EACH LOG
//A HEAP VARIABLE HAS BEEN USED TO GRANT DATA CONSTICENCY, ITS TIME-TO-LIVE IS LOW ENOUGH TO MAKE LOCK'S OVERHEAD NOT WORTH THE USE
char* getTime() {
  char* datetime=(char*)malloc(100*sizeof(char));
  time_t now;
  now = time(NULL);
  struct tm * local = localtime( & now);
  
  sprintf(datetime,"%s %02d, %04d, %02d:%02d:%02d",
    monthNames[local -> tm_mon],
    local -> tm_mday,
    local -> tm_year + 1900,
    local -> tm_hour,
    local -> tm_min,
    local -> tm_sec
  );
  return datetime;
}


//THE IDEA IS TO USE CSV FILE TO FEED THE INFORMATION
//THE ONLY 2 MANDATORY FIELDS ARE TRAIN ID AND SEGMENT NUMBER
void * readFile(itinerary collection[],char* fileName) { 
  char buffer[256];
  char * parsed;
  int counter = 0; //FOR LINES
  int segmentCounter=0;
  //OPENING FILE IN READ MODE
  sprintf(fileName, "%s.csv", fileName);
  FILE * map = fopen(fileName, "r");
  if (map == NULL) { 
    {fprintf(stderr,"failed to open %s file\n",fileName); exit(1);}
    exit(-1);}
  //NOW FOR EACH ROW THE FILE IS ABLE TO PROVE US WITH, WE EXTRACT THE INDIVIDUAL ELEMENTS,
  //WE DO THIS BY PUTTING THEM IN A BUFFER WITH STRTOK USING A COMMA AS OUR TOKEN
  while (fgets(buffer, sizeof(buffer), map)) {
  	
    segmentCounter=0;
    parsed = strtok(buffer, ",");
	if (parsed!=NULL){
    	do{
    		collection[counter].path[segmentCounter]=atoi(parsed);
            //printf("%i,",collection[counter].path[segmentCounter]);
	        segmentCounter++;
	        parsed=strtok(NULL,",");
		}while(parsed!=NULL);	
	}
    collection[counter].trainID=counter+1;
    collection[counter].numStops=segmentCounter;
    //printf("%trainID:%i MemeberNum:%i \n",collection[counter].trainID,segmentCounter);
	
	counter++; 
  }
  if(fclose(map)!=0)
  {fprintf(stderr,"failed to close %s file\n",fileName); exit(1);}
}

