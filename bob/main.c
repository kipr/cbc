/**************************************************************************
 *  Copyright 2008,2009 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of CBC Firmware.                                    *
 *                                                                        *
 *  CBC Firmware is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  CBC Firmware is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with this copy of CBC Firmware.  Check the LICENSE file         *
 *  in the project root.  If not, see <http://www.gnu.org/licenses/>.     *
 **************************************************************************/
 
/*
 * main.c - AT91SAM7 Chumby BOBoard program. 
 * 
 * Copyright (c) 2008 by KIPR 
 * 
 * This file may be distributed and/or modified under the terms of the 
 * GNU General Public License version 2 as published by the Free Software 
 * Foundation. (See COPYING.GPL for details.)
 * 
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 * 
 */

#include "BOBoard.h"
#include "serial.h"
#include "printf.h"
#include "util.h"
#include "dma.h"


// GLOBAL VARIABLE DECLARATION
volatile static unsigned int TenMSCount;
volatile static int startupComplete;
volatile static int PWMportState;
volatile static int Back_EMF[24];
volatile static int MotorCal[8];
volatile static int Motor_Last_Target_Speed[4];
volatile static int Motor_Last_Target_Position[4];
volatile static int Motor_Last_Err[4];	// previous speed the motor was running for Differential term
volatile static int Motor_Next_Position[4];
volatile static int Motor_Err[4];		// accumulated speed error for Integral term
volatile static int Motor_Power[4];	
volatile static int Motor_Max_PID;	// Maximum PID value

volatile static Cbob_Data *CBC_Pack;
volatile static Cbob_Data CBC_InPacket;
volatile static Cbob_Data CBC_OutPacket;
volatile static Cbob_Data CBC_InBuffer;

volatile struct dma_dev spi_dma;

// FUNCTION PROTOTYPES
// external references 
extern void AT91F_Spurious_handler(void); 
extern void AT91F_Default_IRQ_handler(void); 
extern void AT91F_Default_FIQ_handler(void); 

static void Initialize(void);
void InitializePIT(void);
void InitializeADC(void);
int UpdateADC(void);
void UpdateDigitals(void);
void InitializeTWI(void);
void TWIWrite(int addr, char *data, unsigned int NumberOfBytes);
void TWIRead(unsigned char addr, char *data, unsigned int NumberOfBytes);
void TWIUnlockBus(void);
void InitializeSPI(void);
void TogglePin(unsigned long pin);
void PinHigh(int pin);
void PinLow(int pin);
void AllOff(void);
void MotorSetPower(int port, int power);
void CheckSPI();
void CheckInput();
void SystemInterrupt(void);

void ServoPulseOn(int port);
void ServoPulseOff(void);
void WaitMiliSecond (unsigned int MiliSeconds);
void WaitMicroSecond (unsigned int MicroSecond);

/////////////////////////////////////////////////////////////////////////////
#define IRQ_MASK 0x00000080 
#define FIQ_MASK 0x00000040 
#define INT_MASK (IRQ_MASK | FIQ_MASK) 
static inline unsigned __get_cpsr(void) { 
	unsigned long retval; 
	asm volatile (" mrs  %0, cpsr" : "=r" (retval) : /* no inputs */  ); 
	return retval; 
} 
static inline void __set_cpsr(unsigned val) { 
	asm volatile (" msr  cpsr, %0" : /* no outputs */ : "r" (val)  ); 
} 
unsigned disableIRQ(void) { 
	unsigned _cpsr; 
	_cpsr = __get_cpsr(); 
	__set_cpsr(_cpsr | IRQ_MASK); 
	return _cpsr; 
} 
unsigned restoreIRQ(unsigned oldCPSR) { 
	unsigned _cpsr; 
	_cpsr = __get_cpsr(); 
	__set_cpsr((_cpsr & ~IRQ_MASK) | (oldCPSR & IRQ_MASK)); 
	return _cpsr; 
} 
unsigned enableIRQ(void) { 
	unsigned _cpsr; 
	_cpsr = __get_cpsr(); 
	__set_cpsr(_cpsr & ~IRQ_MASK); 
	return _cpsr; 
} 
unsigned disableFIQ(void) { 
	unsigned _cpsr; 
	_cpsr = __get_cpsr(); 
	__set_cpsr(_cpsr | FIQ_MASK); 
	return _cpsr; 
} 
unsigned restoreFIQ(unsigned oldCPSR) { 
	unsigned _cpsr; 
	_cpsr = __get_cpsr(); 
	__set_cpsr((_cpsr & ~FIQ_MASK) | (oldCPSR & FIQ_MASK)); 
	return _cpsr; 
} 
unsigned enableFIQ(void) { 
	unsigned _cpsr; 
	_cpsr = __get_cpsr(); 
	__set_cpsr(_cpsr & ~FIQ_MASK); 
return _cpsr; 
}
/////////////////////////////////////////////////////////////////////////////

// NOTE! Must be compiled in ARM mode as it is directly called from ISR. 
/*void AT91F_Default_IRQ_handler(void)
{
	// Do nothing.
	printf("DI\r\n");
}
*/
// NOTE! Must be compiled in ARM mode as it is directly called from ISR. 
/*void AT91F_Spurious_handler(void)
{
	// Never do anything; just return as quickly as possible. 
}
*/
// Endless loop of DIGITAL0 (PA30) blinks for error diagnosis. 
// Will blink code-many times and then make a longer delay. 
// NOTE: Must be compiled in ARM mode. 
extern void PanicBlinker(uint8 code);
void PanicBlinker(uint8 code)
{
	// Be sure to enable the output so that we can also diagnose 
	// crashes which happen early. 
	volatile AT91PS_PIO pPIO = AT91C_BASE_PIOA;
	pPIO->PIO_PER |= DIGITAL0;
	pPIO->PIO_OER |= DIGITAL0;
	pPIO->PIO_SODR = DIGITAL0;
	
	for(;;)
	{	
		uint8 i;
		unsigned int j;
		for(i=0; i<code; i++)
		{
			pPIO->PIO_CODR = DIGITAL0;  // LOW = turn LED on. 
			for(j=300000; j; j--)  nop();
			pPIO->PIO_SODR = DIGITAL0;  // HIGH = turn LED off. 
			for(j=300000; j; j--)  nop();
		}
		
		for(j=300000*3; j; j--)  nop();
	}	
}

