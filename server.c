#include<stdio.h>
#include<string.h>    
#include<stdlib.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<inttypes.h>
#include<unistd.h>    
#include<pthread.h> 
#include "account.c"
#include "utils.h"
 
void *clientHandler(void *);
char* handleCommand(char* command, acnt currAcct, char* incomData);
char* creditAcct(char* fullCommand, char* incomData);
char* finishAcct(char* fullCommand, char* incomData);
char* debitAcct(char* fullCommand, char* incomData);
char* openAcct(char* fullCommand, char* incomData);
char* startAcct(char* fullCommand, char* incomData);
char* getBalance(char* fullCommand, char* incomData);
char* getSecondPart(char* fullCommand);
char* testNumb="test";

int openingAccount;
acctManager control;
pthread_mutex_t fullStructLock = PTHREAD_MUTEX_INITIALIZER;
acnt tracker[20];

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , *new_sock;
	struct sockaddr_in server , client;
	openingAccount = 0;

     
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

	control = (acctManager)malloc(sizeof(acctManager));	
	control->head=NULL;
	control->numAccts = 0; 	
	control->error = 0; 
	control->trackerCounter=0;

     
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

char* finishAcct(char* fullCommand, char* incomData){




}

char* getBalance(char* fullCommand, char* incomData){

	char trackerID[8]; 
	int y=0; 
	char* error;

	//grab the trackerID 
	for(y;y<8;y++){
		trackerID[y]=incomData[y];
	}

	int trackID = atoi(trackerID);

	printf("Tracker ID is %d\n",trackID);

	//if trackerID there is no started account
 	if (trackID==0) {

                error="Not in customer session.";
                // add error to buffer
                int x=0;
                for(x;x<strlen(error);x++){
                        *(incomData+8+x) = *(error+x);
                }

                return incomData;
        }

	//trackID exists, account is started, find node

        acnt clientAcnt;
        for(y=0;y<20;y++){

                if(tracker[y]->trackerID == trackID) {
                        clientAcnt=tracker[y];
                        break;
                }
        } 

	char balance[25]; 
	sprintf(balance, "%f", clientAcnt->balance);

	int x;
	for(x=0;x<50;x++){
		*(incomData+54+x) = ' ';
	}

	for(x=0;x<strlen(balance);x++){
		*(incomData+54+x) = *(balance+x);
	}

	return incomData;	
	

}

char* debitAcct(char* fullCommand, char* incomData){
	char trackerID[8]; 
	int y=0; 
	char* error;

	//grab the trackerID 
	for(y;y<8;y++){
		trackerID[y]=incomData[y];
	}

	int trackID = atoi(trackerID);

	printf("Tracker ID is %d\n",trackID);

	//if trackerID there is no started account
 	if (trackID==0) {

                error="Not in customer session.";
                // add error to buffer
                int x=0;
                for(x;x<strlen(error);x++){
                        *(incomData+8+x) = *(error+x);
                }

                return incomData;
        }

	//trackID exists, account is started, find node

        acnt clientAcnt;
        for(y=0;y<20;y++){

                if(tracker[y]->trackerID == trackID) {
                        clientAcnt=tracker[y];
                        break;
                }
        }

        //accnt found credit accounti 

        //get amount 
        char* secondPart= getSecondPart(fullCommand);
        float debitAmount = atof(secondPart);

        //error checks 
        if (debitAmount<0) {

                error="Debit amount cannot be less than 0.";
                // add error to buffer
                int x=0;
                for(x;x<strlen(error);x++){
                        *(incomData+8+x) = *(error+x);
                }

                return incomData;
        } 

	if ((clientAcnt->balance - debitAmount)<0){
		
                error="Balance cannot be less than 0.";
                // add error to buffer
                int x=0;
                for(x;x<strlen(error);x++){
                        *(incomData+8+x) = *(error+x);
                }

                return incomData;

	}else { 

        	clientAcnt->balance=clientAcnt->balance-debitAmount;

	}

        printf("Current Balance for %s is %f\n",clientAcnt->name, clientAcnt->balance);



	return incomData;
}

char* creditAcct(char* fullCommand, char* incomData){

	char trackerID[8]; 
	int y=0; 
	char* error;

	for(y;y<8;y++){
		trackerID[y]=incomData[y];
	}

	int trackID = atoi(trackerID);

	printf("Tracker ID is %d\n",trackID);

	//check if there is a started account linked to this client	
	if (trackID==0) {

		error="Not in customer session.";			
		// add error to buffer
		int x=0;	
		for(x;x<strlen(error);x++){
			*(incomData+8+x) = *(error+x);
		}

		return incomData;
	} 

	//trackID exists, account is started, find node

	acnt clientAcnt;
	for(y=0;y<20;y++){

		if(tracker[y]->trackerID == trackID) {
			clientAcnt=tracker[y];
			break;
		}	
	}

	//accnt found credit accounti 

	//get amount 
	char* secondPart= getSecondPart(fullCommand); 
	float creditAmount = atof(secondPart);
	
	//error checks 
	if (creditAmount<0) {

		error="Credit amount cannot be less than 0.";			
		// add error to buffer
		int x=0;	
		for(x;x<strlen(error);x++){
			*(incomData+8+x) = *(error+x);
		}

		return incomData;
	} 

	clientAcnt->balance=clientAcnt->balance+creditAmount;

	printf("Current Balance for %s is %f\n",clientAcnt->name, clientAcnt->balance);
	
	
		
	return incomData;

}

