/********************************************************
 * Host side code to load the adc.bin and sync.bin 		*
 * image to PRU0 and PRU1, and read buffers received	*
 *														*
 * Compile with:                                      	*
 * 	gcc -o main main.c -lprussdrv -lpthread   			*
 * Note: adc.bin and sync.bin must be in the current 	*
 * directory.   										*
 *														*
 * Author: Gustavo Nicolau Gonçalves					*
 ********************************************************/

//-----------------------------------------------------
//                       Headers
//-----------------------------------------------------

// Standard C headers
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
// PRU specific headers
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

//-----------------------------------------------------
//                     Definitions
//-----------------------------------------------------

#define PRU_0 0	// PRU0
#define PRU_1 1	// PRU1

static void *mem;
static unsigned char *mem_byte;

unsigned char fifo[512][6144];		
unsigned int fifoctr = 0;		// FIFO counter
unsigned int fifopush = 0;		// FIFO position for pushing data in
unsigned int fifopop = 0;		// FIFO position for popping data out
pthread_mutex_t fifomutex;		// mutex for FIFO use in threads

int socket_desc , client_sock , c;
struct sockaddr_in server , client;

//--------------------------------------------------------------------------------------
//	receive_block():
//	Waits for interruption copy 6 kb of memory containing the one buffer of ADC data
//	to FIFO structure, update number of blocks to be sent, update pointers of FIFO and
//	clear all events from PRU.
//--------------------------------------------------------------------------------------

void *receive_block(void *args) 
{	
	int buffer = 0;

	while(1)
	{
		prussdrv_pru_wait_event (PRU_EVTOUT_0);
		//memcpy(fifo[fifopush], mem_byte, 6144);

		if(buffer == 0)
		{
			memcpy(fifo[fifopush], &mem_byte[0], 6144);

		}else if(buffer == 1)
		{
			memcpy(fifo[fifopush], &mem_byte[6144], 6144);
		}

		buffer = !buffer;
		
		
		pthread_mutex_lock(&fifomutex);
		fifoctr++; 									//Update number of blocks to be sent to client		
		pthread_mutex_unlock(&fifomutex);

		if(fifopush < 511) fifopush++;				//Update fifo pointer to push data in
		else fifopush = 0;

		prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
	}

}

//--------------------------------------------------------------------------------------
//	print_block():
//	Verify if there are blocks to be print, if so update blocks to be print, print 6 kB.
//  Update poiters of FIFO, and go back to loop.
//	Used as a test.
//--------------------------------------------------------------------------------------

void *print_block(void *args) 
{
	int bank = 0;
	while(1)
	{
		if(fifoctr > 0)
		{
			pthread_mutex_lock(&fifomutex);
	   		fifoctr--;							//Update number of blocks to be sent to client           
			pthread_mutex_unlock(&fifomutex);

			for(i = 0; i < 6144; i++)
			{
       			fprintf(stdout, "%d ", fifo[fifopush][i]);
			}
		
		}
	}
}

//------------------------------------//
//--------------- MAIN ---------------//
//------------------------------------//

int main(int argc, char **argv)
{

	unsigned int i, ret;
	
	pthread_t td_RB , td_PB;				// threads (Receive_Block and Print_Block)
	pthread_mutex_init(&fifomutex, NULL);	// mutex for FIFO access

//---------------------------------//
//------ PRUs inicialization ------//
//---------------------------------//

// PRUSS_INTC_INITDATA structure definition on pruss_intc_mapping.h

	 tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

// initialize the PRU

	prussdrv_init();

// open PRU interrupt

	ret = prussdrv_open(PRU_EVTOUT_0);
    	if(ret)
	{
        	printf("prussdrv_open open failed\n");
        	return (ret);
    	}
    	ret = prussdrv_open(PRU_EVTOUT_1);

	if(ret)
	{
        	printf("prussdrv_open open failed\n");
        	return (ret);
    	}

// map PRU RAM memory

    prussdrv_map_prumem(4, &mem);	// PRU SHARED RAM
    mem_byte = (unsigned char*) mem;

// flush the values in the PRU data memory locations
// PRU DRAM size = 8 kB

    for(i = 0; i < 12288; i++){ 
		mem_byte[i] = 0x00;
	}

// get the interrupt initialized

	prussdrv_pruintc_init(&pruss_intc_initdata);

// execute .bin file on PRU

	prussdrv_exec_program(PRU_0, "./adc.bin");

	prussdrv_exec_program(PRU_1, "./sync.bin");

//------------------------------------//
//------ Threads inicialization ------//
//------------------------------------//

	pthread_create(&td_RB, NULL, &receive_block, NULL);
	pthread_create(&td_PB, NULL, &print_block, NULL);

//  wait for the magic to happen

	while(1);

// finish PRU programs

	prussdrv_pru_disable(PRU_0);
	prussdrv_pru_disable(PRU_1);
	prussdrv_exit();
}
