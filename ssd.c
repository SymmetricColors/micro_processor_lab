#include <mega16.h>
#include <delay.h>
 flash char pat[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x58,0x00,0x10}  ;
   bit H=1;
   bit up=1;
   static int current_mode=0;
   int mode[]={0,1,2};
   bit reset=0;
interrupt [TIM0_OVF] void t0_isr(void)
{
    static char count = 1;
    static int n = 0 , temp = 0 , second = 0;
    int tt;
    
    if(current_mode==0){
        TCNT0 = -80;
    }
    if(current_mode==1){
        TCNT0 = -100;
    }
    if(current_mode==2){
        TCNT0 = -250;
    }
    
    PORTB=0;
    tt = temp%10 ;
    PORTA=pat [tt];
    PORTB=count;
    count<<=1;
    temp/=10;
       
    
    if(reset){
        n=0;
        reset=0;
        tt=0;
    }
    if(count == 0x10)
    {
       count = 1;
       temp = n;
       second ++;
       if(second == 50)
       {
            second = 0;
            if(H==1) 
                n++;
            if(up==0)
                n--;
               
       }
    }
}
void main()
{
       char i,cont;
       int temp=0;
       bit k1p,k1c,k2p,k2c,k3p,k3c,k4p,k4c;
       
       k1c=0;
       k2c=0;     
       k3c=0;
       k4c=0;
       DDRA=0xFF;
       DDRB=0x0f;
       DDRC=0X08;
       PORTC=0b11110000;

       TCCR0=2;
       TCNT0=-10;
       TIMSK = 1;
       #asm("sei")

       while(1)
       {   
        
         k1p=k1c;
         k2p=k2c;
         k3p=k3c;
         k4p=k4c;
         delay_ms(10);  
         k1c=PINC.7;
         k2c=PINC.6;      
         k3c=PINC.5; 
         k4c=PINC.4;
         

         if(current_mode == 0){
            PORTC.3 =1;       
            PORTC.2= 0;
                }
         if(current_mode == 1){
            PORTC.3 = 0;
            PORTC.2 = 1;
                }
         if(current_mode == 2){
            PORTC.3 = 1;
            PORTC.2 = 1;
                }
         if(k1p==1&&k1c==0)
         { 
             delay_ms(50); 
             if(PINC.7==0)
             {
                H =~H;
                PORTC.0 = H;
             }
         }     
         
         
         if(k2p==1&&k2c==0)
         { 
             delay_ms(50); 
             if(PINC.6==0)
             {
                up =~up;
                PORTC.1 = H;
             }
         }  
         
         
         if(k3p==1&&k3c==0)
         { 
             delay_ms(50); 
             if(PINC.5==0)
             {
                if (current_mode>2)  
                    current_mode=0;
                current_mode++;

                
             }
         } 
         
         if(k4p==1&&k4c==0)
         {
             delay_ms(50);
             if(PINC.4==0)
                reset=1;
         } 
         
       }
}