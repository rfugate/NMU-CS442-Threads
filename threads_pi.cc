//NMU Computer Science
//442 - Advanced Network Programming
//Rob Fugate
//Purpose: Construct a program that utilizes threads to compute PI using the dart-throwing method

#include <pthread.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <cstdlib>

using namespace std;

pthread_mutex_t aMutex = PTHREAD_MUTEX_INITIALIZER;				//If we had 2 threads changing the value on the same object use Mutex lock and unlock to keep them from writing at the same time

void* MyThreadFunc(void* arg);

double pi = 0;
double totalPoints = 10000000;

int main(void){
	pthread_t thread1;											//Declare the thread
	pthread_create(&thread1, NULL, MyThreadFunc, (void *)NULL);	//Start running the thread
	pthread_join(thread1, NULL);								//Wait for threads to stop before exiting main

	cout << pi << endl;
}

void* MyThreadFunc(void* arg){
	/*
		The longer this thread runs the closer it will get to the value for PI
	*/
	double x = 0;
	double y = 0;
	double dist = 0;
	double tempsum = 0;
	double theSum = 0;
	double count = 0;
	while(1){
		count++;
		x = (double)rand()/(double)RAND_MAX;
		y = (double)rand()/(double)RAND_MAX;
		dist = ((x*x)+(y*y));
		if(dist <= 1.00){
			theSum++;
		}
		pi = 4*(theSum/count);
	}

	return((void*)NULL);
}
