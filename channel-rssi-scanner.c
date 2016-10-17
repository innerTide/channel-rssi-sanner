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
 *         Yuefeng Wu, y.wu.5@student.tue.nl
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

#define AVERAGE_COUNT 32
#define WORKING_SCAN_COUNT 16

/*---------------------------------------------------------------------------*/
/* This is a set of global variables for this laboratory*/

// int currentChannel = 26;

/*---------------------------------------------------------------------------*/
/* This is a function to calculate average RSSI*/
int cc2420_average_rssi(){
    int averagedRssi = 0;
    int i = 0;
    while(i < AVERAGE_COUNT){
        averagedRssi = averagedRssi + cc2420_rssi();
        i = i + 1;
    }
    return averagedRssi/AVERAGE_COUNT;
}

/*---------------------------------------------------------------------------*/
/* This is a function to calculate approximate working RSSI based on daul-scan*/
int cc2420_working_rssi(){
    int rssiA = -100;
    int rssiB = -100;
    int i,tmpRssi;
    for (i = 0; i < WORKING_SCAN_COUNT; i++){
        tmpRssi = cc2420_rssi();
        if (tmpRssi>rssiB){
            if (tmpRssi>rssiA){
                rssiB = rssiA;
                rssiA = tmpRssi;
            }
            else{
                rssiB = tmpRssi;
            }
        }
    }
    return (rssiA + rssiB)/2;
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
            int i;
            for (i = 11; i<27; i++){
                cc2420_set_channel(i);
                int rssiAverage = cc2420_average_rssi() + 100;
                int rssiWorking = cc2420_working_rssi() + 100;
                if (rssiAverage||rssiWorking){
                    printf("CH-%d RSSI: %d,[%d]\n",i,rssiAverage,rssiWorking);
                }
            }
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
