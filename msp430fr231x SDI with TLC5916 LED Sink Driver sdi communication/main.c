//  MSP430FR2311 - use sdi communication with TLC5916 LED Sink Driver
//  2 working modes: Normal and Special( Error Detection and Current Gain)
//
//
//                  MSP430FR2311
//                -----------------
//            /|\|                 |
//             | |                 |
//             --|RST         |
//               |             P1.0|-> ~OE(ED2)
//               |             P1.1|-> LE(ED1)
//               |             P1.5|-> CLK
//               |             P1.7|-> SDI
//               |             P2.0|<- SDO
//
//
//   April 2020
//******************************************************************************
#include <msp430.h>

unsigned int m=0;

void setLED (unsigned int buffer, unsigned long duration)   // SetLED (LED output, duration) function

{
unsigned int i;
        i=8;
        while(i--)
        {
             if ((buffer & (0x01<<(i))) != 0 )
             {
                 P1OUT |= BIT7;
             }
             else
             {
                 P1OUT &= ~BIT7;
             }
             __delay_cycles(30);
             P1OUT ^= BIT5;                                 // CLK
                          __delay_cycles(30);
            P1OUT ^= BIT5;
                         __delay_cycles(30);
        }
               P1OUT ^= BIT1;                                 // LE active
               __delay_cycles(30);
                P1OUT ^= BIT1;
                __delay_cycles(30);
                P1OUT ^= BIT0;                              // OE active
                while ((duration--)>0)
                __delay_cycles(1);

}

void SetToNormalMode (void)
{

    P1OUT &= ~BIT5;                     // CLK to Low

// 1 clk
    P1OUT &= ~BIT1;                                 // LE deactive
    __delay_cycles(30);

    P1OUT |= BIT0;                              // OE active
    __delay_cycles(30);

    P1OUT ^= BIT5;                                 // CLK
    __delay_cycles(30);
    P1OUT ^= BIT5;
    __delay_cycles(30);

// 2 clk
    P1OUT &= ~BIT0;                              // OE deactive
    __delay_cycles(30);

    P1OUT ^= BIT5;                                 // CLK
    __delay_cycles(30);
    P1OUT ^= BIT5;
    __delay_cycles(30);

// 3 clk
        P1OUT |= BIT0;                              // OE active
        __delay_cycles(30);

        P1OUT ^= BIT5;                                 // CLK
        __delay_cycles(30);
        P1OUT ^= BIT5;
        __delay_cycles(30);

// 4 clk
        P1OUT ^= BIT5;                                 // CLK
        __delay_cycles(30);
        P1OUT ^= BIT5;
        __delay_cycles(30);


// 5 clk
       P1OUT ^= BIT5;                                 // CLK
        __delay_cycles(30);
        P1OUT ^= BIT5;
        __delay_cycles(30);

    }

void SetToSpecialMode (void)
{
    P1OUT &= ~BIT5;

// 1 clk
    P1OUT &= ~BIT1;                                 // LE deactive
    __delay_cycles(30);

    P1OUT |= BIT0;                              // OE active
    __delay_cycles(30);

    P1OUT ^= BIT5;                                 // CLK
    __delay_cycles(30);
    P1OUT ^= BIT5;
    __delay_cycles(30);

// 2 clk
    P1OUT &= ~BIT0;                              // OE deactive
    __delay_cycles(30);

    P1OUT ^= BIT5;                                 // CLK
    __delay_cycles(30);
    P1OUT ^= BIT5;
    __delay_cycles(30);

// 3 clk
        P1OUT |= BIT0;                              // OE active
        __delay_cycles(30);

        P1OUT ^= BIT5;                                 // CLK
        __delay_cycles(30);
        P1OUT ^= BIT5;
        __delay_cycles(30);

// 4 clk
        P1OUT |= BIT1;                                 // LE active
        __delay_cycles(30);

        P1OUT ^= BIT5;                                 // CLK
        __delay_cycles(30);
        P1OUT ^= BIT5;
        __delay_cycles(30);

// 5 clk
        P1OUT &= ~BIT1;                                 // LE deactive
        __delay_cycles(30);

      P1OUT ^= BIT5;                                 // CLK
        __delay_cycles(30);
        P1OUT ^= BIT5;
        __delay_cycles(30);

    }

void writeConfigurationCode (unsigned int code)   // writeConfigurationCode (code=CM|HC|CC0...CC5) function
{

    P1OUT &= ~BIT5;

    unsigned int i;


        i=8;

        P1OUT &= ~BIT1;                                 // LE deactive
        __delay_cycles(30);
        P1OUT |= BIT0;                              // OE active
        __delay_cycles(30);

        while(i--)
        {
             if ((code & (0x01<<(7-i))) != 0 )
             {
                 P1OUT |= BIT7;
             }
             else
             {
                 P1OUT &= ~BIT7;
             }
             __delay_cycles(30);
             P1OUT ^= BIT5;                                 // CLK
                          __delay_cycles(30);
            P1OUT ^= BIT5;
                         __delay_cycles(30);
            if (i == 1) {P1OUT |= BIT1; }                                  // LE active

        }


        P1OUT &= ~BIT1;                                 // LE deactive
                __delay_cycles(30);
}


