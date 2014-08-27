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
#define DEBUG_ON 1
/* delay yime in microseconds*/
#define DELAY_US 100000
#define DEBUG_PRINT(a...) { if(DEBUG_ON) printf(a); }

char **files;


int main() {
	system("ls > files.txt");
	FILE *ptr = NULL;	
	ptr = fopen("files.txt","r");
	if(ptr == NULL) {
		DEBUG_PRINT("list files unavailable\n");
		system("rm files.txt");
		return 0;
	}
	char file [256]; 
	int file_nr = 0,i;
	while ( fgets ( file, sizeof file, ptr ) != NULL ) /* read a line */
    {
		file_nr ++;
	}
	files = (char **)malloc(file_nr*sizeof(char*));
	for(i = 0;i < file_nr;i++){
		files[i] = (char*)malloc(256 * sizeof(char));
	}		
	rewind(ptr);
	i = 0;
	while ( fgets ( file, sizeof file, ptr ) != NULL ) /* read a line */
    {
		if(i==file_nr){
			break;
		}
		strncpy(files[i],file,strlen(file)-1);
		i ++;
	}
	DEBUG_PRINT(" total files %i\n",i);
	system("rm files.txt");
	
	 
}
