/* 
    (c) 2020 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/

#pragma config WDTE = OFF   /* WDT operating mode->WDT Disabled */ 
#pragma config LVP = ON     /* Low voltage programming enabled, RE3 pin is MCLR */   

#include <xc.h>
#include <pic18f47q10.h>

#define _XTAL_FREQ                  1000000UL  

static void CLK_Initialize(void);
static void PORT_Initialize(void);
static void TMR0_Initialize(void);
static void INTERRUPT_Initialize(void);
static void TMR0_ISR(void);

static void CLK_Initialize(void)
{
    OSCCON1 = 0x60;     /* set HFINTOSC as new oscillator source */
    OSCFRQ = 0x00;      /* set HFFRQ to 1 MHz */
}

static void PORT_Initialize(void)
{
    TRISEbits.TRISE0 = 0;   /* configure RE0 as output */
}

static void TMR0_Initialize(void)
{
    /* select LFINTOSC, disable TMR0 sync, set prescaler to 1:32 */
    T0CON1 = 0x95;
              
    /* set TMR0H reload value */ 
    TMR0H = 0xDA;

    /* set TMR0L reload value */ 
    TMR0L = 0x29;
    
    /* clear the interrupt flag */
    PIR0bits.TMR0IF = 0;

    /* enable TMR0 interrupt */
    PIE0bits.TMR0IE = 1;

    /* configure TMR0 in 16-bit mode and enable TMR0 */
    T0CON0 = 0x90;
}

static void INTERRUPT_Initialize (void)
{
    INTCONbits.GIE = 1;    /* Enable the Global Interrupts */
    INTCONbits.PEIE = 1;   /* Enable the Peripheral Interrupts */
}

static void TMR0_ISR(void)
{
    /* clear the TMR0 interrupt flag */
    PIR0bits.TMR0IF = 0;
    
    /* set TMR0H reload value */ 
    TMR0H = 0xDA;

    /* set TMR0L reload value */ 
    TMR0L = 0x29;
}

/* Interrupt handler function */
static void __interrupt() INTERRUPT_InterruptManager (void)
{
    /* Check if TMR0 interrupt is enabled and if the interrupt flag is set */
    if(PIE0bits.TMR0IE == 1 && PIR0bits.TMR0IF == 1)
    {
        TMR0_ISR();
    }
}

void main(void) 
{   
    CLK_Initialize();
    PORT_Initialize();
    TMR0_Initialize();
    INTERRUPT_Initialize();
    
    while(1)
    {
        LATEbits.LATE0 = 0;    /* turn LED ON */
        __delay_ms(100);       /* wait 100 ms */
        LATEbits.LATE0 = 1;    /* turn LED OFF */
        
        SLEEP();
    }
}
