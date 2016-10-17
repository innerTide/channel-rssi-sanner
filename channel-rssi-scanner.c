/*
 * 
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         Coordinator Source Code for NES project
 * \author
 *         Joakim Eriksson, joakime@sics.se
 *          Adam Dunkels, adam@sics.se
 * 			Yuefeng Wu, y.wu.5@student.tue.nl
 */

#include "contiki.h"
#include "net/rime/rime.h"
#include "dev/leds.h"
#include "net/netstack.h"
#include "cc2420.h"
#include "cc2420_const.h"
#include <stdlib.h>
#include "dev/spi.h"
#include <stdio.h>
#include <string.h>
#include "sys/clock.h"
#include "sys/timer.h"



/*---------------------------------------------------------------------------*/
/* This is a set of predefined values for this laboratory*/

#define RSSI_THRESHOLD 20
#define AVERAGE_COUNT 32
#define TOTAL_DELAY_TIME 1397 /*Unit: uSec*/

/*---------------------------------------------------------------------------*/
/* This is a set of global variables for this laboratory*/

int currentChannel = 26;
static struct timer scanTimer;

/*---------------------------------------------------------------------------*/

/* This is a function to calculate average RSSI*/
int cc2420_average_rssi(){
    int averagedRssi = 0;
    int i = 0;
    int delayPerLoop = TOTAL_DELAY_TIME/AVERAGE_COUNT;
    timer_set(&scanTimer, 1);
    while(i < AVERAGE_COUNT){
        if (timer_expired(&scanTimer)){
            averagedRssi = averagedRssi + cc2420_rssi();
            i = i + 1;
            timer_reset(&scanTimer);
        }
    }
    return averagedRssi/AVERAGE_COUNT;
}
PROCESS(channel_scanner, "Channel Scanner");
AUTOSTART_PROCESSES(&channel_scanner);


PROCESS_THREAD(channel_scanner, ev, data)
{
// 	static struct etimer etScan;
	
	
	PROCESS_BEGIN();
	/* switch mac layer off, and turn radio on */

	
	NETSTACK_MAC.off(0);
	cc2420_on();
        
	
	while(1) {
// 		etimer_set (&etScan, CLOCK_SECOND*30);
//                 cc2420_set_channel(currentChannel);
                int rssi = cc2420_average_rssi() + 100;
                if (rssi != 0){
                    printf("CH-%d RSSI: %d\n",currentChannel,rssi);
                }
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
