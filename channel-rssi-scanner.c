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
#include "sys/etimer.h"
#include "dev/leds.h"
#include "net/netstack.h"
#include "cc2420.h"
#include "cc2420_const.h"
#include <stdlib.h>
#include "dev/spi.h"
#include <stdio.h>
#include <string.h>


/*---------------------------------------------------------------------------*/
/* This is a set of predefined values for this laboratory*/

#define RSSI_THRESHOLD 20

/*---------------------------------------------------------------------------*/
/* This is a set of global variables for this laboratory*/

int currentChannel = 26;


/*---------------------------------------------------------------------------*/
PROCESS(channel_scanner, "Channel Scanner");
AUTOSTART_PROCESSES(&channel_scanner);


PROCESS_THREAD(channel_scanner, ev, data)
{
	static struct etimer etScan;
	
	
	PROCESS_BEGIN();
	/* switch mac layer off, and turn radio on */

	
	NETSTACK_MAC.off(0);
	cc2420_on();
        
	
	while(1) {
		etimer_set (&etScan, CLOCK_SECOND*30);
                cc2420_set_channel(currentChannel);
                int rssi = cc2420_rssi() + 100;
                if (rssi != 0){
                    printf("CH-%d RSSI: %d\n",currentChannel,rssi);
                }
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
