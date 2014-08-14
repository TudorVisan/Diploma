#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <pthread.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write

int status, read_thread_online,node_nr;
long long current_timestamp, delta = 15000, file_timestamp;

pthread_mutex_t data_lock;

struct node_data {
	int id;	
	long long time_stamp;
	int power;
};

struct node_data data[256];


long long get_current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

int get_hex(char *p, int nr){ 
	int i;
	int value = 0;
	for(i = 0 ;i < nr;i++){
		if(p[i] >='a') {
			value += value*16 + (p[i] - 'a');
		}	else {
			value += value*16 + (p[i] - '0');
		}
	}
	return value;
}

void add_node_data(long long time_stamp , char *p) {
	int i;
	int id = get_hex(p,2);
	int power = -90 + 3* (get_hex(p + 64,2)-1);
	printf("node id %i %i\n",id,power);
	char file_name[100];
	sprintf(file_name, "/node_logs/%lli_%i",file_timestamp,id);
	FILE *fptr = fopen(file_name,"a");	
	fprintf(fptr,"%s",p);
	fclose(fptr);
	
	for(i = 0 ;i < node_nr;i++) {
		if(data[i].id == id) {
			printf("time_stamp update\n");
			data[i].time_stamp = time_stamp;
			return;
		}
	}
	printf("new node\n");
	data[node_nr].id = id;
	data[node_nr].time_stamp = time_stamp;
	node_nr ++;
}

void delete_node_data(int id) {

	int i;
	for(i = id; i < node_nr -1;i++) {
		data[i] = data[i+1];
	}
	node_nr --;
}
 
/* this function is run by the second thread */
void *pthread_server(void *x_void_ptr)
{

	int socket_desc , client_sock , c , read_size,i;
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
		client_message[0] = '0';

    	pthread_mutex_lock(&data_lock);
        current_timestamp = get_current_timestamp();
		for(i = 0; i < node_nr;i++) { 
			if(current_timestamp - data[i].time_stamp < delta)
			{
				client_message[0]++;
			} else {
				delete_node_data(i);
				i--;
			}
		}
    	pthread_mutex_unlock(&data_lock);
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
	FILE *ptr = NULL;

	mkdir("/node_logs",0777);

	file_timestamp = get_current_timestamp();

	while(ptr == NULL) {	
		sleep(1);
		ptr = fopen("/dev/ttyACM0","r");
	}

	char read_data[100]; 

	node_nr = 0;

	pthread_t inc_x_thread;

	if (pthread_mutex_init(&data_lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

	/* create a second thread which executes inc_x(&x) */
	if(pthread_create(&inc_x_thread, NULL, pthread_server, NULL)) {

		fprintf(stderr, "Error creating thread\n");
		return 1;

	}
 
	while(fgets(read_data,100,ptr) != NULL) 
	{ 	 
		if(strlen(read_data) == 77 && read_data[0] == 'P' && 
		read_data[1] == 'a' && read_data[2]  == 'c' && read_data[3]  == 'k' &&
		read_data[6] ==':') {
   	 		pthread_mutex_lock(&data_lock); 
			add_node_data(get_current_timestamp(),read_data + 7);
    		pthread_mutex_unlock(&data_lock);
		}
	}
	printf("Clossing the program\n");

	return 0;
}
