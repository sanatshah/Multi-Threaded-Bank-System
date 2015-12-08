#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "utils.h"

acctManager add(acctManager control, char* name);

acnt find(acctManager control, char* name); 

void print(acctManager control);


#endif
