//////////////////////////////////////////////////////////
//
// Customers
//
//////////////////////////////////////////////////////////

// Can be compiled with:
// zcc +zx -vn -startup=1 -clib=sdcc_iy -D_TEST_CUSTOMER bread.c util.c music.c customer.c -o customer -create-app

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arch/zx.h>

#include "customer.h"
#include "bread.h"
#include "util.h"
#include "music.h"
#include "face.h"

unsigned int reputation_to_waittime(int reputation) {
    return MAX(50, MIN(2000, 16384 / (reputation / 8 + 1)));
}

void redraw_customers(CustomerBase *base) {
    unsigned char ypos = CUSTOMER_LIST_YPOS;
    Customer* lastCustomer = base->rootCustomer;
    while (lastCustomer->nextCustomer != NULL) {
        lastCustomer = lastCustomer->nextCustomer;
        const char* mood = (lastCustomer->customerMood != 0? ANGRY_CUSTOMER : HAPPY_CUSTOMER);
        printf(PRINTAT"%c%c%s""%-12s", CUSTOMER_LIST_XPOS, ypos, mood, lastCustomer->breadOrder->desc);
        ++ypos;
    }
    for (; ypos < CUSTOMER_LIST_YPOS + MAX_CUSTOMER_COUNT; ++ypos) {
        //Now we are updating attributes each time, we can just make the blank parts
        //of the list white ink on white paper and save a bunch of pixel updates
        //printf(PRINTAT"\x12%c"HAPPY_CUSTOMER"%-12s", ypos, "");
        memset((void *)(ATTRSTART + 32 * (ypos - 1) + CUSTOMER_LIST_XPOS - 1), 
                PAPER_WHITE + INK_WHITE, 12); //'\x3F'
    }
    //IO system remembers the ink and paper
    printf(PRINTAT"\x01\x01%s", HAPPY_CUSTOMER);
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
                params->reputation += 20 // They got served - that is worth something
                        + MAX(0, (50 - abs(bread->toastedness - 100))/ 10) //Decent toast will give a bump
                        + thisCustomer->ticksLeft / 50; // As will speed
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

    // Smoke alarm winds the customers up
    int ticksLeftSubtract = (params->maxToast > BURNT_TOAST? 2 : 1);

    thisCustomer = base->rootCustomer->nextCustomer;
    lastCustomer = base->rootCustomer;
    while (thisCustomer != NULL) {
        thisCustomer->ticksLeft -= ticksLeftSubtract;
        if (thisCustomer->ticksLeft <= 0) {
            //Customer leaves in disgust - reset links
            params->score -= thisCustomer->breadOrder->cost;
            base->customerCount -= 1;
            lastCustomer->nextCustomer = thisCustomer->nextCustomer;
            free(thisCustomer);
            thisCustomer = lastCustomer;
            redraw = 1;
            params->effect = TUNE_EFFECT_BEEP;
            //Big rep hit
            params->reputation -= 100;
        } else if (thisCustomer->ticksLeft < GRUMPY_TICKS && thisCustomer->customerMood == 0) {
            // Customer going grumpy
            thisCustomer->customerMood = 1;
            // Once they are grumpy, even great toast won't cure everything
            params->reputation -= 5;
            redraw = 1;
        }
        lastCustomer = thisCustomer;
        thisCustomer = thisCustomer->nextCustomer;
    }

    //Do we have a new customer? - this is independent of whether we have space
    int waittime = reputation_to_waittime(MAX(1, params->reputation));
    if (rand() % waittime == 0) {
        //Yes we do!
        params->effect = TUNE_EFFECT_SHORT_BEEP;
        if (base->customerCount < MAX_CUSTOMER_COUNT) {
            //Add in a new one then!
            newCustomer = malloc(sizeof(Customer));
            newCustomer->breadOrder = rand_bread_type(params->breadBin);
            newCustomer->nextCustomer = NULL;
            newCustomer->ticksLeft = newCustomer->breadOrder->cost * (8 + rand() % 4);
            newCustomer->customerMood = 0;
            redraw = 1;
            base->customerCount += 1;
            lastCustomer->nextCustomer = newCustomer;
        }
    }

    if (redraw) {
        screenFace(21, 1, params->reputation);
        redraw_customers(base);
        if (params->reputation < 0) {
            params->gameOverFlag = 1;
            params->messageAddress = 999;
            params->message = INK"\x32"PAPER"\x36"FLASHON
            " You have ruined the reputation \n"
            "      of the Hotel Excess!      \n"
            "        YOU ARE FIRED!!         ";
        }
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
                                bin, //breadBin
                                100
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