// Hardware initialization function. 
static void Initialize(void)
{
    // Set Flash Wait state
	// Single Cycle Access at Up to 30 MHz, above (up to 55MHz): 
	//   at least 1 flash wait state. 
	// FMCN: flash microsecond cycle number: Number of MCLK cycles in one usec. 
	//   For 20 MHz, this is 20. Value must be rounded up. 
	
	AT91C_BASE_MC->MC_FMR = ((AT91C_MC_FMCN)&(50 <<16)) | AT91C_MC_FWS_1FWS;
	
    // Disable watchdog. 
	AT91C_BASE_WDTC->WDTC_WDMR= AT91C_WDTC_WDDIS;
	
	AT91PS_PMC pPMC = AT91C_BASE_PMC;
	// set MCK at
	// After reset, CPU runs on slow clock (32kHz).
	// 1 Enabling the Main Oscillator
		// Enable main oscillator running on crystal by setting AT91C_CKGR_MOSCEN bit in PMC_MOR.
		// Slow Clock = 1/32768 = 30.51 uSecond
		// Hence, start up time = 8 * OSCOUNT / 32768 ; result in seconds.
		// startup time = 8*8/SCK = 48 / 32768 = 1.953125 ms
		// OSCOUNT = 8 -> 1.953125 ms. 
		pPMC->PMC_MOR = (( AT91C_CKGR_OSCOUNT & (0x08 <<8) | AT91C_CKGR_MOSCEN ));
		// Wait the startup time for the oscillator to stabilize...
		while(!(pPMC->PMC_SR & AT91C_PMC_MOSCS));
	// 2 Check the Main Oscillator Frequency (optional)
	// 3 Setup PLL and divider
		// Fin = 20 MHz / DIV =	20 / 4 = 5	(1 < Fin < 32 MHz)
		// Fout = Fin * (MUL+1) = 5 * (19+1) = 100 (80 < Fout < 160 MHz)
		// PLLCOUNT = 
		pPMC->PMC_PLLR = AT91C_CKGR_OUT_0 |
					(AT91C_CKGR_PLLCOUNT & (40<<8)) |
					(AT91C_CKGR_MUL & (19<<16)) |
					(AT91C_CKGR_DIV & 4);

		// Wait the startup time (until PMC Status register LOCK bit is set)
		while(!(pPMC->PMC_SR & AT91C_PMC_LOCK)) nop();
		while(!(pPMC->PMC_SR & AT91C_PMC_MCKRDY)) nop();
	// 4 Select Master Clock (MCK) and Processor Clock
		// Bit 0,1: clock source (CSS): 
		//		AT91C_PMC_CSS_SLOW_CLK (0) -> slow clock
		//		AT91C_PMC_CSS_MAIN_CLK (1) -> main clock
		//							   (2) -> res
		//		AT91C_PMC_CSS_PLL_CLK  (3) -> PLL clock 
		// Bit 2..4: Processor clock prescaler: Divide selected clock by 1,2,4,8,16,32,64: 
		//		AT91C_PMC_PRES_CLK, AT91C_PMC_PRES_CLK_2, AT91C_PMC_PRES_CLK_4,...
		// We need to do this in 2 steps conforming to data sheet p.217. 
		// STEP 1: 
		// set prescaler to divide PLL clock by 2
		pPMC->PMC_MCKR = AT91C_PMC_PRES_CLK_2;
		while (!(pPMC->PMC_SR & AT91C_PMC_MCKRDY)) nop();
		// STEP 2:
		// turn on PLL clock, makes MCK 50 MHz
		pPMC->PMC_MCKR |= AT91C_PMC_CSS_PLL_CLK;
		while(!(pPMC->PMC_SR & AT91C_PMC_MCKRDY)) nop();
	
	// Enable user reset, i.e. allow the controller to be reset by 
	// pulling the NRST pin LOW. This aids in debugging. 
	AT91C_BASE_RSTC->RSTC_RMR = 0xa5000400U | AT91C_RSTC_URSTEN;
	
	// Set up the default interrupt handlers for all interrupts. 
	// 0 = FIQ, 1 = SYS. 
	int i;
	AT91C_BASE_AIC->AIC_SVR[0] = (int) AT91F_Default_FIQ_handler;
	for(i=1; i<31; i++)
	{  AT91C_BASE_AIC->AIC_SVR[i] = (int) AT91F_Default_IRQ_handler;  }
	// Set spurious interrupt handler. This does nothing and just returns 
	// quickly. 
	AT91C_BASE_AIC->AIC_SPU = (int) AT91F_Spurious_handler;

	
	volatile AT91PS_PIO	pPIO = AT91C_BASE_PIOA;

//	**************************************************************
	// SETUP DIGITAL IO PINS FOR CHUMBY BEND SENSOR, RESET, PWM_RESET, and BUTTON
	pPIO->PIO_PER = RST_IOPIN | BEND_IOPIN | PWM_RESET | BUTTON; // set IO enable register to allow control 
	pPIO->PIO_OER = RST_IOPIN | BEND_IOPIN | PWM_RESET;			// set output enable register to allow pin to be driven
	pPIO->PIO_ODR = BUTTON;										// set output disable register to allow pin to be input
	pPIO->PIO_IFER = BUTTON;									// set glitch filter on input pins
	pPIO->PIO_PPUDR = BUTTON;									// no pullup resistors enabled
	pPIO->PIO_IDR = BUTTON;										// disable input pin inturrupt
	pPIO->PIO_SODR = RST_IOPIN;									// drive pins HIGH
	pPIO->PIO_CODR = BEND_IOPIN | PWM_RESET;					// drive pins LOW
	
//	**************************************************************
	// ACC G select setup see BOBoard.h for selection details, range is 1.5-6 G's
	pPIO->PIO_PER = ACC_GSEL1 | ACC_GSEL2;
	pPIO->PIO_OER = ACC_GSEL1 | ACC_GSEL2;
	
//	****************************************************************************************
	// Setup DIGITAL PORTS. 
	
	// Allow PIO to control DIGITAL PORT. 
	pPIO->PIO_PER = DIGITAL0 | DIGITAL1 | DIGITAL2 | DIGITAL3 | DIGITAL4 | DIGITAL5 | DIGITAL6 | DIGITAL7;
	// Enable inputs for DIGITAL pins. 
	pPIO->PIO_ODR = DIGITAL0 | DIGITAL1 | DIGITAL2 | DIGITAL3 | DIGITAL4 | DIGITAL5 | DIGITAL6 | DIGITAL7;
	// Disable pullup resistors.
	pPIO->PIO_PPUDR = DIGITAL0 | DIGITAL1 | DIGITAL2 | DIGITAL3 | DIGITAL4 | DIGITAL5 | DIGITAL6 | DIGITAL7;
	// Set outputs HIGH to turn DIGITAL port off. 
	//pPIO->PIO_SODR = DIGITAL0 | DIGITAL1 | DIGITAL2 | DIGITAL3 | DIGITAL4 | DIGITAL5 | DIGITAL6 | DIGITAL7;
	
	pPMC->PMC_PCER = (1U<<2); // turn on peripheral IO clock
}

//////////////////////////////////////////////////////////////////////////////////
// Initialize Periodic Interval Timer
void InitializePIT(void)
{
	volatile AT91PS_PITC pPIT = AT91C_BASE_PITC;
	volatile AT91PS_AIC pAic = AT91C_BASE_AIC;
	
	//	Periodic interval timer setup
	pAic->AIC_IDCR = (1<<AT91C_ID_SYS);			// disable interrupt for SYSTEM Peripheral
	pAic->AIC_SVR[AT91C_ID_SYS] = (int)SystemInterrupt;			// setup interrupt vector function
	pAic->AIC_SMR[AT91C_ID_SYS] = AT91C_AIC_SRCTYPE_INT_POSITIVE_EDGE | 0x7; // interrupt source type and priority
	pAic->AIC_ICCR = (1<<AT91C_ID_SYS); // clear the interrupt on the AIC
	
	pAic->AIC_IECR = (1<<AT91C_ID_SYS); // enable the interrupt on the AIC
	enableIRQ();

	// set the timer interval value (10ms) and enable the interrupt and start the timer
	pPIT->PITC_PIMR = (MCK/1600) | AT91C_PITC_PITIEN | AT91C_PITC_PITEN; 
}

//////////////////////////////////////////////////////////////////////////////////
// Initialize Serial Peripheral Interface
void InitializeSPI(void)
{
	volatile AT91PS_SPI pSPI = AT91C_BASE_SPI;
	volatile AT91PS_PMC pPMC = AT91C_BASE_PMC;
	volatile AT91PS_PIO pPIO = AT91C_BASE_PIOA;
	
	pPIO->PIO_PDR = SPI_CS | SPI_MISO | SPI_MOSI | SPI_SCK; // disable PIO pins 11-14
	pPIO->PIO_ASR = SPI_CS | SPI_MISO | SPI_MOSI | SPI_SCK; // select Peripheral A for pins 11-14
	
	pPMC->PMC_PCER = (1<<AT91C_ID_SPI);		// enable SPI clock
	
	pSPI->SPI_CR = AT91C_SPI_SPIEN;
	pSPI->SPI_MR = 0;
	pSPI->SPI_CSR[0] = AT91C_SPI_CPOL;
}

