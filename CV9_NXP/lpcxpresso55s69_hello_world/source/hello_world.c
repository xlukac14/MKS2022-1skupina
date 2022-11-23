/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "fsl_power.h"
#include "math.h"
#include "string.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
uint32_t DWT1, DWT2;
uint32_t result, x=18, y=0;
char password_stored[20] = "5005";
char input[20];

uint32_t strcmp_safe(const char *input, const char *password){
	uint32_t counter1, counter2 = 0;
	for(uint32_t i = 0; i < 4; i++){

		if(input[i] == password[i]) {
			counter1++;
		}
		else {
			counter2++;
		}
	}
	return counter2;
}

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    char ch;

    /* Init board hardware. */
    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
#if !defined(DONT_ENABLE_FLASH_PREFETCH)
    /* enable flash prefetch for better performance */
    SYSCON->FMCCR |= SYSCON_FMCCR_PREFEN_MASK;
#endif


    //PRINTF("hello world.\r\n");
    //DWT1=DWT -> CYCCNT;
    //result = 2*x - sin(y);
    //DWT2=DWT -> CYCCNT;



/*
---------------------------------------------------------
correct|X X X X| V X X X | V V X X | V V V X | V V V V |
---------------------------------------------------------
  687  |  693  |   690   |   689   |   688   |   687   |
---------------------------------------------------------
*/


    while (1)
    {


    	PRINTF("\r\nEnter password: ");
    	SCANF("%s", input);

    	DWT1=DWT -> CYCCNT;

    	//result = strcmp(input, password_stored);

    	result = strcmp_safe(input, password_stored);

    	DWT2=DWT -> CYCCNT;

    	PRINTF("\r\ninput: %s", input);

    	if(result == 0)
    	{
    		PRINTF("\r\ninput correct");
    	} else {
    		PRINTF("\r\ninput incorrect");
    	}

    	PRINTF("\r\nCycles in function: %d", DWT2-DWT1);
       // ch = GETCHAR();
       // PUTCHAR(ch);


    }
}
