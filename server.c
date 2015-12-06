#include<stdio.h>
#include<string.h>    
#include<stdlib.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>    
#include<pthread.h> 
#include "account.c"
#include "utils.h"
 
void *clientHandler(void *);
void handleCommand(char* command, acnt currAcct);
void creditAcct(char* fullCommand);
char* testNumb="test";
 
int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , *new_sock;
	struct sockaddr_in server , client;

     
	//Create server socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );
     
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
 		perror("bind failed. Error");
		return 1;
	}
     
	listen(socket_desc , 3);

	
	//intialize control structure 

	acctManager control = (acctManager)malloc(sizeof(acctManager));	
	control->head=NULL;
	control->numAccts = 0; 	

     
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	//accept new client connection and make thread
	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )	
	{
		puts("Connection accepted");
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
         
		if( pthread_create( &sniffer_thread , NULL ,  clientHandler , (void*) new_sock) < 0)
        	{
			perror("could not create thread");
			return 1;
		}else {
			printf("New Thread Created");
		}
         
	}
     
    return 0;
}

void creditAcct(char* fullCommand){




}

void handleCommand(char* fullCommand, acnt currAcct){

	char mess[100]; 
	strcpy(mess,fullCommand); 

	char* command; 
        command = strtok(mess," "); 

	char open[50];
        strcpy(open, "open");

        char start[50];
        strcpy(start, "start");

        char credit[50];
        strcpy(credit, "credit");

        char debit[50];
        strcpy(debit, "debit");

        char balance[50];
        strcpy(balance, "balance");

        char finish[50];
        strcpy(finish, "finish");

        char exit[50];
        strcpy(exit, "exit");
	
	if (strcmp(command, open)==0) {




	} else if (strcmp(command, start)==0) {




	} else if (strcmp(command, credit)==0) {

	creditAcct(fullCommand);


	} else if (strcmp(command, debit)==0) {



	} else if (strcmp(command, balance)==0){




	} else if (strcmp(command, finish)==0) {




	} else if (strcmp(command, exit)==0) {






	} else {




	} 	



}
 

void *clientHandler(void *socket_desc)
{
	//get socket information
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[2000];

	unsigned char client_data[sizeof(struct dataInfo)];

	puts("in clientHandler");	

	//communication with client
	while( (read_size = recv(sock , client_data , sizeof(struct dataInfo) , 0)) > 0 )
	{
		puts("got data");
		printf("Size of client_data= %zu and sizeof(dataInfo) = %zu\n",strlen(client_data),sizeof(struct dataInfo));

		//deserialize data
		dataInfo incomData = (dataInfo) malloc(sizeof(struct dataInfo));
		memcpy(&incomData, client_data, sizeof(struct dataInfo)); 

		//send data to command handler function

		free(incomData);
	}

	free(socket_desc);
     
	return 0;
}