//////////////////////////////////////////////////////////////////////////////////
// Internal ADC functions
void InitializeADC(void)
{
	volatile AT91PS_PMC pPMC = AT91C_BASE_PMC;
	volatile AT91PS_ADC iADC = AT91C_BASE_ADC;
	
	// SETUP ADC:
	iADC->ADC_MR =	AT91C_ADC_TRGEN_DIS |			/* disable conversion trigger */
					AT91C_ADC_LOWRES_10_BIT |		/* 10-bit resolution */
					AT91C_ADC_SLEEP_MODE |			/* turn off clock between conversions, save power */
					((MCK/(ADCCLOCK)/2-1) << 8) |	/* clock presacler */
					(ADCSTARTUP << 16) |			/* startup time */
					(ADCSAMPHOLD << 24);			/* sample and hold */
					
	// ENABLE ADC channels
	iADC->ADC_CHER = 0xff; // all channels
	
	// TURN on ADC clock
	pPMC->PMC_PCER = (1U<<4);
}
int UpdateADC(void)
{
	AT91PS_ADC iADC = AT91C_BASE_ADC;
	
	// START conversion
	iADC->ADC_CR = AT91C_ADC_START;
	
	while(!(iADC->ADC_SR & AT91C_ADC_DRDY)) nop();
	
	CBC_Pack->Analog[0] = ANALOG0;
	CBC_Pack->Analog[1] = ANALOG1;
	CBC_Pack->Analog[2] = ANALOG2;
	CBC_Pack->Analog[3] = ANALOG3;
	CBC_Pack->Analog[4] = ANALOG4;
	CBC_Pack->Analog[5] = ANALOG5;
	CBC_Pack->Analog[6] = ANALOG6;
	CBC_Pack->Analog[7] = ANALOG7;
	return 1;
}
//////////////////////////////////////////////////////////////////////////////////
// Internal Digital functions
void UpdateDigitals(void)
{
	static int previous_direction = 0;
	static int previous_state = 0;
	volatile AT91PS_PIO pPIO = AT91C_BASE_PIOA;
	
	if(previous_direction != CBC_Pack->Enable_Digital_Outputs){
		if(CBC_Pack->Enable_Digital_Outputs & 0x1) pPIO->PIO_OER = DIGITAL0;
		else pPIO->PIO_ODR = DIGITAL0;
		if(CBC_Pack->Enable_Digital_Outputs & 0x2) pPIO->PIO_OER = DIGITAL1;
		else pPIO->PIO_ODR = DIGITAL1;
		if(CBC_Pack->Enable_Digital_Outputs & 0x4) pPIO->PIO_OER = DIGITAL2;
		else pPIO->PIO_ODR = DIGITAL2;
		if(CBC_Pack->Enable_Digital_Outputs & 0x8) pPIO->PIO_OER = DIGITAL3;
		else pPIO->PIO_ODR = DIGITAL3;
		if(CBC_Pack->Enable_Digital_Outputs & 0x10) pPIO->PIO_OER = DIGITAL4;
		else pPIO->PIO_ODR = DIGITAL4;
		if(CBC_Pack->Enable_Digital_Outputs & 0x20) pPIO->PIO_OER = DIGITAL5;
		else pPIO->PIO_ODR = DIGITAL5;
		if(CBC_Pack->Enable_Digital_Outputs & 0x40) pPIO->PIO_OER = DIGITAL6;
		else pPIO->PIO_ODR = DIGITAL6;
		if(CBC_Pack->Enable_Digital_Outputs & 0x80) pPIO->PIO_OER = DIGITAL7;
		else pPIO->PIO_ODR = DIGITAL7;
	}
	previous_direction = CBC_Pack->Enable_Digital_Outputs;
	
	if(previous_state != CBC_Pack->Digital_Output_Values){
		if(CBC_Pack->Digital_Output_Values & 0x1) pPIO->PIO_SODR = DIGITAL0;
		else pPIO->PIO_CODR = DIGITAL0;
		if(CBC_Pack->Digital_Output_Values & 0x2) pPIO->PIO_SODR = DIGITAL1;
		else pPIO->PIO_CODR = DIGITAL1;
		if(CBC_Pack->Digital_Output_Values & 0x4) pPIO->PIO_SODR = DIGITAL2;
		else pPIO->PIO_CODR = DIGITAL2;
		if(CBC_Pack->Digital_Output_Values & 0x8) pPIO->PIO_SODR = DIGITAL3;
		else pPIO->PIO_CODR = DIGITAL3;
		if(CBC_Pack->Digital_Output_Values & 0x10) pPIO->PIO_SODR = DIGITAL4;
		else pPIO->PIO_CODR = DIGITAL4;
		if(CBC_Pack->Digital_Output_Values & 0x20) pPIO->PIO_SODR = DIGITAL5;
		else pPIO->PIO_CODR = DIGITAL5;
		if(CBC_Pack->Digital_Output_Values & 0x40) pPIO->PIO_SODR = DIGITAL6;
		else pPIO->PIO_CODR = DIGITAL6;
		if(CBC_Pack->Digital_Output_Values & 0x80) pPIO->PIO_SODR = DIGITAL7;
		else pPIO->PIO_CODR = DIGITAL7;
	}
	previous_state = CBC_Pack->Digital_Output_Values;
	
	if(pPIO->PIO_PDSR & DIGITAL0) CBC_Pack->Digitals |= 0x1;
	else CBC_Pack->Digitals &= ~0x1;
	if(pPIO->PIO_PDSR & DIGITAL1) CBC_Pack->Digitals |= (0x1 << 1);
	else CBC_Pack->Digitals &= ~(0x1 << 1);
	if(pPIO->PIO_PDSR & DIGITAL2) CBC_Pack->Digitals |= (0x1 << 2);
	else CBC_Pack->Digitals &= ~(0x1 << 2);
	if(pPIO->PIO_PDSR & DIGITAL3) CBC_Pack->Digitals |= (0x1 << 3);
	else CBC_Pack->Digitals &= ~(0x1 << 3);
	if(pPIO->PIO_PDSR & DIGITAL4) CBC_Pack->Digitals |= (0x1 << 4);
	else CBC_Pack->Digitals &= ~(0x1 << 4);
	if(pPIO->PIO_PDSR & DIGITAL5) CBC_Pack->Digitals |= (0x1 << 5);
	else CBC_Pack->Digitals &= ~(0x1 << 5);
	if(pPIO->PIO_PDSR & DIGITAL6) CBC_Pack->Digitals |= (0x1 << 6);
	else CBC_Pack->Digitals &= ~(0x1 << 6);
	if(pPIO->PIO_PDSR & DIGITAL7) CBC_Pack->Digitals |= (0x1 << 7);
	else CBC_Pack->Digitals &= ~(0x1 << 7);
}
void TogglePin(unsigned long pin)
{
	volatile AT91PS_PIO pPIO = AT91C_BASE_PIOA;
	if(pPIO->PIO_ODSR & pin)
		pPIO->PIO_CODR = pin;
	else
		pPIO->PIO_SODR = pin;
}
void ToggleDigitalPort(unsigned int port)
{
	volatile AT91PS_PIO pPIO = AT91C_BASE_PIOA;
	unsigned long pin=0;
	
	switch (port) {
		case 0:
			pin = DIGITAL0;
			break;
		case 1:
			pin = DIGITAL1;
			break;
		case 2:
			pin = DIGITAL2;
			break;
		case 3:
			pin = DIGITAL3;
			break;
		case 4:
			pin = DIGITAL4;
			break;
		case 5:
			pin = DIGITAL5;
			break;
		case 6:
			pin = DIGITAL6;
			break;
		case 7:
			pin = DIGITAL7;
			break;
		default:
			return;
	}
	
	if(pPIO->PIO_ODSR & pin)
		pPIO->PIO_CODR = pin;	// drive pins LOW
	else
		pPIO->PIO_SODR = pin;	// drive pins HIGH
}
void PinHigh(int pin)
{
	volatile AT91PS_PIO pPIO = AT91C_BASE_PIOA;
	pPIO->PIO_SODR = pin;
}
void PinLow(int pin)
{
	volatile AT91PS_PIO pPIO = AT91C_BASE_PIOA;
	pPIO->PIO_CODR = pin;
}

