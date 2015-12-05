#include<stdio.h>
#include<string.h>    
#include<stdlib.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>    
#include<pthread.h> 
 
void *clientHandler(void *);
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
 

void *clientHandler(void *socket_desc)
{
	//get socket information
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[2000];

	//communication with client
	while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
	{
		write(sock , client_message  , strlen(client_message));
	}

	free(socket_desc);
     
	return 0;
}
