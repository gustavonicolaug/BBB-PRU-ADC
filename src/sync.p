/********************************************************
 * BBB PRU ADC Reader - sync.p	 						*	
 * Code to wait for TIME and send a signal to the other *				
 * PRU in order to control the sample rate of ADC 		*
 * module.												*
 *														*
 * Compile with: pasm -V3 -b sync.p					  	*
 * - adc.hp must be in the same directory				*
 *														*
 * Author: Gustavo Nicolau Gonçalves		      		*	 
 ********************************************************/

#include "adc.hp"

//------Beginning of the code ------//

	.origin 0
	.entrypoint START
START:

//------ Initialize Syncronization variable ------//

	MOV SYNC, 0x00000000

//----------------------------------------------------------------------------------------------//
//------------------------------------------ Main Loop -----------------------------------------//
// Loop that waits for 1.25 us and send a signal to PRU0 such that sampling frequency is 800kHz //
//----------------------------------------------------------------------------------------------//

MAIN:

	DELAY				// Delay of 1.25 us.
	
	XOUT 14, SYNC, 4	// Syncronize with PRU0.

	JMP MAIN			// Reset the loop.