//////////////////////////////////////////////////////////////////////////////////
// TWI setup, read and write functions
void InitializeTWI(void)
{
	volatile AT91PS_PMC pPMC = AT91C_BASE_PMC;
	volatile AT91PS_PIO pPIO = AT91C_BASE_PIOA;
	volatile AT91PS_TWI pTWI = AT91C_BASE_TWI;
	
	pPIO->PIO_PDR = TWI_SDA | TWI_SCL;	// disable PIO pins 3 and 4
	pPIO->PIO_ASR = TWI_SDA | TWI_SCL;	// select Peripheral A for pins 3 and 4 = TWI
	
	pPMC->PMC_PCER = (1<<AT91C_ID_TWI);			// turn on TWI peripheral clock
	
	pTWI->TWI_CR =	AT91C_TWI_SWRST; 	/* software reset TWI */
	
	pTWI->TWI_CR =	AT91C_TWI_MSEN;		/* Master mode enabled */
	
	AT91C_BASE_TWI->TWI_CWGR =(10<<16)|((unsigned int)10 << 8)|(unsigned int)10; // set TWI clock speed
	WaitMicroSecond (5);
	TWIWriteByte(EADC_ADDR,EADC_SETUP);	//setup the External ADC
}
// Send NumberOfBytes over the TWI to address ADDR
void TWIWrite(int addr, char *data, unsigned int NumberOfBytes)
{
	unsigned int status,count;
	volatile AT91PS_TWI pTWI = AT91C_BASE_TWI;
	
	// Wait until other transmissions to complete to transmit
	status = pTWI->TWI_SR;	
	while(!(status & AT91C_TWI_TXCOMP))
		status = pTWI->TWI_SR;
	
	// set the master mode register
	pTWI->TWI_MMR = (addr << 16) | AT91C_TWI_MWRITE;
	
	for(count=0;count<NumberOfBytes;count++){
		/* Write the data to send into THR. Start conditionn DADDR and R/W bit
		 are sent automatically */
		pTWI->TWI_THR = *data++;
		
		status = pTWI->TWI_SR;
		while(!(status & AT91C_TWI_TXRDY)) status = pTWI->TWI_SR;
	}	
}
// Send a single byte over the TWI to address ADDR
void TWIWriteByte(int addr, char byte)
{
	unsigned int status;
	volatile AT91PS_TWI pTWI = AT91C_BASE_TWI;
	
	// Wait until other transmissions are complete to transmit
	status = pTWI->TWI_SR;	
	while(!(status & AT91C_TWI_TXCOMP))
		status = pTWI->TWI_SR;
	
	// set the master mode register
	pTWI->TWI_MMR = (addr << 16) | AT91C_TWI_MWRITE;
	pTWI->TWI_THR = byte;
}
// Read multiple bytes over TWI from address ADDR and place them in DATA array
void TWIRead(unsigned char addr, char *data, unsigned int NumberOfBytes)
{
	unsigned int status, count, end=0;
	volatile AT91PS_TWI pTWI = AT91C_BASE_TWI;
		
	// wait for the TWI buss ready
	status = pTWI->TWI_SR;
	while(!(status & AT91C_TWI_TXCOMP)) 
		status = pTWI->TWI_SR;
	
	pTWI->TWI_MMR = (addr << 16) | AT91C_TWI_MREAD;		/* Device address and read bit*/
	
	pTWI->TWI_CR = AT91C_TWI_START;			// send START bit
	
	for(count=0;count<NumberOfBytes;count++){
		end=0;
		// if next-to-last byte send STOP
		if(count == (NumberOfBytes-1))
			pTWI->TWI_CR = AT91C_TWI_STOP;
		
		while(!end){
			status = pTWI->TWI_SR;
			if((status & AT91C_TWI_NACK) == AT91C_TWI_NACK)
				end=1;
			//wait until RXRDY is high to read in the next byte
			if((status & AT91C_TWI_RXRDY) == AT91C_TWI_RXRDY)
				end=1;
		}
		// read the char received
		*data++ = pTWI->TWI_RHR;
	}
}
void TWIWriteAddress(unsigned char address, unsigned char iaddress, unsigned char data)
{
	unsigned int status;
	volatile AT91PS_TWI pTWI = AT91C_BASE_TWI;
	
	// Wait until other transmissions are complete to transmit
	status = pTWI->TWI_SR;	
	while(!(status & AT91C_TWI_TXCOMP))
		status = pTWI->TWI_SR;	
	
	pTWI->TWI_MMR = ((unsigned long)address)<<16 | AT91C_TWI_IADRSZ_1_BYTE; // setup slave address and internal write byte address
	
	pTWI->TWI_IADR = iaddress;	// specify what register on slave device to write to
	
	pTWI->TWI_THR = data;	// send data
}
void TWIUnlockBus(void)
{
	int i;
	volatile AT91PS_PIO pPIO = AT91C_BASE_PIOA;
	
	pPIO->PIO_PER = TWI_SDA | TWI_SCL;
	pPIO->PIO_OER = TWI_SCL;	// set SDA and SCL to output
	pPIO->PIO_SODR = TWI_SCL;	// set TWI SDA line to HIGH
	pPIO->PIO_ODR = TWI_SDA;	// set SDA to input
	
	for(i=0;i<8;i++){
		if(pPIO->PIO_PDSR & TWI_SDA) break;
		TogglePin(TWI_SCL);
		WaitMicroSecond(3);
		TogglePin(TWI_SCL);
		WaitMicroSecond(3);
	}
	
	// send STOP bit
	WaitMicroSecond(2);
	pPIO->PIO_OER = TWI_SDA;	// set SDA to output
	TogglePin(TWI_SCL);			// set SCL to LOW
	pPIO->PIO_CODR = TWI_SDA;	// set SDA to LOW
	WaitMicroSecond(2);
	TogglePin(TWI_SCL);			// set SCL to HIGH
	WaitMicroSecond(2);
	TogglePin(TWI_SDA);			// set SDA to HIGH
	
	//InitalizeTWI();
}

/////////////////////////////////////////////////////////////////////////////
// External ADC Setup Functions
// Read from a port on the External ADC
// returns an average of 8 readings
// argument: channel = 0-11 port
//			 diff = 0 for differential, 1 for single ended
int EADC_read_port(int channel,int diff)
{
	int i,j,value=0;
	char eadc_data[16];
	TWIWriteByte(EADC_ADDR, 0x20 | (channel<<1) | diff);
	TWIRead(EADC_ADDR,eadc_data,16);
	for(i=0;i<8;i++){
		j=i*2;
		value += (eadc_data[j] & 0x0f)<<8 | eadc_data[j+1];
	}
	return value/8;
}
int EADC_read_port_single(int channel,int diff)
{
	int value=0;
	char eadc_data[2];
	TWIWriteByte(EADC_ADDR, 0x60 | (channel<<1) | diff);
	TWIRead(EADC_ADDR,eadc_data,2);
	value = (eadc_data[0] & 0x0f)<<8 | eadc_data[1];
	return value;
}
void EADC_Batt_Voltage(void)
{
	// read from the port 3 times to allow voltage to settle in the ADC circuit
	// this reading is affected by the motor Back EMF voltage, Batt Voltage circuit may need a 
	// decoupling cap to ground?
	EADC_read_port_single(VOLT,1);
	EADC_read_port_single(VOLT,1);
	EADC_read_port_single(VOLT,1);
	
	CBC_Pack->Batt_Voltage = EADC_read_port_single(VOLT,1);
}
void EADC_Accelerometer_Update(void)
{
	char eadc_data[12];
	// setup EADC to read 12 bytes from AIN6-AIN11
	TWIWriteByte(EADC_ADDR,EADC_ACCV_CONFIG); // send configuration byte
	TWIRead(EADC_ADDR,eadc_data,12); // read in channel 6-11
	
	//skip the first 6 bytes because channel 6,7 and 8 are attached to motors and batt voltage
	// each channel is 12-bits two bytes of data, dont forget!
	
	CBC_Pack->Acc_X = (eadc_data[6] & 0x0f)<<8 | eadc_data[7];
	CBC_Pack->Acc_Y = (eadc_data[8] & 0x0f)<<8 | eadc_data[9];
	CBC_Pack->Acc_Z = (eadc_data[10] & 0x0f)<<8 | eadc_data[11];
}
void EADC_BEMF_Voltage(void)
{
	int i,j;
	char eadc_data[16];
	// setup EADC to read 12 bytes from AIN0-AIN7
	TWIWriteByte(EADC_ADDR,0xE); // send configuration
	TWIRead(EADC_ADDR,eadc_data,16); // read in channel 0-7
	for(i=0;i<8;i++){
		//Back_EMF[i]=eadc_data[i];
		j=i*2;
		Back_EMF[i] = (eadc_data[j] & 0x0F) << 8 | eadc_data[j+1];
	}
}

