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
void printAccounts();
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

	//began printing server stuff/ in separate thread
	pthread_t serverTimer; 
	pthread_create (&serverTimer, NULL, (void *) printAccounts, NULL);

     
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	//accept new client connection and make thread
	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )	
	{
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
         
		if( pthread_create( &sniffer_thread , NULL ,  clientHandler , (void*) new_sock) < 0)
        	{
			perror("could not create thread");
			return 1;
		}else {
		}
         
	}
     
    return 0;
} 

void printAccounts(){
	acnt acntptr; 
	
	while(1){
	sleep(5); 

		if(control->numAccts==0) {
			printf("\n");
			printf("\n");
			printf("No accounts exist.\n");
			printf("\n");
			printf("\n");
			continue;
		}else {

			acntptr=control->head;

			printf("\n");
			printf("\n");
			printf("\n");
			printf(YELLOW "Current Accounts....\n" RESET);
			printf("\n");
		}
		
		
		do{ 

			char* shortName=strtok(acntptr->name," "); 
			if(acntptr->isf){ 
				printf(GREEN "---------------------------------------------------\n" RESET);
				printf("Account Name --------- %s\n", shortName); 
				printf("Balance -------------- %f\n\n", acntptr->balance);
				printf("------"); 
				printf(RED "IN SERVICE" RESET);
				printf("------ \n"); 
				printf(GREEN "---------------------------------------------------\n" RESET);
			} else {
				printf(GREEN "---------------------------------------------------\n" RESET);
				printf("Account Name --------- %s\n", shortName); 
				printf("Balance -------------- %f\n", acntptr->balance);
				printf(GREEN "---------------------------------------------------\n" RESET);
			}

			printf("\n");
			printf("\n");

			acntptr=acntptr->next;

		}while(acntptr!=NULL);
		printf("\n");
		printf("\n");
		printf("\n");


	}


} 

char* finishAcct(char* fullCommand, char* incomData){

	char trackerID[8]; 
	int y=0; 
	char* error;

	//grab the trackerID 
	for(y;y<8;y++){
		trackerID[y]=incomData[y];
	}

	int trackID = atoi(trackerID);



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

	tracker[y]=NULL;
	clientAcnt->trackerID=0;
	clientAcnt->isf = 0;
	
	char id[8]; 
	sprintf(id, "%d", clientAcnt->trackerID);
	y=0; 
	for(y;y<8;y++) {
		incomData[y]=id[y];

	}

	return incomData;
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

	//add balance amount to client return message
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

	
	
		
	return incomData;

}

char* startAcct(char* fullCommand, char* incomData){
	char* secondPart = getSecondPart(fullCommand);
	int x=0; 
	char* error; 

	char trackerID[8]; 
	int y=0; 

	for(y;y<8;y++){
		trackerID[y]=incomData[y];
	}

	int trackID = atoi(trackerID);

	//check if account already in session
	for(x;x<20;x++){
		if(tracker[x]!=NULL){
		if((trackID==tracker[x]->trackerID)||(strncmp(secondPart,tracker[x]->name, strlen(secondPart)-2)==0)){ 
			error="Account already in session.";			
			// add error to buffer
			int x=0;	
			for(x;x<strlen(error);x++){
				*(incomData+8+x) = *(error+x);
			}

			return incomData;
		}
		}
	} 

	  pthread_mutex_lock(&fullStructLock);

	acnt foundAcct = find(control, secondPart); 

	  pthread_mutex_unlock(&fullStructLock);
	//account doesn't exist
	if (foundAcct==NULL){
		error="Account doesn't exist";

		// add error to buffer
		int x=0;	
		for(x;x<strlen(error);x++){
			//*(incomData+8+x) = *(error+x);
			incomData[8+x] = error[x];
		}

		return incomData;
	} 


	//account is found, start it 
	for(x=0;x<20;x++){
		if(tracker[x]==NULL) {
			tracker[x]=foundAcct;
			control->trackerCounter++;
			foundAcct->trackerID = control->trackerCounter;	
			foundAcct->isf=1;
			char id[8]; 
			sprintf(id, "%d", foundAcct->trackerID);

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

	//get client trackign information	
	char trackerID[8]; 
	int y=0; 

	for(y;y<8;y++){
		trackerID[y]=incomData[y];
	}

	int trackID = atoi(trackerID);

	char name[50];
	strcpy(name,getSecondPart(fullCommand));	
	
	int x=0;
	char* error;
	//check if account already in session DEPRECIATED!!!!!!!!
	/*for(x;x<20;x++){
		if(tracker[x]!=NULL){
		if(trackID==tracker[x]->trackerID){ 
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
	} */

	//try adding it to the list
	
	pthread_mutex_lock(&fullStructLock);
	control=add(control, name);
	pthread_mutex_unlock(&fullStructLock);
	if (control->error==1) { 
	
		// add error to buffer
		int x=0;	
		for(x;x<46;x++){
			*(incomData+8+x) = control->errorMessage[x];
		}

		control->error = 0; 
	}

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
	} else if (strncmp(command, balance,7)==0){
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


	//communication with client
	while( (read_size = recv(sock , client_data , 104 , 0)) > 0 )
	{
		//deserialize data
		char* incomData= (char *) malloc(104);
		memcpy(incomData, client_data, 104); 

		//get command
		char message[50];
		int x=0; 

		for(x;x<50;x++){
			message[x]=*(incomData+54+x);
		}

		//get pointer 
		acnt account=NULL;

		for(x=0;x<50;x++){
			*(incomData+8+x) = ' ';
		}

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
