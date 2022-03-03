#include<stdio.h>

#define SIZE 4

int full=0;
int arr[101],n;
int frame[SIZE];
int reference[SIZE];
int counter[SIZE]={0};
static int time=0;
int rep;
int repsc = 0;
int count=0;

int replaced[4] = {-1};



void display(){

	printf("The pages in the current frame are:\n");
	for(int i=0;i<full;i++)
		printf("%d ",frame[i]);
	printf("\n");
	printf("The last 4 replaced pages are:\n");
	for(int i=0;i<4;i++){
		if(replaced[i]==-1)
			break;
		printf("%d ",replaced[i]);
		}
	printf("\n");
    
}

void replace(int page, int victim){
	for(int i=0;i<4;i++){
		if(replaced[i]==page)
			{
				replaced[i] = victim;
				return;
			}	
	}

	int prev = victim;
	for(int i=0;i<4;i++){
		int temp = replaced[i];
		replaced[i] = prev;
		prev = temp;
	}
	return;
}


//finding the least recently used page using their corresponding counter values
int oldestPage(){

	int i,min;
    min=0;

    for(i=0;i<SIZE;i++){
    	if(counter[min]>counter[i]){
    		  min=i; 
    	}
    }
   
    rep=min;

	return rep;

}


int LRU(int page){

 	int victim;
 	rep=oldestPage();
 	victim=frame[rep];
 	frame[rep]=page;
 	counter[rep]=time;
 
	return victim;

}


void checkfaultLRU(int page){

	if(full!=SIZE){
		counter[full]=time;
        frame[full++]=page;
	}
	else{
		int victim = LRU(page);
		printf("!!!! Page Fault !!!!  The page replaced is %d.\n", victim);
		replace(page, victim);
	}
}



int fifo(int page){
	int victim;
                    
 	victim=frame[rep];
 	frame[rep]=page;
 	rep++;

 	if(rep==SIZE){
 		rep=0;
 	}

	return victim;
}


void checkfaultFIFO(int page){
	if(full!=SIZE){
        frame[full++]=page;
	}
	else{
		int victim = fifo(page);
		printf("!!!! Page Fault !!!!  The page replaced is %d.\n", victim);
		replace(page, victim);
	}
}


int secondChance(int page)
{
 	int victim;
 	while(reference[repsc]!=0){
 		reference[repsc++]=0;
 		if(repsc==SIZE){
 			repsc=0;
 		}
	}                     

	victim=frame[repsc];
 	frame[repsc]=page;
 	reference[repsc]=1;
	return victim;   

}

void checkfaultSC(int page){

	if(full!=SIZE){
		reference[full]=1;
        frame[full++]=page;
	}
	else{
		int victim = secondChance(page);
		printf("!!!! Page Fault !!!!  The page replaced is %d.\n", victim);
		replace(page, victim);
	}
}



int checkPagePresent(int page){

	int i;
    int flag=0;
    
    if(full!=0){

    	for(i=0;i<full;i++){
    		if(page==frame[i]){
    		  	flag=1;
    		  	counter[i]=time; //for LRU
    		  	reference[i] = 1; //for second chance
    			break;
			}
		}
	}

 	return flag;

}


int main(){

	int i;
    FILE *fp;

    fp=fopen("pages.txt","r");
    printf("\nThe number of elements in the reference string are :");
    fscanf(fp,"%d",&n);
    printf("%d",n);
    
    for(i=0;i<n;i++)
    	fscanf(fp,"%d",&arr[i]);

    fclose(fp);

    printf("\nThe elements present in the string are\n");
    
    for(i=0;i<n;i++)
    	printf("%d  ",arr[i]);
    printf("\n\n");
    
    printf("Choose a page replacement policy:\n");
    printf("Enter 1 for FIFO\nEnter 2 for LRU \nEnter 3 for secondChance\n");
    int choice;
    scanf("%d", &choice);

    printf("\n!!!!!!!!!!!!!!!You chose %s!!!!!!!!!!!!!!!!!!\n\n", choice==1? "FIFO" : choice==2 ? "LRU" : "Second Chance");
    for(i=0;i<n;i++){

        printf("page loaded: %d\n", arr[i]);
    	time++;
        
        if(checkPagePresent(arr[i])!=1){
        	
        	if(choice==1)
        		checkfaultFIFO(arr[i]);
        	if(choice==2)
        		checkfaultLRU(arr[i]);
        	if(choice==3)
        		checkfaultSC(arr[i]);

            count++; 

        }
        
        display();
        printf("\n");
                
    }

    printf("\nThe number of page faults are %d\n",count);
	return 0;

}
    