////////////////////////////////////////////////////////////////////////////
// PWM chip PCA9635 setup
void PWM_SwReset(void)
{
	char pwmSetup[3];
	
	TWIWriteByte(0x6, 0x00);	// software reset address of PWM chip
	WaitMiliSecond(10);
	
	pwmSetup[0]=0x80;			// start at address 0x00 and auto increment on
	pwmSetup[1]=0x00;			// mode1 register
	pwmSetup[2]=0x34;			// mode2 register
	TWIWrite(PWM_ADDR,pwmSetup,3);
	PWMportState = 0;
}
void MotorFD(int port)
{
	switch (port) {
		case 0:
			PWMportState = (PWMportState & 0xFFFFFFF0) | 0x04;
			TWIWriteAddress(PWM_ADDR, 0x14, (PWMportState & 0xFF));
			break;
		case 1:
			PWMportState = (PWMportState & 0xFFFFFF0F) | 0x40;
			TWIWriteAddress(PWM_ADDR, 0x14, (PWMportState & 0xFF));
			break;
		case 2:
			PWMportState = (PWMportState & 0xFFFFF0FF) | 0x0400;
			TWIWriteAddress(PWM_ADDR, 0x15, ((PWMportState >> 8) & 0xFF));
			break;
		case 3:
			PWMportState = (PWMportState & 0xFFFF0FFF) | 0x4000;
			TWIWriteAddress(PWM_ADDR, 0x15, ((PWMportState >> 8) & 0xFF));
			break;
		default:
			break;
	}
}
void MotorBK(int port)
{
	switch (port) {
		case 0:
			PWMportState = (PWMportState & 0xFFFFFFF0) | 0x01;
			TWIWriteAddress(PWM_ADDR, 0x14, (PWMportState & 0xFF));
			break;
		case 1:
			PWMportState = (PWMportState & 0xFFFFFF0F) | 0x10;
			TWIWriteAddress(PWM_ADDR, 0x14, (PWMportState & 0xFF));
			break;
		case 2:
			PWMportState = (PWMportState & 0xFFFFF0FF) | 0x0100;
			TWIWriteAddress(PWM_ADDR, 0x15, ((PWMportState >> 8) & 0xFF));
			break;
		case 3:
			PWMportState = (PWMportState & 0xFFFF0FFF) | 0x1000;
			TWIWriteAddress(PWM_ADDR, 0x15, ((PWMportState >> 8) & 0xFF));
			break;
		default:
			break;
	}
}
void MotorOff(int port)
{
	switch (port) {
		case 0:
			PWMportState = (PWMportState & 0xFFFFFFF0) | 0x05;
			TWIWriteAddress(PWM_ADDR, 0x14, (PWMportState & 0xFF));
			break;
		case 1:
			PWMportState = (PWMportState & 0xFFFFFF0F) | 0x50;
			TWIWriteAddress(PWM_ADDR, 0x14, (PWMportState & 0xFF));
			break;
		case 2:
			PWMportState = (PWMportState & 0xFFFFF0FF) | 0x500;
			TWIWriteAddress(PWM_ADDR, 0x15, ((PWMportState >> 8) & 0xFF));
			break;
		case 3:
			PWMportState = (PWMportState & 0xFFFF0FFF) | 0x5000;
			TWIWriteAddress(PWM_ADDR, 0x15, ((PWMportState >> 8) & 0xFF));
			break;
		default:
			break;
	}
}
void AllOff(void)
{
	char data[5];
	
	data[0]= 0x80 | 0x14;	// auto increment from address 0x14
	data[1]= 0x55;
	data[2]= 0x55;
	data[3]= 0x00;
	data[4]= 0x00;
	PWMportState = 0;
	TWIWrite(PWM_ADDR,data,5);
}
void MotorSetPower(int port, int power)
{
	char dir,dutyCycle[3];
	
	if(power > 0) dir = 0;
	else {
		dir = 1;
		power = -power;
	}
	switch (port) {
		case 0:
			dutyCycle[0]= 0x80 | 0x02;	// auto increment from address 0x02
			if(dir) {
				PWMportState = (PWMportState & 0xFFFFFFF0) | 0x02;
				dutyCycle[1]= power;
				dutyCycle[2]= 0x0;
			}
			else {
				PWMportState = (PWMportState & 0xFFFFFFF0) | 0x08;
				dutyCycle[1]= 0x00;
				dutyCycle[2]= power;
			}
			TWIWriteAddress(PWM_ADDR, 0x14, (PWMportState & 0xFF));
			break;
		case 1:
			dutyCycle[0]= 0x80 | 0x04;
			if(dir) {
				PWMportState = (PWMportState & 0xFFFFFF0F) | 0x20;
				dutyCycle[1]= power;
				dutyCycle[2]= 0x0;
			}
			else {
				PWMportState = (PWMportState & 0xFFFFFF0F) | 0x80;
				dutyCycle[1]= 0x00;
				dutyCycle[2]= power;
			}
			TWIWriteAddress(PWM_ADDR, 0x14, (PWMportState & 0xFF));
			break;
		case 2:
			dutyCycle[0]= 0x80 | 0x06;
			if(dir) {
				PWMportState = (PWMportState & 0xFFFFF0FF) | 0x0200;
				dutyCycle[1]= power;
				dutyCycle[2]= 0x0;
			}
			else {
				PWMportState = (PWMportState & 0xFFFFF0FF) | 0x0800;
				dutyCycle[1]= 0x00;
				dutyCycle[2]= power;
			}
			TWIWriteAddress(PWM_ADDR, 0x15, ((PWMportState >> 8) & 0xFF));
			break;
		case 3:
			dutyCycle[0]= 0x80 | 0x08;
			if(dir) {
				PWMportState = (PWMportState & 0xFFFF0FFF) | 0x2000;
				dutyCycle[1]= power;
				dutyCycle[2]= 0x0;
			}
			else {
				PWMportState = (PWMportState & 0xFFFF0FFF) | 0x8000;
				dutyCycle[1]= 0x00;
				dutyCycle[2]= power;
			}
			TWIWriteAddress(PWM_ADDR, 0x15, ((PWMportState >> 8) & 0xFF));
			break;
		default:
			break;
	}
	TWIWrite(PWM_ADDR,dutyCycle,3);
}
void MotorSetAll(void)
{
	char dutyCycle[9],portState[3];
	
	dutyCycle[0] = 0x80 | 0x02;	// auto increment from address 0x02
	
// Motor 0 update
	if(Motor_Power[0] < 0) {
		PWMportState = (PWMportState & 0xFFFFFFF0) | 0x02;
		dutyCycle[1]= -Motor_Power[0];
		dutyCycle[2]= 0x00;
	}
	else {
		PWMportState = (PWMportState & 0xFFFFFFF0) | 0x08;
		dutyCycle[1]= 0x00;
		dutyCycle[2]= Motor_Power[0];
	}
// Motor 1 update
	if(Motor_Power[1] < 0) {
		PWMportState = (PWMportState & 0xFFFFFF0F) | 0x20;
		dutyCycle[3]= -Motor_Power[1];
		dutyCycle[4]= 0x00;
	}
	else {
		PWMportState = (PWMportState & 0xFFFFFF0F) | 0x80;
		dutyCycle[3]= 0x00;
		dutyCycle[4]= Motor_Power[1];
	}
// Motor 2 update
	if(Motor_Power[2] < 0) {
		PWMportState = (PWMportState & 0xFFFFF0FF) | 0x0200;
		dutyCycle[5]= -Motor_Power[2];
		dutyCycle[6]= 0x00;
	}
	else {
		PWMportState = (PWMportState & 0xFFFFF0FF) | 0x0800;
		dutyCycle[5]= 0x00;
		dutyCycle[6]= Motor_Power[2];
	}
// Motor 3 update
	if(Motor_Power[3] < 0) {
		PWMportState = (PWMportState & 0xFFFF0FFF) | 0x2000;
		dutyCycle[7]= -Motor_Power[3];
		dutyCycle[8]= 0x00;
	}
	else {
		PWMportState = (PWMportState & 0xFFFF0FFF) | 0x8000;
		dutyCycle[7]= 0x00;
		dutyCycle[8]= Motor_Power[3];
	}
	
	TWIWrite(PWM_ADDR,dutyCycle,9);
	
	portState[0] = 0x80 | 0x14;	// auto increment from address 0x14
	portState[1] = PWMportState & 0xFF;
	portState[2] = (PWMportState >> 8) & 0xFF;
	TWIWrite(PWM_ADDR,portState,3);
}
int MotorCalibration(void)
{
	int i,j;
	int voltage;
	
	disableIRQ();
	
	AllOff();	// turn off all motors
	
	for(i=0;i<8;i++){
		voltage = 0;
		for(j=0;j<8;j++){
			voltage += EADC_read_port_single((7-i),0);
			WaitMiliSecond(100);
		}
		MotorCal[i] = voltage/7;
	}
	
	enableIRQ();
}
void MotorFeedback(int port)
{
	int speed = 0;
	
	if(!(port%2)){
		if(Back_EMF[port*2] > 0) speed = Back_EMF[port*2];
		else speed = -Back_EMF[(port*2)+1];
	}
	else{
		if(Back_EMF[port*2] > 0) speed = -Back_EMF[port*2];
		else speed = Back_EMF[(port*2)+1];
	}
	
	if(speed < 12 && speed > -12) speed = 0;
	
	CBC_Pack->Motor_TPS[port] = speed;
	CBC_Pack->Motor_Counter[port] += speed;
}
/*void MotorPID(int port,int targetVelocity)
{
	int error;
	int Pterm,Iterm,Dterm;
	
	error = targetVelocity - CBC_Pack->Motor_TPS[port];
	Motor_Err[port] += error;
	// keeps error from running away due to hardware limitations
	//if(Motor_Err[port] > 60000) Motor_Err[port] = 60000;		
	
	if(CBC_Pack->Motor_PID_Gains[port][3] == 0) CBC_Pack->Motor_PID_Gains[port][3] = 1;
	if(CBC_Pack->Motor_PID_Gains[port][4] == 0) CBC_Pack->Motor_PID_Gains[port][4] = 1;
	if(CBC_Pack->Motor_PID_Gains[port][5] == 0) CBC_Pack->Motor_PID_Gains[port][5] = 1;
	
	// Proportional Term
	Pterm = (CBC_Pack->Motor_PID_Gains[port][0] * error)/CBC_Pack->Motor_PID_Gains[port][3];
	// Integral Term
	Iterm = (CBC_Pack->Motor_PID_Gains[port][1] * Motor_Err[port])/CBC_Pack->Motor_PID_Gains[port][4];
	// Derivative Term
	Dterm = (CBC_Pack->Motor_PID_Gains[port][2] * (Motor_TPS_Last[port] - CBC_Pack->Motor_TPS[port]))/CBC_Pack->Motor_PID_Gains[port][5];
	
	// compute motor power
	Motor_Power[port] = ((Pterm + Iterm + Dterm)/10);
	CBC_Pack->Motor_PWM[port] = Motor_Power[port];
}*/
int PID(int port, int err)
{
	int Pterm, Iterm, Dterm;

	// is the error within the maximum or minimum motor power range?
		
		if((Motor_Err[port] > 170000) || (Motor_Err[port] < -170000)) Motor_Err[port] = 170000;
		else Motor_Err[port] += err;
		
		if(CBC_Pack->Motor_PID_Gains[port][3] == 0) CBC_Pack->Motor_PID_Gains[port][3] = 1;
		if(CBC_Pack->Motor_PID_Gains[port][4] == 0) CBC_Pack->Motor_PID_Gains[port][4] = 1;
		if(CBC_Pack->Motor_PID_Gains[port][5] == 0) CBC_Pack->Motor_PID_Gains[port][5] = 1;
		
		// Proportional Term
		Pterm = (CBC_Pack->Motor_PID_Gains[port][0] * err)/CBC_Pack->Motor_PID_Gains[port][3];
		// Integral Term
		Iterm = (CBC_Pack->Motor_PID_Gains[port][1] * Motor_Err[port])/CBC_Pack->Motor_PID_Gains[port][4];
		// Derivative Term
		Dterm = (CBC_Pack->Motor_PID_Gains[port][2] * (Motor_Last_Err[port] - err))/CBC_Pack->Motor_PID_Gains[port][5];
		
		Motor_Last_Err[port] = err;
		//printf("P%d I%d D%d \n",Pterm,Iterm,Dterm);
		//return ((Pterm + Iterm + Dterm)*255)/Motor_Max_PID;
		if((Pterm + Iterm + Dterm) > 255) return 255;
		else if((Pterm + Iterm + Dterm) < -255) return -255;
		else return (Pterm + Iterm + Dterm);
}
void MotorMTP(int port)
{
	int currentPosition = Motor_Next_Position[port];
	
	int diffPosition = CBC_Pack->Motor_Counter_Targets[port] - CBC_Pack->Motor_Counter[port];
	
	if((CBC_Pack->Motor_TPS_Targets[port] < 0 && diffPosition > CBC_Pack->Motor_TPS_Targets[port]) || (CBC_Pack->Motor_TPS_Targets[port] > 0 && diffPosition < CBC_Pack->Motor_TPS_Targets[port])){
		
		if((diffPosition > -CBC_Pack->Motor_Thresholds[0]) && (diffPosition < CBC_Pack->Motor_Thresholds[0])) {
			Motor_Next_Position[port] = CBC_Pack->Motor_Counter[port];	// set zero error for PID loop
			CBC_Pack->Motor_In_Motion &= ~(1 << port);		// clear the Motor in motion bit
		}
		else Motor_Next_Position[port] += diffPosition;
	
	}
	else Motor_Next_Position[port] += CBC_Pack->Motor_TPS_Targets[port];
	
	Motor_Power[port] = PID(port, Motor_Next_Position[port] - CBC_Pack->Motor_Counter[port]);
}

