#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <stdio.h>

using namespace std;

pthread_mutex_t aMutex = PTHREAD_MUTEX_INITIALIZER;

//Global Variable
volatile double pi = 0;				//Global variable to hold pi
volatile double inValue = 0;
volatile double count = 0;
int s;

void *calcPI(void* arg);		//Thread's Start Routine
void *network(void *);			//Thread's Start Routine

void Error(const char *msg){
	perror(msg);
	exit(1);
}

int MakeServerSocket(const char *port) {
	//Basic Make Server Socket Method
	const int MAXNAMELEN = 255;
	const int BACKLOG = 3;
	char localhostname[MAXNAMELEN];
	int s;
	int len;
	struct sockaddr_in sa;
	struct hostent *hp;
	struct servent *sp;
	int portnum;
	int ret;

	//gethostname(localhostname,MAXNAMELEN);
	hp = gethostbyname("euclid.nmu.edu");

	sscanf(port, "%d", &portnum);
	if (portnum ==  0) {
		sp=getservbyname(port, "tcp");
		portnum = ntohs(sp->s_port);
	}
	sa.sin_port = htons(portnum);

	bcopy((char *)hp->h_addr, (char *)&sa.sin_addr, hp->h_length);
	sa.sin_family = hp->h_addrtype;

	s = socket(hp->h_addrtype, SOCK_STREAM, 0);

	ret = bind(s, (struct sockaddr *)&sa, sizeof(sa));

	listen(s, BACKLOG);
	cout << "Waiting for connection on port " << port << " hostname " << localhostname << endl;

	return s;
}

int main(int argc, char *argv[]){
	pthread_t t1, t2, t3;				//Declare Threads
	cout << " Creating Threads... " << endl;
	pthread_create(&t1, NULL, calcPI, NULL);	//Create Thread1
	pthread_create(&t2, NULL, calcPI, NULL);
	s = MakeServerSocket("40960");
	if(s == -1)
		Error("MakeServerSocket Error");
	pthread_create(&t3, NULL, network, NULL);	//Create Thread2
	cout << " Created Threads. " << endl;
	pthread_join(t1,NULL);				//Wait for Threads to Exit
	pthread_join(t2,NULL);				//Wait for Threads to Exit
	exit(0);
}

void *calcPI(void *arg){
	//Thread routine for calculating PI
	double xValue = 0;
	double yValue = 0;
	double distance = 0;
	//double inValue = 0;
	double sum = 0;
	//double count = 0;
	while(1){
		pthread_mutex_lock(&aMutex);
		count++;
		pthread_mutex_unlock(&aMutex);
		xValue = (double)rand() / (double)RAND_MAX;
		yValue = (double)rand() / (double)RAND_MAX;
		distance = ((xValue * xValue) + (yValue * yValue));
		if(distance <= 1.00){ //If point fall inside the area of circle then increment inValue counter
			pthread_mutex_lock(&aMutex);			
			inValue++;
			pthread_mutex_unlock(&aMutex);
		}
		//pthread_mutex_lock(&aMutex);
		//pi = 4*(inValueTotal/countTotal);
		//pthread_mutex_unlock(&aMutex);
	}
}

void *network(void *arg){
	struct sockaddr_in sa;
 	int sa_len = sizeof(sa);
	char buf[64];
	while(1){
		pi = 4*(inValue/count);
		cout << "Current Value of PI: " << pi << endl;
		sprintf(buf, "Current Value of PI: %f \n", pi);
		int fd = accept(s, (struct sockaddr *)&sa, (unsigned int *)&sa_len);
		if(fd == -1)
			Error("Accept Error");
		int len = write(fd,buf,strlen(buf)+1);
		if(len == -1)
			Error("Write Error");
		close(fd);
	}
	return((void*)NULL);
}