int readErrorStatusCode (void) {

    unsigned int i=8;
    unsigned int c=0x00;
    unsigned int code=0x00;

    P1OUT &= ~BIT5;                         // CLK low
    P1OUT &= ~BIT1;                                 // LE deactive
    __delay_cycles(30);
    P1OUT |= BIT0;                              // OE active
    __delay_cycles(30);


    // 1 clk
    P1OUT &= ~BIT0;                              // OE deactive
    __delay_cycles(30);
        P1OUT ^= BIT5;                                 // CLK
        __delay_cycles(30);
        P1OUT ^= BIT5;
        __delay_cycles(30);

    // 2 clk
        P1OUT ^= BIT5;                                 // CLK
        __delay_cycles(30);
        P1OUT ^= BIT5;
        __delay_cycles(30);

    // 3 clk
            P1OUT ^= BIT5;                                 // CLK
            __delay_cycles(30);
            P1OUT ^= BIT5;
            __delay_cycles(30);

     // 2 us from 1clk!!!
      // read Error Status Code
            while(i--)
             {
                P1OUT ^= BIT5;                                  // CLK
                __delay_cycles(30);
                P1OUT |= BIT1;                                   // LE active
                __delay_cycles(30);

                if ((P2OUT & 0x01) != 0 )
                  {
                      code |= (0x01<<i) ;
                  }
                  else
                  {
                      code &= !(0x01<<i) ;
                  }
                  __delay_cycles(30);
                  P1OUT ^= BIT5;                                 // CLK
                    __delay_cycles(30);


             }

    return(code);
}




int main(void)
{
    unsigned int k = 0;

    WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer

    P1DIR |= BIT7 | BIT5 | BIT1 | BIT0;              // P1.0, 1.1, 1.5 & 1.7 as out
    P1OUT &= ~BIT7 | BIT5 | BIT1;                       // P1.1, P1.5 & 1.7 low
    P1OUT |= BIT0;                                                 // P1.0 high

    P2DIR &= ~BIT0;                                           // P2.0 as input
    P2OUT |= BIT0;                                             // P2.0 as low

    P2OUT |= BIT1;                          // Configure P2.1 as pulled-up
    P2REN |= BIT1;                          // P2.1 pull-up register enable
    P2IES |= BIT1;                          // P2.1 Hi/Low edge
    P2IE |= BIT1;



    PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                                                      // to activate previously configured port settings
    P2IFG &= ~BIT1;                         // P2.1 IFG cleared

    setLED (0, 10);

    SetToSpecialMode();
    writeConfigurationCode(0b00000000);
    SetToNormalMode();
    for (k=1; k<=0xff; k*=2){
    setLED (k, 1000);
    setLED (0, 10);
    }
// __delay_cycles(240000);
 while (1)
     {

         __bis_SR_register(LPM3_bits | GIE);       // Enter LPM0,enable interrupts
         __no_operation();   // For debug,Remain in LPM0
         setLED (0xff, 1);


         }
}


#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)

{
    switch (m) {
        case 0:
        {
            SetToSpecialMode();
            writeConfigurationCode(0b00000000);
            SetToNormalMode();
            m=1;
            }
            break;
        case 1:
        {
            SetToSpecialMode();
            writeConfigurationCode(0b01000000);
            SetToNormalMode();
            m=2;
            }
            break;
        case 2:
        {
            SetToSpecialMode();
            writeConfigurationCode(0b01111111);
            SetToNormalMode();
            m=0;
            }
            break;
        default:
            break;
    }

    P2IFG &= ~BIT1;                         // Clear P2.1 IFG
    __bic_SR_register_on_exit(LPM3_bits);   // Exit LPM3
}



// UNUSED_HWI_ISR()
#pragma vector=UNMI_VECTOR
#pragma vector=WDT_VECTOR
#pragma vector=TIMER1_B0_VECTOR
#pragma vector=TIMER1_B1_VECTOR
#pragma vector=TIMER0_B0_VECTOR
#pragma vector=TIMER0_B1_VECTOR
#pragma vector=PORT1_VECTOR
// #pragma vector=PORT2_VECTOR
#pragma vector=ECOMP0_VECTOR
#pragma vector=ADC_VECTOR
#pragma vector=EUSCI_B0_VECTOR
#pragma vector=EUSCI_A0_VECTOR
#pragma vector=RTC_VECTOR
#pragma vector=SYSNMI_VECTOR
#pragma vector=RESET_VECTOR

__interrupt void UNUSED_HWI_ISR (void)
{
__no_operation();
}