void servoTest(int port)
{
	int k;
	for(k=0;k<2043;k+=5) {
		CBC_Pack->Servo_Position[port] = k; 
		WaitMiliSecond(50);
		printf("port %d- %d\n",port,CBC_Pack->Servo_Position[port]);
	}
	for(k=2048;k>5;k-=5) {
		CBC_Pack->Servo_Position[port] = k; 
		WaitMiliSecond(50);
		printf("port %d- %d\n",port,CBC_Pack->Servo_Position[port]);
	}
}
void ServoPulseOn(int port)
{
	int value;
	
	value = 0x01<<(port*2);
	TWIWriteAddress(PWM_ADDR,0x16,value);
}
void ServoPulseOff(void)
{
	TWIWriteAddress(PWM_ADDR,0x16,0x0);
}

////////////////////////////////////////////////////////////////////////////
// UTILITY FUNCTIONS
////////////////////////////////////
//----------------------------------------------------------------------------/
void WaitMiliSecond (unsigned int MiliSeconds)
{
	volatile AT91PS_PITC pPIT = AT91C_BASE_PITC;
	unsigned int currentMiliSecond;
	unsigned int waitTime;
	
	// get the current time, PIT timer value plus the number of ten milisecond interrups that have occured
	currentMiliSecond = (pPIT->PITC_PIIR/3125) + TenMSCount*10;
	waitTime = currentMiliSecond + MiliSeconds;		// calculate how long to wait
	while(currentMiliSecond < waitTime) // wait until that time has passed 
		currentMiliSecond = (pPIT->PITC_PIIR/3125) + TenMSCount*10;
}
//-----------------------------------------------------------------------------/
void WaitMicroSecond (unsigned int MicroSecond)
{
	volatile AT91PS_PITC pPIT = AT91C_BASE_PITC;
	unsigned int currentTime;
	unsigned int waitTime;
	
	currentTime = pPIT->PITC_PIIR;
	
	waitTime = currentTime + MicroSecond*3;		// calculate how long to wait
	if(waitTime > (MCK/1600)) waitTime = (MCK/1600) - 1;
	
	while(currentTime < waitTime) // wait until that time has passed 
		currentTime = pPIT->PITC_PIIR;
}
void TriggerSysInterrupt(void)
{
	AT91C_BASE_AIC->AIC_ISCR = (1<<AT91C_ID_SYS); // trigger an interrupt
}

/* ACC range select
 *	select		range
 *	 1			+/- 1.5g
 *	 2			+/- 2g
 *	 4			+/- 4g
 *	 6			+/- 6g
*/
int ACC_G_Range(int select)
{
	switch (select) {
		case 1:
			AT91C_BASE_PIOA->PIO_CODR = ACC_GSEL1 | ACC_GSEL2;
			break;
		case 2:
			AT91C_BASE_PIOA->PIO_SODR = ACC_GSEL1;
			AT91C_BASE_PIOA->PIO_CODR = ACC_GSEL2;
			break;
		case 4:
			AT91C_BASE_PIOA->PIO_CODR = ACC_GSEL1;
			AT91C_BASE_PIOA->PIO_SODR = ACC_GSEL2;
			break;
		case 6:
			AT91C_BASE_PIOA->PIO_SODR = ACC_GSEL1 | ACC_GSEL2;
			break;
		default:
			return 0;
			break;
	}
	return 1;
}

int Button(void)
{
	// button not pressed
	if(AT91C_BASE_PIOA->PIO_PDSR & BUTTON) return 0;
	// button pressed
	return 1;
}

void ChumbyBend(int state)
{
	if(state)
		AT91C_BASE_PIOA->PIO_CODR = BEND_IOPIN;
	else
		AT91C_BASE_PIOA->PIO_SODR = BEND_IOPIN;
}

