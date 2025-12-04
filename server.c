
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

struct message {
	char source[50];
	char target[50]; 
	char msg[200]; // message body
};

void terminate(int sig) {
	printf("Exiting....\n");
	fflush(stdout);
	exit(0);
}

int main() {
	int server;
	int target;
	int dummy;
    struct message req;

    signal(SIGPIPE,SIG_IGN);
    signal(SIGINT,terminate);

    mkfifo("serverFIFO",0666);       // create FIFO if missing
    server = open("serverFIFO",O_RDONLY);
    dummy  = open("serverFIFO",O_WRONLY); // keep FIFO open

    while (1) {
		// TODO:
		// read requests from serverFIFO
        int n = read(server,&req,sizeof(req));
        if (n <= 0) continue;

		// TODO:
		// open target FIFO and write the whole message struct to the target FIFO
		// close target FIFO after writing the message
        target = open(req.target,O_WRONLY|O_NONBLOCK);
        if (target < 0) {
            printf("User %s not active\n",req.target);
            continue;
        }
        write(target,&req,sizeof(req));
        close(target);
    }
}
