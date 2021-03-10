// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993,2021 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "synch.h"
#include <ctype.h>
#include <sys/wait.h>

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------


typedef struct threadPasser{
   int id;
   int num;
};


void
checkInput()
{
   char str[100];
   printf("enter a string:");
   fgets(str, 100, stdin);
   bool isstr = false;
   bool isneg = (str[0] == '-');
   bool isdec = false;
   
   //do {
//	printf("%c",p[0]);
//        p++;
//	if(isalpha(p[0])){
   //		isstr = true;}
   //	if("."==p[0]){
   //		isdec = true;}
	
   //	}while(*p!= '\0');
   for(int i = 0; str[i] != '\0'; i++){
     if('.'==str[i]){
   		isdec = true;}
     if (!isdigit(str[i]) && str[i] != '\0' && str[i] != '\n' && str[i] != '.'){
        if(!(i==0 and '-' == str[i])){
   	     isstr = true;
       }
     }
   }
   if(isstr){
      printf("it's a string");
   }
   else if(isneg and isdec){printf("It's a negative decimal");}
   else if(isdec){printf("It's a decimal");}
   else if(isneg){printf("It's a negative int");}
   else{printf("it's a int");}
   printf("\n");
   
}
bool isInt(char str[]){
   
   bool isInt = true;
   if(str[0] == '-'){
      
      isInt = false;
   }
   for(int i = 0; i < strlen(str); i++){
     if('.'==str[i]){
                
   		isInt = false;}
     else if (!isdigit(str[i]) && str[i] != '\0' && str[i] != '\n'){
        
   	isInt = false;
     }
   }
   return (isInt);
}

int atoi(char x[]){
	int holder = 0;
	for(int i = 0 ; x[i] != '\0'; i++){
		if(10 != x[i]){		
			holder = holder*10 + x[i] - 48;
		}
	}
	return holder;
}


void shoutingMatch(int param){
   const char *quotes[6] = {"What?","string 1","string 2","string 3","string 4","string 5"};
   threadPasser* x = (threadPasser*)param;
   for(int i = 0; i < x->num;i++){
      printf("Thread %i says: %s \n",x-> id,quotes[Random() % 6]);
      int waittime = Random()%4 +3;
      int counter = 0;
      while(waittime>counter){
         currentThread -> Yield();
         counter++;
      }
      
   }

}
void shoutingHelper(){
   char threads[100];
   printf("enter a the number of threads you want to make:");
   bool notEntered = true;
   int threadsnum;
   do{
      fgets(threads, 100, stdin);
      if(!isInt(threads)){
         printf("Error threads entered is not a int\n");   
      }else{
         notEntered = false;
         threadsnum = atoi(threads);
      }
   }
   while(notEntered);
   char shouts[100];
   printf("enter a the number of shouts you want to make:");
   notEntered = true;
   int shoutsnum;
   do{
      fgets(shouts, 100, stdin);
      if(!isInt(shouts)){
         printf("Error shoutss entered is not a int\n");
      }else{
	 notEntered = false;
         shoutsnum = atoi(shouts);
      }
   }
   while(notEntered);
   printf("you picked %i shouts and %i threads \n", shoutsnum, threadsnum);
   char buffer;
    for(int i = 1; i< (threadsnum+1); i++){
       Thread *t1 = new Thread("test");
       threadPasser *y = new threadPasser();
       y-> num = shoutsnum;
       y-> id = i;
       t1->Fork(shoutingMatch, (int)y);
    }
    currentThread->Finish();
   
}

int counter = 0;
Semaphore *sem = new Semaphore("main sem", 1);

void
SimpleThread(int which)
{
    int num;
    
    for(int i = 0; counter<25; i++){
    	sem->P();
    	num = counter; 
		printf("*** thread %d looped %d times\n", which, counter);
    	num++;
    	int count = 0;
    	
    	counter = num;
    	sem->V();
    	while(Random() % 5+1>count){
         currentThread -> Yield();
         count++;
        }
    }
    
}
bool chopsticks[1000];
bool CLock[1000];// it is Chopstick Lock, not a clock
int meals = 10;
int philosophers= 0;
int numPhil = 0;
void philosopher(int id){
	int right = (id+1)%5;
	philosophers++;
	printf("Philosopher %i walks up to the table and waits \n", id);
	while(philosophers < numPhil){
		currentThread -> Yield();
	}	
	while(meals > 0){
		bool loop = true;
		while(loop == true){
			if(meals <1) break;			
			//printf("Philosopher %i wants to eat \n", id);
			currentThread -> Yield();
			
			while(CLock[id] or CLock[right]) {currentThread -> Yield();
				if(meals <1) {break;
			printf("Philosopher %i leaves the table \n", id);}
				//printf("%i CLocked out\n", id);
			}
				if(meals <1) break;
				if (chopsticks[id] && chopsticks[right]){
				printf("Philosopher %i sees a opening and lock is engaged\n", id);			
				loop = false;
				CLock[id] = true;}
			
				
			
		}
		
		if(meals <1) {break;
			printf("Philosopher %i leaves the table \n", id);}
		printf("Philosopher %i picks up left chopstick %i \n", id, id);
		chopsticks[id] = false;
		currentThread -> Yield();
		if(meals <1) {break;
			printf("Philosopher %i leaves the table \n", id);}
		printf("Philosopher %i picks up right chopstick %i \n", id, right);
		chopsticks[right] = false;
		CLock[id] = false;
		if(meals ==1) printf("all meals have been picked up\n");
		meals--;
		
		if(meals <1) break;
		printf("Philosopher %i starts to eat there are %i meals remaining \n", id, meals);
		int waittime = Random()%4 +3;
		for(int i = 0; i<waittime; i++){
			printf("Philosopher %i is stil eating \n", id);
       			currentThread -> Yield();       			
	    	}
		printf("Philosopher %i ends eating \n", id);
		chopsticks[id] = 1;
		chopsticks[right] = 1;
		if(meals <1) {break;
			printf("Philosopher %i leaves the table \n", id);}	
		waittime = Random()%4 +3;	
		for(int i = 0; i<waittime; i++){
			printf("Philosopher %i is thinking %i out of %i \n", id, i,waittime);
       			currentThread -> Yield();       			
	    	}
		printf("Philosopher %i ends thinking \n", id);
	}
}
Semaphore * SemChopsticks[100000];