/////////////////////////////////////////////////////////////////////////////
// This function is fired everytime there is a system interrupt
void SystemInterrupt(void)
{
	static int svo=0;
	int time=0;
	int actual_time;
	int i;
	unsigned int value;
	unsigned int servo_setting=0;
	volatile AT91PS_PITC pPIT = AT91C_BASE_PITC;
	
	TenMSCount++;	// increment the 10ms counter to keep track of time
	
	value = pPIT->PITC_PIVR; // must read this value to reset the Periodic Interval Timer
	// return right away if the startup sequence is incomplete
	if (!startupComplete) return;
	
// DO the SERVO control pulses
	if(CBC_Pack->Enable_Servos){
		// UPDATE SERVOS!!!!!!!!!!!!!!!!	
		ServoPulseOn(svo); // turn on the servo pulse
		switch (svo) {
			case 0:
				servo_setting = CBC_Pack->Servo_Position[0];
				break;
			case 1:
				servo_setting = CBC_Pack->Servo_Position[1];
				break;
			case 2:
				servo_setting = CBC_Pack->Servo_Position[2];
				break;
			case 3:
				servo_setting = CBC_Pack->Servo_Position[3];
				break;
			default:
				break;
		}
	
		time = (300*(servo_setting + 640))/100;
		actual_time = pPIT->PITC_PIIR;
		while(actual_time < time) actual_time = pPIT->PITC_PIIR;
		
		ServoPulseOff();
		svo++;		// increment the next servo update for next interrupt
		if(svo>3) svo=0;
	}
	
	time = pPIT->PITC_PIIR;
	
// UPDATE SENSOR READINGS
	CBC_Pack->ButtonState = Button();
	UpdateADC();
	UpdateDigitals();
	EADC_Accelerometer_Update();
	EADC_Batt_Voltage();
	
	
// DO motor BACK-EMF here
	for(i=0;i<4;i++){
		MotorOff(i);																	// turn off motor
		
		actual_time = pPIT->PITC_PIIR;								// wait for induction spike
		time = actual_time + INDUCTION_SPIKE_DELAY;
		while(actual_time < time) actual_time = pPIT->PITC_PIIR;
		
		Back_EMF[i*2] = EADC_read_port_single(7-(i*2),0) - MotorCal[i*2];				// read forward channel
		Back_EMF[(i*2)+1] = EADC_read_port_single(7-((i*2)+1),0) - MotorCal[(i*2)+1];	// read backward channel
		
		MotorFeedback(i);																// compute motor position and velocity

		if(CBC_Pack->Motor_TPS_Targets[i] != 0) {
			
			MotorMTP(i);									// compute PID loop 
			
			// reset accumulated error for integral term if motor target settings change
			if((Motor_Last_Target_Speed[i] != CBC_Pack->Motor_TPS_Targets[i]) || (Motor_Last_Target_Position[i] != CBC_Pack->Motor_Counter_Targets[i])) {
				Motor_Err[i]=0;
				Motor_Next_Position[i] = CBC_Pack->Motor_Counter[i];
				CBC_Pack->Motor_In_Motion |= (1<<i);
			}
			Motor_Last_Target_Speed[i] = CBC_Pack->Motor_TPS_Targets[i];
			Motor_Last_Target_Position[i] = CBC_Pack->Motor_Counter_Targets[i];
		}
		else {																	
			Motor_Power[i] = CBC_Pack->Motor_PWM[i];					// OR turn on the motor with PWM
			CBC_Pack->Motor_In_Motion &= ~(1<<i);						// motor is not in BEMF	motion
			Motor_Last_Target_Speed[i] = 0;
		}
		
		// check to see if motor counter should be reset
		if(CBC_Pack->Motor_Clear_Counter & (1 << i)) {
			CBC_Pack->Motor_Counter[i] = 0;					// reset counter
			CBC_Pack->Motor_Clear_Counter &= ~(1 << i);		// reset clear bit for motor
		}
		if(Motor_Power[i] != 0) MotorSetPower(i,Motor_Power[i]); // turn motor back on using new power setting
		else MotorOff(i);
			
	}
	
	value = pPIT->PITC_PIVR; // must read this value to reset the Periodic Interval Timer
}

/*
void data_dump(uint8 *data, int len)
{
	int i;
	
	for(i = 0;i < len;) {
		printf("%02X", (int)data[i++]);
		if(i%24)
			printf(" ");
		else
			printf("\r\n");
	}
}
*/
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
int main(void)
{
	int i,j,value;
	//uint8 bufStuff[60];
	//uint8 bufOut[30];
	CBC_OutPacket.key = 0xDEADBEEF;
	CBC_InPacket.key = 0;
	
	memset(&CBC_InPacket, 0, sizeof(Cbob_Data));

	CBC_Pack = &CBC_OutPacket;
	
	// Global Variable INIT
	TenMSCount = 0;
	startupComplete = 0;
	CBC_Pack->Enable_Digital_Outputs = 0;	// all digital ports are inputs
	CBC_Pack->Digital_Output_Values = 0; // all digital outputs are LOW
	CBC_Pack->Motor_In_Motion = 0;
	CBC_Pack->Motor_Clear_Counter = 0;
	CBC_Pack->Enable_Servos = 0;			// servos are disabled
	CBC_Pack->Motor_Thresholds[0] = 2000;	// Position Threshold
	CBC_Pack->Motor_Thresholds[1] = 0;		// Velocity Threshold
	CBC_Pack->Motor_Thresholds[2] = 1000;	// Acceleration Threshold
	
	for(j=0;j<4;j++){
		Motor_Power[j]=0;
		CBC_Pack->Motor_PWM[j]=0;
		CBC_Pack->Motor_TPS[j]=0;
		CBC_Pack->Motor_TPS_Targets[j]=0;
		Motor_Next_Position[j]=0;
		Motor_Last_Err[j]=0;
		Motor_Err[j]=0;
		Motor_Last_Target_Speed[j]=0;
		Motor_Last_Target_Position[j]=0;
		CBC_Pack->Motor_Counter[j]=0;
		CBC_Pack->Motor_Counter_Targets[j]=0;
		CBC_Pack->Servo_Position[j]=0;
		
		CBC_Pack->Motor_PID_Gains[j][0]=30;		// P term multiplier
		CBC_Pack->Motor_PID_Gains[j][1]=0;		// I term multiplier
		CBC_Pack->Motor_PID_Gains[j][2]=-30;	// D term multiplier
		CBC_Pack->Motor_PID_Gains[j][3]=70;		// P term divider
		CBC_Pack->Motor_PID_Gains[j][4]=1;		// I term divider
		CBC_Pack->Motor_PID_Gains[j][5]=51;		// D term divider
	}
	
// Initialize the AT91SAM7...
	Initialize();				// sets Main Clock, default interrupts, digital and analog pins
// PIT INITALIZER
	InitializePIT();
	
// SPI INITIALIZER
	InitializeSPI();
	DMA_Init(&spi_dma, AT91C_BASE_PDC_SPI);
	
// SERIAL INITIALIZER
	UART_Init(1, CREATE_SERIAL_BAUD);		// Create Serial port
	UART_Init(0, USB_SERIAL_BAUD);			// USB Serial port
	
// ADC INITIALIZER
	InitializeADC();		
	
// TWI INITIALIZER
	TWIUnlockBus();							// Clock the TWI a few times incase of lockup
	InitializeTWI();			
	
	PWM_SwReset();							// software reset PWM chip
	TWIWriteByte(EADC_ADDR,EADC_SETUP);		// setup byte to External ADC
	WaitMiliSecond(50);
	MotorCalibration();						// calibrate motor ports
	
	ACC_G_Range(1);							// set Accelerometer G range to +/-1.5G's
	
	startupComplete = 1;					// startup complete flag
	
// Debug and testing
	//CBC_Pack->Motor_TPS_Targets[2]=0;
	//CBC_Pack->Enable_Servos = 1;
	//UART_WriteByte(1,128);
	//UART_WriteByte(1,131);
	//bufStuff[0]=0;
	i = 0;
	j = 0;
	UART_WriteString(0, FIRMWARE_VER);
	
	while(1){
		CheckSPI();
		/*
		if(Button()){
			CBC_Pack->Motor_Clear_Counter = 1;
			//UART_WriteByte(1, 139);
			//UART_WriteByte(1, 142);
			//UART_WriteByte(1, 7);
			//UART_WriteByte(1, 127);
			//UART_Write(1,bufOut,4);
			//if(i) UART_WriteByte(1,2);
			//else UART_WriteByte(1,8);
			//i=!i;
			
			//CBC_Pack->Motor_Counter_Targets[0] = 1073741824;
			//UART_Write(1,
			WaitMiliSecond(400);
		}
	
		printf("%d\t%d\t%d\t%d\t%d\n",CBC_Pack->Motor_Clear_Counter,Motor_Power[0],CBC_Pack->Motor_TPS[0],CBC_Pack->Motor_Counter[0],Motor_Err[0]);
		//value = 1;
		//UART_Read(1,bufStuff,value);
		//printf("%d\n",bufStuff[0]);
		WaitMiliSecond(50);
		 //*/
	}
}

