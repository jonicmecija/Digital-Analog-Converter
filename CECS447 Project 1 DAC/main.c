// CECS 447: Project 1
// Jonic Mecija 
// Febrary 27, 2018

// The purpose of this project is to learn about digital to analog conversion. 

// LaunchPad built-in hardware
// SW1 left switch is negative logic PF4 on the Launchpad
// SW2 right switch is negative logic PF0 on the Launchpad
// red LED connected to PF1 on the Launchpad
// blue LED connected to PF2 on the Launchpad
// green LED connected to PF3 on the Launchpad

// 1. Pre-processor Directives Section
// Constant declarations to access port registers using 
// symbolic names instead of addresses
#include "TExaS.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"
#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source 4 = 16 MHz counter
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Turns counter mode on
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value

// 2. Declarations Section
//   Global Variables
unsigned long In;  // input from PF4
unsigned long Out; // outputs to PF3,PF2,PF1 (multicolor LED)
unsigned long mode;
int i, j;
unsigned char Index;  

// 3-bit 16-element sine wave
const unsigned char SineWave[256] = {128,131,134,137,140,143,146,149,152,155,158,161,165,167,170,173,176,179,182,185,188,
190,193,196,198,201,203,206,208,211,213,215,218,220,222,224,226,228,230,232,233,235,237,238,240,241,243,244,245,246,248,249,
249,250,251,252,253,253,254,254,254,255,255,255,255,255,255,255,254,254,254,253,253,252,251,250,249,249,248,246,245,244,243,
241,240,238,237,235,233,232,230,228,226,224,222,220,218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,176,173,
170,167,165,161,158,155,152,149,146,143,140,137,134,131,127,124,121,118,115,112,109,106,103,100,97,93,90,88,85,82,79,76,73,
70,67,65,62,59,57,54,52,49,47,44,42,40,37,35,33,31,29,27,25,23,21,20,18,17,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,
0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,17,18,20,21,23,25,27,29,31,33,35,37,40,42,44,47,49,52,54,57,59,62,65,67,70,73,76,
79,82,85,88,90,93,97,100,103,106,109,112,115,118,121,124};

const unsigned int reloadValues[7] = {1192,1062,947,895,797,710,632};
const unsigned int button[7] =       {0x01, 0x02, 0x04, 0x08, 0x04, 0x08, 0x10};

//   Function Prototypes
void PortA_Init(void);
void PortB_Init(void);
void PortE_Init(void);
void PortF_Init(void);
void EnableInterrupts(void);
void DisableInterrupts(void); 
void SysTick_Wait(unsigned long delay);
void PLL_Init(void);          // bus clock at 80 MHz


void DAC_Out(unsigned long data);
void Sound_Init(unsigned long period);
void Delay(void);
void Delay440(unsigned long time);
void Delay1(unsigned long time);

unsigned long Switch_In(void);
unsigned long Switch_In2(void);
unsigned long input,input1,previous, previous2;



// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void)
{    
	PLL_Init();          // bus clock at 80 MHz
	PortA_Init();
	PortE_Init();        // Call initialization of port PE3-0    
  PortB_Init();        // Call initialization of port PB7-0    
	PortF_Init();

	mode = 1; 				   // start at mode 1
	
  while(1)
		{

		// saw tooth wave
			if(mode==1)
			{          
				Sound_Init(11360000);
				GPIO_PORTF_DATA_R = 0x02;  // LED is red in mode 1
				GPIO_PORTB_DATA_R = 0x00;
				
				
				for ( i = 0; i < 256; i++ )
				{
					GPIO_PORTB_DATA_R = GPIO_PORTB_DATA_R + 0x01;
					SysTick_Wait(650);
				}
			} 
			
			// triangle wave
			if(mode==2)
			{     
				Sound_Init(11360000);
				GPIO_PORTF_DATA_R = 0x04;  // LED is blue in mode 2
				GPIO_PORTB_DATA_R = 0x00;
				
				for ( i = 0; i < 255; i++ )
				{
					GPIO_PORTB_DATA_R = GPIO_PORTB_DATA_R + 0x01;
					SysTick_Wait(290);

				}
				
				for ( j = 0; j < 255; j++ )
				{
					GPIO_PORTB_DATA_R = GPIO_PORTB_DATA_R - 0x01;
					SysTick_Wait(290);

				}
			}
			
			// square wave
			if(mode==3)
			{
				Sound_Init(1136000);
				GPIO_PORTF_DATA_R = 0x08; // LED is green in mode 3
				
				for ( j = 0; j < 255; j++ )
				{
					GPIO_PORTB_DATA_R = 0xFF;
					SysTick_Wait(300);
				}
				
				for ( j = 0; j < 255; j++ )
				{
					GPIO_PORTB_DATA_R = 0x00;
					SysTick_Wait(290);
					
				}
			} 
			
			// sine wave
			if (mode==4)
			{
				GPIO_PORTF_DATA_R = 0x0A; // LED is yellow in mode 4
				
				Delay440(1);
				Index = (Index+1)&0xFF;  
				DAC_Out(SineWave[Index]);
			}
			
			// piano keys
			if (mode==5)
			{				
				GPIO_PORTF_DATA_R =0x00; // no light on LED
				
				input = Switch_In()&0x0F; 	// Assign input to button Port E
				input1 = Switch_In2()&0x1C; // Assign input to button Port A
				
				if((input&&(previous==0)) || (input1 && (previous2 ==0))) // just pressed
					{      
						EnableInterrupts();
						if(Switch_In()&0x01){Sound_Init(1192);} 		 // C
						else if(Switch_In()&0x02){Sound_Init(1039);} // D
						else if(Switch_In()&0x04){Sound_Init(947);}  // E
						else if(Switch_In()&0x08){Sound_Init(910);}  // F
						else if(Switch_In2()&0x04){Sound_Init(797);} // G 
						else if(Switch_In2()&0x08){Sound_Init(710);} // A
						else{Sound_Init(635);} 											 // B
					}
					
				if((previous&&(input==0))|| (previous2&&input1==0)) // just released
					{      
						DisableInterrupts();    // stop sound
					}
					
				previous = input; 
				previous2 = input1;
				Delay();  // remove switch bounce  
			}
			
		}
	
}

