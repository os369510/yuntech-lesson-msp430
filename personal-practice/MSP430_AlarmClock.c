//include
#include <msp430x54x.h>
#include"msp430x54x.h" 
#include "stdlib.h"
#include "stdio.h"
/*setup
 * P9 is seven a~g,p
 * P1 is Input, interrupt
 * P2 is seven Addres
 * P3 is power LED
 */
 //Set Seven Array
 int seven_num[11]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00};
 int seven_addres[4]={0x10,0x8,0x4,0x2};
 int time_module1[6]={0,0,1,0,3,2};
 int time_module2[6]={0,0,0,0,0,0};
 int time_module3[6]={0,0,0,0,0,0};
 int time_module4[6]={0,0,0,0,0,0};
 int i=0,j=0,j_addres=0,j_count=0,i_addres=0,Delay_z=0,c=0;
 //module1
 int time_module1_first=0;
 //module2
 int modu2_sw1_count=0,module2_module=0,modu2_sw2_count=0,module2_num=0,time_on=0; //module_2
 int module2_check=0,module2_down=0;//glitter
 int module2_i_addres=0,module2_j_addres=0,module2_j_count=0,module2_start=0;//glitter
 int module2_glitter_count=0,module2_GG=0,module2_glitter_count2=0;//glitter
 int module2_checktime=0,i_c=0,module2_check_time=0;//interrupt
 //module3
 int modu3_sw1_count=0,modu3_sw2_count=0,module3_downmodule=0; // sw
 int module3_jump_down=0;
 int module3_check=0,module3_module=0,module3_num=0,module3_down=0;
 int module3_i_addres=0,module3_j_addres=0,module3_j_count=0,module3_start=0;
 int module3_glitter_count=0,module3_GG=0,module3_glitter_count2=0;//glitter
 //module4
 int module4_start=0;
 int module4_check=0,module4_old=0;
 //module
 int module_target=0;
 
 
 
void module3_clear(void){
	time_module3[0]=0;
	time_module3[1]=0;
	time_module3[2]=0;
	time_module3[3]=0;
	time_module3[4]=0;
	time_module3[5]=0;
}
	
