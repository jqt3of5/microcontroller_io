//***********************************************************************************************
//* Anthony's brilliant integer over flow PWM. Evenly distributes some percentage of bits over 
//* a period of time
//* 
//* Designed for MSP430g
//***********************************************************************************************

#include <msp430.h>
#include "pwm.h"

int r,g,b;
unsigned int R=0,G=0,B=0;
const int precision = 100;
void initPWM()
{
  CCR0 = 200; //100 clock ticks to interrupt
  TACTL = TASSEL_2 + MC_1; 
  r = g = b = 0;
  disablePWM();
}
void enablePWM()
{
  CCTL0 |= CCIE;
}

void disablePWM()
{
  CCTL0 &= ~CCIE;
}

#pragma vector = TIMERA0_VECTOR
__interrupt void TimerPWM(void)
{
  //RED PWM channel
  r += R;
  if (r >=precision)
    {
      r -= precision;
      P1OUT |= REDPIN;
    } 
  else 
    {
      P1OUT &= ~REDPIN;
    }

  //GREEN PWM channel
  g += G;
  if (g >=256)
    {
      g -= 256;
      P1OUT |= GREENPIN;
    } 
  else 
    {
      P1OUT &= ~GREENPIN;
    }

  //BLUE PWM channel
  b += B;
  if (b >=precision)
    {
      b -= precision;
      P1OUT |= BLUEPIN;
    } 
  else 
    {
      P1OUT &= ~BLUEPIN;
    }
}