void SemPhilosopher(int id){
	int right = (id+1)%5;
	int left = id;
	printf("Philosopher %i walks up to the table and waits \n", id);
	philosophers++;
	while(philosophers < numPhil){
		currentThread -> Yield();
	}
	printf("Philosopher %i begins \n", id);
	while(meals >0){
		currentThread -> Yield();
		if(meals ==0) {
			printf("Philosopher %i cannot find any meals and leaves the table \n", id);
			break;}
		if(id%2==0){
			printf("Philosopher %i is trying to pick up left chopstick %i \n", id, id);
			SemChopsticks[id] -> P();
			printf("Philosopher %i picks up left chopstick\n", id);
			printf("Philosopher %i is trying to pick up right chopstick%i \n", id, right);
			SemChopsticks[right]-> P();
			printf("Philosopher %i picks up right chopstick\n", id);
		}	
		else{
			printf("Philosopher %i is trying to pick up right chopstick%i \n", id, right);
			SemChopsticks[right]-> P();
			printf("Philosopher %i picks up right chopstick\n", id)	;		
			printf("Philosopher %i is trying to pick up left chopstick %i \n", id, id);
			SemChopsticks[id] -> P();
			printf("Philosopher %i picks up left chopstick\n", id);
		}		
		;
		if(meals <1) {
			printf("Philosopher %i cannot find any meals and leaves the table \n", id);
			SemChopsticks[id]-> V();
			SemChopsticks[right]-> V();
			
			break;}
		printf("Philosopher %i begins eating\n", id);
		meals--;
		int waittime = Random()%4 +3;
		for(int i = 0; i<waittime; i++){
			//printf("Philosopher %i is stil eating \n", id);
       			currentThread -> Yield();       			
	    	}
		printf("Philosopher %i ends eating\n", id);
		
		printf("meals = %i \n", meals);
		printf("Philosopher %i releasing chopstick %i\n", id, id);
		SemChopsticks[id]-> V();
		printf("Philosopher %i released chopstick %i\n", id, id);
		printf("Philosopher %i releasing chopstick %i\n", id, right);
		SemChopsticks[right]-> V();
		printf("Philosopher %i released chopstick %i\n", id, right);
	}
}


int GetInt(){
	char a[101];
	bool notEntered = true;
	int num;
	
	while(notEntered){
	      fgets(a, 100, stdin);
	      if(!isInt(a)){
	         printf("Error entered is not a int, please re-enter a int\n");   
	      }else{
	         notEntered = false;
	         num = atoi(a);
	      }
      }
	
	return num;
	
}
char *randString(){
	int rand = Random()%5;
	char *a[4]; 
	a[0] = "Never Gonna Give";
	a[1] ="never Gonna let you down";
	a[2]= "Never gonna run around";
	a[3]= "and desert you";
 	a[4]="Never gonna make you cry";
	return a[rand];

}