void module4_clear(void){
	time_module4[0]=0;
	time_module4[1]=0;
	time_module4[2]=0;
	time_module4[3]=0;
	time_module4[4]=0;
	time_module4[5]=0;
}
int TimeModule(int num){
	if ( (!(P1IN & BIT0))&&(!(P1IN & BIT1)) ) return time_module1[num];
	else if ( (!(P1IN & BIT0))&&(P1IN & BIT1) )	return time_module2[num];
	else if ( (P1IN & BIT0)&&(!(P1IN & BIT1)) ) return time_module3[num];
	else if ( (P1IN & BIT0)&&(P1IN & BIT1) ) return time_module4[num];
}
void seven_0(void){
	P9OUT=0xff;
	P2OUT=0xff;
}
void Delay(int Delay_z){
	int q=0;
	for (q=1;q<=Delay_z;q++);
}
void seven(int num,int addres){
	P9OUT=~seven_num[num];
	P2OUT=~seven_addres[addres];
	if (addres==2) P9OUT&=~BIT7;
	else P9OUT|=BIT7;
}
void seven_glitter2(int addres,int num){
	if(addres<5) module2_i_addres=0;
	if(addres>=5) module2_i_addres=1;
	for (j=module2_i_addres*2,module2_j_addres=0;j<=3+(module2_i_addres*2);j++,module2_j_addres++){
		if ((j==addres-1 || ((addres==6&&addres==5)&&j==addres-5)) && module2_GG==0 && module2_glitter_count<=100) {
			module2_glitter_count++;
			continue;
			}
		if (module2_glitter_count==100) {
			module2_GG=!module2_GG;
			module2_glitter_count=0;
			}
		if (module2_GG==1)  module2_glitter_count2++;
		if (module2_glitter_count2==400) {
			module2_GG=!module2_GG;
			module2_glitter_count2=0;
			}
		if (j==addres-1) time_module2[addres-1]=num;
		seven(TimeModule(j),module2_j_addres);
		Delay(5);
		}
}
void seven_glitter3(int addres,int num){
	if(addres<5) module3_i_addres=0;
	if(addres>=5) module3_i_addres=1;
	for (j=module3_i_addres*2,module3_j_addres=0;j<=3+(module3_i_addres*2);j++,module3_j_addres++){
		if ((j==addres-1 || ((addres==6&&addres==5)&&j==addres-5)) && module3_GG==0 && module3_glitter_count<=100) {
			module3_glitter_count++;
			continue;
			}
		if (module3_glitter_count==100) {
			module3_GG=!module3_GG;
			module3_glitter_count=0;
			}
		if (module3_GG==1)  module3_glitter_count2++;
		if (module3_glitter_count2==400) {
			module3_GG=!module3_GG;
			module3_glitter_count2=0;
			}
		if (j==addres-1) time_module3[addres-1]=num;
		seven(TimeModule(j),module3_j_addres);
		Delay(5);
		}
}
void Module_1(void){;}
void Module_2(void){
	if ( (P1IN & BIT2) && (modu2_sw1_count==0) && (!(P1IN & BIT3))) {
		module2_module++;
		module2_num=0;
		modu2_sw1_count=!modu2_sw1_count;
		if (module2_module==7) module2_module=0;
		Delay(50);
		}
	if ( !(P1IN & BIT2) && (modu2_sw1_count==1) && (!(P1IN & BIT3))) modu2_sw1_count=!modu2_sw1_count;
	
	if ( (P1IN & BIT3) && (modu2_sw2_count==0) && (module2_module!=0) ) {
		module2_num++;
		modu2_sw2_count=!modu2_sw2_count;
		if (module2_num==6 && (module2_module%2==0)) module2_num=0;
		if (module2_num==10 && (module2_module%2==1)) module2_num=0;
		Delay(50);
		}
		
	if ( !(P1IN & BIT3) && (modu2_sw2_count==1) && (module2_module!=0)) modu2_sw2_count=!modu2_sw2_count;
	
    if(module2_module!=0) seven_glitter2(module2_module,module2_num);
	
}
void Module_3(void){ // test ok!!
	
	if (module3_check==0){
		module3_clear();
		module3_check=!module3_check;
		}
	if ( (P1IN & BIT2) && (modu3_sw1_count==0) && (!(P1IN & BIT3))&& module3_start==0 ) {
		module3_module++;
		module3_num=0;
		modu3_sw1_count=!modu3_sw1_count;
		if (module3_module==7) module3_module=0;
		Delay(50);
		}
	if ( !(P1IN & BIT2) && (modu3_sw1_count==1) && (!(P1IN & BIT3))&& module3_start==0 ) modu3_sw1_count=!modu3_sw1_count;
	
	if ( (P1IN & BIT3) && (modu3_sw2_count==0) && (module3_module!=0) ) {
		module3_num++;
		modu3_sw2_count=!modu3_sw2_count;
		if (module3_num==6 && (module3_module%2==0)) module3_num=0;
		if (module3_num==10 && (module3_module%2==1)) module3_num=0;
		Delay(50);
		}
		
	if ( !(P1IN & BIT3) && (modu3_sw2_count==1) && (module3_module!=0)) modu3_sw2_count=!modu3_sw2_count;
		
	if ( (module3_jump_down==0) && (P1IN & BIT3) && (P1IN & BIT2) && !(time_module3[0]==0&&time_module3[1]==0&&time_module3[2]==0&&time_module3[3]==0&&time_module3[4]==0&&time_module3[5]==0) ) {
		module3_start=!module3_start;
		Delay(50);
		module3_downmodule=1;
		module3_jump_down=1;
		}
	if (!((P1IN & BIT3) && (P1IN & BIT2))) module3_jump_down=0;
	
	if (module3_module!=0 && module3_start==0 && (!module3_downmodule) ) seven_glitter3(module3_module,module3_num);
	
	if (time_module3[0]==0&&time_module3[1]==0&&time_module3[2]==0&&time_module3[3]==0&&time_module3[4]==0&&time_module3[5]==0) {
		module3_downmodule=0;
		P3OUT|=0x01;
		}
	else P3OUT&=0xfe;
}
void Module_4(void){ //test OK!!
	if (module4_check==0){
		module4_clear();
		module4_check=!module4_check;
	}
	if ( (P1IN & BIT2) &&  module4_old==0) {
		module4_start=!module4_start;
		module4_old=1;
	}
	if ( !(P1IN & BIT2) && module4_old==1 ) module4_old=0;
	if ( (P1IN & BIT3)) module4_clear();
}
void LED_set(void){
	P9DIR|=0xff;//P9.0~P9.7 is Output
	P9OUT&=~0x00;//Power is Low
	
	P1DIR&=0xf0;//P1.0~P1.3 is Input
	P1OUT|=0x0f;//pullup
	
	P2DIR|=0x1f;//P2.0~P2.3 is Output
	P2OUT=~0x00;//Power is Low
	
	P3DIR|=0x03;//P3.0~1 is Output
	P3OUT=0x00;//Power is Low
}
void Set_TimerA0(void){
	WDTCTL = WDTPW + WDTHOLD; 
	TA0CCTL0 = CCIE;                          // CCR0 interrupt enabled
	TA0CCR0 = 550;
	TA0CTL = TASSEL_2 + ID_3 + MC_3 + TACLR;                // SMCLK, contmode 
	TA0EX0 = 0x0001;
}
//Module 1 (00 b), hour and min and sec
//Module 2 (01 b), set AlarmClock , H,M,S
//Module 3 (10 b), CountDown Timer
//Module 4 (11 b), StopWatch
void main(void)
{
	//Setup
	Set_TimerA0();
	LED_set();
	__bis_SR_register(GIE);
	while(1){
		if ( (!(P1IN & BIT0))&&(!(P1IN & BIT1)) ) {
			Module_1();
			module_target=1;
			}
		else if ( (!(P1IN & BIT0))&&(P1IN & BIT1) ){
			Module_2();
			module_target=2;
			}
		else if ( (P1IN & BIT0)&&(!(P1IN & BIT1)) ) {
			Module_3();
			module_target=3;
			}
		else if ( (P1IN & BIT0)&&(P1IN & BIT1) ){
			Module_4();
			module_target=4;
			}
		
		if (  ((module_target!=2)&&(module_target!=3)) || ( ((module_target==3)&& (module3_module==0))  ||  ((module_target==2)&&(module2_module==0)))   ){
			for (j=i_addres*2,j_addres=0;j<=3+(i_addres*2);j++,j_addres++,j_count++){
				seven(TimeModule(j),j_addres);
				Delay(50);
				}
			if(j_count==2000){
				if (i_addres==0) i_addres=1;
				else i_addres=0;
				j_count=0;
				}
			}
		if (module_target!=3) {
			module3_check=0;
			module3_module=0;
			module3_num=0;
			P3OUT|=0x01;
			}
		if (module_target!=4) module4_check=0;
		if (time_on==1 && (P1IN & BIT2) && (P1IN & BIT3) ){
			time_on=0;
			P3OUT&=0xfd;
		}	
		for (i_c=0;i_c<=5;i_c++){
		if (time_module1[i_c]==time_module2[i_c]) module2_check_time++;
		}
		if (module2_check_time==6) module2_checktime=1;
		else module2_check_time=0;
		if (module2_checktime==1) {
			P3OUT|=0x02;
			time_on=1;
			module2_checktime=0;
			}
		}
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void){
		
	
	if (module_target==3 && module3_start==1 && !(P1IN & BIT3) && !(P1IN & BIT2)){
		if (time_module3[0]==0&&time_module3[1]==0&&time_module3[2]==0&&time_module3[3]==0&&time_module3[4]==0&&time_module3[5]==0) {
			module3_start=0;
			module3_num=0;
			}
		time_module3[0]--;
		if (time_module3[0]==-1){
			time_module3[1]--;
			time_module3[0]=9;
			}
		if (time_module3[1]==-1){
			time_module3[2]--;
			time_module3[1]=5;
			}
		if (time_module3[2]==-1){
			time_module3[3]--;
			time_module3[2]=9;
			}
		if (time_module3[3]==-1){
			time_module3[4]--;
			time_module3[3]=5;
			}
		if (time_module3[4]==-1){
			time_module3[5]--;
			time_module3[4]=3;
			}
		if (time_module3[5]==-1)time_module3[5]=0;
		if (time_module3[0]==0&&time_module3[1]==0&&time_module3[2]==0&&time_module3[3]==0&&time_module3[4]==0&&time_module3[5]==0) {
			module3_start=0;
			module3_num=0;
			}
		}
	
	
	
	
	if (module_target==4 && module4_start==1){
		time_module4[0]++;
		if (time_module4[0]==10){
			time_module4[1]++;
			time_module4[0]=0;
			}
		if (time_module4[1]==6){
			time_module4[2]++;
			time_module4[1]=0;
			}
		if (time_module4[2]==10){
			time_module4[3]++;
			time_module4[2]=0;
			}
		if (time_module4[3]==6){
			time_module4[4]++;
			time_module4[3]=0;
			}
		if (time_module4[4]==10){
			time_module4[5]++;
			time_module4[4]=0;
			}
		if (time_module4[5]==2&&time_module4[4]==4){
			time_module4[5]=0;
			time_module4[4]=0;
			}
		}
		
		
		
	time_module1_first++;
	if(time_module1_first==60){
		time_module1_first=0;
		time_module1[0]++;
	}
	if (time_module1[0]==10){
		time_module1[1]++;
		time_module1[0]=0;
		}
	if (time_module1[1]==6){
		time_module1[2]++;
		time_module1[1]=0;
		}
	if (time_module1[2]==10){
		time_module1[3]++;
		time_module1[2]=0;
		}
	if (time_module1[3]==6){
		time_module1[4]++;
		time_module1[3]=0;
		}
	if (time_module1[4]==10){
		time_module1[5]++;
		time_module1[4]=0;
		}
	if (time_module1[5]==2&&time_module1[4]==4){
		time_module1[5]=0;
		time_module1[4]=0;
		}
		
	TA0CCR0 = 550; 
		
}