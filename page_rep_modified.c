/*
Name - Rohit Kumar Sah
UIN - 831000250
Collaborated with: None
*/


#include<stdio.h>
#include <stdlib.h>
#include<time.h>
#include<sys/time.h>
#include <unistd.h>
#include<string.h>

int SIZE;
int n;
int full=0;
static int timer=0;
int rep;
int repLFU=0;
int count= 0;

FILE *fpo;


void display(int *frame){

	printf("The pages in the current frame are:\n");
	for(int i=0;i<full;i++)
		printf("%d ",frame[i]);
	printf("\n");    
}

int leastFrequent(int *counter, int *counterLFU){
	int min;
    min=0;

    // printf("counterLFU: ");

    // for(int i=0;i<SIZE;i++){
    // 	printf("%d ", counterLFU[i]);
    // }
    // printf("\n counter: ");
    // for(int i=0;i<SIZE;i++){
    // 	printf("%d ", counter[i]);
    // }
    // printf("\n");

    for(int i=0;i<SIZE;i++)
        if(counterLFU[min] > counterLFU[i])
    		min=i;

    // if there is a tie between multiple values which are all less frequent pick the oldest	

    int oldest = 0;	
    int oldestValue = 10000; 	
    for(int j=0;j<SIZE;j++){

    	if(counterLFU[min] == counterLFU[j])
    	 if(counter[j] < oldestValue){
    		oldest = j;
    		oldestValue = counter[j];
    	}
    }

    //printf("\n minindex: %d, oldest: %d, minvalue: %d minvalueoldest:%d\n", min, oldest, counterLFU[min], counterLFU[oldest]);
    repLFU = oldest;
	return repLFU;

}

int LFU(int page, int *frame, int *counter, int *counterLFU){

	int victim;
	repLFU = leastFrequent(counter, counterLFU); 
	victim = frame[repLFU];
	frame[repLFU] = page;
	counterLFU[repLFU] = 1;
	counter[repLFU]=timer;
	return victim;   
}

void checkfaultLFU(int page, int *frame, int *counter, int *counterLFU){
	if(full!=SIZE)
	{
		counterLFU[full]++;
		counter[full] = timer;
        frame[full++]=page;
	}
	else{
		int victim = LFU(page, frame, counter, counterLFU);
		printf("!!!! Page Fault !!!!  The page replaced is %d.",victim);
		fprintf(fpo, "!!!! Page Fault !!!!  The page replaced is %d.\n", victim);
	}
}


//finding the least recently used page using their corresponding counter values
int oldestPage(int *counter){

	int min;
    min=0;

    for(int i=0;i<SIZE;i++){
    	if(counter[min]>counter[i]){
    		  min=i; 
    	}
    }
   
    rep=min;

	return rep;

}


int LRU(int page, int *frame, int *counter){

 	int victim;
 	rep=oldestPage(counter);
 	victim=frame[rep];
 	frame[rep]=page;
 	counter[rep]=timer;
 
	return victim;

}


void checkfaultLRU(int page, int *frame, int *counter){

	if(full!=SIZE){
		counter[full]=timer;
        frame[full++]=page;
	}
	else{
		int victim = LRU(page, frame, counter);
		printf("!!!! Page Fault !!!!  The page replaced is %d.\n", victim);
		fprintf(fpo, "!!!! Page Fault !!!!  The page replaced is %d.\n", victim);
	}
}



int fifo(int page, int *frame){
	int victim;
                    
 	victim=frame[rep];
 	frame[rep]=page;
 	rep++;

 	if(rep==SIZE){
 		rep=0;
 	}

	return victim;
}


void checkfaultFIFO(int page, int *frame){
	if(full!=SIZE){
        frame[full++]=page;
	}
	else{
		int victim = fifo(page, frame);
		printf("!!!! Page Fault !!!!  The page replaced is %d.\n", victim);
		fprintf(fpo, "!!!! Page Fault !!!!  The page replaced is %d.\n", victim);
	}
}



