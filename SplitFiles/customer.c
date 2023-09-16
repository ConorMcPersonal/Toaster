//////////////////////////////////////////////////////////
//
// Customers
//
//////////////////////////////////////////////////////////

// Can be compiled with:
// zcc +zx -vn -startup=1 -clib=sdcc_iy -D_TEST_CUSTOMER bread.c util.c customer.c -o customer -create-app

#include <stdio.h>
#include <stdlib.h>

#include "customer.h"
#include "bread.h"
#include "util.h"

void redraw_customers(CustomerBase *base) {
    unsigned char ypos = 4;
    Customer* lastCustomer = base->rootCustomer;
    while (lastCustomer->nextCustomer != NULL) {
        lastCustomer = lastCustomer->nextCustomer;
        printf(PRINTAT"\x12%c""%-12s", ypos, lastCustomer->breadOrder->desc);
        ++ypos;
    }
    for (; ypos < 4 + MAX_CUSTOMER_COUNT; ++ypos) {
        printf(PRINTAT"\x12%c""%-12s", ypos, "");
    }
}

void customer_func(GameComponent* customers, GameParameters* params) {
    CustomerBase* base = (CustomerBase*)(customers->ptr);
    Customer* newCustomer = (Customer*)NULL;
    int redraw = 0;

    Customer* thisCustomer;
    Customer* lastCustomer;

    if (params->messageAddress == 20) {
        //Someone sent toast!
        BreadState* bread = (BreadState*)params->message;
        thisCustomer = base->rootCustomer->nextCustomer;
        lastCustomer = base->rootCustomer;
        while (thisCustomer != NULL) {
            if (bread->type->letter == thisCustomer->breadOrder->letter) {
                //We have a match
                params->score += bread->type->cost * 2 //SHOCKING markup
                         + 100 - abs(bread->toastedness - 100) //100 toastedness gets 100 bonus points
                         + thisCustomer->ticksLeft;  //Speedy toast = points!
                (params->slices) += 1;
                params->messageAddress = 0;
                params->message = NULL;
                params->messageSourceAddress = NULL;
                //Give back memory
                free(bread);
                //Remove customer
                base->customerCount -= 1;
                lastCustomer->nextCustomer = thisCustomer->nextCustomer;
                free(thisCustomer);
                redraw = 1;
                //end loop
                thisCustomer = NULL;
            } else {
                //Step on
                lastCustomer = thisCustomer;
                thisCustomer = thisCustomer->nextCustomer;
            }
        }
        if (!redraw) {
            // No customer for this toast - bad
            params->score -= (bread->type->cost + bread->toastedness);  //+ wasted energy cost
            params->messageAddress = 0;
            params->message = NULL;
            params->messageSourceAddress = NULL;
            free(bread);
        }

    }

    thisCustomer = base->rootCustomer->nextCustomer;
    lastCustomer = base->rootCustomer;
    while (thisCustomer != NULL) {
        thisCustomer->ticksLeft -= 1;
        if (thisCustomer->ticksLeft <= 0) {
            //Customer leaves in disgust - reset links
            params->score -= thisCustomer->breadOrder->cost;
            base->customerCount -= 1;
            lastCustomer->nextCustomer = thisCustomer->nextCustomer;
            free(thisCustomer);
            thisCustomer = lastCustomer;
            redraw = 1;
        }
        lastCustomer = thisCustomer;
        thisCustomer = thisCustomer->nextCustomer;
    }

    //Do we have a new customer? - this is independent of whether we have space
    if (rand() % 150 == 0) {
        //Yes we do!
        if (base->customerCount < MAX_CUSTOMER_COUNT) {
            //Add in a new one then!
            newCustomer = malloc(sizeof(Customer));
            newCustomer->breadOrder = rand_bread_type(params->breadBin);
            newCustomer->nextCustomer = NULL;
            newCustomer->ticksLeft = newCustomer->breadOrder->cost * (8 + rand() % 4);
            redraw = 1;
            base->customerCount += 1;
            lastCustomer->nextCustomer = newCustomer;
        } else {
            //Something about reputation here - enhanced by lots of people waiting?
            ;
        }
    }

    if (redraw) {
        redraw_customers(base);
    }
}

int customer_main() {
    start_frame_count();
    BreadBin* bin = get_bread_bin();

    Customer root = {0, NULL, NULL};
    CustomerBase base = {&root, 0};

    GameComponent comp = {
        &base,
        &customer_func,
        NULL
    };

    //Now the parameters
    GameParameters params =  { 0, //.ticks = 
                                0,//.score =
                                0,//.slices = 
                                0,//.game_over_flag = 
                                0,//.max_toast = 
                                0, //message_address
                                (void*)NULL,//.message 
                                (void*)NULL,//.messageSourceAddress = 
                                0, // effect
                                bin //breadBin
                            };

    while (1) {
        customer_func(&comp, &params);
        params.ticks += 1;
        if (rand() % 250 == 0) {
            //Send some random toast
            BreadState* randoBread = get_bread(params.breadBin,
                            rand_bread_type(params.breadBin)->letter);
            randoBread->toastedness = rand() % 250;
            params.messageAddress = 20;
            params.message = (void*)randoBread;
            printf(PRINTAT"\x01\x12""Sending %s, %-4d, %-6d", randoBread->type->desc
                                            , randoBread->toastedness, params.ticks);
        }
    }

}



#ifdef _TEST_CUSTOMER
int main() {
    return customer_main();
}
#endif