int numMail = 100;
int numSlots = 10;
int numPerson = 5;
//< I got the basics for this code from the disscussion of project #2 
class Person {     
   public:           
	char** mailbox;
	int id;
	Semaphore* sem;
	Person(int ID) {     
		id =ID;
 		mailbox = new char*[numSlots];
		
 		for(int i=0;i<numSlots; i++){
            	mailbox[i]=NULL; 
        }
        sem = new Semaphore("Sem",1);
    }

};
Person **People;
Semaphore *mutex = new Semaphore("main sem", 1);
void Mail(int id){
	int numMailOpened = numMail;
	int waittime;
	bool isEmpty = false;
	while(!isEmpty){
		printf("person %i Enters Post Office\n", id);
		
		printf("person %i waiting to look though mail box\n", id);
		People[id]->sem->V();	
		printf("person %i is looking through mail box\n", id);
		for(int i = 0; i< numPerson; i++ ){ 
			if (!People[id]->mailbox[i] == NULL){
				printf("person %i reads message: %s in slot: %i:\n", id, People[id]->mailbox[i], i);
				People[id]->mailbox[i] = NULL;

			}else if (numMail <1){
				isEmpty = true;
			}
		}
		currentThread->Yield();
		int randPerson= Random() % numPerson;
		currentThread->Yield();
		int rand = Random()%5;
		char *randStr[5] = {"Never Gonna Give"
		,"never Gonna let you down","Never gonna run around","and desert you"
		,"Never gonna make you cry"};
		//char *a[4]; 
		//a[0] = "Never Gonna Give";
		//a[1] ="never Gonna let you down";
		//a[2]= "Never gonna run around";
		//a[3]= "and desert you";
 		//a[4]="Never gonna make you cry";
		//char *randStr = a[rand];
		
		printf("person %i is accessing the mailbox of person %i \n", id, randPerson);
		People[randPerson]->sem->P();
		if(numMail>0){		
			for(int i = 0; i<numSlots; i++){
			if (People[randPerson]->mailbox[i] == NULL){
				mutex -> P();
				printf("person %i is writing message to person %i \n", id, randPerson);
				People[randPerson]->mailbox[i]= randStr[Random()%5];
				numMail--;
				mutex -> V();			
				break;
			}
			}  
		}
		
		
		waittime = Random()%4 +3;	
		for(int i = 0; i<waittime; i++){
			//printf("Philosopher %i is thinking %i out of %i \n", id, i,waittime);
       			currentThread -> Yield();       			
	    	}
	
	}
	//printf("person %i sees the post office is closed and they have no mail and so goes home\n", id);
}
//>
Semaphore *RW = new Semaphore("main sem", 1);
Semaphore *ReadersSem = new Semaphore("Readers's Sem", 1);
Semaphore *WritersSem = new Semaphore("Readers's Sem", 0);
int readers = 0;
int TotalReads = 0;
int max = 5;
int InputReads=20;


void reader(int id){
	ReadersSem->P();	
	mutex->P();
	readers++;
	TotalReads++;


	printf("reader %i is reading\n", id);
	int waittime = Random()%4 +3;
	for(int i = 0; i<waittime; i++){			
       		//random busy work  
		int a =5+10;
		a =5+6;
		a =5+15;  			
	}
	printf("reader%i is done reading\n", id);


	if(readers == max) {
		readers = 0;
		mutex->V();
		WritersSem->V();
	}else{
		mutex->V();
		ReadersSem->V();
	}
}
void writer(int id){
	WritersSem->P();
	printf("writer %i is writing \n", id);
	int waittime = Random()%4 +3;
	for(int i = 0; i<waittime; i++){			
       		//random busy work  
		int a =5+10;
		a =5+6;
		a =5+15;  			
	}
	printf("writer %i is done writing \n", id);
	if (TotalReads==InputReads){
		WritersSem->V();	
	}	
	ReadersSem->V();
	
}

void ThreadTest()
{	
	
	if(someFlag == 1){
		checkInput();
	}else if(someFlag == 2){
		shoutingHelper();
	}
	else if (someFlag ==5){
		printf("Enter number of people: \n");  
		numPerson = GetInt();
		printf("Enter number of slots: \n");  
		numPerson = GetInt();  
		printf("Enter number of Mail: \n");  
		numMail = GetInt();  
	
		People = new Person*[numPerson];
		for(int i = 0; i<numPerson;i++){
			People[i] = new Person(i);
		}
		for(int i = 0; i<numPerson; i++){
       			Thread *t1 = new Thread("test");
       			t1->Fork(Mail, i);
		 }
	}
	else if (someFlag ==3){
		printf("Enter number of philosophers: \n");  
		numPhil = GetInt();
		printf("Enter number of meals: \n");  
		meals = GetInt();
		for(int i = 0; i<numPhil; i++){
			CLock[i] == false;
			chopsticks[i] = true;
       			Thread *t1 = new Thread("test");
       			t1->Fork(philosopher, i);
	  	}
	}
	else if (someFlag ==4){
		printf("Enter number of philosophers: \n");  
		numPhil = GetInt();
		printf("Enter number of meals: \n");  
		meals = GetInt();
		for (int i=0; i<numPhil; i++)
    		{
      			SemChopsticks [i] = new Semaphore ("cstick", 1);
    		}
		for(int i = 0; i<numPhil; i++){
       			Thread *t1 = new Thread("test");
       			t1->Fork(SemPhilosopher, i);
		}
	}
	else if (someFlag ==6){
		printf("Enter number of readers: \n");  
		InputReads = GetInt();
		printf("Enter number of writes: \n");  
		int writes = GetInt();  
		printf("Enter number of max readers: \n");  
		max = GetInt();
		for(int i = 0; i<InputReads; i++){
			Thread *t1 = new Thread("test");
       			t1->Fork(reader, i);
			
	  	}
		for(int i = 0; i<writes; i++){
			Thread *t2 = new Thread("test");
       			t2->Fork(writer, i);
			
	  	}
	
	}else{
	printf("no -A input attached so no function called");
	}
}
