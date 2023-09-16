#ifndef _DEFINE_CUSTOMER_H
#define _DEFINE_CUSTOMER_H

#include "game.h"
#include "bread.h"

#define MAX_CUSTOMER_COUNT 6

struct CustomerStruct;

// 2.) typedef of the structure
// Still no details on the members.
typedef struct CustomerStruct  Customer;

struct CustomerStruct {   
    unsigned int        ticksLeft;    //How much longer they will wait to be served
    BreadType*          breadOrder;   //What toast do they want
    Customer*           nextCustomer; //Who is after them in the queue; singly linked list
};

// This will be pointed to by a GameComponent
struct CustomerBaseStruct {
    Customer*           rootCustomer;  // This is not a real customer, but a constant placeholder
    unsigned char       customerCount; // Count of real customers
};
typedef struct CustomerBaseStruct CustomerBase;

void redraw_customers(CustomerBase *);
void customer_func(GameComponent* , GameParameters* );
#endif