// Lab6.c
// Runs on TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// MOOC lab 13 or EE319K lab6 starter
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 1/17/21  
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********


#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/LaunchPad.h"
#include "../inc/CortexM.h"
#include "Sound.h"
#include "Key.h"
#include "Music.h"
#include "TExaS.h"

#define noTest

void DAC_Init(void);         // your lab 6 solution
void DAC_Out(uint8_t data);  // your lab 6 solution
uint32_t Testdata;
uint32_t beat = 0;
void heartbeat(void){
	beat++;
	if(beat & 0x00010000)
		GPIO_PORTF_DATA_R ^= 0x02;
}

int freqTable[16] = {0, 392, 440, 494, 494, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523}; // notes G, A, B, C 
// lab video Lab6_voltmeter
int voltmetermain(void){ //voltmetermain(void){     
  TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);    // bus clock at 80 MHz
  DAC_Init(); // your lab 6 solution
  Testdata = 15;
  EnableInterrupts();
  while(1){                
    Testdata = (Testdata+1)&0x0F;
    DAC_Out(Testdata);  // your lab 6 solution
  }
}

// lab video Lab6_static
int staticmain(void){   uint32_t last,now;  
  TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);    // bus clock at 80 MHz
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  Testdata = 15;
  EnableInterrupts();
  last = LaunchPad_Input();
  while(1){                
    now = LaunchPad_Input();
    if((last != now)&&now){
       Testdata = (Testdata+1)&0x0F;
       DAC_Out(Testdata); // your lab 6 solution
    }
    last = now;
    Clock_Delay1ms(25);   // debounces switch
  }
}



//**************Lab 6 solution below*******************
#ifdef noTest
int main(void){      
  TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);    // bus clock at 80 MHz
  Key_Init();
  LaunchPad_Init();
  Sound_Init();
  // other initialization
	SYSCTL_RCGCGPIO_R |= 0x20; // initialize port F clock
	__asm__ {
		NOP // delay
		NOP
	}
	GPIO_PORTF_DIR_R |= 0x02; // output PF1
	GPIO_PORTF_DEN_R |= 0x02; // enable PF1
	
	for (int i = 0; i < 16; i++){
		freqTable[i] = 1250000 / freqTable[i]; 
	}
	
  EnableInterrupts();
  while(1){ 
		heartbeat(); 
		int buttons = Key_In(); // buttons will be equal to switch input 
		Sound_Start(freqTable[buttons]); // Input Key will determine what frequency it will make and what sound to generate.
  }           
}
#endif
#ifdef dacTest

int DacTestmain(void){ 
	uint32_t data = 0; // 0 to 15 DAC output
  //PLL_Init();    // like Program 4.6 in the book, 80 MHz
  DAC_Init();
  for(;;) {
    DAC_Out(data);
    data = 0x3F&(data+1); // 0,1,2...,14,15,0,1,2,...
		for (int i=0; i<10000000; i++);
  }
}

#endif



