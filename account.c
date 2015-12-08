#include "account.h"

acctManager add(acctManager control, char* name) {
	acctManager controlSave = control; 

	//first account
	if (controlSave->head==NULL) {

		controlSave->numAccts++;

		acnt newAcct = (acnt)malloc(sizeof(acnt)); 
		newAcct->name = (char*)malloc(50);
		strcpy(newAcct->name, name);
		newAcct->balance = 0; 
		newAcct->isf = 0; 
		newAcct->next = NULL;
		newAcct->trackerID=0;

		controlSave->head = newAcct;

		return controlSave;
	}

	//adding account when total accoutns = 20
	if (controlSave->numAccts == 20) { 
		controlSave->error = 1; 
		strcpy(controlSave->errorMessage, "Reached maximum number of accounts.");
		return controlSave;
	}

	//else add a new accoutn to the front
	acnt temp = controlSave->head; 
	acnt newAcct = (acnt)malloc(sizeof(acnt)); 
	newAcct->name = (char*)malloc(50);
	strcpy(newAcct->name, name);
	newAcct->balance = 0; 
	newAcct->isf = 0; 
	newAcct->next = temp;
	newAcct->trackerID=0;

	

	controlSave->head = newAcct;
	controlSave->numAccts++;

	return controlSave;	
} 


acnt find(acctManager control, char* name){
	int check=0;
	puts("in find");
	if (control->numAccts == 0) { 
		return NULL;
	} 

	acnt acntptr=control->head;

	do { 
		printf("stuck in while %s and %zu\n", name, strlen(name));
		printf("stuck in while %s and %zu\n", acntptr->name, strlen(acntptr->name));

		if (strlen(name)<strlen(acntptr->name)){

		if(strncmp(name, acntptr->name, 3)==0) {
			puts("found ptr");
			return acntptr;
		}

		} else {


		if(strncmp(name, acntptr->name, 3)==0) {
			puts("found ptr");
			return acntptr;
		}

	
		}

		acntptr=acntptr->next;
	}
	while (acntptr!=NULL);


	acntptr=NULL;
	return acntptr;

} 


void print(acctManager control){

	if (control->numAccts == 0){ 
		return;
	}
	
	return;

} 
