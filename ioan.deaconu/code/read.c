#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <pthread.h>
#include <string.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>     
#include <signal.h> 


/* activates/deactivates printf debuf information*/
#define DEBUG_ON 0
/* delay yime in microseconds*/
#define DELAY_US 100000
#define DEBUG_PRINT(a...) { if(DEBUG_ON) printf(a); }

int status, read_thread_online,node_nr, dongle_connected, socket_desc;
long long current_timestamp, delta = 45000, file_timestamp;

pthread_mutex_t data_lock;
struct timespec lock_timeout;  

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
 
    return milliseconds;
}

int get_hex(char *p, int nr){ 
	int i;
	int value = 0;
	for(i = 0 ;i < nr;i++){
		if(p[i] >='a') {
			value += value*16 + 10+(p[i] - 'a');
		}	else {
			value += value*16 + (p[i] - '0');
		}
	}
	return value;
}

void add_node_data(long long time_stamp , char *p) {
	int i;
	int id = get_hex(p,2);

	/* the power of the signal calculated in dB */
// must take account for \n and \0 on normal linux or just \n for parrot drone
	int power = -90 + 3* (get_hex(p + strlen(p)-5,2)-1); 
	/* creating a file with unique name */
	DEBUG_PRINT("node id %s %i %i \n",p,id,power);
	char file_name[100];
	sprintf(file_name, "/node_logs/%lli_%i.txt",file_timestamp,id);

	/* saving the new data at the end of the file */
	FILE *fptr = fopen(file_name,"a");	
	fprintf(fptr,"%lli %s",time_stamp,p);
	fclose(fptr);
	
	/* searching for previous connection of the same node*/
	for(i = 0 ;i < node_nr;i++) {
		if(data[i].id == id) {

			/* timestamp update - node is stil reachable and sending data */
			DEBUG_PRINT("data update\n");
			data[i].time_stamp = time_stamp;
			data[node_nr].power = power;
			return;
		}
	}
	
	/* new node found by the drone */
	DEBUG_PRINT("new node\n");
	data[node_nr].id = id;
	data[node_nr].time_stamp = time_stamp;
	data[node_nr].power = power;
	node_nr ++;
}

void delete_node_data(int id) {

	int i;
	for(i = id; i < node_nr -1;i++) {
		data[i] = data[i+1];
	}
	node_nr --;
}

char * json_encode(){
	int i,msg_index = 0;

	char *client_message = (char*)malloc(3000 * sizeof(char));
	client_message[0]='\0';	

	pthread_mutex_timedlock(&data_lock,&lock_timeout); 
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
	
	int local_node_nr = node_nr;
	struct node_data local_data[node_nr];	
	memcpy(local_data,data,node_nr * sizeof(struct node_data));
	pthread_mutex_unlock(&data_lock);  
	

	msg_index += sprintf(client_message+msg_index, "{ \"dongle_connected\"=%i ,\"nodes\"=[",dongle_connected);
	
    current_timestamp = get_current_timestamp();
	for(i = 0; i <local_node_nr;i++) {
		if(i > 0 ) {
			msg_index += sprintf(client_message+msg_index, ",");
		}	
		msg_index += sprintf(client_message+msg_index, "{\"node_id\"=%i,\"last_connection_time\"=%i,\"power\"=%i}",local_data[i].id,(int)((current_timestamp - local_data[i].time_stamp)/10),local_data[i].power);	
	}

	msg_index += sprintf(client_message+msg_index, "]}\n");

	return client_message;
}

void accept_socket_connection() {
	
    struct sockaddr client;
    char *client_message ;
	int  client_sock , c , read_size;

    //Accept and incoming connection
    DEBUG_PRINT("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);
     
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        DEBUG_PRINT("accept failed\n");
       return ;
    }
    DEBUG_PRINT("Connection accepted\n");
     
    //Receive a message from client 
	while(1)    
	{	
		// sleep in order to avoid flooding the socket
		usleep(DELAY_US);
 
		client_message = json_encode();
		// error writing - client disconected
		if (write(client_sock , client_message , strlen(client_message)) != strlen(client_message)) 		{		
			free(client_message);
        	DEBUG_PRINT("Client disconnected \n" );
			// reseting listening procces			
			return;
		}
		free(client_message);
    }
     
    if(read_size == 0)
    {
        DEBUG_PRINT("Client disconnected\n");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        DEBUG_PRINT("recv failed\n");
    }
} 
 
/* this function is run by the second thread */
void *pthread_server(void *x_void_ptr)
{
	signal(SIGPIPE, SIG_IGN);

    struct sockaddr_in server;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        DEBUG_PRINT("Could not create socket\n");
		return;
    }
    DEBUG_PRINT("Socket created\n");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        DEBUG_PRINT("bind failed. Error\n");
        return ;
    }
    DEBUG_PRINT("bind done\n");
     
    //Listen
    listen(socket_desc , 3);
     

	while(1){
	
		accept_socket_connection();
	
	}
	/* the function must return something - NULL will do */
	return;

}

int check_message_format(char *msg) {
	if(strlen(msg) > 20 && msg[0] == 'P' && msg[1] == 'a' 
		&& msg[2]  == 'c' && msg[3]  == 'k' && msg[6] ==':') {
		return 1;
	}
	return 0;

}

int main() {

	mkdir("/node_logs",0777);

	file_timestamp = get_current_timestamp();

	node_nr = 0;
	dongle_connected = 0;

	pthread_t inc_x_thread;

	lock_timeout.tv_sec=0;
	lock_timeout.tv_nsec= 1000 * DELAY_US;

	if (pthread_mutex_init(&data_lock, NULL) != 0)
    {
        DEBUG_PRINT("\n mutex init failed\n");
        return 1;
    }

	/* create a second thread which executes inc_x(&x) */
	if(pthread_create(&inc_x_thread, NULL, pthread_server, NULL)) {

		DEBUG_PRINT("Error creating thread\n");
		return 1;

	}

	while(1) {
		DEBUG_PRINT("attemting connection to Sparrow Dongle\n");
		FILE *ptr = NULL;	
		while(ptr == NULL) {	
			usleep(DELAY_US);
			ptr = fopen("/dev/ttyACM0","r"); 
		}

		dongle_connected = 1;
		DEBUG_PRINT("Sparrow Dongle connected\n");
		char read_data[100]; 
	 
		while(fgets(read_data,100,ptr) != NULL) 
		{
			
			if(check_message_format(read_data)) {
	   	 		pthread_mutex_timedlock(&data_lock,&lock_timeout); 
				add_node_data(get_current_timestamp(),read_data + 7);
				pthread_mutex_unlock(&data_lock);
			}
		}
		dongle_connected = 0;
		fclose(ptr);
		DEBUG_PRINT("Sparrow Dongle Disconnected\n");	
	}

	DEBUG_PRINT("Clossing the program\n");

	return 0;
}
