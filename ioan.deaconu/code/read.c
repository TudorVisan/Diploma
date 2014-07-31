#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <pthread.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write

int status, read_thread_online;
long long read_time_stamp, current_timestamp, delta = 10000;

long long get_current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

/* this function is run by the second thread */
void *pthread_server(void *x_void_ptr)
{

	int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return NULL;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
       return NULL;
    }
    puts("Connection accepted");
     
    //Receive a message from client
    //while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
	while(1)    
	{
		sleep(1);
        current_timestamp = get_current_timestamp();
		if(current_timestamp - read_time_stamp < delta)
		{
			client_message[0] = '1';
		} else {
			client_message[0] = '0';	
		}
		//printf("%lli\n\r",current_timestamp - read_time_stamp);
		client_message[1] = '\n';
		write(client_sock , client_message , 2);
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
	/* the function must return something - NULL will do */
	return NULL;

}

int main() {
	FILE *ptr = fopen("/dev/ttyACM0","r");
	
	char data[100]; 

	int node_online = 1;

	pthread_t inc_x_thread;

	/* create a second thread which executes inc_x(&x) */
	if(pthread_create(&inc_x_thread, NULL, pthread_server, NULL)) {

		fprintf(stderr, "Error creating thread\n");
		return 1;

	}


	while(fgets(data,100,ptr) != NULL) 
	{ 	
		read_time_stamp = get_current_timestamp();
		//printf("node online\n");
	}


	return 0;
}
