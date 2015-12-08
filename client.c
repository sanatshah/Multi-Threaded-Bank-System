#include<stdio.h> 
#include<string.h>    
#include<sys/socket.h>    
#include<arpa/inet.h> 
#include "account.c" 
#include "utils.h"

int checkMessage(char message[]);
 
int main(int argc , char *argv[])
{
	int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];
     
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
     
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );
 
	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
 		perror("connect failed. Error");
		return 1;
	}
     
	printf("Connected to server...\n");

	//create buffer of information to be sent to server 

	char* buffer = (char *) malloc(104); 
	char* rbuffer = (char *) malloc(104); 
	int x=0; 
	int trackerID=0;

	//keep communicating with server
	while(1)
	{
		printf("> ");

		fgets(message,50, stdin);

		if(!checkMessage(message)) 
		{
			puts("Please enter proper syntax."); 
			continue;
		} 

		//add command to buffer
		for(x=0;x<50;x++){
	
			*(buffer+54+x) = ' ';
		}

		for(x=0;x<strlen(message);x++){
	
			*(buffer+54+x) = *(message+x);
		}

		//Send buffer to server
		if( send(sock , buffer , 104 , 0) < 0)
		{
			puts("Send failed");
			return 1;
		}

		//recieve buffer from server
		if( recv(sock , rbuffer , 2000 , 0) < 0)
		{
 			puts("recv failed");
			break;
		} 

		char trckerID[8];
        	int y=0;

        	for(y;y<8;y++){
                	trckerID[y]=rbuffer[y];
			buffer[y]=rbuffer[y];
 		}

        	trackerID = atoi(trckerID);
	
         
	}
     
	close(sock);
	return 0;
}

int checkMessage(char message[]){

	char mess[100];
	strcpy(mess,message);
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

	int check=0;

	if ((strcmp(command,open)==0)||(strcmp(command,start)==0)||(strcmp(command,credit)==0)||(strcmp(command,debit)==0)||(strcmp(command,balance)==0)||(strcmp(command,finish)==0)||(strcmp(command,exit)==0)) 
		check=1;

	if (check) 
		return 1;
	else 
		return 0;

	return 0;
}
