#include<stdio.h> 
#include<string.h>    
#include<sys/socket.h>    
#include<arpa/inet.h> 

char* checkMessage(char message[]);
 
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
     
	//keep communicating with server
	while(1)
	{
		printf("> ");

		fgets(message,100, stdin);
		checkMessage(message);
		
         
		//Send command to server
		if( send(sock , message , strlen(message) , 0) < 0)
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

char* checkMessage(char message[]){

	char mess[100];
	strcpy(mess,message);
	char* command; 
	command = strtok(mess," "); 
	printf("first word is: %s\n",command);

	return command;
}
