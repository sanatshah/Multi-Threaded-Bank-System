#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>


typedef struct acct* acnt;
struct acct{
	char* name;
	float balance;
	int isf;  
	int error; 
	char errorMessage[50];
	int trackerID;
	acnt next;	
};

struct manager{
	acnt head;
	int numAccts; 
	int error; 
	char errorMessage[50];
	int trackerCounter;
};
typedef struct manager* acctManager;

struct dataInfo{
        acnt acctPtr;
        char* command;
	char* message;
};
typedef struct dataInfo* dataInfo;


#endif