char* startAcct(char* fullCommand, char* incomData){
	char* secondPart = getSecondPart(fullCommand);
	int x=0; 
	char* error; 

	puts("in start account");
	//check if account already in session
	for(x;x<20;x++){
		if(tracker[x]!=NULL){
		if (strcmp(secondPart,tracker[x]->name)==0){ 
			error="Account already in session.";			
			puts("in first ifthen");	
			// add error to buffer
			int x=0;	
			for(x;x<strlen(error);x++){
				*(incomData+8+x) = *(error+x);
			}

			return incomData;
		}
		}
	} 


	acnt foundAcct = find(control, secondPart); 

	//account doesn't exist
	if (foundAcct==NULL){
		error="Account doesn't exist";

		// add error to buffer
		int x=0;	
		for(x;x<strlen(error);x++){
			*(incomData+8+x) = *(error+x);
		}

		return incomData;
	} 


	//account is found, start it 
	for(x=0;x<20;x++){
		puts("in accout is found");
		if(tracker[x]==NULL) {
			tracker[x]=foundAcct;
			control->trackerCounter++;
			foundAcct->trackerID = control->trackerCounter;	

			char id[8]; 
			sprintf(id, "%d", foundAcct->trackerID);

			printf("id is %s\n", id); 
			printf("id is %d\n", foundAcct->trackerID); 

			int y=0; 
			for(y;y<8;y++) {
				incomData[y]=id[y];

			}

			return incomData;			
		}
	}
	
	return NULL;
}

char* openAcct(char* fullCommand, char* incomData){ 
	pthread_mutex_lock(&fullStructLock);
	
	char name[50];
	strcpy(name,getSecondPart(fullCommand));	
	printf("%s",name);

	//try adding it to the list
	control=add(control, name);
	if (control->error==1) { 
		printf("Error: %s\n", control->errorMessage);
	
		// add error to buffer
		int x=0;	
		for(x;x<46;x++){
			*(incomData+8+x) = control->errorMessage[x];
		}

		control->error = 0; 
	}

	pthread_mutex_unlock(&fullStructLock);	

	acnt temp = control->head; 
	printf("test %s\n", temp->name);
	return incomData;
} 

char* getSecondPart(char* fullCommand){ 
	char* second =(char *) malloc(50);
	strcpy(second, fullCommand);
	char *temp = strchr(second, ' ');
	if(temp != NULL)
    		second = temp + 1;

	return second;
}	

char* handleCommand(char* fullCommand, acnt currAcct, char* incomData){

	puts("seg fault check");

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
		incomData=openAcct(fullCommand, incomData);
	} else if (strcmp(command, start)==0) {
		incomData=startAcct(fullCommand, incomData);
	} else if (strcmp(command, credit)==0) {
		incomData=creditAcct(fullCommand, incomData);
	} else if (strcmp(command, debit)==0) {
		incomData=debitAcct(fullCommand, incomData);
	} else if (strcmp(command, balance)==0){
		incomData=getBalance(fullCommand, incomData);
	} else if (strcmp(command, finish)==0) {
		incomData=finishAcct(fullCommand, incomData);
	} else if (strcmp(command, exit)==0) {

	} else {

	} 	

	return incomData;
}
 

void *clientHandler(void *socket_desc)
{
	//get socket information
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[2000];

	char* client_data = (char *) malloc(104);

	puts("in clientHandler");	

	//communication with client
	while( (read_size = recv(sock , client_data , 104 , 0)) > 0 )
	{
		puts("seg check");
		//deserialize data
		char* incomData= (char *) malloc(104);
		memcpy(incomData, client_data, 104); 

		//get command
		char message[50];
		int x=0; 

		puts("seg check");
		for(x;x<50;x++){
			message[x]=*(incomData+54+x);
		}

		puts("seg check");
		//get pointer 
		acnt account=NULL;


		//send data to command handler function 
		incomData=handleCommand(message, account, incomData);

		//send data back to client
		write(sock , incomData , 104);

		free(incomData);
	}

	
	free(client_data);
	free(socket_desc);
     
	return 0;
}
