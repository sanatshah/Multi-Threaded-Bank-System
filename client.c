#include<stdio.h> 
#include<string.h>    
#include<sys/socket.h>    
#include<arpa/inet.h> 
#include "account.c" 
#include "utils.h"

int checkMessage(char message[]);
int bal=0;
int err=0;
 
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
		bal=0;
		err=0;
		printf(GREEN "> " RESET);
		sleep(2);
		fgets(message,50, stdin);

		if(!checkMessage(message)) 
		{
			printf("\n");
			printf(RED "Please enter proper syntax." RESET); 
			printf("\n");
			printf("\n");
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
		char error[42]; 
		for(y=0;y<42;y++){

			if (y==0) 
				printf("\n");
			error[y]=rbuffer[8+y];
		
			if ((rbuffer[8+y]==' ')&&(rbuffer[9+y]==' ')) {


			} else {
				err=1;
				printf(RED "%c" RESET,rbuffer[8+y]);
			}
		
			if((y==41)&&(!bal)&&(err)){ 
				printf("\n");	
				printf("\n");	
			}else if ((y==41)&&(bal)){ 
			}
		}
	

		if(!err) {

			printf(GREEN "Command executed without errors.\n" RESET); 
			printf("\n"); 
		}
		

		if ((bal)&&(!err)) { 
			printf("\n");	

		char balance[42]; 
		printf("$");
		for(y=0;y<50;y++){

			printf("%c",rbuffer[54+y]);
		}
		printf("\n");
		printf("\n");


		}
	
         
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
	if (strncmp(command, balance,7)==0)
		bal=1;


 	char finish[50];
 	strcpy(finish, "finish");

 	char exit[50];
 	strcpy(exit, "exit");

	int check=0;

	if ((strcmp(command,open)==0)||(strcmp(command,start)==0)||(strcmp(command,credit)==0)||(strcmp(command,debit)==0)||(strncmp(command,balance,7)==0)||(strcmp(command,finish)==0)||(strcmp(command,exit)==0)) 
		check=1;

	if (check) 
		return 1;
	else 
		return 0;

	return 0;
}