void CheckSPI()
{
	uint8 packet_data;
	int count;
	AT91PS_PDC pPDC = AT91C_BASE_PDC_SPI;

	
	if(pPDC->PDC_TCR == 0) {
		count=UART_Read(0, CBC_Pack->UART0_Buffer, 48);
		CBC_Pack->UART0_Buffer_Count = count & 0xff;
		
		count = UART_Read(1, CBC_Pack->UART1_Buffer, 48);
		CBC_Pack->UART1_Buffer_Count = count & 0xff;
		
		CheckInput();
		
		pPDC->PDC_RPR = (uint32)(&CBC_InBuffer);
		pPDC->PDC_TPR = (uint32)CBC_Pack;
		CBC_Pack->key=0xDEADBEEF;
		
		disableIRQ();
		pPDC->PDC_RCR = sizeof(Cbob_Data);
		pPDC->PDC_TCR = sizeof(Cbob_Data);
		enableIRQ();

		if(CBC_InPacket.key == 0xDEADBEEF) {
			ProcessSPI();
			CBC_InPacket.key = 0;
		}
	}
}

void ProcessSPI()
{	
	UART_Write(0, CBC_InPacket.UART0_Buffer, CBC_InPacket.UART0_Buffer_Count); //FIXME: Use DMA_WriteBlock
	UART_Write(1, CBC_InPacket.UART1_Buffer, CBC_InPacket.UART1_Buffer_Count); //FIXME: Use DMA_WriteBlock

	CBC_Pack->Enable_Digital_Outputs = CBC_InPacket.Enable_Digital_Outputs;
	CBC_Pack->Digital_Output_Values = CBC_InPacket.Digital_Output_Values;

	CBC_Pack->Motor_Thresholds[0] = CBC_InPacket.Motor_Thresholds[0];
	CBC_Pack->Motor_Thresholds[1] = CBC_InPacket.Motor_Thresholds[1];
	CBC_Pack->Motor_Thresholds[2] = CBC_InPacket.Motor_Thresholds[2];

	CBC_Pack->Motor_PID_Gains[0][0] = CBC_InPacket.Motor_PID_Gains[0][0];
	CBC_Pack->Motor_PID_Gains[0][1] = CBC_InPacket.Motor_PID_Gains[0][1];
	CBC_Pack->Motor_PID_Gains[0][2] = CBC_InPacket.Motor_PID_Gains[0][2];
	CBC_Pack->Motor_PID_Gains[0][3] = CBC_InPacket.Motor_PID_Gains[0][3];
	CBC_Pack->Motor_PID_Gains[0][4] = CBC_InPacket.Motor_PID_Gains[0][4];
	CBC_Pack->Motor_PID_Gains[0][5] = CBC_InPacket.Motor_PID_Gains[0][5];

	CBC_Pack->Motor_PID_Gains[1][0] = CBC_InPacket.Motor_PID_Gains[1][0];
	CBC_Pack->Motor_PID_Gains[1][1] = CBC_InPacket.Motor_PID_Gains[1][1];
	CBC_Pack->Motor_PID_Gains[1][2] = CBC_InPacket.Motor_PID_Gains[1][2];
	CBC_Pack->Motor_PID_Gains[1][3] = CBC_InPacket.Motor_PID_Gains[1][3];
	CBC_Pack->Motor_PID_Gains[1][4] = CBC_InPacket.Motor_PID_Gains[1][4];
	CBC_Pack->Motor_PID_Gains[1][5] = CBC_InPacket.Motor_PID_Gains[1][5];

	CBC_Pack->Motor_PID_Gains[2][0] = CBC_InPacket.Motor_PID_Gains[2][0];
	CBC_Pack->Motor_PID_Gains[2][1] = CBC_InPacket.Motor_PID_Gains[2][1];
	CBC_Pack->Motor_PID_Gains[2][2] = CBC_InPacket.Motor_PID_Gains[2][2];
	CBC_Pack->Motor_PID_Gains[2][3] = CBC_InPacket.Motor_PID_Gains[2][3];
	CBC_Pack->Motor_PID_Gains[2][4] = CBC_InPacket.Motor_PID_Gains[2][4];
	CBC_Pack->Motor_PID_Gains[2][5] = CBC_InPacket.Motor_PID_Gains[2][5];

	CBC_Pack->Motor_PID_Gains[3][0] = CBC_InPacket.Motor_PID_Gains[3][0];
	CBC_Pack->Motor_PID_Gains[3][1] = CBC_InPacket.Motor_PID_Gains[3][1];
	CBC_Pack->Motor_PID_Gains[3][2] = CBC_InPacket.Motor_PID_Gains[3][2];
	CBC_Pack->Motor_PID_Gains[3][3] = CBC_InPacket.Motor_PID_Gains[3][3];
	CBC_Pack->Motor_PID_Gains[3][4] = CBC_InPacket.Motor_PID_Gains[3][4];
	CBC_Pack->Motor_PID_Gains[3][5] = CBC_InPacket.Motor_PID_Gains[3][5];

	CBC_Pack->Motor_PWM[0] = CBC_InPacket.Motor_PWM[0];
	CBC_Pack->Motor_PWM[1] = CBC_InPacket.Motor_PWM[1];
	CBC_Pack->Motor_PWM[2] = CBC_InPacket.Motor_PWM[2];
	CBC_Pack->Motor_PWM[3] = CBC_InPacket.Motor_PWM[3];

	CBC_Pack->Motor_TPS_Targets[0] = CBC_InPacket.Motor_TPS_Targets[0];
	CBC_Pack->Motor_TPS_Targets[1] = CBC_InPacket.Motor_TPS_Targets[1];
	CBC_Pack->Motor_TPS_Targets[2] = CBC_InPacket.Motor_TPS_Targets[2];
	CBC_Pack->Motor_TPS_Targets[3] = CBC_InPacket.Motor_TPS_Targets[3];

	CBC_Pack->Motor_Counter_Targets[0] = CBC_InPacket.Motor_Counter_Targets[0];
	CBC_Pack->Motor_Counter_Targets[1] = CBC_InPacket.Motor_Counter_Targets[1];
	CBC_Pack->Motor_Counter_Targets[2] = CBC_InPacket.Motor_Counter_Targets[2];
	CBC_Pack->Motor_Counter_Targets[3] = CBC_InPacket.Motor_Counter_Targets[3];

	CBC_Pack->Motor_Clear_Counter = CBC_InPacket.Motor_Clear_Counter;

	CBC_Pack->Servo_Position[0] = CBC_InPacket.Servo_Position[0];
	CBC_Pack->Servo_Position[1] = CBC_InPacket.Servo_Position[1];
	CBC_Pack->Servo_Position[2] = CBC_InPacket.Servo_Position[2];
	CBC_Pack->Servo_Position[3] = CBC_InPacket.Servo_Position[3];

	CBC_Pack->Enable_Servos = CBC_InPacket.Enable_Servos;
}

void CheckInput()
{
	int i;
	uint32 inKey = 0;
	uint8 *inKeyPtr, *bufPtr;

	inKeyPtr = (uint8 *)&inKey;
	bufPtr = &CBC_InBuffer;

	for(i = 0;i < CBOB_PADDING_COUNT;i++,bufPtr++) {
		inKeyPtr[0] = inKeyPtr[1];
		inKeyPtr[1] = inKeyPtr[2];
		inKeyPtr[2] = inKeyPtr[3];
		inKeyPtr[3] = *bufPtr;
		
		if(inKey == 0xDEADBEEF) {
			i -= 3;
			if (i) memcpy((uint8 *)&CBC_InPacket, (uint8 *)(((uint32)&CBC_InBuffer)+i), sizeof(Cbob_Data)-i);
			return;
		}
	}
	
}
