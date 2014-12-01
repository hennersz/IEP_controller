#include <msp430.h>
#include "print.h"
 
#define LED0 BIT0
#define LED1 BIT6
 
unsigned int value=0;
 
void ConfigureAdc(void);
void UARTConfigure(void);
 
/*
 * main.c
 */
void main(void) {
 WDTCTL = WDTPW + WDTHOLD; // Stop WDT
 BCSCTL1 = CALBC1_1MHZ; // Set range
 DCOCTL = CALDCO_1MHZ;
 BCSCTL2 &= ~(DIVS_3); // SMCLK = DCO = 1MHz
 P1DIR |= LED0 + LED1;
 P1SEL |= BIT5; //ADC Input pin P1.5
 P1OUT &= ~(LED0 + LED1);
 UARTConfigure();
 ConfigureAdc();
 __enable_interrupt(); // Enable interrupts.
 
while(1)
 {
 __delay_cycles(1000); // Wait for ADC Ref to settle
 ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion start
 __bis_SR_register(CPUOFF + GIE); // LPM0 with interrupts enabled
 value = ADC10MEM;
 printformat("Input Value: %i\r\n",value);
 if (value>511)
 {
 P1OUT &= ~(LED0 + LED1);
 P1OUT |= LED0;
 }
 else
 {
 P1OUT &= ~(LED0 + LED1);
 P1OUT |= LED1;
 }
 }
}
// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
 __bic_SR_register_on_exit(CPUOFF); // Return to active mode
}
 
void ConfigureAdc(void)
{
 /* Configure ADC Channel */
 ADC10CTL1 = INCH_5 + ADC10DIV_3 ; // Channel 5, ADC10CLK/4
 ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE; //Vcc & Vss as reference
 ADC10AE0 |= BIT5; //P1.5 ADC option
}

 
void UARTConfigure(void){
/* Configure hardware UART */
P1SEL = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
P1SEL2 = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
UCA0CTL1 |= UCSSEL_2; // Use SMCLK
UCA0BR0 = 104; // Set baud rate to 9600 with 1MHz clock (Data Sheet 15.3.13)
UCA0BR1 = 0; // Set baud rate to 9600 with 1MHz clock
UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1
UCA0CTL1 &= ~UCSWRST; // Initialize USCI state machine
IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt
}