// Read in Port E buttons PE3-0
unsigned long Switch_In(void)
{ 
  return (GPIO_PORTE_DATA_R&0x0F)^0x0F;
}

// Read in Port A buttons PA4-2
unsigned long Switch_In2(void)
{ 
  return (GPIO_PORTA_DATA_R&0x1C)^0x1C;
}


// changing modes
void GPIOPortF_Handler(void)
{
	In = GPIO_PORTF_DATA_R&0x10; // read PF4:sw1 into In
	Delay();
	
		// increment through modes
	if(In==0x00)
		{
			if (mode<5) //red
			{ 
				mode+=1; 
			}
			else 
			{
				mode = 1;
			}
		}
		
	GPIO_PORTF_ICR_R = 0x11; // acknowledge and clear PF4
}





// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Input: interrupt period
//        Units of period are 12.5ns
//        Maximum is 2^24-1
//        Minimum is determined by lenght of ISR
// Output: none

void Sound_Init(unsigned long period){
  Index = 0;
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1   //was 0x2000   
  NVIC_ST_CTRL_R = 0x0007;  // enable SysTick with core clock and interrupts
}


void SysTick_Wait(unsigned long delay){
  volatile unsigned long elapsedTime;
  unsigned long startTime = NVIC_ST_CURRENT_R;
  do{
    elapsedTime = (startTime-NVIC_ST_CURRENT_R)&0x00FFFFFF;
  }
  while(elapsedTime <= delay);
}
// **************DAC_Out*********************
// output to DAC
// Input: 8-bit data, 0 to 255
// Output: none
void DAC_Out(unsigned long data){
  GPIO_PORTB_DATA_R = data;
}

// Interrupt service routine
void SysTick_Handler(void){
  Index = (Index+1)&0xFF;  
  DAC_Out(SineWave[Index]); 
}


void Delay(void){unsigned long volatile time;
  time = 2854500;  // 20msec
  while(time){
		time--;
  }
}

void Delay440(unsigned long volatile time)
{
  time = 50;  // 20msec
  while(time){
		time--;
  }
}

void Delay1(unsigned long volatile time){
  time = 2854500;  // 20msec
  while(time){
		time--;
  }
}









// *************************************************** //
//
//								PORT INITIALIZATIONS
//
// *************************************************** //

// Port B used for DAC outout
void PortB_Init(void){ 
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000002;     // 1) B clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTB_LOCK_R = 0x4C4F434B;   // 2) unlock PortB PF0  
  GPIO_PORTB_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTB_AMSEL_R = 0x00;        // 3) PB7-0 Disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R = 0xFF;          // 5) PB7-0 Outputs  
  GPIO_PORTB_AFSEL_R = 0x00;        // 6) PB7-0 No alternate function
	GPIO_PORTB_PUR_R = 0x11;
  GPIO_PORTB_DEN_R = 0xFF;          // 7) Enable digital pins PB7-0        
}

// Port E used for Piano Keys
void PortE_Init(void)
{ 
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000010;     // 1)  clock E
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTE_CR_R = 0x0F;           // allow changes to PE3-0       
  GPIO_PORTE_AMSEL_R = 0x00;        // 3) PE3-0 Enable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R = 0x00;          // 5) PE3-0 Inputs 
  GPIO_PORTE_AFSEL_R = 0x00;        // 6) PE3-0 No alternate function
	GPIO_PORTE_PUR_R = 0x0F;
  GPIO_PORTE_DEN_R = 0x0F;          // 7) Enable digital pins PE3-0   
}

void PortA_Init(void)
{ 
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000001;     // 1)  clock A
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTA_CR_R = 0x1C;           // allow changes to PA4-2       
  GPIO_PORTA_AMSEL_R = 0x00;        // 3) PA4-2 Enable analog function
  GPIO_PORTA_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTA_DIR_R = 0x00;          // 5) PA4-2 Inputs 
  GPIO_PORTA_AFSEL_R = 0x00;        // 6) PA4-2 No alternate function
	GPIO_PORTA_PUR_R = 0x1C;
  GPIO_PORTA_DEN_R = 0x1C;          // 7) Enable digital pins PE3-0   
}

// Port F is used for LEDs and SW 1
// PF4 and PF0 are input SW1 and SW2 respectively
// PF3,PF2,PF1 are outputs to the LED
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
	GPIO_PORTF_PUR_R = 0x11;
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
	GPIO_PORTF_IS_R &= ~0x11;       // (d) PF4,PF0 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;      //     PF4,PF0 is not both edges
  GPIO_PORTF_IEV_R &= ~0x11;      //     PF4,PF0 falling edge event
  GPIO_PORTF_ICR_R = 0x11;        // (e) clear flags 4,0
  GPIO_PORTF_IM_R |= 0x11;        // (f) arm interrupt on PF4,PF0
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00400000; // (g) priority 2
  NVIC_EN0_R = 0x40000000;        // (h) enable interrupt 30 in NVIC
}

// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06

// Subroutine to wait 0.1 sec
// Inputs: None
// Outputs: None
// Notes: ...
