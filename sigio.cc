//Rob Fugate
//CS442 - Advanced Networking - Randy Appleton
//9-23-2013
//Purpose: Demonstrate how to use Signals while computing PI and listening for
//connections. When someone connects reply with current value of pi.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <math.h>
#include <cstdlib>

using namespace std;
double volatile pi = 0;
int s;

int MakeServerSocket(const char *port){
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

	gethostname(localhostname,MAXNAMELEN);
	hp = gethostbyname("euclid.nmu.edu");
	sscanf(port, "%d", &portnum);
	if(portnum == 0){
		sp = getservbyname(port, "tcp");
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

//This gets called whenever there is an event on a fd
void sighandler(int signo, siginfo_t *siginfo, void *junk){
	struct sockaddr_in sa;
	int sa_len = sizeof(sa);
	char buf[64];
	int fd = 0;
	int len = 0;
	if(signo==SIGIO){
		fd = accept(s, (struct sockaddr *)&sa, (unsigned int *)&sa_len);
		cout << "User Connected!" << endl;
		sprintf(buf, "Current Value of PI: %f \n", pi);
		cout << "Sending PI information... " << endl;
		len = write(fd,buf,strlen(buf)+1);
		close(fd);
	}
	return;
}


int main(int argc, char **argv){
	double count = 0;
	double inValue = 0;
	double x = 0;
	double y = 0;
	double z = 0;

	s = MakeServerSocket("40932");

	//Set up the signal handler to call the function above
	//struct sigaction action;
	//memset(&action, 0, sizeof(action));
	//action.sa_sigaction = sighandler;
	//action.sa_flags = SA_SIGINFO;
	//sigaction(SIGIO, &action, NULL);

	//Set up the file descriptor to generate a signal
	//fcntl(STDIN_FILENO, F_SETOWN, getpid());
	//fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | FASYNC);


	//sigaction(SIGIO, &action, NULL);

	//Sleep. Read when a signal is generated
	while(1){
		struct sigaction action;
		memset(&action, 0, sizeof(action));
		action.sa_sigaction = sighandler;
		action.sa_flags = SA_SIGINFO;
		sigaction(SIGIO, &action, NULL);

		fcntl(STDIN_FILENO, F_SETOWN, getpid());
		fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | FASYNC);
		count++;
		x = (double)rand() / (double)RAND_MAX;
		y = (double)rand() / (double)RAND_MAX;
		z = ((x*x) + (y*y));
		if(z <= 1.00)
			inValue++;
		pi = 4*(inValue / count);
	}
}
