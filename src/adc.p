/********************************************************
 * BBB PRU ADC Reader - adc.p 							*
 * Code to work as a ADC reader, reading the input from *
 * AIN0 pin and writting data into the PRU SHARED RAM 	*
 * data memory so the CPU can read.						*
 *														*
 * Compile with: pasm -V3 -b adc.p 						*
 * - adc.hp must be in the same directory 				*
 *														*
 * The data read from channel1(AIN0) ADC module is 		*
 * stored in two buffers of 6kb of memory each.			*
 *														*
 * BeagleBone Black ADC module pins support voltage in 	*
 * range of 0V to 1.8V, DO NOT EXCEED THIS VALUE!		*
 *														*
 * Author: Gustavo Nicolau Gonçalves					*
 ********************************************************/

#include "adc_reader.hp"

//------Beginning of the code ------//

	.origin 0
	.entrypoint START

START:

//------ Initial Configurations ------//

	MEMCONF

//------ Configures ADC ------//

	ADCCONF
	
//------ Variables initial values ------//

	MOV	MEM_OFFSET, 1			//starts at position 0 of RAM.
	MOV	ADCVALUE, 0				//starts with 0.
	MOV	TOTALBW, 0				//starts with 0 bytes written.
	MOV	B0_END, 6143			//fixed value of 6143, last position of bank 0.
	MOV	B1_END, 12287			//fixed value of 12287, last position of bank 1.
	MOV	FIFO_ADD, 0x44E0D100	//fixed value of FIFO_0 address.

//-----------------------------------------------------------------------------------------------------//
//------------------------------------------ Main Loop ------------------------------------------------//
//---------- Sincronizes with PRU1, arm the trigger for the next ADC reading of channel1, reads the ---//
//---------- value of current reading and save it in RAM. Ram is divided in two banks of 6 kb. --------//
//---------- When BUFFER0 or BUFFER1 are full an interruption is generated to the loader program.------//
//-----------------------------------------------------------------------------------------------------//

SAMPLE:
	XIN 14,SYNC,4 										//Wait for the other PRU send timing signal. This make the sample rate more consistant.

	TRIGGER												//Arm the trigger for the next analog-to-digital conversion of channel1.

	ADCSAMPLE FIFO, ADCVALUE, MEM_OFFSET, TOTALBW		//Read ADC value, save this value in RAM, update TOTALBW and MEM_OFFSET.

	QBEQ	BUFFER0, TOTALBW, BUFFER0_END				//If offset is pointing to last position of Buffer 0, arm the trigger to read from channel2

	QBEQ	BUFFER1, TOTALBW, BUFFER1_END				//If offset is pointing to last position of Buffer 1, arm the trigger to read from channel2

	QBA	SAMPLE

BUFFER0:

	MOV r31.b0, PRU_INTERRUPT+16						//Send signal to loader.c that BANK0 is full.

    QBA	SAMPLE

BUFFER1:
	
	MOV r31.b0, PRU_INTERRUPT+16						//Send signal to loader.c that BANK1 is full.

	MOV	MEM_OFFSET,0									//Reset offset to initial position									
	MOV	TOTALBW, 0

    QBA	SAMPLE											//Reset number of bytes written and Ram offset, then jump to beginning of the loop.

