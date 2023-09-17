#ifndef _DEFINE_CUSTOMER_H
#define _DEFINE_CUSTOMER_H

#include "game.h"
#include "bread.h"

#define MAX_CUSTOMER_COUNT 6
#define ANGRY_CUSTOMER "\x10\x37\x11\x32"
#define HAPPY_CUSTOMER "\x10\x30\x11\x37"
#define GRUMPY_TICKS 200
#define CUSTOMER_LIST_XPOS 0x12
#define CUSTOMER_LIST_YPOS 0x04

struct CustomerStruct;

// 2.) typedef of the structure
// Still no details on the members.
typedef struct CustomerStruct  Customer;

struct CustomerStruct {   
    unsigned int        ticksLeft;    //How much longer they will wait to be served
    BreadType*          breadOrder;   //What toast do they want
    Customer*           nextCustomer; //Who is after them in the queue; singly linked list
    unsigned  char      customerMood; //zero if the customer is happy
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