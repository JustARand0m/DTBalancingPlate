#include "TouchPlate.h"

/**
 * This interrupt routine gets executed when the ADC returns with an result after getting triggered with ADC_MEASUREMENT_StartConversion.
 * After receiving the value of the x axis, the ADC needs be re-triggered to swap 2 pins to access the y axis and call this routine again.
 * This routine sends the x and y coordinates via UART.
 *
 * The PIN mapping on the Panel:
 * 2.15 => PIN 1 (UR)
 * VDD 3.3V => PIN 2 (LR)
 * GND => PIN 4 (UL)
 * 2.14 => PIN 5 (LL)
 */
void irAdcMeasurementPlate()
{
	static uint16_t x = 0, y = 0;

	/* Read out conversion results */
    uint16_t result_Channel_A = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Channel_A_handle);

    if (DIGITAL_IO_GetInput(&DIGITAL_IO_PLATE_UR) == 1) {
    	x = result_Channel_A;

    	/* Swap x and y axis on the panel */
        DIGITAL_IO_ToggleOutput(&DIGITAL_IO_PLATE_LL);
        DIGITAL_IO_ToggleOutput(&DIGITAL_IO_PLATE_UR);

    } else {
    	y = result_Channel_A;

    	/* Swap x and y axis on the panel */
        DIGITAL_IO_ToggleOutput(&DIGITAL_IO_PLATE_LL);
        DIGITAL_IO_ToggleOutput(&DIGITAL_IO_PLATE_UR);
        
        /* minimal value is checked*/
        x -= 1500;
        y -= 1500;

        if (x<0) x = 0;
        if (y<0) y = 0;



        /* Transmit the x and y position via UART */
        const int BUFFSZ = 12;
        uint8_t buff[BUFFSZ];

        /* Message data format */
        sprintf((char*)buff, "$%04d %04d!", x, y);

        UART_Transmit(&UART_0, buff, BUFFSZ);
    }
}


/**
 * Interrupt routine for Timer_Plate_Scan.
 * This handler starts the measurement of the plate.
 */
void irTimerPlateScan() {
	ADC_MEASUREMENT_StartConversion(&ADC_MEASUREMENT_0);
	TIMER_ClearEvent(&TIMER_PLATE_SCAN);
}
