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

	//prepare struct for sending data to server 
	dataInfo info = (dataInfo) malloc(sizeof(dataInfo));
	info->acctPtr = NULL;
	info->command=(char*) malloc(100);
    	strcpy(info->command, "testing data serialization"); 
	info->message= NULL;

	unsigned char* data=NULL;

	//keep communicating with server
	while(1)
	{
		printf("> ");

		fgets(message,100, stdin);

		if(!checkMessage(message)) 
		{
			puts("Please enter proper syntax."); 
			continue;
		} 
	
		data=(unsigned char*) malloc(sizeof(struct dataInfo)); 	
		memcpy(data, info, sizeof(struct dataInfo));
        	printf("sizeof data = %zu\n",sizeof(data)); 
		//Send command to server
		if( send(sock , data , sizeof(struct dataInfo) , 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
         
		//Receive information from server
		if( recv(sock , server_reply , 2000 , 0) < 0)
		{
			puts("recv failed");
			break;
		}
         
		puts("Server reply :");
		puts(server_reply);
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