int checkPagePresent(int page, int *frame, int *counter, int *counterLFU){

	int i;
    int flag=0;
    
    if(full!=0){
    	for(i=0;i<full;i++){
    		if(page==frame[i]){
    		  	flag=1;
    		  	counter[i]=timer; //for LRU
    		  	counterLFU[i]++;
    			break;
			}
		}
	}
	
	if(full < SIZE){
		printf("!!!Page Fault!!!! No replacement needed.\n");
		fprintf(fpo, "%s", "!!!Page Fault!!!! No replacement needed.\n");
	}
 	return flag;

}


int main(){

	int i;
	time_t t;
	struct timeval te; 
	FILE *fp;
    fp=fopen("sahr_proj2_input.txt","r");
    
	srand((unsigned) time(&t));
	n = 100;
	int arr[100];

	// printf("Generating 1000 random integers between 0 and 15");
    // for(int i=0;i<n;i++){
    //	 	arr[i] = rand() % 16;
    // 		fprintf(fp, "%d ", arr[i]);
    //}
	

    printf("\nThe number of elements in the reference string are 100\n");
  
    
    for(i=0;i<n;i++)
    	fscanf(fp,"%d",&arr[i]);

    fclose(fp);
    

    printf("\nThe elements present in the string are\n");
    
    for(i=0;i<n;i++){
    	printf("%d ",arr[i]);
    }
    printf("\n\n");
    
    printf("Choose a page replacement policy:\n");
    printf("Enter 1 for FIFO\nEnter 2 for LRU \nEnter 3 for LFU\n");
    int choice;
    scanf("%d", &choice);

    printf("\n!!!!!!!!!!!!!!!You chose %s!!!!!!!!!!!!!!!!!!\n\n", choice==1? "FIFO" : choice==2 ? "LRU" : "LFU");

 
    printf("Enter no of pages in frame: 4 or 8\n\n");
    scanf("%d", &SIZE);

    printf("no of pages in frame:%d\n",SIZE);

    int frame[SIZE];
	int counter[SIZE];
	int counterLFU[SIZE];

	if(SIZE==4)
		fpo = fopen("sahr_proj2_output_4 frames.txt", "a");
	if(SIZE==8)
		fpo = fopen("sahr_proj2_output_8 frames.txt", "a");

	fprintf(fpo, "%s", "\nThe number of elements in the reference string are 100\n");
	fprintf(fpo, "%s", "\nThe elements present in the string are\n");
	for(i=0;i<n;i++){
    	fprintf(fpo, "%d ",arr[i]);
    }
    fprintf(fpo, "%s","\n\n");

    fprintf(fpo, "\n!!!!!!!!!!!!!!!You chose %s!!!!!!!!!!!!!!!!!!\n\n", choice == 1 ? "FIFO" : choice==2 ? "LRU" : "LFU");
    fprintf(fpo, "no of pages in frame:%d\n",SIZE);

	for(int i=0;i<SIZE;i++){
		counter[i] = 0;
		counterLFU[i] = 0;
	}
    
    for(i=0;i<n;i++){
    	gettimeofday(&te, NULL); // get current time
    	unsigned long time_in_micros = 1000000 * te.tv_sec + te.tv_usec;

        printf("\nSystem time: %ld, page loaded: %d\n", time_in_micros, arr[i]);
        fprintf(fpo, "\nSystem time: %ld, page loaded: %d\n", time_in_micros, arr[i]);
    	timer++;
        
        if(checkPagePresent(arr[i], frame, counter, counterLFU)!=1){
        	
        	if(choice==1)
        		checkfaultFIFO(arr[i], frame);
        	if(choice==2)
        		checkfaultLRU(arr[i], frame, counter);
        	if(choice==3)
        		checkfaultLFU(arr[i], frame, counter, counterLFU);

            count++; 

        }
        else {

        	printf("No Page Fault\n");
            fprintf(fpo, "%s", "No Page Fault\n");
        }
        
        display(frame);
        printf("\n");
    }

    printf("\nTotal number of page faults = %d\n",count);
    fprintf(fpo,"\nTotal number of page faults = %d\n",count);
    fclose(fpo);
    
	return 0;

}
    