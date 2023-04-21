#define ABS_ctrl_LIB
#include "ABS_ctrl_lib.h"

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_TAUD0_5.h"
#include "Config_TAUD0_7.h"
#include "Config_TAUD0_9.h"
#include "Config_TAUD0_11.h"
/*==============================================
		main Routine
  ==============================================*/            
/*-----------------------------------------------*/

/*-----------------------------------------------*/

//#define XS 56520



/*-----------------------------------------------*/



/*-----------------------------------------------*/

/*-----------------------------------------------*/



///////////////////////////////////////////////////////////////////////////
//extern init_can20b(unsigned char in_baudrate);
   


/*--------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                     //////////////////////////////////////////////////////// ABS/ASR, CAN BUS, SAE J1939, Add on 06.8.18 



//////////////////////////////////////////////////////////////////////////////////////////////////////////Subroutines
#pragma INTERRUPT bt_int    // M30290 have Add on 07.9.28
//#pragma INTERRUPT icoc0_int // M30290 have
//#pragma INTERRUPT icoc1_int // M30290 have 

unsigned int Tawj2[4]={0,0,0,0};
unsigned char ASR2_Entry_Flag2=0,ASR3_Entry_Flag3=0;
unsigned char LV_SwitchFlag_ByteCount; // added at 2014.2.23
unsigned char LV_OffRoad_ByteCount[4],LV_OffRoad_st1_Count[4],LV_OffRoad_ControlFlag[4]; // added at 2014.2.25
unsigned char LV_Count_ij; // added at 2014.3.7
unsigned char GV_st5_Enter_group[4]; // added at 2015.10.16 by sz

void Vspeed(void)
{ 
  unsigned char j=0,i=0;
  
  if((ctrlflag!=1)||(End_Break==1))
  {
/////////////2007.12.4
   Vssou=(unsigned int)(Vwo);
   
   Vmax=Vwo; Vs2=Vwo; Vsmin=Vwo; Vmin=Vwo; Vs1=Vs;

    Tgro=0; Vsr=Vwo+100;
    for(j=0;j<4;j++)  
    {
     if(Aw[j]>1600&&Vw[j]>(Vs+200)) {Tkcn[j]=1;TStepNum[j]=0;}

     TStepNum[j]++;
     if(TStepNum[j]>0) {TStepNum[j]=0;Vw1[j]=Vw[j];}

     if(Tkcn[j]>0) 
      {
      	Tkcn[j]++; if(Tkcn[j]>200) {Tkcn[j]=0;Vw1[j]=Vw[j];}
      }
     
     if((Vw1[j]>Vmin)&&(Tkcn[j]<1)) {Vmin=Vw1[j];Kvh_old=j;}
     if(Vs_st[j]>Vssou) {Vssou=Vs_st[j];}
     if(Vw1[j]>Vmax) {Vmax=Vw1[j];}
     Vmax2[0]=Vmax;////Add on 08.4.30
     if(Vw[j]<Vsr) Tgro++;
     
    }

    //if(Tgro>2){Tvmaxsr++;Vs1=Vwo;Vs=Vwo;Vmax=Vwo;Vmin=Vwo;} 
    if(Tgro>2){Tvmaxsr++;Vs1=Vwo;Vs=Vwo;Vmax2[0]=Vwo;Vmin=Vwo;}////Modify on 08.4.29;08.4.30
    else{ Tvmaxsr=0; }
    if(Tvmaxsr>5){ Tvmaxsr=6; }

    Tgro=0;
    for(j=0;j<4;j++)  
     { ////////////////////////////////////////Modify on 08.4.30
       if(Vw1[j]<Vmax2[0]) {SR[j]=(signed int)(100-(100*Vw1[j])/Vmax2[0]);}
       else {if(Vw1[j]>Vmax2[0]){SR[j]=(signed int)((100*Vmax2[0])/Vw1[j]-100);}else{SR[j]=0;}}
       
       if(SR[j]>-10&&SR[j]<10) Tgro++;
     }
    if(Tgro>2) Vmin=Vmax2[0];////Modify on 08.4.30
     
    for(j=0;j<4;j++)  
    {
     if((Vw1[j]>Vsmin)&&(Vw1[j]<=Vmin)) {if(j!=Kvh_old) {Vsmin=Vw1[j];Tckg2=j;}}
     if(j<2) {if(Vw1[j]>Vs2) {Vs2=Vw1[j];} }
    }

    for(j=0;j<4;j++)  
     {
       if(Vw1[j]<Vsmin) {SR[j]=(signed int)(100-(100*Vw1[j])/Vsmin);}
        else {if(Vw1[j]>Vsmin) {SR[j]=(signed int)((100*Vsmin)/Vw1[j]-100);} else {SR[j]=0;}}
     
       if(SR[j]>-10&&SR[j]<10&&Vw1[j]>Vwo) 
        {
         if(Vw0[j]<9000) Vw0[j]++; 
         if(Vw0[j]>5) Tkcn[j]=0;
        }
        else Vw0[j]=0;
     }

    Tgro=0; Kvq_old=0;
    for(j=0;j<4;j++)  
     {
       if(Vw1[j]>Vsr) Tgro++;
     }
    if(Tgro>3) Kvq_old=5;

    Tgro=0; 
    for(j=0;j<4;j++)  
     {
       if(Vw0[j]>0) Tgro++;
       if(SR[j]>0&&SR[j]<6&&Vw1[j]>Vwo) {if(Vw1[j]<Vsmin) Vsmin=Vw1[j];}
     }

////////////////////2007.12.4
  

    if((Vw0[0]>0)||(Vw0[1]>0))
    {
     if((Vw0[0]>0)&&(Vw0[1]>0)) {if(Vsmin<Vs2) Vs1=Vsmin; else Vs1=Vs2-100;}
      else 
      {if(Vw0[0]>0) {if(Vsmin<Vw1[0]) Vs1=Vsmin; else Vs1=Vw1[0]-100;}
        else if(Vw0[1]>0) {if(Vsmin<Vw1[1]) Vs1=Vsmin; else Vs1=Vw1[1]-100;}
      }
    }
    else
    {

      if(Tgro>1)
       {
       	if(Kvq_old<5)
       	{
         if(Vw0[Tckg2]>6000) Vs1=Vsmin-100; 
          else 
          {if(Vs2<Vsmin) Vs1=Vs2; else Vs1=Vsmin-100;}
        }
        else 
         {
          if(Vw0[Tckg2]>4000) Vs1=Vsmin-100; 
           else 
           {if(Vs2<Vsmin) Vs1=Vs2; else Vs1=Vsmin-100;}
         }
       }
       else 
       {
        for(j=0;j<4;j++) { Vw0[j]=0; }
        Vs1=Vs;
       }

      //if(Vs<2800&&Vs2<2800) {Vs1=Vs2-100;}//Must be move!!!
    }

    Vs1=(Vs1+Vs+Vs+Vs)>>2;
    Vsso=(long int)(Vssou);
    Vsr=(long int)(Vs_w);
    
    if(Vs1>(Vsso+Vsr)) Vs1=Vsso+Vsr; 
   
    Vs=Vs1;
    	
         if(Tslip>0)
         { 
          Vsh=Vsh-45;
          if(Vs>(Vsh+160)) Vsh=(Vsh+Vs-50)>>1;              //2007.5.18
         }
         else {Vsh=(Vsh+Vs)>>1;}
          
          for(j=0;j<SS;j++)
          {
           if(Vsh>Vw[j]) { SR[j]=(signed int)(100-(100*Vw[j])/Vsh);}  //2008.1.28
             else {if(Vsh<Vw[j]) {SR[j]=(signed int)((100*Vsh)/Vw[j]-100);} else {SR[j]=0;}} //SR[j]=0;  //2006.2.16
           if(SR[j]>100) SR[j]=0;
           if(SR[j]<-100) SR[j]=0;
          } 

  }

////////////////////////////////////  A Way 2
  
     Tvswitch++;
     if(Tvswitch==2)  ////2-->1,2006.12.15; 1 --> 2,Modify on 07.1.30------------
      {
       for(i=0;i<SS;i++)
        {  
          if(Tvswitch==1) 
           {Vsr=Aw[i]>>1;
            //Aw[i]=6*(Vwm[i][3]-Vwm[i][0])+2*(Vwm[i][2]-Vwm[i][1]);////2  4 point 10.24  5ms
            Aw[i]=(long int)(3*(Vwm[i][6]-Vwm[i][0])+(Vwm[i][4]-Vwm[i][2]));////2  4 point 10.24  10ms

            Awr[i]=(Aw[i]+Vsr);
            Aw[i]=Aw[i]+Aw[i];
           } 
           if(Tvswitch==2) 
           {Vsr=Aw[i]>>1;         ////07.3.14
             
            Aw[i]=3*(Vwm[i][6]-Vwm[i][0])+(Vwm[i][4]-Vwm[i][2]);               //  2  4 point 20ms
            // Aw[i]=Aw[i]>>1; //2005.12.28
            
            Awr[i]=(Aw[i]+Vsr);       ////07.3.14
            Aw[i]=Aw[i]+Aw[i];  ////07.3.14
           }
           //Aw[i]=3*(Vwm[i][3]-Vwm[i][0])+(Vwm[i][2]-Vwm[i][1]);               //  2  4 point 10ms
           if(ctrlflag!=1) Awb[i]=(Awb[i]+Aw[i])>>1;
           //Awb[i]=Aw[i];
           
            Vwm[i][0]=Vwm[i][1];
            Vwm[i][1]=Vwm[i][2];
            Vwm[i][2]=Vwm[i][3];
            Vwm[i][3]=Vwm[i][4];
            Vwm[i][4]=Vwm[i][5];
            Vwm[i][5]=Vwm[i][6];
            Vwm[i][6]=(int)(Vw[i]>>1);

            if(Kv>19) {Vsr=Vwm[i][5];} else {Vsr=Vwm[i][4];}
            if(Vwm[i][6]>Vsr) {if(Tvnold[i]>0) Tvnold[i]++; else Tvnold[i]=1;}
            	 else 
            	 	{if(Vwm[i][6]<Vsr) 
            	 		 {if(Tvnold[i]<0) Tvnold[i]--; else Tvnold[i]=-1;}
            	 		else 
            	 		 {if(ctrlflag==1)
            	 		 	 {if(SR[i]<85) {if(Tvnold[i]>0) Tvnold[i]++; else Tvnold[i]=1;}
            	 		    else {if(Tvnold[i]<0) Tvnold[i]--; else Tvnold[i]=-1;}
            	 		   }
            	 		 }
            	 	}

            if(Tvnold[i]<-3200) Tvnold[i]=-3200;
            if(Tvnold[i]> 3200) Tvnold[i]= 3200;

        }
       Tvswitch=0; 
      }

}

//////////////////////////////////////////////////////////////////////////////RSC-----------------
///////////////////////////////////////////////////////////////G_Acc g_ad,RSC_Ctrl, Add on 09.3.4
void RSC_Ctrl(void)
{
/* //2009.11.17
 {if(G_RSC_flag<4) 
	{if((G_acc[1]>55)&&(Vs>10000&&ASR_Switch_Flag!=0xa5&&unctrl!=1))////0.2g   Vs=6000 ->13000
	 { G_RSC_flag++;
	 	 if(G_RSC_flag>3)
	 	 { G_RSC_flag=4;
	 	 	 ctrlflag=5; 
       ASRLamp=1;////////////////////////////////////ASR_Lamp=1
       Lamp_Condition &= 0xbf;////ASR Lamp Light, bit6==0
       Check_Flag_Clear();
       KI2=8;KP2=24;
       G_RSC0=0;////0g
       if(Vs<9000){ G_RSC0=57; }
       else if(Vs<13000){ G_RSC0=53; }
       else if(Vs<18000){ G_RSC0=52; }
       else{ G_RSC0=50; }
     }
	 }
	 else{ G_RSC_flag=0; }
	}
	if(G_RSC_flag>3) 
  {
  	 PIDE2[2]=PIDE2[1];
     PIDE2[1]=PIDE2[0];
     PIDE2[0]=G_RSC0-G_acc[1];
     //PID_out=KI*PIDE[0]+KP*(PIDE[0]-PIDE[1])+KD*(PIDE[0]-2*PIDE[1]+PIDE[2]);
     PID_out2=KI2*PIDE2[0]+KP2*(PIDE2[0]-PIDE2[1]);
     PID_out2=PID_out2>>3; 
     if(PID_out2>5){ PID_out2=5; }
     if(PID_out2<-5){ PID_out2=-5; }
     	
     TSC1_Control=0xef;//////0xef:both Speed&Torque,prioity==2
     Eng_Request_Speed=0xfaff;
     if(CAN_Eng_Torque>124&&CAN_Eng_Torque<225)      
     { Eng_Request_Torque=CAN_Eng_Torque+PID_out2;////Eng_Torque: -125%~125%   
     }
     else{ Eng_Request_Torque=CAN_Eng_Torque; }
     if(Eng_Request_Torque<125){ Eng_Request_Torque=0x7d; }//08.3.1
     if(Eng_Request_Torque>224){ Eng_Request_Torque=0xe0; }
     
     if(G_acc[1]>58)
     {ASR_DIFCtrl=1;/////ASR_DIF=1
      if(ctrlflag!=1)
      {F_ctrl_SetHigh;//able Vbb
       Fearth=1;//able EARTH 
       if(SR[2]>8||SR[3]>8)	
       { DI[2]=3;DI[3]=3; }
       else if(SR[2]>5||SR[3]>5)	
       { DI[2]=1;DI[3]=1; }
       else
       { DI[2]=0;DI[3]=0; }
       Val_Act(2);Val_Act(3);
      }
     }
     if(G_acc[1]<50)
     { RSC_Exit_Flag++;
     	 if(RSC_Exit_Flag>7)
     	 { RSC_Exit_Flag=0;G_RSC_flag=0;
     	 	 ctrlflag=0;ASRLamp=0;
     	 	 ASR_DIFCtrl=0;
     	 	 if(ctrlflag!=1)
     	 	 { DI[2]=0;DI[3]=0;
     	 	 	 Val_Act(2);Val_Act(3);
     	 	 } 
         Lamp_Condition |= 0x40;
         TSC1_Control=0xfc;/////0xfc:No RSC Engine Ctrl
         Eng_Request_Speed=0xfaff; 
         Eng_Request_Torque=0xfa;
     	 }
     }		
     else{ RSC_Exit_Flag=0; }
  }
 }
*///2009.11.17
}

///////////////////////////////////////////////////////////////////////////////ASR-----------------
#if 0
void CANJ1939_Identify(void)
{
 ///////////////////////////////////////////////////////////////////////////////// ABS-ASR Status,Modify on 06.12.30
 //if(ctrlflag==0){ Status_EBC1=0xc0; }
 //else if(ctrlflag==1){ Status_EBC1=0xd0;ASR_Entry_Flag[1]=0; } /////// 06.12.8 
 if(ctrlflag==0){ Status_EBC1=0x00; }	//20190716 change by KXL from 0xc0 to 0x00 for CAN cancel RQ      
 else if(ctrlflag==1){ Status_EBC1=0x01;ASR_Entry_Flag[1]=0; } /////// 06.12.8      //20190716 change by KXL from 0xd0 to 0x01 for CAN cancel RQ   
 else if(ctrlflag==2)
 //{if(ASR_Entry_Flag[1]<6){ Status_EBC1=0xc4; } 
 {if(ASR_Entry_Flag[1]<6){ Status_EBC1=0x04; } //20190716 change by KXL from 0xc4 to 0x04 for CAN cancel RQ   
  else
  //{ if(ASR_Entry_Flag[0]<6&&ASR_Entry_Flag[2]<6&&ASR_Entry_Flag[4]<6){ Status_EBC1=0xc1; }
  //  else{ Status_EBC1=0xc5; }
  { if(ASR_Entry_Flag[0]<6&&ASR_Entry_Flag[2]<6&&ASR_Entry_Flag[4]<6){ Status_EBC1=0x10; } //20190716 change by KXL from 0xc1 to 0x10 for CAN cancel RQ 
    else{ Status_EBC1=0x14; } //20190716 change by KXL from 0xc5 to 0x14 for CAN cancel RQ 
  }
 }
 //else if(ctrlflag==3){ Status_EBC1=0xc1; }
 else if(ctrlflag==3){ Status_EBC1=0x10; } //20190716 change by KXL from 0xc1 to 0x10 for CAN cancel RQ
 
 ////////////////////////////////////////////////////////////////////////Add on 08.4.11;Modify on 08.4.22
 if(Switch_Flag==0xa5)    //////////ABS off-road switch
 //{ Status_EBC1 |= 0x30; }
 { Status_EBC1 |= 0x03; }	//20190716 change by KXL from 0x30 to 0x01 for CAN cancel RQ
 if(ASR_Switch_Flag==0xa5||Diagnose_Data[1]!=0||Diagnose_Data[2]!=0||
    Diagnose_Data[3]!=0||Diagnose_Data[4]!=0||Diagnose_Data[5]!=0||Diagnose_Data[6]!=0)//ASR off-road switch,Sensors
 //{ Status_EBC1 |= 0x0f; }
 { Status_EBC1 |= 0x3c; }	//20190716 change by KXL from 0x0f to 0x3c for CAN cancel RQ
 if(unctrl!=0)    
 //{ Status_EBC1 |= 0x3c; }
 { Status_EBC1 |= 0x0f; }	//20190716 change by KXL from 0x3c to 0x0f for CAN cancel RQ
 if(Diagnose_Data[9]!=0||Diagnose_Data[10]!=0||Diagnose_Data[11]!=0||Diagnose_Data[12]!=0||Diagnose_Data[18]!=0)//Valves
 { Status_EBC1 |= 0x0c; }
 //////////////////////////////////////////////////////////////////////////////////
 
 if(Switch_Flag==0xa5)    ////////// ABS off-road switch
 { Status_EBC2=0xc1; }
 else{ Status_EBC2=0xc0; }
 
 ////////////////////////////////////////////////////////////Add on 08.4.11
 if(ASR_Switch_Flag==0xa5)    ////////// ASR off-road switch
 { Status_EBC2 &= 0xf7;Status_EBC2 |= 0x04; }
 else{ Status_EBC2 &= 0xf3; }
 ///////////////////////////////////////////////////////////
 
 ///////////////////////////////////// Modify on 07.9.14;Modify on 07.12.7
 ///////////////// Add on 07.9.17
 if(ABSLamp==0)
 { Lamp_Condition &= 0x7f; }   //// ABS Lamp Light, bit7==0; 
 if(ABSLamp==1)
 { Lamp_Condition |= 0x80; }   //// ABS Lamp die, bit7==1;       
 if(ASRLamp==0) 
 { Lamp_Condition |= 0x40; }   //// ASR Lamp die, bit6==1; 
 if(ASRLamp==1)  
 { Lamp_Condition &= 0xbf; }   //// ASR Lamp Light, bit6==0;   
 
 if((Lamp_Condition|0x7f)==0x7f)  ////// ABS amber warning state,Lamp On
 { Status_EBC_State &= 0xdf;
   Status_EBC_State |= 0x10; 
   //////////////////////////// Add on 07.12.7
   ABS_LED_Condition &= 0xf7; 
   ABS_LED_Condition |= 0x04;
 }
 if((Lamp_Condition&0x80)==0x80)  ////// ABS amber warning state,Lamp Off
 { Status_EBC_State &= 0xcf;
   ABS_LED_Condition &= 0xf3; /////// Add on 07.12.7
 }
 if((Lamp_Condition|0xbf)==0xbf)  ////// ASR amber warning state,Lamp On
 { Status_EBC_State &= 0x7f;
   Status_EBC_State |= 0x40;
 }
 if((Lamp_Condition&0x40)==0x40)  ////// ASR amber warning state,Lamp Off
 { Status_EBC_State &= 0x3f; }
 
 //////////////////////////////////ABS fully operational,Add on 08.4.11
 if(ABSLamp==0||Switch_Flag==0xa5)
 { Status_EBC_State &= 0xfc; }
 if(ABSLamp==1&&Switch_Flag==0)
 { Status_EBC_State &= 0xfd; 
   Status_EBC_State |= 0x01;
 }
 
	///////////////////////////////////////////////////////////////////// Have or No CAN SAE J1939,06.10.16
 if(HaveorNoJ1939_Flag<200)
 { HaveorNoJ1939_Flag++; }
 if(ctrlflag==0&&HaveorNoJ1939_Flag>80)  /////// 80*5ms==400ms
 { CAN_Status_EEC1=0xff;CAN_Eng_Speed=0xfaff;HaveorNoJ1939_Flag=0;
 }
 if(CAN_Status_EEC1==0xff&&CAN_Eng_Speed==0xfaff)
 { if(HaveCANJ1939_Flag<40)
   { HaveCANJ1939_Flag=0; }
   if(NoCANJ1939_Flag<200)////140*5ms==700ms;100->140,08.2.19;140->200,08.6.25  
   { NoCANJ1939_Flag++; }
   if(NoCANJ1939_Flag==200&&(HaveCANJ1939_Flag==40||CANJ1939_Read==0xa5))////200,08.6.25
   {if(Diagnose_Data24_CAN==0x5a)
    { Diagnose_Data[24]=Diagnose_Data[24]|0x0100;
    }
    else
    { Diagnose_Write(24,0x0100);
    }
      Error_Flash[35]=0x0075;         // Flash Code
      //Error_Flash[0]=0x0075;         // Current Error Flash Code;Move on 08.4.11
      Diagnose_Data[0]=0x1805;   // Current Error,be Move!!!!
      
      //ASRLamp=1;  /////////////// ASR-Lamp Light;Move on 07.4.10
   }
   /*///////////////Move on 08.7.8-4
   if(NoCANJ1939_Flag>6)    /////////// Add on 06.10.16 
   { if(Lampad_High==0x55)  
     { WRITE(59,0x00);
       CANJ1939_Read=0;
    
       Select_Control&=0xfb; 
     }
   }
   */   
 } 	
 else
 { NoCANJ1939_Flag=0;
   if((CAN_Status_EEC1!=0xff&&CAN_Status_EEC1!=0xfc&&CAN_Status_EEC1!=0xfd)&&(CAN_Eng_Speed!=0xfaff))       
   { if(HaveCANJ1939_Flag<40)///////////40*5ms==200ms  
     { HaveCANJ1939_Flag++; }
     if(HaveCANJ1939_Flag==40)
     { 
       if(Error_Flash[35]==0x0075){ Error_Flash[35]=0; }
       //if(Error_Flash[0]==0x0075){ Error_Flash[0]=0x0011; };Move on 08.4.11
       if(Diagnose_Data[0]==0x1805){ Diagnose_Data[0]=0; }
       if(((Diagnose_Data[24]&0xfff0)&0x0100)==0x0100)
       { Diagnose_Data24_CAN=0x5a;
         Diagnose_Data[24]=Diagnose_Data[24]&0xfef0;   //////// 0xfeff:Have Error Times,0xfef0:No Error Times     
       }
             
       if(CANJ1939_Read!=0xa5)
       { WRITE(59,0xa5);
         CANJ1939_Read=0xa5;
         
         Select_Control|=0x04;   /////////// Add on 06.10.16
       }
     }     
   }
 }
}
#endif

void ASR_Engine_PID(void)  /////// Add on 06.7.24,Modify on 06.9.12
{
 unsigned char j=0;
 
 //if(ctrlflag!=2&&ctrlflag!=3)
 if(ASR_Entry_Flag[1]<6)       //////////////////////Modify on 08.2.19
 { TSC1_Control=0xfc;  //////0xfc:No ASR Engine Ctrl
   //if(ctrlflag==1){ TSC1_Control=0xff; }
   Eng_Request_Speed=0xfaff; 
   Eng_Request_Torque=0xfa;    ////////0xfa-->0xe1,06.12.8;Modify on 07.10.16
   //Eng_Request_Torque=0xe1;  ////////0xfa-->0xe1,06.12.8
 }
 ////////////////////////////////////////KWP2000-Ctrl Eng,Add on 08.2.28
 if(KWP_Control_Require==1)
 { TSC1_Control=0xef;  //////
   Eng_Request_Speed=0xfaff; 
   if(KWP_Eng_Torque==0x19){ KWP_Eng_Torque=0x1f; }//(WABCO:25%->9C)Add on 08.2.28
   Eng_Request_Torque=KWP_Eng_Torque+0x7d;  
 }	 
 //////////////////////////////////////////////////// ABS-ASR Status,Move to CANJ1939_Identify(),Modify on 06.12.30
 
 if(ctrlflag!=1)
 {  
  //PID_10msflag++;      ////Add on 06.9.12,Move on 07.1.31
  //if(PID_10msflag==2)  //// 2:10ms         
  { //PID_10msflag=0;
    /*
    ////////////////////////////////////////Add on 08.2.19
    SRmino=SRmin[0];  
    if(SRmin[1]>SRmino){ SRmino=SRmin[1]; }
    if(SRmin[2]>SRmino){ SRmino=SRmin[2]; }
    if(SRmin[3]>SRmino){ SRmino=SRmin[3]; }
    //if(SRmin[4]>SRmino){ SRmino=SRmin[4]; } //// 6S/6M
    //if(SRmin[5]>SRmino){ SRmino=SRmin[5]; }
    
    if(SRmaxo[0]>0){ SRref=SRmaxo[0]; }  
    if(SRmaxo[2]>0&&SRmaxo[2]>SRref){ SRref=SRmaxo[2]; }
    //if(SRmaxo[4]>SRref){ SRref=SRmaxo[4]; } //// 6S/6M,Move on 06.12.30
    */
  }
  
  if(ASR_Entry_Flag[1]<6)   
  { 
    ///////////////////////////////////////////////////////////////////Add on 06.12.22
    /*/////////////////////Move on 08.7.8-2
    No_sigl_ASR=0;
    if(Vs_ASR<1400)//////////////////////////// if,Add on 07.2.6-2;4-->9;08.7.4
    { if((nosiglflag[0]==0&&nosiglflag[1]>9)||(nosiglflag[1]==0&&nosiglflag[0]>9)){ No_sigl_ASR=2; } 
      if((nosiglflag[2]==0&&nosiglflag[3]>9)||(nosiglflag[3]==0&&nosiglflag[2]>9)){ No_sigl_ASR++; } 
      //if((nosiglflag[4]==0&&nosiglflag[5]>9)||(nosiglflag[5]==0&&nosiglflag[4]>9)){ No_sigl_ASR++; }////6S/6M   
      if(No_sigl_ASR>0&&No_sigl_ASR<3&&SRref>59){ No_sigl_ASR=0; }////Add on 07.2.6-2;79->59,08.6.25 
    }
    */      
    No_sigl_ASR_2=0;
    for(j=0;j<SS;j++)
    { if(Diagnose_Data[j+1]!=0)
      { No_sigl_ASR_2++; }
    }
    ////////////////////////////////==WABCO????,Add on 08.2.29;Move on 08.7.3
    //if(No_sigl_ASR_2==0)////4->9,08.6.25
    //{ if((Nocontrols[0]>9&&Nocontrols[1]>9)||(Nocontrols[2]>9&&Nocontrols[3]>9)){ No_sigl_ASR_2=2; } }
   
   ////30,37,07.2.5;Thoq,07.3.11;16,18,14,8400,08.2.19;18,20,15,08.5.26;16,18,14,08.6.5;18->20,08.7.8-2 
   if(((SRref>26&&Vs_ASR>2000&&Vs_ASR<8401)||(SRref>30&&Vs_ASR<2001)||
   	   (SRref>24&&Vs_ASR>8400))&&(Thoq==0x5a))
   { if(ASR_Entry_Flag[5]<200){ ASR_Entry_Flag[5]++; } }
   else{ ASR_Entry_Flag[5]=0; }
   
   //4S/4M,>9-> >6,07.2.4;>26-> >24,07.2.6;Diagnose_Data[],unctrl,SRmino,24->15,6->4,08.2.19
   //No_sigl_ASR_2<2 --> ==0,Move Diagnose_Data[],08.3.1-3
   ////&&Drive_Num==1(4X4),&&Thoq==0x5a,Add on 08.4.22;15->20,20->25,17->22,08.5.26
   ////17,23,20,Move:&&Thoq==0x5a,08.6.5;unctrl:==0->!=1,08.7.18
   if(((SRmax[0]<13&&SRmax[1]<13&&SRmaxo[0]>27&&Aw[0]>180&&Aw[1]>180&&Drive_Num==1)||(SRmino>33&&SRref>30)||
       (SRmax[2]<13&&SRmax[3]<13&&SRmaxo[2]>27&&Aw[2]>180&&Aw[3]>180)||(ASR_Entry_Flag[5]>4))&&
       ((No_sigl_ASR>2||No_sigl_ASR==0)&&No_sigl_ASR_2==0)&&
       (ASR_Switch_Flag!=0xa5&&ASR_Close_Flag!=0x55&&unctrl!=1))
   { ASR3_Entry_Flag3++;
   	 if(ASR3_Entry_Flag3>6) {ASR_Entry_Flag[1]++; ASR3_Entry_Flag3=0;}
     if(ASR_Entry_Flag[1]>5) ////////5->4,08.2.19;4->5,08.3.6
     {
      ///////////////////////////////////////////////////////////////////Add on 06.12.22
      No_sigl_ASR=0;   
      No_sigl_ASR_2=0;
      if(ASR_Entry_Flag[5]<5){ T_Single_SR=0x55; } ////Add on 08.2.19;7->5,08.2.26
      else{ T_Single_SR=0; } 
      Thoq=0;////07.3.11
      PID_out=0;////Add on 08.2.19
      Eng_Req_Torque_min=125;////Add on 08.2.19
      ASR_Entry_Flag[1]=6; 
      if(ctrlflag!=2){ ctrlflag=3; }
      ASRLamp_SetHigh;   ////////////////////////////////////  ASR_Lamp=1
      Lamp_Condition &= 0xbf; //// ASR Lamp Light, bit6==0,Add on 07.4.10
      
      Check_Flag_Clear();   ////////Add on 07.2.6
      
      //Status_EBC1|=0x01;  //// ASR engine control active,Add on 07.1.30
      
      ////////////KI,KP,07.2.5;KI,KP+4,08.2.19
      /*
      if(SRref>65) 
      {KI=10;KP=24;KD=0;}   
      else if(SRref>48) 
      {KI=8;KP=22;KD=0;}   
      else
      {KI=6;KP=20;KD=0;}   
      */
      KI=6;KP=20;KD=0;
      /////////////////////////////SR0+1,07.2.5;SR--,08.2.27    
      //if(ASR_Entry_Flag[5]>6)//////ASR_Entry_Flag[5]>9 --> >6,07.2.4 
      if(T_Single_SR!=0x55&&SRmino<12&&Vs_ASR<8401&&ctrlflag==2)//// Modify on 08.2.19
      {if(Vs_ASR>3000)////SR0\KI\KP,06.9.12 >75 -> >65,06.12.22;SRmaxo->Vs_ASR,08.2.19
       {SR0=16;} 
       else if(Vs_ASR>1400)     
       {SR0=18;}
       else
       {SR0=20;}
      }
      else 
      {if(Vs_ASR>3000)////SR0\KI\KP,06.9.12 >75 -> >65,06.12.22;SRmaxo->Vs_ASR,08.2.19
       {SR0=14;} 
       else if(Vs_ASR>1400)     
       {SR0=16;}
       else
       {SR0=18;}
      }
     }
   }
   else{ ASR_Entry_Flag[1]=0; }   
  }

  if((ctrlflag==2||ctrlflag==3)&&ASR_Entry_Flag[1]>5)
  {  
     PID_out_Old[2]=PID_out_Old[1];////Add on08.2.19
     PID_out_Old[1]=PID_out_Old[0];////Add on08.2.19  
     PID_out_Old[0]=PID_out;////Add on 07.1.30
    
     PIDE[2]=PIDE[1];
     PIDE[1]=PIDE[0];
     PIDE[0]=SR0-SRref;
     //PID_out=KI*PIDE[0]+KP*(PIDE[0]-PIDE[1])+KD*(PIDE[0]-2*PIDE[1]+PIDE[2]);
     PID_out=KI*PIDE[0]+KP*(PIDE[0]-PIDE[1]);
     PID_out=PID_out>>3;  ///// 06.12.8
    
     /////////////////////////////////////////// Add on 07.1.30
     if(PID_out>100){ PID_out=100; }
     if(PID_out<-100){ PID_out=-100; }
    
     //PID_out=(signed int)((PID_out+PID_out_Old[1]+PID_out_Old[2]+PID_out_Old[2])>>2);////Modify on 07.2.2;08.2.19
     //PID_out=(signed int)((PID_out+PID_out_Old[0]+PID_out_Old[1]+PID_out_Old[2])>>2);////Add on 07.2.5;08.2.19
        
     //PID_out=(signed int)((PID_out+PID_out_Old)>>1);
     //PID_out=(signed int)((PID_out+PID_out_Old)>>1);//// Add on 07.1.31
     
     if(PID_out>2){ PID_out=2; }
     if(PID_out<-2){ PID_out=-2; }
    
     ////////////////////when Vs>30km/h,SR0-4,Add on 06.12.30;36km/h(10000)->30km/h(8400),08.2.19 
     if(Vs_ASR>8400) //////08.2.19
     {////////////KI,KP,07.2.5;KI,KP+4,08.2.19
      /*
      if(SRref>45) 
      {KI=7;KP=24;KD=0;}   
      else if(SRref>35) 
      {KI=6;KP=22;KD=0;}   
      else
      {KI=6;KP=20;KD=0;}    
      */
      KI=6;KP=20;KD=0;
      /////////////////////////////SR0,07.2.5	
      //if(ASR_Entry_Flag[5]>6)//////ASR_Entry_Flag[5]>9 --> >6,07.2.4 
      if(T_Single_SR!=0x55&&SRmino<12)       ////// Modify on 08.2.19  
      {/*
       if(SRref>45)         ////SR0,08.2.19
       {SR0=22;} 
       else if(SRref>35)     
       {SR0=21;}
       else
       {SR0=20;}
       */
       SR0=16;//15->16,08.3.1
      }
      else 
      {/*
       if(SRref>45)         ////SR0,08.2.19
       {SR0=20;} 
       else if(SRref>35)     
       {SR0=19;}
       else
       {SR0=18;}
       */
       SR0=14;//13->14,08.3.1
      }
     }
     else                      ///////////////SR0-2,07.1.29
     {////////////KI,KP,07.2.5;KI,KP+4,08.2.19
      /*
      if(Vs_ASR>4800) 
      {KI=7;KP=22;KD=0;}   
      else if(Vs_ASR>3600) 
      {KI=6;KP=21;KD=0;}  
      else if(Vs_ASR>2500) 
      {KI=6;KP=20;KD=0;} 
      else if(Vs_ASR>1400) 
      {KI=6;KP=19;KD=0;}  
      else
      {KI=6;KP=18;KD=0;}      
      */
      KI=6;KP=20;KD=0;
      /////////////////////////////SR0,07.2.5
      //if(ASR_Entry_Flag[5]>6)//////ASR_Entry_Flag[5]>9 --> >6,07.2.4 
      if(T_Single_SR!=0x55&&SRmino<12&&ctrlflag==2)       ////// Modify on 08.2.19   
      {/*
       if(Vs_ASR>4800)         ////SR0+,SRmaxo->Vs_ASR,08.2.19
       {SR0=23;} 
       else if(Vs_ASR>3600)     
       {SR0=24;}
       else if(Vs_ASR>2500)     
       {SR0=25;}
       else if(Vs_ASR>1400)     
       {SR0=26;}
       else
       {SR0=30;}
       */
       SR0=18;//16->18,08.3.1
      }
      else 
      {/*
       if(Vs_ASR>4800)         ////SR0+,SRmaxo->Vs_ASR,08.2.19
       {SR0=20;} 
       else if(Vs_ASR>3600)    
       {SR0=21;}
       else if(Vs_ASR>2500)    
       {SR0=22;}
       else if(Vs_ASR>1400)     
       {SR0=23;}
       else
       {SR0=27;}
       */
       SR0=16;//13->16,08.3.1
      }
     }
      
      ////////////////////////////////////////////// Both Engine Speed && Engine Torque Ctrl Mode
      
      //TSC1_Control=0xc3;  //////0xc3:both Speed&Torque,prioity==0
      TSC1_Control=0xef;  //////0xef:both Speed&Torque,prioity==2
      Eng_Request_Speed=0xfaff;   //// Eng_Speed: 0~8031.875rpm,1/8 rpm/bit
      
      //if(CAN_Eng_Speed<4800||CAN_Eng_Speed>24000){ CAN_Eng_Torque=0xe0; }  //////// <600rpm,>3000rpm
      //if(CAN_Eng_Torque<125||CAN_Eng_Torque>225){ CAN_Eng_Torque=0xe0; }   //////// <0x7d,>0xe1
      
      ///////////////////////////////Move on 08.2.19
      //if(CAN_Eng_Torque==0xe0&&Eng_Request_Torque==0x7d&&PID_out>0){ CAN_Eng_Torque=0x7d; }////????,on 06.9.12      
      //if(CAN_Eng_Torque==0x7d&&PID_out<0){ CAN_Eng_Torque=0xe0; }////????,on 06.9.12      
      
      ///////////////////////////////////////////////Modify on 08.2.19
      if(CAN_Eng_Torque>124&&CAN_Eng_Torque<225)      
      { Eng_Request_Torque=CAN_Eng_Torque+PID_out;   //// Eng_Torque: -125%~125%   
      }
      else{ Eng_Request_Torque=CAN_Eng_Torque; }
      //////////////////////////////////////////////////////////Add on 08.2.19  
      if((SRref-SR0)<5){ if(Eng_Request_Torque<CAN_Eng_Torque){ Eng_Request_Torque=CAN_Eng_Torque; } }
      if((SR0-SRref)<5){ if(Eng_Request_Torque>CAN_Eng_Torque){ Eng_Request_Torque=CAN_Eng_Torque; } }
      
      //if(Torque_Increase_Flag>12)////10->12,08.2.26
      if(Torque_Increase_Flag>10)////08.2.19
      {	//if((SRref<SR0)&&((SRref>(SR0-3)&&SRmino>9)||(SRref>(SR0-5)&&SRmino<10)))//SRmino(Double SR),Add on 08.2.26
      	if((SRref<SR0)&&(SRref>(SR0-5)))////08.2.19
      	{ Eng_Req_Torque_min=CAN_Eng_Torque; }////08.2.19
      }
      if(Eng_Request_Torque<Eng_Req_Torque_min){ Eng_Request_Torque=Eng_Req_Torque_min; }
      
      //if(PID_out<0&&((SRmino>4&&SRref>(SR0+4))||(SRmino<5&&SRref>SR0)))////5->4,SRmino,Add on 08.2.26
      if(PID_out<0&&(SRref>(SR0+5)))////08.2.19
      { Torque_max_Flag++;
      	if(Torque_max_Flag>20)
      	{ //if(CAN_Eng_Torque>124&&CAN_Eng_Torque<225)//Modify on 08.3.6 
      		if(CAN_Eng_Torque>129&&CAN_Eng_Torque<225)     
          { Eng_Request_Torque=CAN_Eng_Torque+PID_out;   //// Eng_Torque: -125%~125%   
          }
          else{ Eng_Request_Torque=CAN_Eng_Torque; }
        }
      }
      else{ Torque_max_Flag=0; }
      
      /*
      if(Vs_ASR>4800) 
      { if(Eng_Request_Torque<141){ Eng_Request_Torque=141; } } 
      else if(Vs_ASR>3600) 
      { if(Eng_Request_Torque<140){ Eng_Request_Torque=140; } }   
      else if(Vs_ASR>2500) 
      { if(Eng_Request_Torque<138){ Eng_Request_Torque=138; } }    
      else if(Vs_ASR>1400) 
      { if(Eng_Request_Torque<134){ Eng_Request_Torque=134; } }   
      else
      { if(Eng_Request_Torque<130){ Eng_Request_Torque=130; } }
      */
      
      //if(Eng_Request_Torque<130){ Eng_Request_Torque=130; }////08.2.27;Move on 08.3.1	
         
      ////////////////////////////////////////////////////////if()else,Add on 08.7.4
      /*
      if((T_Single_SR!=0x55)&&(Vs_ASR<1801))////2001->1801,08.7.8-3
      { if(Eng_Request_Torque<145){ Eng_Request_Torque=145; }////140:little,160:big,150:ok(1500,1200),08.7.8-3
      }
      else
      */
      { if(Eng_Request_Torque<125){ Eng_Request_Torque=0x7d; }//08.3.1
      }
      if(Eng_Request_Torque>224){ Eng_Request_Torque=0xe0; }
      
      //////////////////////////////////////////////////////////////// Add 06.12.15
      if(CAN_Control_Eng_Address!=0x0b) ////// No ASR engine Control
      { if(ASR_engine_Control<20)  ////////// <3 --> <20,06.12.22
      	{ ASR_engine_Control++; }
        else
        { TSC1_Control=0xc3;  //////0xc3:both Speed&Torque,prioity==0
        }
      }  
      else
      { ASR_engine_Control=0; }    
      
      if(PID_out>=0)////>0 -> >=0,08.2.19     
      {if(Torque_Increase_Flag<250) /////////////////// <200 --> <250,08.2.19
       { Torque_Increase_Flag++; }
      }
      else{ Torque_Increase_Flag=0; }
                    
      ////////////////////////////////////////////Add on 08.2.19
      if(PID_out<0&&SRref<10){ if(Torque_Del_Flag<240){ Torque_Del_Flag++; } }
      else{ Torque_Del_Flag=0; }
      
      ////Exit ASR-Engine-Ctrl,<12 --> <10,06.12.30;Torque_Increase_Flag,Torque_Del_Flag>120,Add on 08.2.19      
      if((SRref<10&&Torque_Increase_Flag>80)||Torque_Del_Flag>120)
      { ASR_Entry_Flag[3]++; }
      else{ ASR_Entry_Flag[3]=0; } 
      
      //////////////////////////////////////////Modify on 08.3.1-3;ASR_Entry_Flag[3],08.3.6 
      if(Torque_Increase_Flag>200)//120->200,06.12.15;->120,07.1.26;->240,08.2.19;->120,08.3.3;->200,08.3.6
      { ASR_Entry_Flag[3]=200; } 
      /////////////////////////////////////////////////////Diagnose_Data[],Modify on 08.3.1-3;08.3.6
      if(Diagnose_Data[1]!=0||Diagnose_Data[2]!=0||Diagnose_Data[3]!=0||Diagnose_Data[4]!=0)
      { if(ctrlflag==3){ ASR_Entry_Flag[3]=150; }
        else if(Error_ASRExit_Flag==0x55){ ASR_Entry_Flag[3]=180; }
        else{ ASR_Entry_Flag[3]=0; }	
      } 
      ////// 200*5ms==1000ms,80-->200,06.12.15,-->120,07.1.26	 
      if(ASR_Entry_Flag[3]>120)
      { Torque_Increase_Flag=0;
       	ASR_Entry_Flag[3]=0;
        ASR_Entry_Flag[1]=0;
        ASR_Entry_Flag[5]=0;  /////// 06.12.22
        T_Single_SR=0; ////Add on 08.2.19
        if(ctrlflag==3)
        { 
        	ctrlflag=0;
			ASRLamp_SetLow; 
          	Lamp_Condition |= 0x40;   //// ASR Lamp die, bit6==1,Add on 07.4.10
        }
       	
       	//Status_EBC1&=0xfe;  //// ASR engine control end,Add on 07.1.30
       	
       	/*
       	if(ctrlflag==2)
        { if(ASR_Entry_Flag[0]==0&&ASR_Entry_Flag[2]==0&&ASR_Entry_Flag[4]==0)
          { ctrlflag=0;
            ASRLamp=0;   ////////////////////////////////////  ASR_Lamp=0
          }
        }
        */
                
        TSC1_Control=0xfc;  //////0xfc:No ASR Engine Ctrl
        Eng_Request_Speed=0xfaff; 
        Eng_Request_Torque=0xfa;    ////////0xfa-->0xe1,06.12.8;Modify on 07.10.16
        //Eng_Request_Torque=0xe1;  ////////0xfa-->0xe1,06.12.8
      }                
  }
 }    
}


void ASR_Control(void)
{
 unsigned char jni=0,i=0,ji=0,Vwo_Flag=0;  //////// ji,Add on 06.12.15,06.12.31
 
 //////////////////////////////////////////////////////////////////////////ASR-Engine Ctrl, Add on 06.7.24
 //CANJ1939_Identify(); /////////// Add on 06.10.16,Move on 06.12.30
 
 //ASR_Engine_PID();  ////////(20,4,2,0), Modify on 06.9.12,Move on 07.12.20
 
 ///*
 if(ctrlflag==1||Brake_Switch>10) ////Brake Lamp Switch ,Add at 06.5.25
 { if(G_RSC_flag<4)////RSC,09.4.17
 	 { ASR_DIFCtrl_SetLow;   ////////////////////////////////////  ASR_DIF=0
     //ASR_DIF=0;////////////////////////////////////
   }
   ////////Add on 07.4.10;RSC:&&G_RSC_flag<4,09.4.20
   if((ASR_Close_Flag!=0x55&&ASR_Switch_Flag==0&&G_RSC_flag<4)&&(Diagnose_Data[18]==0&&Diagnose_Data[24]==0))
   { 
   		ASRLamp_SetLow; //// ASR Lamp Off
     Lamp_Condition |= 0x40;   //// ASR Lamp die, bit6==1
   } 
   ASR_Entry_Flag[0]=0;ASR_Entry_Flag[2]=0;ASR_Entry_Flag[4]=0;
 } 
 //*/

 if(ctrlflag==2&&Vs<1000)
 	{
 	 if((SR[2]<-85)||(SR[3]<-85))
 	 	{if(ASR_DIFCtrl==0) {ASR_DIFCtrl_SetHigh;}}
 	}


 if(ctrlflag!=1)
 {
  //if(G_HaveorNo_Flag!=0x5a)//////// VsG, Add at 06.7.24,will be test,Move on 06.12.30 
  {
   /////////////////////////////////////////////// Add on 06.12.31
   Vwo_Flag=0;
   if(Vw[0]==Vwo){ Vwo_Flag++; }
   if(Vw[1]==Vwo){ Vwo_Flag++; }
   if(Vw[2]==Vwo){ Vwo_Flag++; }
   if(Vw[3]==Vwo){ Vwo_Flag++; }
 
   if(Vstq>Vwo)
   { if(Vw[0]>Vwo&&Vw[1]>Vwo)      ////Add on 07.2.4
     { if(Vs>Vstq)
       { //if(Aw[0]>-700||Aw[1]>-700)///////// Modify on 07.1.28
       	 //if((Aw[0]>-700&&Nocontrols[0]<4)&&(Aw[1]>-700&&Nocontrols[1]<4))//Nocontrols,Add on 08.2.29;||->&&,08.3.6
         if((Aw[0]>-700)&&(Aw[1]>-700))////Modify on 08.7.3
         { Vs_ASR=(long int)((Vstq+Vs)>>1);////Modify on 08.4.22 
         }
         else
         { Vs_ASR=Vs; } 
       }
       else
       { Vs_ASR=(long int)((Vstq+Vs)>>1); }
     }
     else           ////Add on 07.2.4
     { Vs_ASR=Vs; }
   }
   else
   { if(Vwo_Flag>2)
     { Vs_ASR=Vstq; }
     else
     { Vs_ASR=Vs; }
   }
   
   if(Vs_ASR<1360){ Vs_ASR=1360; } ////////Add on 07.2.4-2,1400-->1360,Modify on 07.2.6-2
   
  }  
  //else{ if(Diagnose_Data[22]==0){ Vs_ASR=VsG; }else{ Vs_ASR=Vs; } } ///////// will be test,Move on 06.12.30 
  
  /////////////////////////////////////////////////////////////////////// Single Wheel Enter ASR,Add on 07.3.11 
  if(SRref>20)////14->13,08.2.19;13->14,08.5.26;13,08.6.5
  { for(i=0;i<SS;i++)
    { if(Aw[i]>500){ if(Aw_single_ASR[i]<200){ Aw_single_ASR[i]++; } }
      if(Aw[i]<-400){ Aw_single_ASR[i]=0; }//-300->-500,08.3.6;-500->-300,08.5.26;-400,08.6.5
      if(SRref>79)
      { if(Aw_single_ASR[i]<4){ Aw_single_ASR[i]=0; } }//5->4,08.3.6
      if(Aw_single_ASR[i]>3){ Thoq=0x5a; }////4->3,08.2.19
      if(Aw_single_ASR[i]>2){ Tho=0x5a; } 
    } 
  }
  else
  { Aw_single_ASR[0]=0;Aw_single_ASR[1]=0;Aw_single_ASR[2]=0;Aw_single_ASR[3]=0;Aw_single_ASR[4]=0;Aw_single_ASR[5]=0; 
    Thoq=0;Tho=0;
  }
  ////////////////////////////////////////////////////////////////////// Single Wheel Enter ASR          
       
  /////////////////////////////////////////Modify on 07.2.6;08.2.29;Move "&&Nocontrols[i]<4",08.7.3       
  if((Diagnose_Data[1]==0)&&(Diagnose_Data[2]==0))
  { if((Nocontrols[0]<4)||(Nocontrols[1]<4))
  	 {Vstq=(long int)((Vw[0]+Vw[1])>>1); }
     else if(Nocontrols[0]<4)
      Vstq=(long int)(Vw[0]); 
       else if(Nocontrols[1]<4)
        Vstq=(long int)(Vw[1]); 
  }  ////////// Vs=(Vw[0]+Vw[1])/2;
  else if(Diagnose_Data[1]==0)
  { Vstq=(long int)(Vw[0]); }
  else if(Diagnose_Data[2]==0)
  { Vstq=(long int)(Vw[1]); }
  else{ Vstq=Vwo; }
     
  if((Diagnose_Data[3]==0)&&(Diagnose_Data[4]==0))
  { Vsth=(long int)((Vw[2]+Vw[3])>>1); }  ////////// Vs=(Vw[2]+Vw[3])/2;
  else if(Diagnose_Data[3]==0)
  { Vsth=(long int)(Vw[2]); }
  else if(Diagnose_Data[4]==0)
  { Vsth=(long int)(Vw[3]); }
  else{ Vsth=Vwo; }   
  
  //////////////////////6S/6M   
  /*
  if((Diagnose_Data[5]==0)&&(Diagnose_Data[6]==0))
  { Vsth2=(long int)((Vw[4]+Vw[5])>>1); }  ////////// Vs=(Vw[4]+Vw[5])/2;
  else if(Diagnose_Data[5]==0)
  { Vsth2=(long int)(Vw[4]); }
  else if(Diagnose_Data[6]==0)
  { Vsth2=(long int)(Vw[5]); }
  else{ Vsth2=Vwo; }
  */
  
  ///////////////////////////////////////SRmin,Add on 08.2.19;Vw[]<2000,Vst*<1500,Add on 08.11.14
  //if(ASR_Entry_Flag[0]<6)     //// Modify on 07.1.25 
  {  if(Vw[0]>Vw[1])
     { //if(Tgr[0]==0)
     	 {SRmax[0]=(signed int)(100-(100*Vw[1])/Vw[0]);SRmax[1]=0;
     	 	SRmin[1]=(signed int)(100-(100*Vs_ASR)/Vw[1]);SRmin[0]=0;
     	 }
     	 //else {SRmax[0]=0;SRmax[1]=0;} 
     	 
     	 if(Vw[0]<2000&&ctrlflag==0){SRmax[0]=0;SRmax[1]=0;SRmin[0]=0;SRmin[1]=0;} 
     }
     else 
     { //if(Tgr[1]==0)
   	   {SRmax[1]=(signed int)(100-(100*Vw[0])/Vw[1]);SRmax[0]=0;
   	   	SRmin[0]=(signed int)(100-(100*Vs_ASR)/Vw[0]);SRmin[1]=0;
   	   }
   	   //else {SRmax[1]=0;SRmax[0]=0;}
   	   
   	   if(Vw[1]<2000&&ctrlflag==0){SRmax[0]=0;SRmax[1]=0;SRmin[0]=0;SRmin[1]=0;}
     }
     //if(Tgr[1]==0)
     {SRmaxo[0]=(signed int)(100-(100*Vs_ASR)/Vstq);
     	if(SRmaxo[0]<0){ SRmaxo[0]=0; } if(SRmaxo[0]>100){ SRmaxo[0]=100; }////Add on 08.2.19
     } ////////Modify on 08.2.19 
     //else {SRmaxo[0]=0;} 
     
     if(Vstq<1500&&ctrlflag==0){SRmaxo[0]=0;} 
  }
  //if(ASR_Entry_Flag[2]<6)     //// Modify on 07.1.25 
  {  if(Vw[2]>Vw[3])
     { //if(Tgr[2]==0)
     	 {SRmax[2]=(signed int)(100-(100*Vw[3])/Vw[2]);SRmax[3]=0;
     	 	SRmin[3]=(signed int)(100-(100*Vs_ASR)/Vw[3]);SRmin[2]=0;
     	 }
     	 //else {SRmax[2]=0;SRmax[3]=0;}  
     	 
     	 if(Vw[2]<2000&&ctrlflag==0){SRmax[2]=0;SRmax[3]=0;SRmin[2]=0;SRmin[3]=0;}
     }
     else 
     { //if(Tgr[3]==0)
   	   {SRmax[3]=(signed int)(100-(100*Vw[2])/Vw[3]);SRmax[2]=0;
   	   	SRmin[2]=(signed int)(100-(100*Vs_ASR)/Vw[2]);SRmin[3]=0;
   	   }
   	   //else {SRmax[3]=0;SRmax[2]=0;}
   	   
   	   if(Vw[3]<2000&&ctrlflag==0){SRmax[2]=0;SRmax[3]=0;SRmin[2]=0;SRmin[3]=0;}
     }
     //if(Tgr[3]==0)
     {SRmaxo[2]=(signed int)(100-(100*Vs_ASR)/Vsth);
      if(SRmaxo[2]<0){ SRmaxo[2]=0; } if(SRmaxo[2]>100){ SRmaxo[2]=100; }////Add on 08.2.19
     } ////////Modify on 08.2.19
     //else {SRmaxo[2]=0;}
     
     if(Vsth<1500&&ctrlflag==0){SRmaxo[2]=0;}  
  }
  /*                  //////////////////////6S/6M,Move on 06.12.30   
  //if(ASR_Entry_Flag[4]<6)     //// Modify on 07.1.25 
  {  if(Vw[4]>Vw[5])
     { //if(Tgr[4]==0)
     	 {SRmax[4]=(signed int)(100-(100*Vw[5])/Vw[4]);SRmax[5]=0;
     	  SRmin[5]=(signed int)(100-(100*Vs_ASR)/Vw[5]);SRmin[4]=0;
     	 }
     	 //else {SRmax[4]=0;SRmax[5]=0;} 
     	 
     	 if(Vw[4]<2000&&ctrlflag==0){SRmax[4]=0;SRmax[5]=0;SRmin[4]=0;SRmin[5]=0;} 
     }
     else 
     { //if(Tgr[5]==0)
   	   {SRmax[5]=(signed int)(100-(100*Vw[4])/Vw[5]);SRmax[4]=0;
   	    SRmin[4]=(signed int)(100-(100*Vs_ASR)/Vw[4]);SRmin[5]=0;
   	   }
   	   //else {SRmax[5]=0;SRmax[4]=0;}
   	   
   	   if(Vw[5]<2000&&ctrlflag==0){SRmax[4]=0;SRmax[5]=0;SRmin[4]=0;SRmin[5]=0;} 
     }     
     //if(Tgr[5]==0)
     {SRmaxo[4]=(signed int)(100-(100*Vs_ASR)/Vsth2);
      if(SRmaxo[4]<0){ SRmaxo[4]=0; } if(SRmaxo[4]>100){ SRmaxo[4]=100; }////Add on 08.2.19
     } ////////Modify on 08.2.19
     //else {SRmaxo[4]=0;} 
     
     if(Vsth2<1500&&ctrlflag==0){SRmaxo[4]=0;}
  }
  */
   ////////////////////////////////////////Modify on 08.2.19
   if(Drive_Num==0)/////////////////4X2,Add on 08.4.11
   {SRmino=SRmin[2];  
    if(SRmin[3]>SRmino){ SRmino=SRmin[3]; }
    //if(SRmin[4]>SRmino){ SRmino=SRmin[4]; } //// 6S/6M
    //if(SRmin[5]>SRmino){ SRmino=SRmin[5]; }
    
    SRref=SRmaxo[2];  
    //if(SRmaxo[4]>SRref){ SRref=SRmaxo[4]; } //// 6S/6M
   }
   if(Drive_Num==1)/////////////////Drive_Num==1:4X4,Add on 08.4.11
   {SRmino=SRmin[0];  
    if(SRmin[1]>SRmino){ SRmino=SRmin[1]; }
    if(SRmin[2]>SRmino){ SRmino=SRmin[2]; }
    if(SRmin[3]>SRmino){ SRmino=SRmin[3]; }
    //if(SRmin[4]>SRmino){ SRmino=SRmin[4]; } //// 6S/6M
    //if(SRmin[5]>SRmino){ SRmino=SRmin[5]; }
    
    SRref=SRmaxo[0];  
    if(SRmaxo[2]>SRref){ SRref=SRmaxo[2]; }
    //if(SRmaxo[4]>SRref){ SRref=SRmaxo[4]; } //// 6S/6M
   }
 }  
 
 if(ASR_HaveorNo_Flag==0x5a&&ctrlflag!=1) ////ASR-DIF Ctrl Flag,Add on 06.7.24
 {
  ////////////////////////////////////////////////////////////////////////////No_sigl_ASR,Add on 06.12.22
  /*/////////////////////Move on 08.7.8-2
  No_sigl_ASR=0;
  if(Vs_ASR<1400)///////////////////////////if,Add on 07.2.6-2;1600->1400,4-->9;08.7.4
  { if((nosiglflag[0]==0&&nosiglflag[1]>9)||(nosiglflag[1]==0&&nosiglflag[0]>9)){ No_sigl_ASR=2; } 
    if((nosiglflag[2]==0&&nosiglflag[3]>9)||(nosiglflag[3]==0&&nosiglflag[2]>9)){ No_sigl_ASR++; } 
    //if((nosiglflag[4]==0&&nosiglflag[5]>9)||(nosiglflag[5]==0&&nosiglflag[4]>9)){ No_sigl_ASR++; }//4S/4M,Move 0n 06.12.30 
    if(No_sigl_ASR>0&&No_sigl_ASR<3&&SRref>53){ No_sigl_ASR=0; }////Add on 07.2.6-2;73->53,08.6.25 
  }
  */
  ///////////////////////////////////////////////////////////// 4S/4M
  //Drive_Num=0;////Move on 08.4.11

  if(Drive_Num==0){ ASR_For1=2;ASR_For2=3; }  ///////// 4X2, 2
  else if(Drive_Num==1){ ASR_For1=0;ASR_For2=3; }  ///////// 4X4, 0,2  
  
  ////////////////////////////////// Add on 07.1.26
  No_sigl_ASR_2=0;
  for(ji=0;ji<SS;ji++)
  { ///////////////////////////////Modify on 08.6.5
  	if(Diagnose_Data[ji+1]!=0)/////Sensor,Add on 08.3.1-3
    { No_sigl_ASR_2++; }
  }
  ///////////////////////////////Modify on 08.6.5
  if(Drive_Num==0)////4X2
  { for(ji=0;ji<2;ji++)
  	{ if(Diagnose_Data[ji+9]!=0)
      { No_sigl_ASR_2++; }
    }
  }
  else ////if(Drive_Num==1)////4X4
  { for(ji=0;ji<SS;ji++)
  	{ if(Diagnose_Data[ji+7]!=0)
      { No_sigl_ASR_2++; }
    }
  } 
  ////////////////////////////////==WABCO????,Add on 08.2.29;Move on 08.7.3
  //if(No_sigl_ASR_2==0)////4->9,08.6.25
  //{ if((Nocontrols[0]>9&&Nocontrols[1]>9)||(Nocontrols[2]>9&&Nocontrols[3]>9)){ No_sigl_ASR_2=2; } }
     
  //for(i=0;i<5;i=i+2)                      ////////// ASR:0,2,4,(6X6); // 2,4,(6X4); // 2,(6X2)
  for(i=ASR_For1;i<ASR_For2;i=i+2)   
  {//if(Vs<10000&&ctrlflag==0&&unctrl==0)//Brake_Switch,Add at 06.5.25;End_,06.12.15;Diagnose_Data[18],8401,08.2.19
   //unctrl:==0->!=1,08.7.18
   if(Vs_ASR<8401&&ASR_Entry_Flag[i]<6&&unctrl!=1&&ctrlflag!=1&&
   	  End_Break==0&&End_Break1==0&&Brake_Switch==0&&Diagnose_Data[18]==0)
   {  
      //if(((SRmax[0]>12||SRmax[1]>12)&&SRmaxo[0]>15)||((SRmax[2]>12||SRmax[3]>12)&&SRmaxo[2]>15)||
      //  ((SRmax[4]>12||SRmax[5]>12)&&SRmaxo[4]>15)) 
      ////////////////////Aw[]>200,Tgr,Dia,No_sigl_ASR,Add on 06.12.15;Move Diagnose_Data[],08.3.1-3
      ///////////>34 --> >30,>28 --> >24,07.2.5,Move Tgr[],07.2.6;Tho,07.3.11;
      ////30->18,70->40,24->15,||SRmino>38,08.2.19;Move:||SRmino>38,08.4.22;18->22,40->44,15->19,08.5.26
      ////18,40,15,Add:||SRmino>38,08.6.5;Move:||SRmino>38,08.7.4
      if(((((SRmax[i]>28&&Aw[i]>300)||(SRmax[i]>45&&Aw[i]!=0&&Tho==0x5a))||
           ((SRmax[i+1]>28&&Aw[i+1]>300)||(SRmax[i+1]>45&&Aw[i+1]!=0&&Tho==0x5a)))&&SRmaxo[i]>28)&&
           (No_sigl_ASR>2||No_sigl_ASR==0)&&(No_sigl_ASR_2==0))
      {ASR2_Entry_Flag2++;
   	   if(ASR2_Entry_Flag2>8) {ASR_Entry_Flag[i]++;ASR2_Entry_Flag2=0;}
       if(ASR_Entry_Flag[i]>5)  ////5->4,08.2.19;4->5,08.3.6
       {ASR_Entry_Flag[i]=6;
        No_sigl_ASR=0;      /////// Add on 06.12.15 
        No_sigl_ASR_2=0;   //// Add on 07.1.26
        Tho=0;         ////07.3.11
        ////////////////////////////////////////////////////////////////////////////////////// Entry
        //if(Kvs[i]!=0xaa) 
        {//Kvs[i]=0xaa;
         
         { if(SRmax[i]>SRmax[i+1])
           { ASR_Ch[i]=i;ASR_ANCh[i]=i+1;ASR_Entry_Flag[i]=0x55; }
           if(SRmax[i+1]>SRmax[i])
           { ASR_Ch[i]=i+1;ASR_ANCh[i]=i;ASR_Entry_Flag[i]=0xaa; }
         }
         
         ctrlflag=2;  
        
         Check_Flag_Clear();   ////////Add on 07.2.6
        
         //Status_EBC1|=0x04;  //// ASR brake control active,Add on 07.1.30
        
         step[ASR_Ch[i]]=0;step[ASR_ANCh[i]]=0;Zp[ASR_Ch[i]]=18;Zp0[ASR_Ch[i]]=2;                    
         T1[ASR_Ch[i]]=0;T2[ASR_Ch[i]]=0;T3[ASR_Ch[i]]=0;T30[ASR_Ch[i]]=0;TVw[ASR_Ch[i]]=0;Tst[ASR_Ch[i]]=0;
         ZNum[ASR_Ch[i]]=0;Kvs[ASR_Ch[i]]=0;////08.2.19
         case0_flag[ASR_Ch[i]]=0;////Add on 08.6.30
         Jb[ASR_Ch[i]]=0;Tzz[ASR_Ch[i]]=0;
         T_Double_SR=0;  ////// 07.1.12
         if(T_Single_SR!=0x55){ T_Single_SR=0; }  //////Add on 08.2.19
                  	
         ZY3[0]=0;ZY3[1]=0;ZY3[2]=0;ZY3[3]=0;ZY3[4]=0;ZY3[5]=0;  ////// Modify on 06.12.30
         
         ASR_DIFCtrl_SetHigh;   ////////////////////////////////////  ASR_DIF=1
         //ASR_DIFCtrl=1;
         //ASR_DIF=1;////////////////////////////////////
         ASRLamp_SetHigh;   ////////////////////////////////////  ASR_L=1
         Lamp_Condition &= 0xbf; //// ASR Lamp Light, bit6==0,Add on 07.4.10
         
         F_ctrl_SetHigh;    // able Vbb (F_ctrl==1),Add on 07.3.15
         Fearth_SetHigh;    // able EARTH (Fearth==1),Add on 06.12.30
         
        } 
        if(SRmax[ASR_Ch[i]]<40&&SRmaxo[ASR_Ch[i]]<27) /////&&SRmaxo,Add on 08.2.19
        { st[ASR_Ch[i]]=2;
        }
        else                    /////08.2.19
        { st[ASR_Ch[i]]=1;    
        } 
       }
      } 
      else
      { ASR_Entry_Flag[i]=0; } 
   }
     
   ////////////////////////////////////////////////////////////////////////// ASR_Control
   if(ctrlflag==2)
   { 
    if(ASR_Entry_Flag[i]>5)
    {       
       //if(Tgr[ASR_Ch[i]]==0)
       {
       	/*       //// Modify on 07.1.25 
        if(Vw[ASR_Ch[i]]>Vw[ASR_ANCh[i]])
        {SRmax[ASR_Ch[i]]=(signed int)(100 - (100*Vw[ASR_ANCh[i]])/Vw[ASR_Ch[i]]);SRmax[ASR_ANCh[i]]=0;}
        else 
        {SRmax[ASR_ANCh[i]]=(signed int)(100 - (100*Vw[ASR_Ch[i]])/Vw[ASR_ANCh[i]]);SRmax[ASR_Ch[i]]=0;}
        */
        
        //if(SRmax[ASR_Ch[i]]<3&&SRmax[ASR_ANCh[i]]>19&&Vw[ASR_Ch[i]]>=Vs_ASR&&Vw[ASR_ANCh[i]]>=Vs_ASR)
        if(SRmax[ASR_Ch[i]]<3&&SRmax[ASR_ANCh[i]]>10)////19->10,08.2.19
        {Kvs[ASR_Ch[i]]++;
         if(Kvs[ASR_Ch[i]]>10)////5-->10,07.1.25
         {if(SRmax[ASR_Ch[i]]<1) 
       	  {
       	   jni=ASR_Ch[i];
       	   ASR_Ch[i]=ASR_ANCh[i];
       	   ASR_ANCh[i]=jni;
       	   jni=0;
           st[ASR_Ch[i]]=1;  ////1-->2,06.1.8,2-->1,08.2.19
           st[ASR_ANCh[i]]=0;//////////// Add on 08.2.19
           ZNum[ASR_Ch[i]]=0x55; //// 07.1.25
           
           //////////////////////////Modify on 08.7.8-2
           if(T_Single_SR!=0x55)
           { T_Single_SR++; 
             if(T_Single_SR>2){ T_Single_SR=0x55; }
           }
           //T_Single_SR=0x55;  //////Add on 08.2.19
           
           ///////////////////////////////////////////////////////////////////////////// Modify on 06.12.31
           step[ASR_Ch[i]]=0;step[ASR_ANCh[i]]=0;Zp[ASR_Ch[i]]=18;Zp0[ASR_Ch[i]]=2;                    
           T1[ASR_Ch[i]]=0;T2[ASR_Ch[i]]=0;T3[ASR_Ch[i]]=0;T30[ASR_Ch[i]]=0;TVw[ASR_Ch[i]]=0;Tst[ASR_Ch[i]]=0;
           Jb[ASR_Ch[i]]=0;Tzz[ASR_Ch[i]]=0;ZY3[ASR_Ch[i]]=0;ZY3[ASR_ANCh[i]]=0; 
           T_Double_SR=0;  ////// 07.1.12
           
          } 
          Kvs[ASR_Ch[i]]=0; ////07.1.25
         }  
        }
        else
        { ZNum[ASR_Ch[i]]=0;Kvs[ASR_Ch[i]]=0; }
       }
       
       /*
       if(i==0)
       {SRmaxo[0]=(signed int)(100 - (100*Vs_ASR)/Vstq);} //////// Vs --> Vs_ASR, Modify on 06.7.24
       if(i==2)
       {SRmaxo[2]=(signed int)(100 - (100*Vs_ASR)/Vsth);} //////// Vs --> Vs_ASR, Modify on 06.7.24
       ////////// 4S/4M,Move on 06.12.31
       //if(i==4)
       //{SRmaxo[4]=(signed int)(100 - (100*Vs_ASR)/Vsth2);} //////// Vs --> Vs_ASR, Modify on 06.7.24  
       *////////// Move on 07.1.25
          	
       ///////////////////////////////////////////////////////////////////////////////////	ASR_ANCh[i] 	   
       if(ZY3[ASR_ANCh[i]]<60) ////////600ms;120->60(300ms),08.2.19
       { ZY3[ASR_ANCh[i]]++;
       	 DI[ASR_ANCh[i]]=3; Val_Act(ASR_ANCh[i]); 
       }
       else
       { DI[ASR_ANCh[i]]=1; Val_Act(ASR_ANCh[i]);
       }	   
              
     //for(i=2;i<4;i++)
     {
      switch(st[ASR_Ch[i]])
      {              	
       case 1:
        //////////////////////////////////////T1,50,&&SRmin,Modify on 08.2.19
  	    if(((Aw[ASR_Ch[i]]<-280&&SRmax[ASR_Ch[i]]<29)||(SRmax[ASR_Ch[i]]<34&&Aw[ASR_Ch[i]]<-560)||
  	       ((Aw[ASR_Ch[i]]<-700&&(T1[ASR_Ch[i]]==0x55||SRmax[ASR_Ch[i]]<50))&&Vs_ASR<1801)||
  	       (SRmax[ASR_Ch[i]]<26)||(Aw[ASR_Ch[i]]<-1100))&&(SRmin[ASR_ANCh[i]]<25))
  	    { st[ASR_Ch[i]]=2;
          step[ASR_Ch[i]]=0;
          T1[ASR_Ch[i]]=0x55;Zp[ASR_Ch[i]]=18;Zp0[ASR_Ch[i]]=2;////T1=0->0x55,08.2.19         
  	      DI[ASR_Ch[i]]=1;  //////Add on 08.2.19
  	    }
  	    else
  	    {/*////Move on 08.7.3
  	     ///////////////////////////case0_flag,Add on 08.6.30
  	     if(case0_flag[ASR_Ch[i]]<255)
  	     { case0_flag[ASR_Ch[i]]++; } 
  	     if(case0_flag[ASR_Ch[i]]>160&&Vs_ASR<1801)////no JY,800ms
  	     {if(case0_flag[ASR_Ch[i]]<250)////ZY,1250ms
  	      { Zp[ASR_Ch[i]]=18;Zp0[ASR_Ch[i]]=3; }
  	      else
  	     	{ Zp[ASR_Ch[i]]=17;Zp0[ASR_Ch[i]]=4; }
  	     } 
  	     else//////////////Add on 08.6.30 
  	     */
  	     {if(T1[ASR_Ch[i]]==0x55) ////Add on 08.2.19  	 
  	      {if(Vs_ASR>1800)    ///////////////////// Add on 07.2.4-2
  	       {if(SRmax[ASR_Ch[i]]>65)  /////////////55->65,08.2.19 
  	        { Zp[ASR_Ch[i]]=16;Zp0[ASR_Ch[i]]=3; } //12->16,3->2,08.2.19
  	        else if(SRmax[ASR_Ch[i]]>50)/////40->50,08.2.19 
  	        { Zp[ASR_Ch[i]]=18;Zp0[ASR_Ch[i]]=3; } //15->18,08.2.19
  	        else
  	        { Zp[ASR_Ch[i]]=20;Zp0[ASR_Ch[i]]=2; } //18->20,08.2.19
  	       }
  	       else              ///////////////////// Add on 07.2.4-2
  	       { Zp[ASR_Ch[i]]=24;Zp0[ASR_Ch[i]]=3; }////18->24,08.2.19;24->14,08.6.25;08.7.3
  	      }
  	      else
  	      { Zp[ASR_Ch[i]]=14;Zp0[ASR_Ch[i]]=2; }////08.2.19;14->12,2->4,08.6.25;08.7.3
  	     }
  	     ////////////////////////////////////////////////////////////////Modify on 08.2.19
  	     step[ASR_Ch[i]]++;
  	     if(step[ASR_Ch[i]]<=Zp0[ASR_Ch[i]]) ////// 10ms
  	     { if(Aw[ASR_Ch[i]]>-600)  ////-100 --> -600,06.12.30
  	       { DI[ASR_Ch[i]]=0; 
  	       } 
  	       else
  	       { if((SRmax[ASR_Ch[i]]<50&&Vs_ASR<1801)||(SRmax[ASR_Ch[i]]<40&&Vs_ASR>1800))
  	         { DI[ASR_Ch[i]]=1; }
  	         else
  	         { DI[ASR_Ch[i]]=0; }
  	       }                   ///////// Add on 06.12.15     	     	 	     
  	     }	
  	     else                    //////// 70ms  
  	     { DI[ASR_Ch[i]]=1;
  	       if(step[ASR_Ch[i]]>=(Zp[ASR_Ch[i]]+Zp0[ASR_Ch[i]])){ step[ASR_Ch[i]]=0; }
  	     }  	 
        }
        /*
        if(SRmax[ASR_ANCh[i]]>20&&SRmax[ASR_ANCh[i]]<50&&SRmax[ASR_Ch[i]]<35)   //////////// 06.1.8
  	    { st[ASR_Ch[i]]=2;
          step[ASR_Ch[i]]=0;
          T1[ASR_Ch[i]]=0;Zp[ASR_Ch[i]]=18;Zp0[ASR_Ch[i]]=2;         
   	    }
  	    */
  	   break;
  	                       
       case 2:
        ////////////////////////////////////////||SRmin,Modify on 08.2.19         
  	    if((SRmax[ASR_Ch[i]]>28&&Aw[ASR_Ch[i]]>850)||(Aw[ASR_Ch[i]]>1300)||(SRmax[ASR_Ch[i]]>38)||
  	    	 (SRmin[ASR_ANCh[i]]>26)||(SRmin[ASR_ANCh[i]]<5&&SRmax[ASR_Ch[i]]>18)||(SRmax[ASR_Ch[i]]>30&&Vs_ASR>1800))
  	    { st[ASR_Ch[i]]=1;
          step[ASR_Ch[i]]=0;
          DI[ASR_Ch[i]]=1;  //////Add on 08.2.19
  	    }
  	    ///////////////////////////////////////////////////////////Modify on 08.2.19
  	    if(((((SRmax[ASR_Ch[i]]<25&&Aw[ASR_Ch[i]]<-360)||(SRmax[ASR_Ch[i]]<27&&Aw[ASR_Ch[i]]<-600)||
  	    	    (SRmax[ASR_Ch[i]]<21)||(Aw[ASR_Ch[i]]<-1400))&&Vs_ASR<1801)||
  	        (((SRmax[ASR_Ch[i]]<20&&Aw[ASR_Ch[i]]<-360)||(SRmax[ASR_Ch[i]]<22&&Aw[ASR_Ch[i]]<-600)||
  	          (SRmax[ASR_Ch[i]]<16)||(Aw[ASR_Ch[i]]<-2000))&&Vs_ASR>1800)||(SRmaxo[i]<14))&&(SRmin[ASR_ANCh[i]]<17)) 
  	    { st[ASR_Ch[i]]=3;
          step[ASR_Ch[i]]=0;
          case0_flag[ASR_Ch[i]]=0;////Add on 08.6.30
  	    }
  	    else
  	    { DI[ASR_Ch[i]]=1; }
  	  	
  	    /*
  	    if(SRmaxo[i]>80&&SRmax[ASR_Ch[i]]<20&&SRmax[ASR_ANCh[i]]>80)
        { st[ASR_Ch[i]]=1;
          step[ASR_Ch[i]]=0;
        }
  	    */                  
  	   break;
  	                       
       case 3:
        //////////////////////////////////////////06.12.30;Modify on 08.2.19               
  	    if((SRmax[ASR_Ch[i]]>35&&Aw[ASR_Ch[i]]>700&&Vs_ASR<1801)||(SRmax[ASR_Ch[i]]>10&&SRmax[ASR_Ch[i]]<20)||
  	    	 (SRmax[ASR_Ch[i]]>23&&Aw[ASR_Ch[i]]>500&&Vs_ASR>1800))
  	    { st[ASR_Ch[i]]=2;
          step[ASR_Ch[i]]=0;
          T3[ASR_Ch[i]]=0;
          T_Double_SR=0;////Add on 08.3.6
  	    }
  	    else
  	    { 
  	      step[ASR_Ch[i]]++;
  	      if(step[ASR_Ch[i]]<11) //// 10*5==50ms
  	      { DI[ASR_Ch[i]]=3; 
  	      }
  	      else                     //////// 50ms  
  	      { DI[ASR_Ch[i]]=1; 
  	        if(step[ASR_Ch[i]]>20){ step[ASR_Ch[i]]=0; }
  	      }
  	      //////////////////////////////////////////////////// Modify on 06.12.31
  	      if(Aw[ASR_Ch[i]]<500&&Aw[ASR_Ch[i]]>-500) /////300->500,08.2.19 
  	      { TVw[ASR_Ch[i]]=0;
  	        if(SRmax[ASR_Ch[i]]>=0)
  	        { if(T3[ASR_Ch[i]]<200)     
  	          { T3[ASR_Ch[i]]++; }
  	        }
  	        else
  	        { T3[ASR_Ch[i]]=0; }
  	        if(SRmax[ASR_Ch[i]]<13&&SRmax[ASR_Ch[i]]>=0) ////18->13,08.2.19  
  	        { if(T3[ASR_Ch[i]]>30)////220ms;44->30(150ms),08.2.19
  	          { st[ASR_Ch[i]]=4;T3[ASR_Ch[i]]=0; } 
  	        }
  	        else 
  	        { if(T3[ASR_Ch[i]]>60)////600ms;120->60(300ms),08.2.19
  	          { st[ASR_Ch[i]]=4;T3[ASR_Ch[i]]=0; } 
  	        }
  	      }
  	      else
  	      { T3[ASR_Ch[i]]=0;
  	        if(SRmaxo[i]<13&&SRmax[ASR_Ch[i]]<12&&SRmax[ASR_Ch[i]]>=0)//&&Aw[ASR_Ch[i]]>-10,19,18->13,12,08.2.19
            { TVw[ASR_Ch[i]]++;
              if(TVw[ASR_Ch[i]]>32)///////320ms;64->32(180ms),08.2.19
              { TVw[ASR_Ch[i]]=0;st[ASR_Ch[i]]=4; }
            }
            else
            { TVw[ASR_Ch[i]]=0; }
  	      }
  	    }
        //////////////////////////////////////////////////////////// Add on 07.1.12
  	    if(ASR_Entry_Flag[1]>5)    ////////////////////////////////// ASR-Eng-Ctrl,Add on 07.1.26
  	    { ////////////////////////////////////no Double SR,50->20,40->18,08.2.19;||SRmax,08.2.29
  	    	if((SRmaxo[i]<20&&SRmax[ASR_Ch[i]]<18)||(SRmax[ASR_Ch[i]]<10))
  	      { if(T_Double_SR<200)  ////1000ms
            { T_Double_SR++; }
          }
          else
          { T_Double_SR=0; }  	  
  	    }
  	    else
  	    { if((SRmaxo[i]>70&&SRmax[ASR_Ch[i]]<60&&Aw[ASR_Ch[i]]<300)||(SRmax[ASR_Ch[i]]<10)||
  	    	   (SRmaxo[i]<27&&SRmax[ASR_Ch[i]]<20&&Aw[ASR_Ch[i]]<900))//Modify on 08.2.19
  	      { if(T_Double_SR<200)  ////1000ms
            { T_Double_SR++; }
          }
          else
          { T_Double_SR=0; }
  	    }
  	
  	    /*
  	    if(SRmaxo[i]>80&&SRmax[ASR_Ch[i]]<19&&SRmax[ASR_ANCh[i]]>80)
        { st[ASR_Ch[i]]=1;
          step[ASR_Ch[i]]=0;
          T3[ASR_Ch[i]]=0; 
        }
  	    */                 
  	   break;
  	
       case 4:
      	
  	    ///////////////////////////////////////////////Exit,18->12,20->15,08.2.19
        if(((SRmax[ASR_Ch[i]]<12||SRref<15)&&SRmax[ASR_Ch[i]]>=0)||(Aw[ASR_Ch[i]]<200&&Aw[ASR_Ch[i]]>-200))//06.12.31
        {  
           if(T30[ASR_Ch[i]]<20) ///////////// 100ms,Modify on 06.12.31
           { T30[ASR_Ch[i]]++;
             if(Aw[ASR_Ch[i]]>0){ DI[ASR_Ch[i]]=1; } 
             if(Aw[ASR_ANCh[i]]>0){ DI[ASR_ANCh[i]]=1; }	
           
             if(Aw[ASR_Ch[i]]<300){ DI[ASR_Ch[i]]=3; } 
             if(Aw[ASR_ANCh[i]]<300){ DI[ASR_ANCh[i]]=3; } 
           
             //Val_Act(ASR_Ch[i]);
             Val_Act(ASR_ANCh[i]);
           }
           else
           {             
             //if(ASR_Entry_Flag[0]==0&&ASR_Entry_Flag[2]==0&&ASR_Entry_Flag[4]==0)
             { ASR_DIFCtrl_SetLow;   ////////////////////////////////////  ASR_DIF=0
               //ASR_DIF=0;////////////////////////////////////
               
               if(Tst[ASR_Ch[i]]<20) ////100ms,Tst[],Add on 06.12.30
               { Tst[ASR_Ch[i]]++;
                                                         ///////// Modify on 06.12.31
                 DI[ASR_Ch[i]]=3; Val_Act(ASR_Ch[i]);
                 DI[ASR_ANCh[i]]=3; Val_Act(ASR_ANCh[i]);
               }
               else
               { Tst[ASR_Ch[i]]=0;
                 
                 step[ASR_Ch[i]]=0;step[ASR_ANCh[i]]=0;Zp[ASR_Ch[i]]=18;Zp0[ASR_Ch[i]]=2;                    
                 T1[ASR_Ch[i]]=0;T2[ASR_Ch[i]]=0;T3[ASR_Ch[i]]=0;T30[ASR_Ch[i]]=0;TVw[ASR_Ch[i]]=0;Tst[ASR_Ch[i]]=0;
                 ZNum[ASR_Ch[i]]=0;Kvs[ASR_Ch[i]]=0;////08.2.19
                 case0_flag[ASR_Ch[i]]=0;////Add on 08.6.30
                 Jb[ASR_Ch[i]]=0;Tzz[ASR_Ch[i]]=0;
                 T_Double_SR=0;  ////// 07.1.12
                 T_Single_SR=0;  //////Add on 08.2.19
                   	
                 ZY3[0]=0;ZY3[1]=0;ZY3[2]=0;ZY3[3]=0;ZY3[4]=0;ZY3[5]=0;  ////// Modify on 06.12.30
                 ASR_Entry_Flag[0]=0;ASR_Entry_Flag[2]=0;ASR_Entry_Flag[4]=0;////Add on 06.12.31
                 
                 //Fearth=0;        // Fearth==0,Add on 06.12.30;Move on 08.2.11 
                 //ctrlflag=0;
                 if(ASR_Entry_Flag[1]>5){ ctrlflag=3; }  //////Modify on 08.2.19
                 else
                 { ctrlflag=0;
                   if(Diagnose_Data[18]==0) 
                   { ASRLamp_SetLow;   ////////////////////////////////////  ASR_L=0
                     Lamp_Condition |= 0x40;   //// ASR Lamp die, bit6==1,Add on 07.4.10
                   }	             
                 }
                 
                 //Status_EBC1&=0xfb;  //// ASR brake control end,Add on 07.1.30
                 
                 for(ji=0;ji<SS;ji++)     /////////// 06.12.22
                 { DI[ji]=0;Val_Act(ji); 
                   st[ji]=0; //// Add on 06.12.30  
                 }
               }  
             }
           } 
        }   	
        else
        { T30[ASR_Ch[i]]=0;
          //T2[ASR_Ch[i]]=0;
          st[ASR_Ch[i]]=3;	 //////////////// 06.1.8 , Test 1-->3
          
          ASR_DIFCtrl_SetHigh;   ////////////////////////////////////  ASR_DIF=1,06.12.31
        }
        
        break;	                       
      }
   
      //if(DI[ASR_ANCh[i]]==0){ DI[ASR_Ch[i]]=3; }  //2006.2.24,Move on 06.12.15
   
      Val_Act(ASR_Ch[i]);
     }
     
    }
    else                      /////////// This Axis No ASR
    {if(ASR_Entry_Flag[0]>5||ASR_Entry_Flag[2]>5||ASR_Entry_Flag[4]>5)  //////// Add on 06.7.24 
     {      
      if(ZY3[i]<120) //////// 600ms
      { ZY3[i]++;
      	DI[i]=3;Val_Act(i);  
        DI[i+1]=3;Val_Act(i+1);  
      }
      else          ///////// 200ms
      { DI[i]=1;Val_Act(i);  
        DI[i+1]=1;Val_Act(i+1);  
      }	    
     } 
    }
    
    ////////////////////////////////////////////////////Error Exit,Add on 08.3.1-3;Move on 08.3.6
    ///////////Error Exit,Modify on 08.3.6
    if((Diagnose_Data[1]!=0||Diagnose_Data[2]!=0||Diagnose_Data[3]!=0||Diagnose_Data[4]!=0)||
    	 (Diagnose_Data[18]!=0)||(Diagnose_Data[9]!=0||Diagnose_Data[10]!=0))
    { Error_ASRExit_Flag=0xaa; }
    else{ Error_ASRExit_Flag=0; }
    	        
    //if(SRmaxo[i]<5&&st[ASR_Ch[i]]>2)////&&st[ASR_Ch[i]]>2,Add on 06.12.15
    ////T_Double_SR,07.1.12;Diagnose_Data[18],10000->8400,08.2.19;80->40,08.3.1;40->80,Error_ASRExit_Flag,08.3.6
    if((Vs_ASR>8400)||(T_Double_SR>80)||(Error_ASRExit_Flag==0xaa))
    { ////////////////////////////////////////////////////////////////////////////////////// Exit
      if(Error_ASRExit_Flag==0xaa){ Jb[ASR_Ch[i]]=20; }////Add on 08.3.6
      if(Jb[ASR_Ch[i]]<20)/////////////100ms,Modify on 06.12.30
      { Jb[ASR_Ch[i]]++;
        if(Aw[ASR_Ch[i]]>0){ DI[ASR_Ch[i]]=1; } 
        if(Aw[ASR_ANCh[i]]>0){ DI[ASR_ANCh[i]]=1; }	

        if(Aw[ASR_Ch[i]]<300){ DI[ASR_Ch[i]]=3; }
        if(Aw[ASR_ANCh[i]]<300){ DI[ASR_ANCh[i]]=3; }
        Val_Act(ASR_Ch[i]);
        Val_Act(ASR_ANCh[i]);
      }
      else
      {
       ASR_DIFCtrl_SetLow;   ////////////////////////////////////  ASR_DIF=0,Add on 06.12.30
       //ASR_DIFCtrl=0;
              
       //if(Tzz[ASR_Ch[i]]<20)////100ms,Modify on 08.3.6       
       if((Tzz[ASR_Ch[i]]<20&&Error_ASRExit_Flag!=0xaa)||(Tzz[ASR_Ch[i]]<40&&Error_ASRExit_Flag==0xaa))//100ms or 200ms
       {Tzz[ASR_Ch[i]]++; //2006.1.4
                          //////////////////// Modify on 06.12.31
        DI[ASR_Ch[i]]=3; Val_Act(ASR_Ch[i]);
        DI[ASR_ANCh[i]]=3; Val_Act(ASR_ANCh[i]); 
       }
       else
       {Tzz[ASR_Ch[i]]=0; 
        Jb[ASR_Ch[i]]=0;
        Error_ASRExit_Flag=0x55;////Add on 08.3.6
        //if(ASR_Entry_Flag[0]==0&&ASR_Entry_Flag[2]==0&&ASR_Entry_Flag[4]==0) ////Move on 06.12.30
        { 
          step[ASR_Ch[i]]=0;step[ASR_ANCh[i]]=0;Zp[ASR_Ch[i]]=18;Zp0[ASR_Ch[i]]=2;                    
          T1[ASR_Ch[i]]=0;T2[ASR_Ch[i]]=0;T3[ASR_Ch[i]]=0;T30[ASR_Ch[i]]=0;TVw[ASR_Ch[i]]=0;Tst[ASR_Ch[i]]=0;
          ZNum[ASR_Ch[i]]=0;Kvs[ASR_Ch[i]]=0;////08.2.19
          case0_flag[ASR_Ch[i]]=0;////Add on 08.6.30
          Jb[ASR_Ch[i]]=0;Tzz[ASR_Ch[i]]=0;
          T_Double_SR=0;  ////// 07.1.12
          T_Single_SR=0;  //////Add on 08.2.19
                   	
          ZY3[0]=0;ZY3[1]=0;ZY3[2]=0;ZY3[3]=0;ZY3[4]=0;ZY3[5]=0;  ////// Modify on 06.12.30
          ASR_Entry_Flag[0]=0;ASR_Entry_Flag[2]=0;ASR_Entry_Flag[4]=0;////Add on 06.12.31
                    
          //Fearth=0;        // Fearth==0,Add on 06.12.30;Move on 08.2.11
          //ctrlflag=0;
          
          if(ASR_Entry_Flag[1]>5){ ctrlflag=3; }  //////Modify on 08.2.19
          else
          { ctrlflag=0; 
            if(Diagnose_Data[18]==0) 
            { ASRLamp_SetLow;   ////////////////////////////////////  ASR_L=0
              Lamp_Condition |= 0x40;   //// ASR Lamp die, bit6==1,Add on 07.4.10
            }	
          }
          
          //Status_EBC1&=0xfb;  //// ASR brake control end,Add on 07.1.30
          
          for(ji=0;ji<SS;ji++)     /////////// 06.12.22
          { DI[ji]=0;Val_Act(ji); 
            st[ji]=0; //// Add on 06.12.30  
          } 
        }
       }
      }   	
    }
    else
    { Jb[ASR_Ch[i]]=0;Tzz[ASR_Ch[i]]=0; }
    
   } 
   ////////////////////////////////////////////////////////////////////////// ASR_Control
  }
 } 
 if(ctrlflag==2)                        ////////// ASR Exit
 {  
    if(Brake_Switch>10)  //////////// Brake Lamp Switch, Add at 06.5.25
    { ASR_DIFCtrl_SetLow;   ////////////////////////////////////  ASR_DIF=0
      if(ASR_Entry_Flag[1]>5){ ctrlflag=3; }  //////Modify on 08.2.19
      else
      { ctrlflag=0;
        if(Diagnose_Data[18]==0) 
        { ASRLamp_SetLow;   ////////////////////////////////////  ASR_L=0
          Lamp_Condition |= 0x40;   //// ASR Lamp die, bit6==1,Add on 07.4.10
        }	             
      }
    }
 }   
}
/////////////////////////////////////////////////////////////////////////

void det_control()
{
	unsigned char j=0,i=0;
	cancelflag1++;
	if(cancelflag1>9)
	{  
		cancelflag1=0;


		if(ctrlflag!=1)  //////////// Modify at 05.12.27
		{    
			if(VsFlag0==0)  
			{
				Vss=Vss1;
				Vsao=Vss1;
				if(Tslip==0) {Vsh=Vs_old[0];Vsq=Vsh;}  //2005.11.25
			}

			Vss1=Vs_old[2];
			Vs_old[2]=Vs_old[1];
			Vs_old[1]=Vs_old[0];
			Vs_old[0]=Vss2;

			if(Vs1>Vss2)
			{
				Vss2=(Vs1+Vs_old[1]+Vs_old[0]+Vs1)>>2;  //2007.1.28
			}
			else Vss2=(Vss2+Vs_old[1]+Vs_old[0]+Vs1)>>2;  //2007.1.28

		} 
	}

    if(BreakNum==0&&ctrlflag!=1)  /////////// Modify at 05.12.27
    {
		cancelflag0++;
		if(cancelflag0>4) cancelflag0=7;
    }
    else cancelflag0=0;

	if(cancelflag0>4&&VsFlag0==0&&ctrlflag!=1)  /////////// Modify at 05.12.27
	{
		cancelflag0=0;

		Vsao=Vss1;
		for(i=0;i<4;i++)  
		{
			Tnum[i]=0;
			if(ABS_Valves_Flag==0&&ctrlflag==0&&End_Break==0&&End_Break1==0&&selfcheck_flag!=200){ DI[i]=0;Val_Act(i); } 
		} 
	}
     

	{ 
		if(ctrlflag==2||ctrlflag==3){if(cxq0<100) cxq0++; }  //2007.1.28

		if(ctrlflag!=1) ////if(ctrlflag==0),Modify on 06.9.12,ASR------------
		{
			//Tgro=Tgr[0]+Tgr[1]+Tgr[2]+Tgr[3]; 2009.12.15 cut

			if(Vsao>3000&&VsFlag0==0)
			{

				Tckg1=0;	

				if((Awb[0]<-900&&SR[0]>10&&Nocontrols[0]<5)&&((Awb[1]<-400)||(Awb[2]<-400)||(Awb[3]<-400)))  Tckg1=2;// else Tckg1=0;
				if((Awb[1]<-900&&SR[1]>10&&Nocontrols[1]<5)&&((Awb[0]<-400)||(Awb[2]<-400)||(Awb[3]<-400)))  Tckg1=2;// else Tckg1=0;
				if(Awb[0]<-550&&Awb[1]<-550) Tckg1=2;

				if(ctrlflag!=5) //2009.12.15 add
				{
					if((Awb[2]<-900&&SR[2]>10&&Nocontrols[2]<5)&&((Awb[0]<-400)||(Awb[1]<-400)||(Awb[3]<-400)))  Tckg1=2;// else Tckg1=0;
					if((Awb[3]<-900&&SR[3]>10&&Nocontrols[3]<5)&&((Awb[0]<-400)||(Awb[1]<-400)||(Awb[2]<-400)))  Tckg1=2;// else Tckg1=0;
					if(Awb[0]<-550&&Awb[2]<-450) Tckg1=2;
					if(Awb[1]<-550&&Awb[3]<-450) Tckg1=2;
					if(Awb[0]<-550&&Awb[3]<-450) Tckg1=2;
					if(Awb[1]<-550&&Awb[2]<-450) Tckg1=2;
					if(Awb[3]<-450&&Awb[2]<-450) Tckg1=2;
					if(Awb[2]<-450&&Awb[3]<-450&&Awb[0]<-450&&Awb[1]<-450) Tckg1=2;
				}

				if(Tckg1>0) 
				{  
					BreakNum0++;
					if(BreakNum0>1)                              //2-->1: 2007.1.27
					{ 
						BreakNum0=0;
						VsFlag0=1;
						Vsh=Vs_old[1];           //2005.11.25
					}             
				}
				else 
				BreakNum0=0;
			}

			if(((Awb[0]>-400&&Awb[1]>-400&&Awb[2]>-400&&Awb[3]>-400)||(Tbreak0>150))&&VsFlag0==1)  
			{   
				VsFlag0=0;Tbreak0=0;   //Tslip=0;  05.1.3
				BreakNum0=0; 

				if(ctrlflag==0)        ///////// ASR----;if(),Add on 06.12.15 
				{
					DI[2]=0; Val_Act(2); 
					DI[3]=0; Val_Act(3); 
				}
				Tslip=0;  
			}

			if(VsFlag0==1)
			{
				if(Vss>0) Vss=Vss-1;
				if(Tnum[0]<5000) Tnum[0]++;
			}


			if(VsFlag0==1)  Tbreak0++; else Tbreak0=0;

			if(Vsao>3000) ////if(ctrlflag==0&&Vsao>3000),Modify on 06.9.12,ASR------------
			{
				Tgro=0;	
				AAr=(Awb[0]+Awb[1])>>1;
				if(AAr<-220) Tgro=1;

				if(ctrlflag!=5) //2009.12.15 add
				{
					AAr=(Awb[2]+Awb[3])>>1;
					if(AAr<-220) Tgro=1;
				}

				if(Tgro==1) 
				{
					Tslip++; Tslipk=0;
					if(Tslip>250) Tslip=250; 
					if(Vsq>Vmax2[0]){SRby=(signed int)(100-(100*Vmax2[0])/Vsq);} else {SRby=0;}//SRmaxo[3]->SRby,06.9.12;08.4.30
				}
				else
				{ 
					if(AAr>-140&&VsFlag0==0) { if(Tslipk<8) Tslipk++; if(Tslipk>5) Tslip=0;}     //2006.2.21
					else Tslipk=0;
				}

				///////////////////////////////////////////////////// EBD 2007.10.19
				if(Switch_Flag==0) // added at 2014.3.5
				{
					EBL_EBD(2);
				}
				///////////////////////////////////////////////////// EBD              

				if(VsFlag0==0&&Awb[0]>-200&&Awb[1]>-200&&Awb[2]>-200&&Awb[3]>-200) Tslip=0;


				if(VsFlag0==1||VsFlag==1)
				{           
				} 
				else 
				{ 
					if(End_Break1==0&&ctrlflag==0)       ///////// ASR----;&&ctrlflag==0,Add on 06.12.15
					{
						DI[2]=0;DI[3]=0;}
					}
				}

				if(ABS_Valves_Flag==0&&ctrlflag==0&&selfcheck_flag!=200) //// ASR----;&&ctrlflag==0,Add on 06.12.15,07.12.20
				{ Val_Act(2);Val_Act(3); }

				Vss1=Vs_old[2];
				Vs_old[2]=Vs_old[1];
				Vs_old[1]=Vs_old[0];
				Vs_old[0]=Vss2;
				//     Vss2=(Vsso+Vs_old[1]+Vs_old[0]+Vs1)>>2;
				if(Vs1>Vss2)
				{
					Vss2=(Vs1+Vs_old[1]+Vs_old[0]+Vs1)>>2;  //2007.1.28
				}
				else Vss2=(Vss2+Vs_old[1]+Vs_old[0]+Vs1)>>2;  //2007.1.28


				/////////////////////////////////////////////////////////////////

				if(Vsao>3000&&Vs>1200&&VsFlag==0)
				{  
					Tckg=0;
					Tgro=0;

					if(Awb[0]<-1000&&SR[0]>25&&Nocontrols[0]<5) Tgro=1;
					if(Awb[1]<-1000&&SR[1]>25&&Nocontrols[1]<5) Tgro=1;

					if(ctrlflag!=5) //2009.12.15 add
					{
						if(Awb[2]<-1000&&SR[2]>25&&Nocontrols[2]<5) Tgro=1;
						if(Awb[3]<-1000&&SR[3]>25&&Nocontrols[3]<5) Tgro=1;
					}
					if(Tgro>0) 
					{ 
						if((cxq0==100)||(SRby>-1&&cxq0<100)) { Tckg=2; } ////SRby 3-->-1 2007.1.27
					}

					if((Awb[0]<-1300&&SR[0]>12&&Nocontrols[0]<5)&&((Awb[1]<-700)||(Awb[2]<-700)||(Awb[3]<-700)))  Tckg=2;
					if((Awb[1]<-1300&&SR[1]>12&&Nocontrols[1]<5)&&((Awb[0]<-700)||(Awb[2]<-700)||(Awb[3]<-700)))  Tckg=2;
					if(Awb[0]<-1200&&Awb[1]<-1200)  Tckg=2;

					if(ctrlflag!=5) //2009.12.15 add
					{
						if((Awb[2]<-1300&&SR[2]>12&&Nocontrols[2]<5)&&((Awb[0]<-700)||(Awb[1]<-700)||(Awb[3]<-700)))  Tckg=2;
						if((Awb[3]<-1300&&SR[3]>12&&Nocontrols[3]<5)&&((Awb[0]<-700)||(Awb[1]<-700)||(Awb[2]<-700)))  Tckg=2;
						if(Awb[0]<-1200&&Awb[2]<-1200)  Tckg=2;
						if(Awb[1]<-1200&&Awb[3]<-1200)  Tckg=2;
						if(Awb[0]<-1200&&Awb[3]<-1200)  Tckg=2;
						if(Awb[1]<-1200&&Awb[2]<-1200)  Tckg=2;
						if(Awb[3]<-1200&&Awb[2]<-1200)  Tckg=2;
						if(Awb[2]<-1000&&Awb[3]<-1000&&Awb[0]<-1000&&Awb[1]<-1000)  Tckg=2;
					}

					if(Tckg>1)
					{   BreakNum++;
						//if(Awb[0]>-1000&&Awb[1]>-1000&&Awb[2]<-2000&&Awb[3]<-2000)
						//BreakNum=0;
						if(BreakNum>1)                              //2.14  ---  3
						{ 
							BreakNum=0;

							if(ctrlflag<2)    /////////////////////////////////////// Add on 07.1.26
							{ BreakNum1=0;
							//Vss=Vs;
							VsFlag=1;
							VsFlag0=0;
							}
							else     //////////////////////////////////////////////// Add on 07.1.26
							{ /////////////////////////////////////////////////////// Modify on 07.1.28
							if(Vw[2]<=Vs&&Vw[3]<=Vs)
							{ if(Aw[0]<-400||Aw[1]<-400) 
							{ BreakNum1=0;VsFlag=1;VsFlag0=0; }                              
							}
							else
							{ if(Vw[2]<=Vs||Vw[3]<=Vs)
							{ if(Aw[0]<-800||Aw[1]<-800) 
							{ BreakNum1=0;VsFlag=1;VsFlag0=0; }  
							}
							else
							{ if((Aw[0]<-900&&Aw[1]<-900)||(Aw[0]<-1400)||(Aw[1]<-1400)) 
							{ BreakNum1=0;VsFlag=1;VsFlag0=0; }
							}    
							}

							}  
						}                    
					}
					else 
						{ BreakNum=0; }
				}

				if(((Awb[0]>-450&&Awb[1]>-450&&Awb[2]>-450&&Awb[3]>-450)||(Tbreak>100))&&VsFlag==1)  
				{   
					VsFlag=0;Tbreak=0; //Tslip=0; 05.1.3sw
					BreakNum=0; BreakNum1=0; 

					VsFlag0=0;Tbreak0=0;   //Tslip=0;  05.1.3
					BreakNum0=0;  
				}

				if(VsFlag==1)  Tbreak++;else Tbreak=0;

				if(Vs>6000)  
				{ Vszo=(long int)(11*Vss/13); SRJY=7;}
				else   
				{ Vszo=(long int)(5*Vss/6);SRJY=7;}


				if(Vs>2000&&Vs<8000)   ////////////// 8000-->6000 /////05.11.8
				{
					SRJY=13-(unsigned char)(Vs/1200);  ///(15,10  :   2000,8000) // 17--> 14 ,05.7.24
				}

				if(Vs<2001) 
				{
					SRJY=13;  ///(30,15  :   1000,2000)  // 45--> 42 ,05.7.24
				}

		if(Vsao<4000) {SRJY=SRJY+6;SRJY1=SRJY+2;SRHF=25; }                  ///2006.6.28
		else if(Vsao<5000) {SRJY=SRJY+6;SRJY1=SRJY+1;SRHF=24;}
		else if(Vsao<6000) {SRJY=SRJY+5;SRJY1=SRJY+1;SRHF=23;}
		else if(Vsao<6900) {SRJY=SRJY+4;SRJY1=SRJY+1;SRHF=22;}
		else if(Vsao<7500) {SRJY=SRJY+4;SRJY1=SRJY+1;SRHF=21;}
		else if(Vsao<8000) {SRJY=SRJY+3;SRJY1=SRJY+1;SRHF=20;}
		else {SRJY=SRJY+3;SRJY1=SRJY+1;SRHF=18;}

		////////////////////////////////////////////////2007.11.27
		if(ctrlflag==0) // 2007.11.26 
		{
		if(Vsso>1700) // 2009.12.13 2200->1700
		{
		for(j=0;j<SS;j++)
		{
		if(Tkc[j]<20&&Aw[j]>-380) Tkc[j]++;

		if(SR[j]>35&&Aw[j]<-1800&&Tjj[j]<10)
		{
		Tjj[j]++; Tbb[j]=0;
		if(Nocontrols[j]>0&&Nocontrols[j]<51&&Tkc[j]>1) {Tkc[j]=0; Nocontrols[j]++; }//10-->5, 2007.11.29
		}
		else                 
		{
		Tjj[j]=0;
		if(Tjjc[j]>500&&Diagnose_Data[j+1]==0) {Nocontrols[j]=0;}
		}

		if(Tjj[j]>1) 
		{ if(Nocontrols[j]==0) {Nocontrols[j]=1; Tkc[j]=0;}}
		}
		}
		for(j=0;j<SS;j++)////4->SS,08.2.29
		{
		if(Nocontrol_w[j]>10) Nocontrols[j]=15;

		if(SR[j]<10) Tjjc[j]++; else Tjjc[j]=0;
		if(Vs<=Vwo) {Nocontrol_w[j]=0;Nocontrols[j]=0;Tjjc[j]=501;}
		if(Tjjc[j]>500) {Nocontrol_w[j]=0;Tjjc[j]=501;}
		if(Diagnose_Data[j+1]!=0) {Nocontrols[j]=18;}  //2007.12.7
		}
		}  
		////////////////////////////////////////////////////////////////////2007.11.27
		////////////////////////////////	 
		Tckg3=0;
		if(cxq0<100)
		{
		if((Vsao>10000&&SRby>1)||(Vsao>6000&&SRby>2)||(Vsao>3000&&SRby>2)) ////07.3.14
		{ 
		for(i=0;i<2;i++)
		{
		if(Nocontrols[i]<3&&SR[i]>SRHF&&Awb[i]<-1500)  Tckg3=i+1; //2007.3.8
		}

		if(ctrlflag!=5) //2009.12.15 add
		{
		for(i=2;i<4;i++)
		{
		if(Nocontrols[i]<3&&SR[i]>SRHF&&Awb[i]<-1500)  Tckg3=i+1; //2007.3.8
		}
		}

		}
		}
		else 
		{
		for(i=0;i<4;i++)
		{
		if(Nocontrols[i]<3&&SR[i]>SRHF&&Awb[i]<-1500)  Tckg3=i+1; //2007.3.8
		}
		}
		////////////////////////////////////////2007.1.28
		if((Nocontrols[0]<5&&SR[0]>SRJY1&&Awb[0]<-1500)&&(Nocontrols[1]<5&&SR[1]>7&&Awb[1]<-860))  Tckg3=11;
		if((Nocontrols[1]<5&&SR[1]>SRJY1&&Awb[1]<-1500)&&(Nocontrols[0]<5&&SR[0]>7&&Awb[0]<-860))  Tckg3=11;
		if(Nocontrols[0]<5&&SR[0]>SRJY&&Awb[0]<-1180&&Nocontrols[1]<5&&SR[1]>SRJY&&Awb[1]<-1180) Tckg3=11;

		if(ctrlflag!=5) //2009.12.15 add
		{
		if(Nocontrols[2]<5&&SR[2]>SRJY&&Awb[2]<-1180&&Nocontrols[0]<5&&SR[0]>SRJY&&Awb[0]<-1180) Tckg3=12;
		if(Nocontrols[0]<5&&SR[0]>SRJY&&Awb[0]<-1180&&Nocontrols[3]<5&&SR[3]>SRJY&&Awb[3]<-1180) Tckg3=13;
		if(Nocontrols[1]<5&&SR[1]>SRJY&&Awb[1]<-1180&&Nocontrols[2]<5&&SR[2]>SRJY&&Awb[2]<-1180) Tckg3=14;
		if(Nocontrols[1]<5&&SR[1]>SRJY&&Awb[1]<-1180&&Nocontrols[3]<5&&SR[3]>SRJY&&Awb[3]<-1180) Tckg3=15;
		if(Nocontrols[3]<5&&SR[3]>SRJY1&&Awb[3]<-1180&&Nocontrols[2]<5&&SR[2]>SRJY1&&Awb[2]<-1180) Tckg3=16;

		if((Nocontrols[0]<5&&SR[0]>SRJY1&&Awb[0]<-1500)&&(Nocontrols[2]<5&&SR[2]>7&&Awb[2]<-860))  Tckg3=12;
		if((Nocontrols[2]<5&&SR[2]>SRJY1&&Awb[2]<-1500)&&(Nocontrols[0]<5&&SR[0]>7&&Awb[0]<-860))  Tckg3=12;

		if((Nocontrols[0]<5&&SR[0]>SRJY1&&Awb[0]<-1500)&&(Nocontrols[3]<5&&SR[3]>7&&Awb[3]<-860))  Tckg3=13;
		if((Nocontrols[3]<5&&SR[3]>SRJY1&&Awb[3]<-1500)&&(Nocontrols[0]<5&&SR[0]>7&&Awb[0]<-860))  Tckg3=13;

		if((Nocontrols[1]<5&&SR[1]>SRJY1&&Awb[1]<-1500)&&(Nocontrols[2]<5&&SR[2]>7&&Awb[2]<-860))  Tckg3=14;
		if((Nocontrols[2]<5&&SR[2]>SRJY1&&Awb[2]<-1500)&&(Nocontrols[1]<5&&SR[1]>7&&Awb[1]<-860))  Tckg3=14;

		if((Nocontrols[1]<5&&SR[1]>SRJY1&&Awb[1]<-1500)&&(Nocontrols[3]<5&&SR[3]>7&&Awb[3]<-860))  Tckg3=15;
		if((Nocontrols[3]<5&&SR[3]>SRJY1&&Awb[3]<-1500)&&(Nocontrols[1]<5&&SR[1]>7&&Awb[1]<-860))  Tckg3=15;

		if((Nocontrols[2]<5&&SR[2]>SRJY1&&Awb[2]<-1500)&&(Nocontrols[3]<5&&SR[3]>7&&Awb[3]<-860))  Tckg3=16;
		if((Nocontrols[3]<5&&SR[3]>SRJY1&&Awb[3]<-1500)&&(Nocontrols[2]<5&&SR[2]>7&&Awb[2]<-860))  Tckg3=16;
		}

		if(ctrlflag!=5) //2009.12.15 add
		if(Nocontrols[2]>10&&Nocontrols[3]>10) Tckg3=0;

		if(Tvmaxsr>3) Tckg3=0;

		/////////////////////////////////////////////////////////////////////// Single Wheel Enter ABS,Add on 07.3.11
		for(i=0;i<SS;i++)
		{ if(Tckg3==0&&Vsao>3800)
		{ if(SR[i]>15&&SRby>1&&Nocontrols[i]<3)         //2007.10.30 ,&&SRby>-1; <3,07.11.2
		{ if(Awb[i]<-650){ if(Aw_single_Flag[i]<200){ Aw_single_Flag[i]++; } }
		if(Awb[i]>260){ Aw_single_Flag[i]=0; }
		if(SR[i]>90)        ////45-->90,07.4.18
		{ if(Aw_single_Flag[i]>6){ Aw_single_Flag[i]=0;Tckg3=i+1;VsFlag=1; }//07.3.14;Tckg3=20-->=i+1,Modify 07.4.10
		else
		{ if(SR[i]>98){ Aw_single_Flag[i]=0; } } ////92-->98,07.4.18
		}
		}
		else{ Aw_single_Flag[i]=0; }
		}
		else{ Aw_single_Flag[i]=0; }
		}  
		if(Tckg3>0&&VsFlag==1) ////Tckg3==1 --> >0,Modify on 07.4.10
		{ for(i=0;i<SS;i++)
		{ Aw_single_Flag[i]=0; }
		}
		/////////////////////////////////////////////////////////////////////// Single Wheel Enter ABS          

		////////////////////////// Modfy on 07.9.7    
		//          if((Switch_Flag==0xa5&&OffRoad_40km_Flag!=0)||(unctrl!=0))//OFF ROAD,07.11.30,08.2.11 
		if((unctrl!=0))//added at 2014.2.24
		{ Tckg3=0; }
		if(Tckg3>0&&VsFlag==1)
		{
		ctrlflag=1;
		kword_main();
		Tslipk=Tslip;
		VsFlag=0;
		VsFlag0=0;
		for(i=0;i<4;i++) {Tnum[i]=Tnum[0];}
		End_Break=0; End_Break1=0;
		Tckg1=0;Tckg2=0;
		BreakNum=0;BreakNum0=0;BreakNum1=0;
		DI[2]=0;Val_Act(2);DI[3]=0;Val_Act(3); //2009.11.17
		F_ctrl_SetHigh;    // able Vbb (F_ctrl==1)
		Fearth_SetHigh;    // able EARTH (Fearth==1)
		Check_Flag_Clear();   //////// Modify on 07.2.6
		init_control();  

		} 

		}//if(ctrlflag==0)

	if(Diagnose_Data[3]!=0&&Diagnose_Data[4]!=0&&ABS_Valves_Flag==0){ctrlflag=0; for(i=0;i<4;i++){DI[i]=0;Val_Act(i);}}


	if(ctrlflag==1&&Ktcontrol>99)
	{
	control();
	//////////////////////////////////////////////////////////////////////////
	//ABSLamp=0;       // LAMP_ctrl1==0(LAMP_ctrl2==1 or 0 !!!); warn LED light  
	/////////////////////////////////////////////////////////////////////////
	}

	//if(End_Break1==1&&ctrlflag==0)
	if(End_Break1==1)
	{     EndControl1();
	//////////////////////////////////////////////////////////////////////////
	//ABSLamp=1;       // LAMP_ctrl1==1(LAMP_ctrl2==1 or 0 !!!); warn LED off  
	/////////////////////////////////////////////////////////////////////////
	}
	//if(End_Break==1&&ctrlflag==0)
	if(End_Break==1&&ctrlflag==0)
	{     
	EndControl();

	for(i=0;i<4;i++) 
	{
	Tawj1[i]=0;
	Tawj[i]=0;
	Tbb[i]=0;
	Tnum[i]=0;
	GV_st5_Enter_group[i]=0; // added at 2016.10.16
	}

	Vss1=Vs1; Vss=Vss1;
	Vs_old[2]=Vs1;
	Vs_old[1]=Vs1;
	Vs_old[0]=Vs1;
	Vss2=Vs1;
	Vsao=Vs1;
	Tslip=0;

	//////////////////////////////////////////////////////////////////////////
	//ABSLamp=1;       // LAMP_ctrl1==1(LAMP_ctrl2==1 or 0 !!!); warn LED off  
	/////////////////////////////////////////////////////////////////////////
	}

	} //if(p1_4==0&&unctrl==0)


   if(Ktcontrol<101){ Ktcontrol++; }
	 
}

void EBL_EBD(unsigned char T_ch)
{
//     if(((Switch_Flag!=0xa5)||(OffRoad_40km_Flag==0))&&(unctrl==0))//OFF ROAD,06.10.31;OffRoad_40km_Flag,Add on 07.12.20
     if((unctrl==0))//added at 2014.2.24
      {
       if(ctrlflag!=5) //2009.12.15 add
       {       	
       	if(Vs>7600&&(ctrlflag==0||ctrlflag==4))
        {
//         if(Tzzo[0]>20) { Tzzo[0]=0; DI[0]=0;}
  //       if(Tzzo[1]>20) { Tzzo[1]=0; DI[1]=0;}
          /////////////////////////SR+5,Aw-400,08.10.20;09.2.11
          if(SR[T_ch]>4&&Aw[T_ch]<-1100)
           {if(Nocontrols[T_ch]<3&&SRby>-1){DI[T_ch]=1;ctrlflag=4;}}
            if(((SR[T_ch]<2)||(Aw[T_ch]>100))&&DI[T_ch]==1) DI[T_ch]=0; if(DI[T_ch]==1&&SR[T_ch]<0) {DI[T_ch]=0;}
          if(SR[T_ch+1]>4&&Aw[T_ch+1]<-1100)
           {if(Nocontrols[T_ch+1]<3&&SRby>-1){DI[T_ch+1]=1;ctrlflag=4;}}
            if(((SR[T_ch+1]<2)||(Aw[T_ch+1]>100))&&DI[T_ch+1]==1) DI[T_ch+1]=0; if(DI[T_ch+1]==1&&SR[T_ch+1]<0) {DI[T_ch+1]=0;}
         
         if(ctrlflag==4)
         {
          if(DI[T_ch]==1&&Tzzo[T_ch]<29) Tzzo[T_ch]++; if(Tzzo[T_ch]>15&&Aw[T_ch]>-900) {DI[T_ch]=0;Tzzo[T_ch]=0;}
          if(DI[T_ch+1]==1&&Tzzo[T_ch+1]<29) Tzzo[T_ch+1]++; if(Tzzo[T_ch+1]>15&&Aw[T_ch+1]>-900) {DI[T_ch+1]=0;Tzzo[T_ch+1]=0;}

          if(SR[T_ch]<1&&SR[T_ch+1]<1)
          {ctrlflag=0;DI[T_ch]=0;DI[T_ch+1]=0;}
          else 
          {
           if((Diagnose_Data[T_ch+1]!=0)||(Diagnose_Data[T_ch+2]!=0)) {ctrlflag=0;DI[T_ch]=0;DI[T_ch+1]=0;}
           if(Nocontrols[T_ch]>2) {DI[T_ch]=0;}
           if(Nocontrols[T_ch+1]>2) {DI[T_ch+1]=0;}  //3-->2, 2007.11.27
          }
         }
         
//          if(ctrlflag==4)       
          {Val_Act(T_ch);  Val_Act(T_ch+1); } 
//          else
          {
            if(DI[T_ch]==0) {Val_Act(T_ch); } 
            if(DI[T_ch+1]==0) {Val_Act(T_ch+1); } 
          }
          if(DI[T_ch]==0&&DI[T_ch+1]==0){ ctrlflag=0;} 
        } //if(Vs>7600&&(ctrlflag==0||ctrlflag==4))
       }
      }
        
        if(Vs<7601&&ctrlflag==4){ctrlflag=0;DI[T_ch]=0;DI[T_ch+1]=0; Val_Act(T_ch);Val_Act(T_ch+1);}
        
        if(ctrlflag!=4) {Tzzo[T_ch]=0;Tzzo[T_ch+1]=0;}
}


void Val_Act(char i)   //阀激活。
{    
     if(Diagnose_Data[i+1]!=0&&ctrlflag<2)
	 	{ DI[i]=0; }  ////////////////// Add at 2007.1.25
     if(Diagnose_Data[i+7]!=0)
	 	{ DI[i]=0; }  ////////////////// Add at 2005.2.18
     if(Nocontrols[i]>5&&ctrlflag<2)
	 	{ DI[i]=0; }  // Modify at 2006.1.5;ctrlflag!=2--> <2,06.12.22; 15--25:2007.1.26


     //if(No_Signal[i]==0)
     { 
         switch(DI[i])
         {
            case 0:   ////ZY
              if(i==0) 
              {  
                 F11Ctrl_SetLow;
                 F12Ctrl_SetLow;  
              }
              else if(i==1)
              {
                 F21Ctrl_SetLow;
                 F22Ctrl_SetLow;
              } 
              else if(i==2) 
              {
                 F31Ctrl_SetLow;
                 F32Ctrl_SetLow;  
              }
              else if(i==3)
              {
                 F41Ctrl_SetLow;
                 F42Ctrl_SetLow;
              } 

              break;
           case 1:    ////////BY
              if(i==0) 
              {
                 F11Ctrl_SetHigh;
                 F12Ctrl_SetLow;  
              }
              else if(i==1)
              {
                 F21Ctrl_SetHigh;
                 F22Ctrl_SetLow;
              } 
              else if(i==2) 
              {
                 F31Ctrl_SetHigh;
                 F32Ctrl_SetLow;  
              }
              else if(i==3)
              {
                 F41Ctrl_SetHigh;
                 F42Ctrl_SetLow;
              } 
              break;
           case 3:///////JY
              if(i==0) 
              {
                 F11Ctrl_SetHigh;
                 F12Ctrl_SetHigh;  
              }
              else if(i==1)
              {
                 F21Ctrl_SetHigh;
                 F22Ctrl_SetHigh;
              } else if(i==2) 
              {
                 F31Ctrl_SetHigh;
                 F32Ctrl_SetHigh;  
              }
              else if(i==3)
              {
                 F41Ctrl_SetHigh;
                 F42Ctrl_SetHigh;
              } 
              break;
         }
        }
}
//////////////////////////////////////////////////////////////////   control
void init_control()
{   
   unsigned char i;
   
   /////////////////////////////////////////// ASR Have Use, Clear Zero,Add on 06.12.15

   Vstq=958;Vsth=958;
   //Vsth2=958;       //////// 4S/4M,Move on 06.12.30
   /////////////////////////////////////////// ASR Have Use, Clear Zero,Add on 06.12.15   

   if(Vsq>Vsao) Vsao=Vsq;
   if(Vsh>Vsao) Vsao=Vsh;
   
   SRHF=10;
   Tquit=0;
   Vs_mid=Vmax>>2; Vsr=Vmax>>1;
   Vs_mid=Vs_mid+Vsr;
   LV_SwitchFlag_ByteCount=0; // added at 2013.2.23
   
   for(i=0;i<2;i++)
   {  
      AA1=AA0;       //-6000
      BB0=450;
      BB[i]=0;   
      st[i]=0;
      DI[i]=0;
      
      SR[i] = 0;
      SRmax[i]=0;
      SRmaxo[i]=0;
      step[i] = 0;
      T1[i] = 0;
      
      T2[i]=0;
      T3[i]=0;
      Tbb[i]=0;
      WheelBadFlag[i]=0;
      WheelBadNum[i]=0;
      Vs_old[i]=Vsq;
      

      //Tnum[i]=0;
//      Kvs[i]=15;
      Zp[i]=2;
      Zp0[i]=2;
      ZNum[i]=0;
      Jb[i]=0;
      
      Tzz[i]=0;
      SRflag[i]=1;
      Vw0[i]=Aw[i];
      Vw1[i]=0;
      ZY3[i]=0;
      TStepNum[i]=22;
      TVw[i]=0;

      Jbk[i]=0;
      Tzzo[i]=0;
      Tcjy[i]=0;
      Vsm_st[i]=(unsigned int)(Vs_old[i]);

      Tkg_DKF[i]=0;
   }

   for(i=2;i<4;i++)
   {
      AA1=AA0;       ///800
      BB1=400;
      BB[i]=0;   
      //Thf[i]=0;
      st[i]=0;
      DI[i]=0;
      
      SR[i] = 0;
      SRmax[i]=0;
      Tbb[i]=0;
      step[i] = 0;
      T1[i] = 0;
      T2[i]=0;
      T3[i]=0;      
      WheelBadFlag[i]=0;
      WheelBadNum[i]=0;
      Vs_old[i]=Vsq;
      //Tnum[i]=0;
//      Kvs[i]=15;
      Zp[i]=2;
      Zp0[i]=2;
      ZNum[i]=0;
      Jb[i]=0;
      Tzz[i]=0;
      Vw0[i]=Aw[i];
      Vw1[i]=0;
      ZY3[i]=0;
      TStepNum[i]=22;
      TVw[i]=0;
      Tzzo[i]=0;
      Vsm_st[i]=(unsigned int)(Vsq);

   }
   
//   Tskt=0;
//   Tpm=0;
   Jbcz[0]=0;Jbcz[1]=0;  //////////// 05.11.10
   Tckg1=0;
   Tckg=0;
   Kvo=30;    //2009.5.23
   Tdjk=0;
   Tksjz=0;
   DKF=0;
   VsFlag=0;
   EBj=0;
   EBk=6;
   EBj1=0;
   EBk1=6;
   T0=0;
   StepNum1=20;
   StepNum2=20;
   StepNum10=16;
   StepNum20=16;
   T10=50; 
   ZYhz=0;
   
   Vpmaxo=Vwo;
   Vpmaxo_st=Vwo;
   Tho=0;
   Tvpmaxo=0;
   Tst5=0;
   Tdkbz=0;
   Jk_gd=1;
   Tvn=0;
   Tdk=0;
    Tgro=Tgr[0]+Tgr[1]+Tgr[2]+Tgr[3];
   Ttjs=0;Tqjs_k=0;
   Tstep=1;Tstep1=1;
    Vmax=1100;
    if(Tgr[0]==0&&Vw[0]>Vmax) Vmax=Vw[0];    
    if(Tgr[1]==0&&Vw[1]>Vmax) Vmax=Vw[1];    
    if(Tgr[2]==0&&Vw[2]>Vmax) Vmax=Vw[2];    
    if(Tgr[3]==0&&Vw[3]>Vmax) Vmax=Vw[3];    

//       Vsq=(11*Vss)/13;   //(0.76)*Vsao

     if(Tgro<=3)
      {if(Tslip>=250)
       {
         Vsq=Vs2;	
       }
       else if(Tslip>=120)
       {
       	Vsq=(7*Vs2+Vszo)>>3;
       	Vsq=(Vs2+Vsq)>>1;
       }
       else if(Tslip>=60)
       {
       	Vsq=(7*Vs2+Vszo)>>3;
       }
       else if(Tslip>=30)
       {
       	Vsq=(3*Vs2+Vszo)>>2;
       }
       else if(Tslip>=12)
       {
       	if(Aw[0]>-850||Aw[1]>-850||Aw[2]>-850||Aw[3]>-850)
       	  Vsq=(5*Vmax+Vszo+Vszo+Vszo)>>3;
       	 else
       	  Vsq=(3*Vmax+Vszo)>>2;
       }
       else if((Tslip>=8)||(Vsao>5000))
       {
        Vsq=(Vmax+Vszo)>>1;    
       }      
       else
       {
         Vsq=Vss-500; 
       }
      }
      else Vsq=Vszo; 
 
        //SR[0]=(signed int)(100-(100*Vw[0])/Vsh);
        //SR[1]=(signed int)(100-(100*Vw[1])/Vsh);
   
    if(Vsao>18000)
    {
       Kv=32;   //if(Tslip<14) Vsq=Vsh-100;                         //2005.11.29    
    }
    else if(Vsao>12000)
    {
       Kv=31;     //  if(Tslip<13) Vsq=Vsh-100;                         //2005.11.29    
    }
    else if(Vsao>8000)
    {
       Kv=30;       //if(Tslip<12) Vsq=Vsh-100;                         //2005.11.29    
    }
    else if(Vsao>7000)
    {
       Kv=28;    //  if(Tslip<12) Vsq=Vsh-100;                         //2005.11.29    
    }
    else if(Vsao>6000)
    {
       Kv=27;     //  if(Tslip<11) Vsq=Vsh-100;                         //2005.11.29    
    }
    else if(Vsao>5000)
    {
       Kv=25;     //  if(Tslip<11) Vsq=Vsh-100;                         //2005.11.29    
    }
    else if(Vsao>4000)
    {
       Kv=23;     //  if(Tslip<10) Vsq=Vsh-100;                         //2005.11.29    
    }
    else 
    {
       Kv=21;    //   if(Tslip<10) Vsq=Vsh-100;                         //2005.11.29    
    }
///////////////////2006.1.5
    
//    Vsh=Vsao-40*Tslipk;   //2006.1.6
      if(Tslip>60) 
       {
       	if(Vmax<Vsh)
       	 {if(SR[0]>6&&SR[1]>6) { Vsq=(Vmax+Vsh)>>1;Vsq=(Vmax+Vsq)>>1;}}
       }	
//    if(Vsq<Vsh) Vsq=Vsh;
       // Vsq=Vsh;  // 2013.7.1
       Vsq=Vsh-1000;  // 2013.7.1
     
       Vstq=Vsq;
       Vsth=Vsq;
       Vss=Vsq;
       Vsso=Vsq;
       
       Vs=Vsq;

/*   //2006.1.5
    if(Vsao<8000)
    {
     if(Tslip<9)  {Kv=3;} else if(Tslip<16)  {Kv=6;}   //6,9
    }
    else if(Vsao<13000)
    {
     if(Tslip<12)  {Kv=4;} else if(Tslip<17)  {Kv=7;}  //8,11
    }
    else 
    {
     if(Tslip<14)  {Kv=5;} else if(Tslip<18)  {Kv=8;}  //10,13 
    }
*/

    Tslipk=0;
    //Tslip=0;
    Tvmaxsr=0;
    //      Kv=20;
       Kvh_old=Kv;Kvq_old=Kv;Kvq=Kv;Kvh=Kv;
    
    Tkcqh=0;
    Tkvcs=0;      
   for(i=0;i<4;i++)
   {
    Tvnold[i]=-1;   	
    Vwzo[i]=0;
    Tkcn[i]=0;
    Tkc[i]=0;
    Tawj2[i]=0;
    Tst[i]=0;
    Awr[i]=0;
    Jb_bak[i]=0; 
    //Tvnew[i]=0;
    Vnold[i]=Vw[i];
    Dzbz[i]=0;
    Tjj[i]=0;
    Tzy[i]=0;
    Tawj[i]=0;
    Tawj1[i]=0;
    Awb[i]=Vw[i];
    Vsck[i]=Vsq;
    if(Tslip>25) Vsck[i]=(unsigned int)(Vsq); 
     else {V30=(Vsao+Vsq)>>1; Vsck[i]=(unsigned int)(V30);}

    Tgr1[i]=1;
    Taw200[i]=0;
    T30[i]=0;
    LV_OffRoad_ByteCount[i]=0; // added at 2014.2.25
    LV_OffRoad_ControlFlag[i]=0; // added at 2014.2.27

    Kvs[i]=Kv;
    Vpmax[i]=Vwo;
    Vw2[i]=0;
    if(i<2)
     { if(Vw[i]>Vsq) {Vsr=Vsq>>3; if(Vw[i]<(Vsq+Vsr)) Vstq=(Vstq+Vw[i])>>1;}}
    Tqjs[i]=0;
    T_tire[i]=0;
    GV_st5_Enter_group[i]=0; // added at 2015.10.16
   }

//   if(Kv<13) Kv=13;        //2005.11.25

}

void EndControl(void)
{      unsigned int i,jn;
       if(EBk<10)  //8
       { 
          if(EBj<Tstep)
          {  for(i=0;i<2;i++)
             {  
                for(jn=0;jn<1000;jn++)
                {  
                   DI[i]=0;	
                }
                Val_Act(i);
             }
             EBj++;         
          }
          else if(EBj<15)   //20
          {  for(i=0;i<2;i++)
             {
               for(jn=0;jn<1000;jn++)
                {  
                   DI[i]=1;	
                }
                  Val_Act(i);
             }
             EBj++;
          }
          else 
          {  EBj=0;
             Tstep++;
             if(Tstep>4) Tstep=4;
             EBk++;
          }       
        }
        else if(EBk==10)
        {  for(i=0;i<2;i++)
           {
                for(jn=0;jn<1000;jn++)
                {  
                   DI[i]=0;	
                }
               Val_Act(i);
           }
           
           EBk=11;
        }
        else 
        { 
         for(i=0;i<SS;i++)  
         { 
                for(jn=0;jn<1000;jn++)
                {  
                   DI[i]=0;	
                }
               Val_Act(i);  //2005.12.12
         } 
           End_Break=0;
           End_Break1=0;
           //if(Ktcontrol<80)  Ktcontrol=80; // 2006.1.9
           if(Vs>3000) Ktcontrol=80; // 2009.11.16
           //ctrlflag=0;  //2005.12.11
         
           //F_ctrl=0;    // disable Vbb (F_ctrl==0)
           //Fearth=0;    // disable EARTH (Fearth==0)
           if(ctrlflag==0&&ABS_Valves_Flag==0)  //2005.12.30
//           if(ABS_Valves_Flag==0) 
           {
             //p3=0x00; // ABS Valves Ctrl == 0;
			F11Ctrl_SetLow;//F1-1 IN
			F12Ctrl_SetLow;//F1-2 IN
			F21Ctrl_SetLow;//F2-1 IN
			F22Ctrl_SetLow;//F2-2 IN
			F31Ctrl_SetLow;//F3-1 IN
			F32Ctrl_SetLow;//F3-2 IN
			F41Ctrl_SetLow;//F4-1 IN
			F42Ctrl_SetLow;//F4-2 IN
           }  
           if(ABS_Lamp_RET_Flag==0)   /// Add at 2004.12.22 
             H_ctrl_Set_Low;
           EBj=0;
           EBk=6;
           EBj1=0;
           EBk1=6;
           Tslip=0;
        }
   
   Tgro=0;
   // if(Tslipk>26&&((Vw[0]<V30&&Vw[3]<V30)||(Vw[1]<V30&&Vw[2]<V30)||(Vw[0]<V30&&Vw[1]<V30))) Tgro=1;  //2009.10
   //if(Vw[0]<V30&&Vw[3]<V30&&Vw[1]<V30&&Vw[2]<V30) Tgro=1; //2009.10
   
   if((Tckg==26)||(Tgro==1))
   {   
         for(i=0;i<SS;i++)  
         { 
                for(jn=0;jn<1000;jn++)
                {  
                   DI[i]=0;	
                }
               Val_Act(i);  //2005.12.12
         } 
           End_Break=0;
           End_Break1=0;
           if(Tckg==26&&Vs>3000) Ktcontrol=60; // 2009.11.16
           //ctrlflag=0;
           //F_ctrl=0;    // disable Vbb (F_ctrl==0)
           //Fearth=0;    // disable EARTH (Fearth==0)
           if(ctrlflag==0&&ABS_Valves_Flag==0)
           {
            // p3=0x00; // ABS Valves Ctrl == 0;
            F11Ctrl_SetLow;//F1-1 IN
			F12Ctrl_SetLow;//F1-2 IN
			F21Ctrl_SetLow;//F2-1 IN
			F22Ctrl_SetLow;//F2-2 IN
			F31Ctrl_SetLow;//F3-1 IN
			F32Ctrl_SetLow;//F3-2 IN
			F41Ctrl_SetLow;//F4-1 IN
			F42Ctrl_SetLow;//F4-2 IN
           }   
           if(ABS_Lamp_RET_Flag==0)   /// Add at 2004.12.22
             H_ctrl_Set_Low;
           EBj=0;
           EBk=6;
           EBj1=0;
           EBk1=6;
      // ctrlflag=0;  
        
   }   
}


void EndControl1(void)
{      unsigned int i;
       if(EBk1<10)  //8
       { 
          if(EBj1<Tstep1)
          {  for(i=2;i<4;i++)
             {  DI[i]=0;
                Val_Act(i);
             }
             EBj1++;         
          }
          else if(EBj1<15)   //20
          {  for(i=2;i<4;i++)
             {  DI[i]=1;
                Val_Act(i);
             }
             EBj1++; 
          }
          else 
          {  EBj1=0;
             Tstep1++;
             if(Tstep1>4) Tstep1=4;
             EBk1++;
          }       
        }
        else if(EBk1==10)
        {  for(i=2;i<4;i++)
           {   DI[i]=0;
               Val_Act(i);
           }
           
           EBk1=11;
        }
        else 
        { for(i=2;i<4;i++)  
         { DI[i] = 0; Val_Act(i);} 
           //End_Break1=0;   //2009.12.4 cut
           //ctrlflag=0;
           //F_ctrl=0;    // disable Vbb (F_ctrl==0)
           //Fearth=0;    // disable EARTH (Fearth==0)
           /*
           if(ctrlflag==0&&ABS_Valves_Flag==0)
           {
             p3=0x00; // ABS Valves Ctrl == 0;
           } 
           */ 
           //if(ABS_Lamp_RET_Flag==0)   /// Add at 2004.12.22 
             //H_ctrl=0;
           //EBj1=0;
           //EBk1=6;
        }
}

void control()
{
	   long int V10=1300,Vs3=958;//SRHF=10,;Vsmin=0,Move on 06.12.31
	   unsigned int Kvmax=10,i,j,Tabk,Vsck_b;//SRJY,SRZY,SRJY1,SRZY1;
	   unsigned char Jb0[4]={10,10,10,10},ij,ijs;
	
	///////////////////////////////disable RET (H-ctrl==1) //2007.12.2
	 if(Nocontrols[0]<6) 
	   {if(SR[0]>7) {if(Tgro_ret==2) {H_ctrl_SetHigh;} else Tgro_ret=1;}}
	   else Tgro_ret=1;
	 if(Nocontrols[1]<6) 
	   {if(SR[1]>7) {if(Tgro_ret==1) {H_ctrl_SetHigh;} else Tgro_ret=2;}}
	   else Tgro_ret=2;
	 if(Nocontrols[0]>5&&Nocontrols[1]>5) H_ctrl_SetHigh;
	
	   Jb0[0]=10;Jb0[1]=10;
	
	   Jb0[2]=10;Jb0[3]=10;
	
	   if(Tksjz<250) Tksjz++; if(Tksjz<20) {Vflag=0;}
	
	
	//if(Tksjz>5&&Tksjz<50)   //2006.2.13
	if(Tksjz>5&&Tksjz<50&&Tslip<36)   //2006.2.13
	{
	 Tgro=0;Tvs1=0; 	
	
	  if(Tkcqh>(Tslipk+9))
	   {
			 if(SRmax[2]>20&&SRmax[3]>20) Tgro=1; 
			  else if(SRmax[2]>5&&SRmax[3]>20) Tgro=1; 
			   else if(SRmax[2]>20&&SRmax[3]>5) Tgro=1; 
	   }
	   else  
	   {if(SRmax[0]>20&&SRmax[1]>20) Tgro=1;
		 else if(SRmax[0]>5&&SRmax[1]>20) Tgro=1;
		 else if(SRmax[0]>20&&SRmax[1]>5) Tgro=1;
	   }
	 
	
	 for(i=0;i<SS;i++)
	 { 
	  if(Taw200[i]>0) {Tvs1++;Tgro=2;}
	 }
	
	 for(i=0;i<2;i++)
	 { 
	  if(Diagnose_Data[i+1]!=0) Tgro=0;
	 }
	 
	
	 if(Tkvcs<7&&Tslipk<26&&Tgro>0)
	 { 
	  if(Tgro==2&&Tksjz>19) Tkvcs++;
	  Tgro=0;	
	  Tvs1=0;	
	  if(Tgro==0)
	  {
	   for(i=0;i<SS;i++) 
	   {
		if(SRmax[i]<5) {Tvs1++;Kvs[i]=30;}
		if(Tvs1>1) {Kv=30; Tgro=1;}
	   }
	  }
	 
	  Tvs1=0;	
	  if(Tgro==0)
	  {
	   for(i=0;i<SS;i++) 
	   {
		if(SRmax[i]<10) {Tvs1++;Kvs[i]=26;}
		if(Tvs1>1) {Kv=26; Tgro=2;}
	   }
	  }
	 
	  Tvs1=0;	
	  if(Tgro==0)
	  {
	   for(i=0;i<SS;i++) 
	   {
		if(SRmax[i]<16) {Tvs1++;Kvs[i]=23;}
		if(Tvs1>1) {Kv=23; Tgro=3;}
	   }
	  }
	
	  Tvs1=0;	
	  if(Tgro==0)
	  {
	   for(i=0;i<SS;i++) 
	   {
		if(SRmax[i]<21) {Tvs1++;Kvs[i]=18;}
		if(Tvs1>1) {Kv=18; Tgro=4;}
	   }
	  }
	
	  Tvs1=0;	
	  if(Tgro==0)
	  {
	   for(i=0;i<SS;i++) 
	   {
		if(SRmax[i]<27) {Tvs1++;Kvs[i]=15;}
		if(Tvs1>1) {Kv=15; Tgro=5;}
	   }
	  }
	
	  Tvs1=0;	
	  if(Tgro==0)
	  {
	   for(i=0;i<SS;i++) 
	   {
		if(SRmax[i]<36) {Tvs1++;Kvs[i]=10;}
		if(Tvs1>1) {Kv=11; Tgro=5;}
	   }
	  }
	
	  Tvs1=0;	
	  if(Tgro==0)
	  {
	   for(i=0;i<SS;i++) 
	   {
		if(SRmax[i]<50) {Tvs1++;Kvs[i]=7;}
		if(Tvs1>1) {Kv=7; Tgro=6;}
	   }
	  }
	
	  Tvs1=0;	
	  if(Tgro==0)
	  {
	   for(i=0;i<SS;i++) 
	   {
		if(SRmax[i]<60) {Tvs1++;Kvs[i]=5;}
		if(Tvs1>1) {Kv=5; Tgro=7;}
	   }
	  }
	
	  if(Tkvcs==7)
	  {
	   if(Tkcqh>(Tslipk+9))
	   {
		if(SRmax[2]<5&&SRmax[3]<5) Tgro=1; 
		 else if(SRmax[2]<10&&SRmax[3]<10) Tgro=2;
		  else if(SRmax[2]<16&&SRmax[3]<16) Tgro=3;
		   else if(SRmax[2]<21&&SRmax[3]<21) Tgro=4; 
			else if(SRmax[2]<27&&SRmax[3]<27) Tgro=5;
			 else if(SRmax[2]<36&&SRmax[3]<36) Tgro=6;
			  else if(SRmax[2]<50&&SRmax[3]<50) Tgro=7;    
			   else if(SRmax[2]<101&&SRmax[3]<101) Tgro=8;	
			   
	   }
	   else  
	   {
		if(SRmax[0]<5&&SRmax[1]<5) Tgro=1; 
		 else if(SRmax[0]<10&&SRmax[1]<10) Tgro=2;
		  else if(SRmax[0]<16&&SRmax[1]<16) Tgro=3;
		   else if(SRmax[0]<21&&SRmax[1]<21) Tgro=4; 
			else if(SRmax[0]<27&&SRmax[1]<27) Tgro=5;
			 else if(SRmax[0]<36&&SRmax[1]<36) Tgro=6;
			  else if(SRmax[0]<50&&SRmax[1]<50) Tgro=7;    
			   else if(SRmax[0]<101&&SRmax[1]<101) Tgro=8;	
			   
	   }
		
		for(i=0;i<SS;i++) 
		{
		 if(Tgro==1) {Kvs[i]=30;Kv=30;}
		  else if(Tgro==2) {Kvs[i]=26;Kv=26;}
		   else if(Tgro==3) {Kvs[i]=23;Kv=23;}
			else if(Tgro==4) {Kvs[i]=18;Kv=18;}
			 else if(Tgro==5) Kv=15;
			  else if(Tgro==6) Kv=10;
			   else if(Tgro==7) Kv=7;
				else if(Tgro==8) {Kv=5;}
		}
	  } 
	
	
	  if(Tslipk<20&&((SRmax[0]>25&&SRmax[1]<8)||(SRmax[1]>25&&SRmax[0]<8)))
	   for(i=0;i<SS;i++) 
	   {
		if(Kvs[i]>16&&Diagnose_Data[i+1]==0) {Kvs[i]=17; Kv=17;}
	   }
	
		  Kvh_old=Kv;Kvq_old=Kv;
		  Kvq=Kv;Kvh=Kv;
		  
	   if(Kv<19&&Tslipk<16&&Tslip<30) Jk_gd=0;	 
	 }
	}
	
	
	  Kv=(Kvq+Kvh)>>1;
	
	   Vsmin=Vwo; 
	   Vmin=64000; 
	   Vsmax_st=0;
	   Tvs1=0;
		   
	   for(i=0;i<SS;i++) 
	   {
		//////////////////2008.6.5
		if(Tcjy[i]>6000) {Tcjy[0]=Tcjy[0]>>1;Tcjy[1]=Tcjy[1]>>1;}
		if(i<2)
		 { if(st[i]==3) {Tcjy[0]++;Tcjy[0]++;} else {Tcjy[1]++;}
		 }
		//////////////////2008.6.5
	
		if(WheelBadFlag[i]==1) Tgr[i]=1;   //2006.2.23
	
		if(Tawj1[i]<100) 
		{	
		 if(((st[i]==3)||(st[i]==0)||(DI[i]==0))&&Awr[i]>-1100&&Vw[i]>1300)
		  {
			Ttawj1[i]++; if(Ttawj1[i]>1) {Tawj1[i]++;Ttawj1[i]=0;}
		  }
		  else Tawj1[i]=0; 
		 
		 if(Tawj1[i]>98) 
		  {
			Tawj1[i]=101;Tawj[i]=100;Tslip=13;
	/*			
			Tgro=0;
			for(j=0;j<4;j++) 
			 {
			   if(Tawj1[j]>100) Tgro=Tgro+1;
			 }
			 
			if(Tgro<2) Tgr[i]=1;	//2007.1.27
	*/		}
	
		 if(Kgvw[i]<100&&Vw[i]>1300)
		  {if((st[i]==3||st[i]==0)&&Kgvw[i]>90&&Aw[i]>-480) {Tawj[i]=100;Tawj1[i]=101;Tslip=15;}}
	
		}
		
	   ///////////////////////////////////////////////////////////////////////Modify on 07.10.25	
	   if(Tawj2[i]<100) 
		{	
		 if(Vw[i]>1300&&SR[i]<7&&Aw[i]>-1100)
		 {
		  if((SR[i]>-9&&Vw[i]>5500)||(SR[i]>-10&&Vw[i]<5501)||(SR[i]>-11&&Vw[i]<4000))
		   {Ttawj[i]++; if(Ttawj[i]>3) {Tawj[i]++;Ttawj[i]=0;Tawj2[i]++;}
		   }
		   else {Tawj[i]=0;Tawj2[i]=0;}
		 
		  if(Tawj[i]>98) 
			{
			  Tawj[i]=100;Tawj1[i]=101;Tslip=14;
	/*
			 Tgro=0;
			 for(j=0;j<4;j++) 
			  {
				if(Tawj[j]>98) Tgro=Tgro+1;
			  }
			 
			 if(Tgro<2) Tgr[i]=1;	 //2007.1.27
	*/		  
			}		 //2005.2.22.17.52
		 }
		 else {Tawj[i]=0;Tawj2[i]=0;}
		}
		else
		{
		 if(Tawj2[i]<1000)
		  {
			Tawj2[i]++;
			if(Vsck[i]<Vssou)
			 {
			   Tslip=19;//Tgr[i]=1; 	 2007.1.27
			 }
			 else 
			 {
			   Tslip=100;//Tgr[i]=1;	  2007.1.27
			 }
	
			//if(Aw[i]<-600) Tslip=17; 
	
			if(Aw[i]<-700&&Vw[i]<Vs) {Tawj1[i]++; Tslip=17; Vsck[i]=Vs_st[i];}	//-600 ---> -700,2007.10.27
			   else  Tawj1[i]=101;
			   
			  if(Tawj1[i]>105) {Tawj[i]=0;Tawj1[i]=0;Tslip=18;Tawj2[i]=0;}
	
			if(Vsck[i]>Vssou) { Vsck[i]=Vssou;}
			//if(Aw[i]<-200&&SR[i]>-10) { if(Tawj10[i]<10) Tawj10[i]++;} else Tawj10[i]=0; //2007.10.27
			 if(Aw[i]<-660&&SR[i]>2) { if(Tawj10[i]<10) Tawj10[i]++;} else Tawj10[i]=0;
			  if(Tawj10[i]>6) {Tawj[i]=0;Tawj1[i]=0;Tslip=20;Tawj10[i]=0;Tawj2[i]=0;}
	
			 Tgro=0;
			 for(j=0;j<4;j++) 
			  {
				if(Tawj[j]>98) Tgro=Tgro+1;
			  }
			 
			 if(Tgro<2&&Vw[i]>Vs) Tgr[i]=1;    //2008.1.15
			
		  }
		  else {Tawj[i]=0;Tawj1[i]=0;Tawj2[i]=0;}
		}  
		
		if(Tgr[i]==0)
		 {
			if(Vw[i]>Vsmin) { Vsmin=Vw[i];} 
			if(Vw[i]<Vmin) { Vmin=Vw[i];}  
			if(Vs_st[i]>Vsmax_st) Vsmax_st=Vs_st[i];	 
	
		  if(st[0]==0||st[1]==0)
			{
			  if(SR[i]<5&&Kvs[i]<8&&Tslipk>120)  Tvs1++;
			   else {if(ZY3[i]>0&&Kvs[i]<13&&Tslipk>100) Tvs1++;}
			}
		   else
			{if(ZY3[i]>1&&Kvs[i]<10) Tvs1++;}			  //14-->8:2007.1.27
		 }
	   } 
	
	   if(Vsao>Vsmin) SRmaxo[3]=(signed int)(100-(100*Vsmin)/Vsao); 
		  else SRmaxo[3]=(signed int)((100*Vsao)/Vsmin-100);				   
	
	   if(Vsmin==Vwo) SRmaxo[3]=100;
	
	   Tckg=0;
	
	   if(Vs==Vwo) Tquit++; 	  //2007.12.6
	   if(Tquit>350) Tckg=26;
	
	   if((st[0]==0||st[1]==0)) 				//2006.4.23
		{
		  if(Vs>(Vsao+200)&&Tksjz>100) Tckg2++; else Tckg2=0;
		}
		else
		{
		 if(Vs>(Vsao+200)&&Tksjz>100) Tckg2++; else Tckg2=0;				//2006.4.23
		}
	
	   if(Tckg2>10) Tckg=26;
	
		if(SRmaxo[3]<5&&Tksjz<200&&SRmaxo[2]<200) 
		 {
		  SRmaxo[2]++; 
		  if(SRmaxo[2]>160) 
		   {
			Tckg=26; 
			
			Tcpd++; 
			if(Tcpd>4)
			 {
			  if(Tckg3==1)	{Nocontrols[0]=Nocontrols[0]+1;}
			  if(Tckg3==2)	{Nocontrols[1]=Nocontrols[1]+1;}
			  if(Tckg3==3)	{Nocontrols[2]=Nocontrols[2]+1;}
			  if(Tckg3==4)	{Nocontrols[3]=Nocontrols[3]+1;}	
			  if(Tckg3==11) {Nocontrols[0]=Nocontrols[0]+1;Nocontrols[1]=Nocontrols[1]+1;}
			  if(Tckg3==12) {Nocontrols[0]=Nocontrols[0]+1;Nocontrols[2]=Nocontrols[2]+1;}
			  if(Tckg3==13) {Nocontrols[0]=Nocontrols[0]+1;Nocontrols[3]=Nocontrols[3]+1;}
			  if(Tckg3==14) {Nocontrols[1]=Nocontrols[1]+1;Nocontrols[2]=Nocontrols[2]+1;}
			  if(Tckg3==15) {Nocontrols[1]=Nocontrols[1]+1;Nocontrols[3]=Nocontrols[3]+1;}
			  if(Tckg3==16) {Nocontrols[2]=Nocontrols[2]+1;Nocontrols[3]=Nocontrols[3]+1;}
			  Tcpd=0;
			 }
		   }
		 }
		 else 
		  {SRmaxo[2]=0;  if(Tckg==26) Tcpd++; else Tcpd=0;}
	   
	   if(Tvs1>1) Tckg=26; 
		
	
	   Vmax=Vsmin;
	   Vs2=Vwo; 
	   Tvs1=0;
	   Tgro=Tgr[0]+Tgr[1]+Tgr[2]+Tgr[3];
	   if(Tgro<4) {Vssou=(unsigned int)(Vwo);}
	   
	   for(i=0;i<SS;i++) 
	   {
		if(Tgr[i]==0)
		{
		 if(Vw[i]==Vmax) Tvs1++;
		 if(Vw[i]>Vs2&&Vw[i]<Vmax) Vs2=Vw[i]; 
		} 
		if(Tgr[i]==0&&Vsck[i]>Vssou) Vssou=(Vsck[i]);	//2005.12.20	 
	   }
	   
	   if(Tvs1>1) { Vs2=Vmax; } //2008.7.10, if(Tvs1>1) { Vs2=(Vmax+Vs2)>>1; } 
	   Vsso=(long int)(Vssou); 
	
	   Vsmin=(Vmax+Vs2)>>1; 	 //2008.7.10
	   for(i=0;i<SS;i++)   //2008.1.15
	   {
		 if(Vw[i]<Vsmin) Vmax_SR[i]=(signed int)(100-(100*Vw[i])/Vsmin);
		  else {if(Vw[i]>Vsmin) Vmax_SR[i]=(signed int)((100*Vsmin)/Vw[i]-100); else Vmax_SR[i]=0;}
	   }
	   
	   Vsmin=Vmax;				//2008.7.10
	   Vso_m2=0;	 Tvs1=0; 
	   for(i=0;i<SS;i++) 
	   {
		if(Tgr[i]==0)
		{
		 if(Vs_st[i]==Vsmax_st) Tvs1++;
		 if(Vs_st[i]>Vso_m2&&Vs_st[i]<Vsmax_st) Vso_m2=Vs_st[i];
		} 
	   }
	   if(Tvs1>1) { Vso_m2=Vsmax_st; }
	   
	   
	   if(Vso_m2<4000) Vso_zj=(Vso_m2)>>5; else Vso_zj=125; 
	   
	   Vso_max=(long int)(Vso_m2-Vso_zj);
	
	   //Vs10=0; 
	   Tvs1=0;
	   Vs1=Vwo;
	   
	   Tgrotv=0;
	   Tgro=0;
	   for(i=0;i<SS;i++) 
	   {
		//if(Tgr[i]==0) {Vs10=Vs10+Vw[i];Tvs1++;}
		if(Vw[i]>Vs1&&Vw[i]<Vs2) Vs1=Vw[i]; 
		if(Vmax_SR[i]>-10&&Vmax_SR[i]<6) {if(Awr[i]>-800) Tgrotv++;} //2008.1.15
	
	//////////////////////////////////////////////*/2008.1.22	 
		Vsr=(Vw[i]-Vw2[i]);
		if((Vw[i]>1300)&&(Vsr>-90)&&(Vsr<90)) {Tgrovw2[i]++; }
		 else 
		 {
			if(Tgrovw2[i]<41) {Tgrovw2[i]=0; }
			  else 
			   {Tgrovw2[i]++; 
				if(Tgrovw2[i]>80) { if(Vw[i]<Vs) Tgrovw2[i]=0;}
			   }
			Vw2[i]=(Vw[i]+Vw2[i])>>1;
		 }
		
		if(Tgrovw2[i]>40) {if(Vw[i]>Vs) Tgro++; if(Tgrovw2[i]>80) Tgrovw2[i]=81;}
	///////////////////////////////////////////////*/	 
	   }
	
	   if(Tgro==1) Tvmaxsr1=65; else Tvmaxsr1=0;
	   
	   Tgro=Tgr[0]+Tgr[1]+Tgr[2]+Tgr[3];
	   
		if(Tgro==3) Vs1=Vmax; 
		 else if(Tgro==2) Vs1=Vs2; 
		  else Vs1=(Vs1+Vs2)>>1; 
	
	   if(Vmax>(Vsso+(long int)(Vs_w))) 
		{
		 Vmax=Vsso+(long int)(Vs_w); 
		 if(Vs2>Vmax) Vs2=Vmax-150; 
		} 
	
	   if(Vmax<Vwo) Vmax=Vwo;
					
	
	 
	///////////////////////////2005.12.23
	
	   Vs3=Vmax;
	//	 if(Vsso<9000) Vs3=(Vs3)>>4; else if(Vsso<13000) Vs3=(Vs3)>>5; else Vs3=(Vs3)>>6;
		  if(Vsso<8000) Vs3=(Vs3)>>5; else Vs3=250;   //  11111,347 --> 8000,250	2006.2.23  
		  //if(Vsso<11111) Vs3=(Vs3)>>4; else Vs3=694; 
	   Vmax=Vmax-Vs3;
	   
	/////////////////////////////2006.4.8
		 if(Vw[0]>Vw[3]) 
		  {
		   if(Tgr[0]==0) Vmax2[0]=Vw[0]; 
			 else 
			 if(Tgr[3]==0) Vmax2[0]=Vw[3]; 
			   else Vmax2[0]=Vstq-Kv-3; 	 //2006.4.4
		  }
		  else
		  {
		   if(Tgr[3]==0) Vmax2[0]=Vw[3];
			 else 
			 if(Tgr[0]==0) Vmax2[0]=Vw[0]; 
			   else Vmax2[0]=Vstq-Kv-3; 
		  }
	
		 if(Vw[1]>Vw[2]) 
		  {
		   if(Tgr[1]==0) Vmax2[1]=Vw[1]; 
			 else 
			 if(Tgr[2]==0) Vmax2[1]=Vw[2];
			   else Vmax2[1]=Vstq-Kv-3; 
		  }
		  else
		  {
		   if(Tgr[2]==0) Vmax2[1]=Vw[2];
			 else 
			 if(Tgr[1]==0) Vmax2[1]=Vw[1]; 
			   else Vmax2[1]=Vstq-Kv-3; 
		  }
	
		 if(Vmax2[0]>Vmax2[1])
		 {
		  if(Vstq<5000) Vs3=(Vmax2[0])>>5; else Vs3=156;   //  Vsso --> Vstq	2006.4.4  
		  Vmax2[0]=Vmax2[0]-Vs3;
		 }
		 else
		 { if(Vstq<5000) Vs3=(Vmax2[1])>>5; else Vs3=156;	//	Vsso --> Vstq	 2006.4.4  
		  Vmax2[1]=Vmax2[1]-Vs3;
		 }
		  
		  if(Tgr[0]==0&&Vmax2[1]>Vmax2[0]) 
		  { Vsr=Vmax2[0]>>3;
			if(Vmax2[1]<(Vmax2[0]+Vsr)&&Tgr[1]==0) 
			  {Vmax2[0]=(Vmax2[0]+Vmax2[1])>>1;Vmax2[0]=(Vmax2[0]+Vmax2[1])>>1;}
			  else	
			  { Vsmin=(Vsr)>>1; Vsmin=Vsr+Vsmin;
				if(Vmax2[1]<(Vmax2[0]+Vsmin)&&Tgr[1]==0)
				 {Vmax2[0]=(Vmax2[0]+Vmax2[1])>>1;}
			  }  
		  }
		  
		  if(Tgr[1]==0&&Vmax2[0]>Vmax2[1]) 
		  { Vsr=Vmax2[0]>>3;
			if(Vmax2[0]<(Vmax2[1]+Vsr)&&Tgr[0]==0) 
			  {Vmax2[1]=(Vmax2[0]+Vmax2[1])>>1;Vmax2[1]=(Vmax2[0]+Vmax2[1])>>1;}
			  else	
			  { Vsmin=(Vsr)>>1; Vsmin=Vsr+Vsmin;
				if(Vmax2[0]<(Vmax2[1]+Vsmin)&&Tgr[0]==0)
				 {Vmax2[1]=(Vmax2[0]+Vmax2[1])>>1;}
			  }  
		  }
			
		Tgro=Tgr[0]+Tgr[1];
		if(Tgro==0)
		{
		 if(Vmax2[0]>Vmax2[1]) { Vsr=Vmax2[0]; Vmax2[0]=Vmax2[1]; Vmax2[1]=Vsr;}  //2006.4.9
		}
		else 
		if(Tgro==1)
		 {
		  if(Tgr[1]==0) {Vmax2[0]=Vmax2[1];} else { Vmax2[1]=Vmax2[0];}
		 }
	
		if(Vmax2[1]>Vstq)
		   { Vsr=(Vstq)>>3;
			 if(Vmax2[1]<(Vstq+Vsr)) {Vmax2[0]=(Vmax2[1]+Vmax2[0])>>1;Vmax2[0]=(Vmax2[1]+Vmax2[0])>>1;}
			  else	
			  { V10=(Vsr)>>1; Vsr=Vsr+V10;
				if(Vmax2[1]<(Vstq+Vsr))
				 {Vmax2[0]=(Vmax2[1]+Vmax2[0])>>1;}
			  }  
		   }
	
	   if(Vsck[0]>Vsck[1]) Vs3=(long int)(Vsck[0]); else  Vs3=(long int)(Vsck[1]);
	   
	  if(Tgr[0]==0||Tgr[1]==0)
	  {
	   if(Vmax2[0]>(Vs3+(long int)(Vs_w))) 
		{
		 Vmax2[0]=Vs3+(long int)(Vs_w); 
		} 
	
		if(Tslipk<20) Vsr=Vs_w>>1; else Vsr=0;
		if(Vmax2[1]>(Vs3+(long int)(Vs_w+Vsr))) 
		{
		 Vmax2[1]=Vs3+(long int)(Vs_w+Vsr); 
		} 
	  }
	  else 
	  {if(Vmax2[0]>(Vsso+(long int)(Vs_w))) {Vmax2[0]=Vsso+(long int)(Vs_w); Vmax2[1]=Vmax2[0];}}
		
	/////////////////////////////2006.4.8
	
	   if(Vsmax_st==Vpmaxo_st)
		 { if(Tvpmaxo<55) Tvpmaxo++; }
		 else {Tvpmaxo=0;Vpmaxo_st=Vsmax_st;}
	
	///////////////////////////////////////////2006.4.9
	   Vst_max=Vwo;
	   Tgro=Tgr[0]+Tgr[1];
	   
	   if(Tvpmaxo<50&&Tslipk<15)
	   {
		if(Vs_st[1]>Vs_st[0]) 
		 {
		  if(Tgr[1]==0) Vst_max=(long int)(Vs_st[1]); 
		   else if(Tgr[0]==0) Vst_max=(long int)(Vs_st[0]);
		 } 
		 else 
		 {
		  if(Tgr[0]==0) Vst_max=(long int)(Vs_st[0]);
		   else if(Tgr[1]==0) Vst_max=(long int)(Vs_st[1]);
		 }
		  
		if(Vmax2[1]>Vst_max)
		   { Vsr=Vst_max>>3;
			 if(Vmax2[1]<(Vst_max+Vsr)) {Vst_max=(Vmax2[1]+Vst_max)>>1;Vst_max=(Vmax2[1]+Vst_max)>>1;}
			  else	
			  { V10=(Vsr)>>1; Vsr=Vsr+V10;
				if(Vmax2[1]<(Vst_max+Vsr))
				 {Vst_max=(Vmax2[1]+Vst_max)>>1;}
			  }  
		   }
		
	   }
	   else if(Tgro==0)
	   {
		
		if(Vs_st[1]>Vs_st[0]) 
		 {Vst_max=(long int)(Vs_st[0]);Vsmin=(long int)(Vs_st[1]);} 
		  else {Vst_max=(long int)(Vs_st[1]);Vsmin=(long int)(Vs_st[0]);}
		
		if(Vsmin>Vstq)
		   { Vsr=Vstq>>3;
			 if(Vsmin<(Vstq+Vsr)) {Vst_max=(Vsmin+Vst_max)>>1;Vst_max=(Vsmin+Vst_max)>>1;}
			  else	
			  { V10=(Vsr)>>1; Vsr=Vsr+V10;
				if(Vsmin<(Vstq+Vsr))
				 {Vst_max=(Vsmin+Vst_max)>>1;}
			  }  
		   }
		   else
		   Vst_max=Vsmin;
		   
		 if(Vst_max>Vs2) Vst_max=(Vs2+Vst_max)>>1;
	   }
	   else 
	   if(Tgro==1)
	   { if(Tgr[0]==0) Vst_max=(long int)(Vs_st[0]); else Vst_max=(long int)(Vs_st[1]);
	   } 
	
		if(Tvmaxsr1>30&&Vst_max>Vs2) Vst_max=Vs2;  //2008.1.15
			
		if(Tgro<2)
		{  
		   //if(Vmax2[0]>Vstq&&Vstq>1200&&Vmax2[0]<(Vsao+6000)&&(Vpmaxo_2!=Vmax2[0]))  
		   if(Vst_max>Vstq&&Vstq>1200&&Vst_max<(Vsao+6000)&&(Vpmaxo_2!=Vst_max))	   
		   {
			Vpmaxo_2=(Vst_max+Vpmaxo_2)>>1;
			Vflag2++;
		   } 
		   else 
		   {
			 Vflag2=0;
		   }
		}
	
		if(Vflag2>3)
		  {
			Vflag2=0;
	
		   if(Vst_max>Vstq&&Tslipk<20) 
		   { Vsr=Vstq>>3;
			 if(Vst_max<(Vstq+Vsr)) Vstq=(Vstq+Vst_max)>>1;
			  else	
			  { Vsmin=(Vsr)>>1; Vsmin=Vsr+Vsmin;
				if(Vst_max<(Vstq+Vsmin))
				 {Vsmin=(Vstq+Vst_max)>>1; Vstq=(Vstq+Vsmin)>>1;}
			  }  
		   }
	
			if(Tgro<2)
			{
			 if(Vstq<Vmax2[0]) 
			  {Vstq=(Vstq+Vmax2[0])>>1; if(Tslipk<10) Vstq=(Vstq+Vmax2[0])>>1;}
			  else
			  {if(Tgro==0&&Vstq<Vmax2[1]) Vstq=Vstq+30;}
			} 
		  }
	
	//////////////////////////////////////////////2007.1.27
	
	   Tgro=0;Tkgo=0;
	   for(i=0;i<SS;i++)  
		{
		 if(Vsso>3000&&Tgr[i]==0)
		 {	 
		  if(SRmax[i]>50) Tgro++;	  
		   if(SR[i]<-5) Tkgo++; 	
		 }
		}  
	//////////////////////////////////////////////2006.4.9 
		 
		if(Tvpmaxo<50)
		{ 
		 if(Tslip<35&&Tslipk<25) {Vs3=Vmax;}
		  else if((Tgro>2)||(Tgro>1&&Tkgo>0)) 
		   { Vs3=(long int)(Vsmax_st);
			 if(Vs3<Vs2) Vs3=Vs2;
			  else 
			  if(Vst_max<Vs3) Vs3=(Vst_max+Vs2)>>1;
			   else Vs3=Vs2;	   
		   }  
		   else 
			{
			  Vs3=(long int)(Vsmax_st); 
			  if(Vs2<Vs3) Vs3=Vs2; else Vs3=(Vs3+Vs2)>>1;
			}
		}
		else { Vs3=(long int)(Vsmax_st); if(Vs2<Vs3) Vs3=Vs2; else Vs3=(Vs3+Vs2)>>1;}
	  
	   if(Tvmaxsr1>40) Vs3=Vs2;  //2008.1.15
	   
	   if(Vs1>Vs2) Vs1=Vs2;
	
		 if(Vs<Vso_max&&Vs>1200) 
		  {
			Tvso_max++;
		  } else Tvso_max=0;
		  
		 if(Tvso_max==2)
		  {Tvso_max=0;
		   if(Vstq<Vso_max)  {if(Tslipk<15&&Vstq<Vmax2[1]) Vstq=(Vso_max+Vstq)>>1;}
		   if(Vsth<Vso_max)  Vsth=(Vso_max+Vsth)>>1;
		  } 
	
	   if(Vstq<8000) Vsr=(Vst_max)>>5; else Vsr=250;   //  Vsso --> Vstq	2006.4.4  
	   Vst_max=Vst_max-Vsr;
	
	   Tgro=0;
	   for(j=0;j<SS;j++)
	   {
		if(Nocontrols[j]<4) {if(SR[j]<4) Tgro=Tgro+Tawj[j];} else Tgro=Tgro+100;  //2007.10.27 2011.12.31
	   }
	   
	   if(Tgro>390) {Tckg=26; } 		 //300 ;;>400 :  2007.1.26
	
	   Tgro=0;
	   for(j=0;j<SS;j++)
	   {
		if(Nocontrols[j]<4) {if(SR[j]<4) Tgro=Tgro+Tawj1[j];} else Tgro=Tgro+100;	//2007.1.27
	   }
	   
	   if(Tgro>390) {Tckg=26; } 		 //300 ;;>400 :  2007.1.26
	
		if(Tvnold[0]>18&&Vw[0]>Vs3) 
			{Vs3=(Vw[0]+Vs3)>>1;Vs2=(Vs3+Vs2)>>1;Vsck[0]=(Vsck[0]+Vs_st[0])>>1;}
		if(Tvnold[1]>18&&Vw[1]>Vs3) 
			{Vs3=(Vw[1]+Vs3)>>1;Vs2=(Vs3+Vs2)>>1;Vsck[1]=(Vsck[1]+Vs_st[1])>>1;}
		if(Tvnold[2]>20&&Vw[2]>Vs3) 
			{Vs3=(Vw[2]+Vs3)>>1;Vs2=(Vs3+Vs2)>>1;Vsck[2]=(Vsck[2]+Vs_st[2])>>1;}
		if(Tvnold[3]>20&&Vw[3]>Vs3) 
			{Vs3=(Vw[3]+Vs3)>>1;Vs2=(Vs3+Vs2)>>1;Vsck[3]=(Vsck[3]+Vs_st[3])>>1;}
	
	   Tgro=Tgr[0]+Tgr[1]+Tgr[2]+Tgr[3];
	
		if(Tgro<4)
		{
		   if(Vs3>Vs&&Vs>1200&&Vs3<(Vsao+6000)&&(Vpmaxo!=Vs3))	  
		   {
			
			Vpmaxo=Vs3;
			Vpmax[Vflag]=Vs3;				
	
			Vflag++;
		   } 
		   else 
		   {
			 Vflag=0;
		   }
		}
		else 
		{
		   Tckg=26;  
		}
		
		if(Vflag>3)
		  {
		   Vflag=0;
		   Vsq=min(Vpmax[0],Vpmax[1],Vpmax[2],Vpmax[3]);
	
			  if(Vstq<Vmax2[0])  Vstq=(Vmax2[0]+Vstq)>>1;
			   else if(Vstq<Vmax2[1])  Vstq=Vstq+20;
			   
			  if(Vsth<Vsq)	Vsth=(Vsq+Vsth)>>1;
	
		  if(Tslipk<15)  //2006.1.8  Kv<5
		   {
			  if(Vstq<Vsq&&Tslip<30&&(Kgvw[2]<20&&Kgvw[3]<20))	Vstq=(Vsq+Vstq)>>1;  //2006.5.21
				
				  if(Tkgo>1) 
				   { 
					if(Vstq<Vmax2[0])  Vstq=(Vmax2[0]+Vstq)>>1;
					if(Vsth<Vs2)  Vsth=(Vs2+Vsth)>>1;
				   }
				//	 else
				  // { if(Vstq<Vsq&&Kv<20)	Vstq=(Vsq+Vstq)>>1;}
		   }
		   
		  }
	
		   for(i=0;i<SS;i++)		  //2005.11.30
			{
			 if(Tgr[i]==0) Kvq=(Kv+Kvs[i])>>1;
			}
	
		   for(i=0;i<SS;i++)		  //2005.11.30
			{
			 if(Tgr[i]==0) Kvq=(Kvq+Kvs[i])>>1;
			}
	
	
		   for(i=0;i<SS;i++)		  //2005.11.30
			{
			 if(Tgr[i]==0)
			 {
			  if(Vs_st[i]>Vsck[i])
			  {Vsck[i]=(Vsck[i]+Vs_st[i])>>1;Vsck[i]=(Vsck[i]+Vs_st[i])>>1;}
			 } 
	
			 if(Tgr[i]==1) Kvs[i]=Kvq+1;		 
			}
	
		   Kvq=(Kvs[0]+Kvs[1])>>1;
		   Kvh=(Kvs[2]+Kvs[3])>>1;
	
		   Kv=(Kvq+Kvh)>>1;
	
		   if(DKF!=0||Tkc[0]>1||Tkc[1]>1||Tkc[2]>1||Tkc[3]>1)		 //2005.11.29
		   {
			if(Tslipk>30)
			{
			 Kvmax=Kvq+6;
			 Vsth=Vsth-Kvh-6;
			}
			else
			{
			 Kvmax=Kvq+5;
			 Vsth=Vsth-Kvh-5;
			}
		   }
		   else
		   {
			if(Tslipk>30)
			{
			 Kvmax=Kvq+6;
			 Vsth=Vsth-Kvh-6;
			}
			else if(Tslipk>20)
			{
			 Kvmax=Kvq+5;
			 Vsth=Vsth-Kvh-5;
			}
			else if(Tslipk>15)
			{
			 Kvmax=Kvq+4;
			 Vsth=Vsth-Kvh-4;
			}
			else if(Tslipk>11)
			{
			 Kvmax=Kvq+3;
			 Vsth=Vsth-Kvh-3;
			}
			else if(Tslipk>7)
			{
			 Kvmax=Kvq+2;
			 Vsth=Vsth-Kvh-2;
			}
			else if(Tslipk>3)
			{
			 Kvmax=Kvq+1;
			 Vsth=Vsth-Kvh-1;
			}
			else
			{
			 Kvmax=Kvq;
			 Vsth=Vsth-Kvh;
			}
		   }
	
		   Vstq=Vstq-Kvmax;
	
			if(Vstq<Vwo) Vstq=Vwo;
			if(Vsth<Vwo) Vsth=Vwo;
	
		   Vs=(Vstq+Vsth)>>1;
	
	   Tgro=Tgr[0]+Tgr[1]+Tgr[2]+Tgr[3];
	   if(Tgro<4) Vssou=Vwo; else Vssou=Vssou-Kv;
	
	   for(i=0;i<SS;i++)	  //   2005.11.25
	   {
		 if(Tgr[i]==0) Vsck[i]=Vsck[i]-Kvs[i]; else { if(Vsck[i]>100) Vsck[i]=Vsck[i]-Kv-5; }
		 if(Vsck[i]<Vwo) Vsck[i]=Vwo;
		 if(Tgro<4) {if(Tgr[i]==0&&Vsck[i]>Vssou) Vssou=Vsck[i];}	//2005.12.20
	   }
	   
	   Vsso=(long int)(Vssou);
	   
	/////////////////////////////////////////////////////////////////
	
	
	  if(Vsao>9000)  
	  {
	 //  if(Kv<20) V10=(long int )(37*Kv+1260);  //(1500,3000)	//2006.1.9
	   //  else V10=(long int )(38*Kv+1360);  //(1500,3000)  //////V10=(long int )(38*Kv+1560); 05.12.30
	   if(Tslipk<10)
		{ if(Tkcn[0]<2&&Tkcn[1]<2&&Tkcn[2]<2&&Tkcn[3]<2) V10=1350; else V10=(long int )(20*Kv+1200);
		}
		else 
		if(Tslipk<16)
		 { if(Tkcn[0]<1&&Tkcn[1]<1&&Tkcn[2]<1&&Tkcn[3]<1) V10=1350; else V10=(long int )(25*Kv+1200);
		 }
		 else V10=(long int )(30*Kv+1200);
		
	   if(V10>2200) V10=2200; 
	  }
	  else if(Vsao>7000)
		V10=1420;
		 else if(Vsao>5000)
		  V10=1370; 
		   else V10=1320;
	
	  if(Ttjs>3)
		{ 
			if(Kv>24) {V10=(long int )(60*Kv+1500);}  //2009.10
			 else {if(Kv>18) {V10=(long int )(35*Kv+1300);}}  //2009.10
			if(V10>2700) V10=2700;
		}
	   
	  if(Kv>20) 
	   {
		V30=V10+200;  
		Vsr=V10+250;				 //(1400,3000)
	   }
	   else  if(Kv>15) 
	   {
		V30=V10+100;  
		Vsr=V10+200;				 //(1400,3000)
	   } 
	   else  if(Kv>10) 
	   {
		V30=V10+50;  
		Vsr=V10+150;				 //(1400,3000)
	   }
	   else  if(Kv>5) 
	   {
		V30=V10+20;  
		Vsr=V10+100;				 //(1400,3000)
	   } 
	   else 
	   {
		V10=1270;
		V30=V10+10;  
		Vsr=V10+50; 				//(1400,3000)
	   } 
	
	   Tgro=0;
	   //if(Tkc[0]>0&&Tkc[1]>0&&Kv>25&&Vs<(Vsr+200)) Tgro=1;
	   if(Tslipk<15&&Tkc[0]>0&&Tkc[1]>0&&Kv>23&&Vs<(Vsr+100)) Tgro=1;
		 else if(Tslipk>14&&Kv>23&&Vs<(Vsr+100)) Tgro=1;
	
	   if(Tgro==1&&Vw[0]<Vsr&&Vw[1]<Vsr&&Vw[2]<Vsr&&Vw[3]<Vsr)	//Kv>10, 2005.12.16
	   {   for(i=2;i<SS;i++)  
			{  DI[i] = 0;	Val_Act(i);
			  WheelBadFlag[i]=1;
			}
		   End_Break1=1;
		   //ctrlflag=0;  
			
	
		   EBk1= (unsigned char) (Kv/5);
		   Tstep1=1; 
		   if(EBk1<2) EBk1=2;
	   } 
	
	   if(Vs>3000&&Vw[0]<Vwo&&Vw[1]<Vwo&&Vw[2]<Vwo&&Vw[3]<Vwo)
	   {
		   Tckg=26;  
	   }  
	   
		Tgro=0;
		if(Kv>15&&(Tkc[0]>0||Tkc[1]>0)) Tgro=1;   
		if((Tgro==1&&Vs<Vsr&&Vw[0]<V10&&Vw[1]<V10&&Vw[2]<V10)||
		   (Tgro==1&&Vs<Vsr&&Vw[0]<V10&&Vw[1]<V10&&Vw[3]<V10)||
		   (Tgro==1&&Vs<Vsr&&Vw[0]<V10&&Vw[2]<V10&&Vw[3]<V10)||
		   (Tgro==1&&Vs<Vsr&&Vw[1]<V10&&Vw[2]<V10&&Vw[3]<V10))
		  {
			Tckg1++;  if(Tckg1>19) Tckg=20; 
		  } 
	   
	   Tgro=0;
	   for(i=0;i<SS;i++)  
		{
		 //if(Vw[i]<V30&&Vs<V10) Tgro++;  
		 if((Vw[i]<V30&&Vs<V10)||(Tgr[i]!=0)) Tgro++;	  //2007.12.26	 
		}	
	   if(Tgro==4) {Tckg=17;}
	
	   Tgro=0;
	   for(i=0;i<SS;i++)  
		{ if(Vw[i]<V10&&Vs<V30) Tgro++; 	
		}	
	   if(Tgro==4) {Tckg=18;}
	   if(Switch_Flag==0xa5) // added at 2014.2.23
	   {
		 if(GV_OffRoad_Mode_Flag==0) // added at 2014.2.23
		 {
				ij=0; // added at 2014.3.7
				if(st[0]==3&&SR[0]<1&&Aw[0]>-500&&Vw[0]<3600) //  added at 2014.3.7
				{
					ij++; // added at 2014.3.7
				}
				if(st[1]==3&&SR[1]<1&&Aw[1]>-500&&Vw[1]<3600) //  added at 2014.3.7
				{
					ij++; // added at 2014.3.7
				}
				if(st[2]==3&&SR[2]<1&&Aw[2]>-500&&Vw[2]<3600) //  added at 2014.3.7
				{
					ij++; // added at 2014.3.7
				}
				if(st[3]==3&&SR[3]<1&&Aw[3]>-500&&Vw[3]<3600) //  added at 2014.3.7
				{
					ij++; // added at 2014.3.7
				}
				if(Vw[0]>3900||Vw[1]>3900||Vw[2]>3900||Vw[3]>3900) //  added at 2014.3.7
				{
					ij=0; // added at 2014.3.7
					if(LV_SwitchFlag_ByteCount>0) LV_SwitchFlag_ByteCount--; // added at 2014.3.7
					LV_Count_ij=0; // added at 2014.3.7
				}
			 if(ij>0) //  added at 2014.2.27
			 {
					if(LV_Count_ij==0) LV_Count_ij=1; // added at 2014.3.7
				 if(LV_Count_ij>100||LV_SwitchFlag_ByteCount>10) Tckg=30; // added at 2014.2.23
				 if(LV_SwitchFlag_ByteCount<200) LV_SwitchFlag_ByteCount++; // added at 2014.3.2
			 }
			 if(LV_Count_ij>0&&LV_Count_ij<250) LV_Count_ij++; // added at 2014.2.23
		 }
		 else // added at 2014.2.23
		 {
			 LV_SwitchFlag_ByteCount=0; // added at 2014.2.23
					LV_Count_ij=0; // added at 2014.3.7
		 }
	   }
	
	   if(Tckg>16)
	   {
		   for(i=0;i<SS;i++)  
			 {
				DI[i] = 0;	  //2009.12.4 add
	  //		  Val_Act(i);
				Vs_old[i]=0;
				Tnum[i]=0;
				Tawj[i]=0;Tawj1[i]=0;
				Tbb[i]=0;			
				if(Diagnose_Data[i+1]==0) Tgr[i]=0; else Tgr[i]=1;
			//	  Vw[i] = 0;
				Tjj[i]=0;
				 SRmax[i]=0;
				 SRmaxo[i]=0;
				 Kvs[i]=0;
				 Tkc[i]=0;
				 //Vs=Vwo;		  //2009.10, 2009.11
				 Vw0[i]=0;		//2007.12.6
				 Vw1[i]=Vs; 	 //2007.12.6
				 Vmin=Vs;		 //2007.12.6
				 Tkcn[i]=0; 	 //2007.12.6
				 TStepNum[i]=0;  //2007.12.6
			 }
	
				Tslip=0; Tslipk=0;								 /////////////2005.2.22.17:52
			 
	////////////////////////////////////////////////////
		   //if(Vs2<2600) Ktcontrol=60; //2009.11.16
		   Ktcontrol=0; 
		  
		   ctrlflag=0;	
		   Tgro_ret=0;	//2007.12.2
		   Kvq_old=0;Kvh_old=0;  Kvq=0;Kvh=0;  //2007.1.27
			
		   EBk= (unsigned char) (Kv/5);   
		   Tstep=1; 
	
		   //EBk= (unsigned char) (Kv>>3);	 
		   if(EBk<2) EBk=2; 			  //2006.1.8  2-->1
	
		   if(Vs>5000) {EBk=6;Ktcontrol=60;} //2009.12.5 add
			
		   if(Tckg<25) 
			{ End_Break=1; if(End_Break1==0) {End_Break1=1;EBk1=EBk;Tstep1=Tstep;}} 
			else  { End_Break=0;End_Break1=0; } 		
	
				  V30=V10-100;	  //200
				  Vs1=Vs;
				  Vss1=Vs1; Vss=Vss1;
				  Vs_old[2]=Vs1;
				  Vs_old[1]=Vs1;
				  Vs_old[0]=Vs1;
				  Vss2=Vs1;
				  Vsao=Vs1;
				  Tvmaxsr=0;
				  Tvpmaxo=0;
				  
			   if(ctrlflag==0&&ABS_Valves_Flag==0)
			   {
			 for(i=0;i<SS;i++)	
			 { 
					for(j=0;j<1000;j++)
					{  
					   DI[i]=0; 
					}
				   Val_Act(i);	//2005.12.12
			 } 
	//			   p3=0x00; // ABS Valves Ctrl == 0;
			   }   
			  
			   if(ABS_Lamp_RET_Flag==0)   /// Add at 2004.12.22
				 H_ctrl_Set_Low;
	/*////////////////////////////////////////////// 2010.1.4 add
			if(T_tire[1]>T_tire[0]) {total_a=(signed int)(T_tire[1]-T_tire[0]); total_a=total_a-total_1;}
				 else {total_a=(signed int)(T_tire[0]-T_tire[1]);total_a=total_a+total_1;}
				
				T_tire[0]=0;T_tire[1]=0; 
				T_tire[2]=0;T_tire[3]=0;
				if(total_a>100) 
				 {
			  if(Diagnose_Data[29]==0)
			  {
			   Diagnose_Write(29,0x0800);
			   Error_Flash[31]=0x0078;		   // Flash Code	
			   Error_Flash[0]=0x0078;		  // Current Error Flash Code
			   Diagnose_Data[0]=0x4f08;   // Current Error
			   unctrl=1;
			  }
			 } 
	*/////////////////////////////////////////////////
	   }   
	   else
	   {
		 
	  //	 V30=Vs;	   
		   if(Vs>13000)
			{Tbb0=5;
			}
			else if(Vs>9000)
			{Tbb0=5;
			}
			else if(Vs>7000)
			{Tbb0=4;
			}
			else if(Vs>6000)
			{Tbb0=4;
			}
			else if(Vs>5000)
			{Tbb0=3;
			}
			else if(Vs>4000)
			{Tbb0=3;
			}
			else if(Vs>3000)
			{Tbb0=2;
			}
			else if(Vs>2000)
			 {Tbb0=2;
			}
			else  {Tbb0=2;}
	
	
			  if(Kv>28)
				 {T10=80;T11=30;}
			  else if(Kv>23)
				 {T10=90;T11=45;}
			  else if(Kv>15)
				 {T10=100;T11=50;}
			  else if(Kv>8)
				 {T10=110;T11=55;}	
			  else 
				 {T10=120;T11=60;}
				 
			  if(Tkc[2]==0&&Tkc[3]==0) {T10=130;T11=80;} 
	
		   /*
		   SRJY=10;  //12
		   SRZY=11;    //22
		   SRJY1=11;  //8
		   SRZY1=12;	//18
		   */
		   ///*  ///////////////////////////////// Modify at 05.7.24
	
		   SRJY=6;	//12
		   SRZY=7;	  //22
		   SRJY1=6;  //8
		   SRZY1=7;    //18
	
		   //*/  ///////////////////////////////// Modify at 05.7.24
	
	   if(Vs>2000&&Vs<6001)   ////////////// 8000-->6000 /////05.11.8
		{
		 SRJY=12-(unsigned char)(Vs/1200);	///(15,10  :   2000,8000) // 17--> 14 ,05.7.24
		}
	
		if(Vs<2001) 
		{
		 SRJY=42-(unsigned char)(Vs/66);  ///(30,15  :	 1000,2000)  // 45--> 42 ,05.7.24
		}
	
	
		 SRZY=SRJY;  //+1
		 SRZY1=SRZY;
		 SRJY=SRJY+1;
		 SRJY1=SRJY;
	
	
	
	
	
		if(Kv>30)
		{
		 SRJY=SRJY+1;	//2009.5.24  0->1
		 SRJY1=SRJY1+1;
		 SRZY=SRZY+1;
		 SRZY1=SRZY1+1;
		 AA1=AA0; AAr0=AA0+150;
		}else if(Kv>25)
		{
		 SRJY=SRJY+1;
		 SRJY1=SRJY1+1;
		 SRZY=SRZY+2;
		 SRZY1=SRZY1+2;
		 AA1=AA0-11; AAr0=AA0+140;
		}else if(Kv>22)
		{
		 SRJY=SRJY+1;
		 SRJY1=SRJY1+2;
		 SRZY=SRZY+3;
		 SRZY1=SRZY1+3;
		 AA1=AA0-22;AAr0=AA0+120;
		}else if(Kv>18)
		{
		 SRJY=SRJY+3;
		 SRJY1=SRJY1+3;
		 SRZY=SRZY+3;
		 SRZY1=SRZY1+3;
		 AA1=AA0-55;AAr0=AA0+90;
		}else if(Kv>14)
		{
		 SRJY=SRJY+3;
		 SRJY1=SRJY1+3;
		 SRZY=SRZY+3;
		 SRZY1=SRZY1+3;
		 AA1=AA0-80;AAr0=AA0+50;
		}else if(Kv>10)
		{
		 SRJY=SRJY+4;
		 SRJY1=SRJY1+4;
		 SRZY=SRZY+3;
		 SRZY1=SRZY1+3;
		 AA1=AA0-105; AAr0=AA0+10;
		}else if(Kv>6)
		{
		 SRJY=SRJY+4;
		 SRJY1=SRJY1+4;
		 SRZY=SRZY+3;
		 SRZY1=SRZY1+3;
		 AA1=AA0-130; AAr0=AA0-10;
		}else if(Kv>3)
		{
		 SRJY=SRJY+5;
		 SRJY1=SRJY1+5;
		 SRZY=SRZY+3;
		 SRZY1=SRZY1+3;
		 AA1=AA0-155; AAr0=AA0-30;
		}
		else
		{
		 SRJY=SRJY+5;
		 SRJY1=SRJY1+5;
		 SRZY=SRZY+3;
		 SRZY1=SRZY1+3;
		 AA1=AA0-180; AAr0=AA0-50;	
		}
	   
		if(Kv<11)
		{
		 if(Vs>12000)	{SRJY=SRJY+2;SRJY1=SRJY1+1;}
			else if(Vs>8000)	{SRJY=SRJY+1;SRJY1=SRJY1+1;}
		}	
	
		////////////////////////////////////////////////////
		if(Vs>17000)
		{
		 //SRJY=SRJY;
		 //SRJY1=SRJY1;
		 SRZY=SRZY+1;
		 SRZY1=SRZY1+1; 
		}
	
	   if(Vs<7000)
	   { 
		
		if(Vs>6000)
		{
		 AAr=AAr0-40;
		}
		else if(Vs>5000)
		{
		 AAr=AAr0-50;
		}
		else if(Vs>3500)
		{
		 AAr=AAr0-65;
		}
		else
		{
		 AAr=AAr0-80;
		}
	
		if(Vs<3500)
		 { if(Kv>20) 
		   {if(AAr>-1150) AAr=-1150;}		   //2009.11 -950->-1150
			else {if(AAr<AA1) AAr=AA1;} 	 //2009.11 -950->AA1
		 }
	
	   }
	   else AAr=AAr0;
	
		////////////////////////////////////////////////////
	
	   if(Kv>30)
	   { 
		SRby1=50;
	   }					 
	   else 
		if(Kv>26)
		 SRby1=45;
		 else
		 { 
		  SRby1=55-(signed int)(Vs/600);		//( 30,50)
		  if(SRby1<25) SRby1=25;
		 }					   
	
	
		SRby=SRby1; 						 //( 35,55)
	
	/*
	   if(Vs>5000) 
		 { if(Tslip<18) Vsh=Vs-80; else Vsh=Vs-80;} 
		else Vsh=Vs-120;
	*/
	   if(Vs>8000) 
		 { if(Tslipk<18) Vsh=Vs-180; else Vsh=Vs-190;} 
		else Vsh=Vs-210;
	
	/*
	   if(Vs>8000) Vsh=Vsso-500; 
		else if(Vs>5000) Vsh=Vsso-300;
		else if(Vs>3000) Vsh=Vsso-200;
		 else Vsh=Vsso-100;
	*/
	
	   for(i=0;i<SS;i++) 
	   {
		  if(Vw[i]<Vsh)
		  {
			if(i<2)
			{if(st[i]==0) Vsh=Vs+120; else Vsh=Vstq+120;   //  2006.4.10  
			 SR[i]=(signed int)(100-(100*Vw[i])/Vsh);
			 if((Tvnold[i]<-16)&&(SR[i]<20)) {if((Vstq>Vmax)&&(Aw[i]>-1800)) Vstq=(Vmax+Vstq)>>1;}
			} 
			else 
			 {Vsh=Vs+30;  //150 -- 130 2006.1.20
			  SR[i]=(signed int)(100-(100*Vw[i])/Vsh);
			  if((Tvnold[i]<-16)&&(SR[i]<20)) {if((Vsth>Vmax)&&(Aw[i]>-1800)) Vsth=(Vmax+Vsth)>>1;}
			 }
			 
			if(Vw[i]<=Vwo) SR[i]=100; 
		  }
		  else	SR[i]=(signed int)((100*Vsh)/Vw[i]-100);
		  
		  
	 // 	if(SR[i]<0) SR[i]=0;   //2005.12.21
	
		  if(SR[i]>SRmax[i])  {SRmax[i]=SR[i]; Taw200[i]=0; }
	
			if(st[i]!=3&&SR[i]<SRmax[i])
			{ if(Taw200[i]<100) {if(SR[i]>1) Taw200[i]++;} }
	
		 // Awr[i]=(Awr[i]+Aw[i])>>1;	//Awr[i]=(Awr[i]+Aw[i]+Aw[i])/3;	  //2006.1.17
	
		 /////////////////////////////////////////////////////////////
			  if(st[i]!=3)
			   {
				 if(Tzzo[i]>2)
				 {				
				   if(Vw[i]>(Vwo+200)) Tzzo[i]++;
				   
				   if(Tzzo[i]==4)  Vw0[i]=Vw[i];
				  
					if(Tzzo[i]==8) 
				{
				  if(Vw[i]>Vw0[i]) Vw1[i]=(Vw[i]-Vw0[i])>>2; else Vw1[i]=0; 
				  if(Vw1[i]>500) Vw1[i]=500;
				  if(Vw1[i]<0) Vw1[i]=0;
				} 
				 }
				
				 if(Tzzo[i]<3)
				 {	
				  if(Awr[i]<=Vw0[i]) { Vw0[i]=Awr[i]; Tzzo[i]=0;}
				   else 
					{if(Awr[i]>(Vw0[i]+500)) Tzzo[i]++; else Tzzo[i]=0;}
				 }
				 
			   }	
	   }
	
		if(Tgrotv>2)			  //2008.1.15
		 {
			if(Vmax>1350&&Tvmaxsr<80) Tvmaxsr++; 
			if(Tvmaxsr>60&&Vs>Vmax) {Vstq=Vmax;Vsth=Vmax;Tvmaxsr=0;}
		 }
		 else Tvmaxsr=0;
	
	   if(Vs<2000) DKF=0;
	
	   if(Kv>18)  {BB0=500;BB1=450;} else	{BB0=450; BB1=400;}
	
	//	 if(DKF!=0) Tjbn=(unsigned char)(Vs/200+10);
	
	   for(i=0;i<SS;i++) 
	   {
		  if(T130[i]==0) 
		   {
			if(SR_vf[i]>SR[i]) {SR_vf[i]=SR[i]+1;} 
			else {if(SR_vf[i]<SR[i]) {SR_vf[i]=SR[i]-1;} else {SR_vf[i]=SR[i];}}
		   }
			SR_xx=SR_vf[i]-2;SR_sx=SR_vf[i]+2;
			if((Awr[i]>-1800)&&(SR[i]<30))
			 {if((SR[i]<SR_sx)&&(SR[i]>SR_xx))
				 {if(T130[i]<100) T130[i]++; }
			   else {if(T130[i]<31) T130[i]=0;}
			 } else {T130[i]=0;}
	   }
	
	   Vs=Vstq;
	   for(i=0;i<2;i++) 					   ///// ǰ��	 
	   { 
		  Tnum[i]++;	  //2006.1.8
		
		if(st[i]!=0)
		{
	//		if(st[i]!=3) Tcjy[i]++; if(Tcjy[i]>5000) Tcjy[i]=5000;
	//		Vs=Vsq;
	
	
	
			   if(Tksjz>52)
			   {
				if(Tkc[0]>1&&Tkc[1]>1&&Tkc[2]>1&&Tkc[3]>1)
				{
				 if(((Tkcn[0]<2&&Tkcn[1]<2&&Tkcn[2]<2)|| 
					 (Tkcn[0]<2&&Tkcn[1]<2&&Tkcn[3]<2))&&Kv>17) 
				  {
				   if(SR[0]>60&&SR[1]>60)
				   { 
					for(j=0;j<SS;j++)	  
					 { 
					  if(Zp0[j]>2) {Zp0[j]=2;Zp[j]=Zp0[j];}
					   else  {Zp0[j]=1;Zp[j]=Zp0[j];}
					 }	
					 Kvq=(17+Kvq)>>1;Kvh=(17+Kvh)>>1;
				   }  
				  }
				}
			   }
	
			   if(Tkc[i]>1)
				{
				  
				  if(((ZY3[0]>0&&ZY3[1]>0&&ZY3[2]>0)|| 
					  (ZY3[0]>0&&ZY3[1]>0&&ZY3[3]>0))&&Kv<20)
				  {
				   if(st[0]==3&&st[1]==3&&st[2]==3&&st[3]==3)
				   {
	
					   if(Kv<9) {Kvq=16;Kvh=16;}
						 else  if(Kv<20) {Kvq=22;Kvh=22;} 
					  
					for(j=0;j<SS;j++)		   
					{ 
	
					 if(SR[j]<6)
					  {
					   if(Zp0[j]<4) {Zp0[j]=4;Zp[j]=Zp0[j];} else { Zp[j]=Zp0[j];} 
					  }
					  else
					   if(SR[j]<10)
					   {
					   if(Zp0[j]<3) {Zp0[j]=3;Zp[j]=Zp0[j];} else { Zp[j]=Zp0[j];} 
					   }
					} 
				   }
				  }
				}  
	
	//////////////////////////////2009.5.23
	
		   if((Kv>16)&&(Vs>4000)) 
			{
			 if(SRmax[0]>90&&SRmax[1]>90) {Tdjk=2;if(Kvo>18) Kvo=18;}
			  else
			  {if(SRmax[0]>75&&SRmax[1]>75) {Tdjk=1;if(Kvo>21) Kvo=21;}
				else 
				{if(SRmax[0]>60&&SRmax[1]>60) {Tdjk=1;if(Kvo>25) Kvo=25;}	
				}
			  }
			 
			 if(SRmax[2]>60&&SRmax[3]>60) {if(Tdjk==2) {Tdjk=3;if(Kvo>16) Kvo=16;}}
			}
	
			 if(Tvnold[0]>13&&Tvnold[1]>13) {if(Kv>17) {Kvo=17;Tdjk=1;} }
			 if((Tvnold[0]>20)||(Tvnold[1]>20)) {if(Kv>17) {Kvo=17;Tdjk=1;} }
			 if((Tvnold[2]>23)||(Tvnold[3]>23)) {if(Kv>17) {Kvo=17;Tdjk=1;} }
	
			  if(Tdjk>0) 
			  {
			   Tslip=10;Tslipk=8;
			   
			   if(Vs>5500)
			   {
				for(j=0;j<4;j++)
					{
						if(Kvs[j]>Kvo) Kvs[j]=(Kvs[j]+Kvo)>>1;
					}
				
				Kvq=(Kvs[0]+Kvs[1])>>1;Kvh=(Kvs[2]+Kvs[3])>>1;
				Kvh_old=Kvh; Kvq_old=Kvq;
				Kv=(Kvq+Kvh)>>1;
			   }
			  } 
	////////////////////////////////////////////////////////////////////////////////////2006.1.24
	
	////////////////////////////////////2009.9.17
			 if(Tdjk>0) 
				{
				 if((Tvnold[0]>1)||(Tvnold[1]>1)) 
					{
					 if(Tvnold[0]>Tvn) Tvn=Tvnold[0];
					 if(Tvnold[1]>Tvn) Tvn=Tvnold[1];
				 if(Vw[0]>Vs_mid) {if(Tdk==0) Tdk=1; else {if(Tdk==2)Tdk=3;}}
					 if(Vw[1]>Vs_mid) {if(Tdk==0) Tdk=2; else {if(Tdk==1)Tdk=3;}}
					}
				}
				
				if(Tdk>2) 
				{
					if((Tvnold[0]<0)&&(Tvnold[1]<0)) 
						{ 
							Tdjk=0;
							if(Tdk<25) Tdk++; 
							if(Tvn<15)
							 {
							  if(Tdk>20) 
								 {Tvn=0; 
									if((st[0]!=3)&&(st[1]!=3))
										{Tbb[0]=Tbb0+1;st[0]=5; DI[0]=1; SRmax[0]=0; Tbb[1]=Tbb0+1; st[1] = 2; DI[1]=1;T1[1]=10;Tzz[1]=10;}
								 }
						   }
						}
				}
			  
	////////////////////////////////////2009.9.17
	
	
				  if(DKF==1)  {Zp[0]=Zp[1]; Zp0[0]=Zp0[1];}
				  if(DKF==2)  {Zp[1]=Zp[0]; Zp0[1]=Zp0[0];}
	
	
		  if(Vw[i]<(Vwo+20)&&WheelBadFlag[i]==0)
		  {  
			 if(TVw[i]<210) TVw[i]++;
		  }
		  else TVw[i]=0;
	
		if((st[i]>1)&&(Vs>2700))
		 {
		  if((SR[i]>70&&Aw[i]<-1000)||(TVw[i]>30&&TVw[i]<200))	 //2009.12.2
		  {
			 Zp[i]=2;Zp0[i]=2;
			 TVw[i]=0;
			 Vw1[i]=0;
			 Vw0[i]=Aw[i];
			 if(st[i]==3) Tjj[i]=0; 
			 
			if((st[i]!=3)&&(st[i]!=5))
			 {st[i] = 1;
			  DI[i] = 3;
	
			  T1[i]=0;
			  T2[i]=0;	
			  Tzz[i]=0;
			  Tbb[i]=0;
			  T3[i]=0;
			  step[i] = 0; 
			  Jb_bak[i]=0; 
						   
			  if(st[i]==3) Tjj[i]=0; 
			  ZY3[i]=0;
			  
			  Dzbz[i]=0;
			  Jb[i]=0;
			  Jbk[i]=0;
			 }
			 else
			  Jb[i]=2; 
			  //if(Tjj[i]>90) {st[i]=2,  DI[i]=1; Tjj[i]=0; Tbb[i]=3;step[i] = 0;}
	
		  }
		 }
		 
		  if(st[i]!=3)
		  {  Tst[i]++;
			 if(Tst[i]>500) 
				Tst[i]=500; 
		  }   
		  else Tst[i]=0;
		  
		 if(WheelBadFlag[0]==1)
		  { 
			  if(st[1]==5) {st[1]=2; Tbb[1]=Tbb0+1;st[0]=5;DI[0]=1;DI[1]=1;}
			  Jb[1]=0;Jb0[1]=4;Jbk[1]=0;
		  }   
	
		 if(WheelBadFlag[1]==1)
		  { 
			  if(st[0]==5) {st[0]=2; Tbb[0]=Tbb0+1;st[1]=5;DI[0]=1;DI[1]=1;}
			  Jb[0]=0;Jb0[0]=4;Jbk[0]=0;
		  }   
		  
		  
		  if((Tst[i]>400&&Aw[i]<20)||(Tst[i]>450&&Aw[i]>50))
		  {  Tst[i]=0;
			 st[i]=3;
			 DI[i]=0; 
			 ZNum[i]=0; 
			 Zp0[i]=2;
			 Zp[i]=2;
			 ZY3[i]=1;
					  
			 step[i] = 0;
			 T1[i] = 0;
			 Jb[i]=0; 
			 
			 T2[i]=0;  
			 Tzz[i]=0;
			 Tbb[i]=0;
			 T3[i]=0;
			 //step[i] = 0; 
			 Jb_bak[i]=0; 
			 //Jb[i]=0;  
						  
			 Tjj[i]=0; 
			 Dzbz[i]=0;
			 Vw1[i]=0;
			 Vw0[i]=Vw[i];
	
		  }
	
	
			  if(Diagnose_Data[i+1]!=0) 
				{st[i]=5; if(i==0) {Jbk[1]=0;Jb[1]=0;} else {Jbk[0]=0;Jb[0]=0;}}  ////////////////// Add at 2006.3.26
			  if(Diagnose_Data[i+7]!=0) 
				{st[i]=5; if(i==0) {Jbk[1]=0;Jb[1]=0;} else {Jbk[0]=0;Jb[0]=0;}}  ////////////////// Add at 2006.3.26
			  if(Nocontrols[i]>5)	   
				{st[i]=5; DI[i]=0; if(i==0) {Jbk[1]=0;Jb[1]=0;} else {Jbk[0]=0;Jb[0]=0;}}  ////////////////// Add at 2006.3.26
			   else  
				{if(Nocontrols[0]>2||Nocontrols[1]>2) {Jbk[1]=0;Jb[1]=0;Jbk[0]=0;Jb[0]=0;}}
		  
		 if(Tkc[i]==0)	{SRmaxo[i]=SRmax[i];}	//2006.2.13
	
	
	/////////////////////////////////////////////////////////////
	
				//if(DKF==1&&Tkcn[1]>2) {DKF=0; Jb_bak[0]=0;Jb_bak[1]=0;}
				//if(DKF==2&&Tkcn[0]>2) {DKF=0; Jb_bak[1]=0;Jb_bak[0]=0;}
	
	
		  switch(st[i])
		  {
			case 1: 			   ////��ѹ-��ѹ
	{
				T1[i]++;
	
					if(st[0]!=3&&st[1]!=3)
					 {	   
							 if(Tkcn[0]>2&&Tkcn[1]>2) {Jk_gd=3;}
							  else
							  if(Tkcn[0]>1&&Tkcn[1]>1) {Jk_gd=2;}
								else { if(Tkcn[0]!=0&&Tkcn[1]!=0) Jk_gd=1;}
					 }	   
	
				
				if(Jbcz[i]>60) Jbcz[i]=60;
				Jb0[i]=Jbcz[i];
				if((DKF==1&&i==1)||(DKF==2&&i==0))
				  Jb0[i]=Jbcz[i];// Jb0[i]=10; 2006.2.18
	
				if(DKF==1&&i==0)			   
				   Jb0[i]=Jbcz[i];
				if(DKF==2&&i==1)
				   Jb0[i]=Jbcz[i];
	
			  if(Vw[i]<Vsmax_st)
				{
				 if((Tvnold[i]<-1)&&(Aw[i]<-1600)&&(SR[i]>17)) {if(DI[i]==1) DI[i]=3;}
				 //if((Tvnold[i]<-1)&&(SR[i]>50)) {if(DI[i]==1) DI[i]=3;} //2009.12.9
				}
	
			  Tgro=0;
			  if((T1[i]>T10&&Tbb[i]==0)||(T1[i]>T11&&Tbb[i]>0)) {Tgro=10;}
				else if(T1[i]>Jb0[i])
				{
				  if(Awr[i]>200&&SR[i]<(SRHF+3)) {Tgro=2;}
				   else
				   if(Awr[i]>140&&SR[i]<18&&Tbb[i]>0) {Tgro=3;}
					else
					if(Awr[i]>300&&SR[i]<(SRby+2)) {Tgro=4;}
					 else
					 if(SR[i]<4&&Awr[i]>-1000) {Tgro=1;}  //2012.2.21 add &&Awr[i]>-1000
					  else 
					  if(SR[i]<6&&Tslipk>30&&Kv>25&&Awr[i]>-2000) {Tgro=7;}  // 2012.2.21 add &&Awr[i]>-2000
					  
				  if(Awr[i]>140&&Awr[i]<230&&SR[i]<(SRby-2)) {Tgro=6;}
				   else
					if(Awr[i]>230&&Awr[i]<300&&SR[i]<(SRby)) {Tgro=5;}
					 else 
					 if(Awr[i]>-1&&SR[i]<14) {Tgro=7;}
				  
				  if((T130[i]>20)&&(SR[i]<20)) {Tgro=7;Tzz[i]=3;}
					if((Tzz[i]>0)&&(SR[i]<5)) {Tgro=7;}
	
	//				if(Switch_Flag==0xa5&&GV_OffRoad_Mode_Flag<2) // added at 2014.2.23
	//				{
	//					Tgro=0; // added at 2014.2.23
	//					if(Awr[i]>100&&SR[i]<(SRHF+30)) // added at 2014.2.23
	//					{
	//						Tgro=8; // added at 2014.2.23
	//					}
	//				}
				}
				if(Switch_Flag==0xa5) // added at 2014.3.9
				{
					if(Tgro>0&&SR[i]>95) Tgro=0; // added at 2014.3.9
					if(Tgro>0&&Vw[i]<1200) Tgro=0; // added at 2014.3.9
				}
				if(Tjj[i]>185) {Tgro=7;Tjj[i]=185;} //240 -> 185 :2013.3.28
				
				if(Tgro>0)
				{
				  st[i] = 2;
				  DI[i] = 1;
				  step[i] = 0;
				  T1[i] = 0;
				  if(DKF==1) Jb[1]=0;  //2006.2.6
				  if(DKF==2) Jb[0]=0;
				  //Tzz[i]=0;
			  //	Awb[i]=Awr[i];
				}
				else
				{ 
				
				 if(Awr[i]<-200&&Vw[i]<Awb[i]&&Tbb[i]==0) Awb[i]=Vw[i];
			   
				 if(Vw[i]>(Vwo+100)&&Tbb[i]>0)
				  {
				  if(Vw[i]>Awb[i]&&Awr[i]>100) 
				   {
					 Awb[i]=Vw[i];
				   }
	/*				 else  //2009.12.15 cut
					{
					 if(Tvnold[i]<0) T30[i]++;
					 if(T30[i]>70) T30[i]=70; //2009.6.4 if(T30[i]>70) Tbb[i]=Tbb0+1;
					}
	*/
				  }  
				 
	//			   if(Tkc[i]>=0)
				 { 
				   
				  //if(Aw[i]>100) step[i]=step[i]+1; else { step[i]=0; DI[i]=3;}
	
				  if(Aw[i]>100) step[i]=step[i]+1; 
				   else 
					{ 
					 if(Tslip>20&&Tslipk>10&&Tbb[i]==0&&Vw[i]>(Awb[i]+70)) {step[i]=step[i]+1;} 
					  else { step[i]=0; if(Aw[i]<-1000) DI[i]=3;}
					}
	
				  if(Tdkbz!=0&&Jb[i]==1&&T1[i]<Jb0[i]) step[i]=0;  //or DI[i]=3;
	
				  Tkgo=0; 
				  if(Aw[i]>2000&&Tdkbz==0) 
				   {
					if(Tslipk>15) Tkgo=1;
					 else { if(step[i]>2) Tkgo=1;}
				   }	
				  
				  if(Jb[i]==1)
				   { 
					if(step[i]>Jb0[i])	Tkgo=1;
					if(step[i]>1&&Tjj[i]>Jb0[i])  Tkgo=1;
					if(SR[i]<20&&Tslipk>20&&Aw[i]>-600&&Tjj[i]>Jb0[i]) Tkgo=1;	// add 2006.3.28 
					if(SR[i]<9&&Tslipk>20&&Aw[i]>-800&&Tjj[i]>Jb0[i]) Tkgo=1;  // add 2006.4.4
				   }
				  else	
				   { if(SR[i]<6&&Tslipk>20&&Aw[i]>-280&&Tjj[i]>Jb0[i]) Tkgo=1;	// add 2006.4.4
	
					if(Tslipk>12)
					{ 
					 if(Kv>25&&step[i]>1)  Tkgo=1;
					  else if(Kv>20&&step[i]>2)  Tkgo=1;
					   else if(Kv>15&&step[i]>3)  Tkgo=1;
						else if(step[i]>4)	 Tkgo=1;  
					}
					else 
					{
					  if(Tkc[i]==0&&step[i]>Jb0[i])  Tkgo=1;
					   else if(Tkc[i]>0&&step[i]>5)   Tkgo=1;  
					}
				   } 
					
				  if(Tkgo==1&&DI[i]==3)
				   {
					if(SR[i]<100) DI[i] = 1;
					step[i]=Jb0[i]+1;
					Tkg_DKF[i]=1;
					
					if(WheelBadFlag[0]==1||WheelBadFlag[1]==1){ Tkg_DKF[i]=2; }
	
					if(Nocontrol_w[0]>0||Nocontrol_w[1]>0){ Tkg_DKF[i]=2; }////No_Signal[]==1->Nocontrol_w[]>0,08.2.19
				   }
				   
				   if(Jb_bak[i]==1) Tkg_DKF[i]=2; //2009.12.2
	
	//////////////////////////////////
				 
				 if(Tkc[0]==0&&Tkc[1]==0)
				 {
				  if(Tksjz<100&&Tslipk<13&&Tslip<39) //if(Tksjz<80&&Tslipk<10&&Tslip<35) 2007.1.26
				   {
					if(Vsao>8000&&SR[i]>50&&Tdkbz==0)	 //55 -->50 :2007.1.26
					{
					 if(i==1)
					 {	
					  if((SRmax[1]-SRmaxo[0])>30)
						{Tdkbz=1;}
						//{ Jb[0]=1;Tdkbz=1;Tkg_DKF[i]=2;}
					 }
					 else
					 {
					  if((SRmax[0]-SRmaxo[1])>30)		//10
						{Tdkbz=2;}
						//{ Jb[1]=1;Tdkbz=2;Tkg_DKF[i]=2;}
					 } 
					 
					 if(Tdkbz>0)
					 {
					  for(j=0;j<SS;j++) 
					  {
						 if(Kvs[j]>16) {Kvs[j]=17;}
					  }
					  
					  Kvq=(Kvs[0]+Kvs[1])>>1;
					  Kvh=(Kvs[2]+Kvs[3])>>1;
					  Kvh_old=Kvh;Kvq_old=Kvq;
					  
					  Kv=(Kvq+Kvh)>>1;
					 } 
					} //SR>60
				   
				   }
	 
				  if(Tdkbz!=0)
				  {
				   if(i==0)
				   {
					if(SRmax[1]>50)
					{
					  if((SRmax[1]-SRmax[0])>30)
						{ if(Tjj[1]>4) {Tabo=(Tjj[1]+2)>>2; Tabk=(Tjj[1]+4)>>1;Jbcz[0]=Tabo+Tabk;} else {Jbcz[0]=6;}}
					}
					else if(SRmax[1]>35)
					{
					  if((SRmax[1]-SRmax[0])>25)
						{ if(Tjj[1]>3) {Tabo=(Tjj[1]+1)>>2; Tabk=(Tjj[1]+3)>>1;Jbcz[0]=Tabo+Tabk;} else {Jbcz[0]=5;}}
					}
					else
					{
					  if((SRmax[1]-SRmax[0])>21)
						{ if(Tjj[1]>3) {Tabo=(Tjj[1]+1)>>2; Tabk=(Tjj[1]+2)>>1;Jbcz[0]=Tabo+Tabk;} else {Jbcz[0]=5;}}
					}
				   } 
				   
				   if(i==1)
				   {
					if(SRmax[0]>50)
					{
					  if((SRmax[0]-SRmax[1])>30)	   //10
						{ if(Tjj[0]>4) {Tabo=(Tjj[0]+2)>>2; Tabk=(Tjj[0]+4)>>1; Jbcz[1]=Tabo+Tabk;} else {Jbcz[1]=6;}}
					}
					else if(SRmax[0]>35)
					{
					  if((SRmax[0]-SRmax[1])>25)	   //10
						{ if(Tjj[0]>4) {Tabo=(Tjj[0]+1)>>2; Tabk=(Tjj[0]+3)>>1; Jbcz[1]=Tabo+Tabk;} else {Jbcz[1]=6;}}
					}
					else
					{	 
					  if((SRmax[0]-SRmax[1])>21)	   //10
						{ if(Tjj[0]>3) {Tabo=(Tjj[0]+1)>>2; Tabk=(Tjj[0]+2)>>1; Jbcz[1]=Tabo+Tabk;} else {Jbcz[1]=5;}}
					}
				   } 
				  } // Tdkbz!=0
				  
				 }
	
				 }		 
	
				}//8.27 	 
				
			  if((Tvnold[i]>0)&&(T1[i]>Jb0[i])) DI[i]=1;
	
			if(DI[i]==1) 
				{
				 if((i==0)&&(st[1]==3)) {Jb[1]=1;Jbk[1]=0;}
			   if((i==1)&&(st[0]==3)) {Jb[0]=1;Jbk[0]=0;}
			  }
	
	////////////////////////////////// 2006.2.18 b			   
			 if((DI[i]==1)&&(Tkg_DKF[i]==0)) {Tkg_DKF[i]=1;}
						
			 if(Tkg_DKF[i]==1)
			 { 
			   Tkg_DKF[i]=2;
			   if(Vs>2000)
			   {
				  if(i==1)
				  { 
				   if(DKF==1&&Tkcn[1]>2) DKF=0; 
	
				   if((DKF==1)||(DKF==0))  //////////05.11.10 
				   {
					if((SRmax[1]-SRmaxo[0])>20)
					  { Zp0[0]=1;Zp0[1]=1; Zp0[2]=1;Zp0[3]=1;Zp[0]=1;Zp[1]=1;DKF=1;}
					else 
					 if((SRmax[1]-SRmaxo[0])>10)
					  { DKF=1; Zp0[0]=1;Zp0[1]=1;Zp[0]=1;Zp[1]=1;}
					else {DKF=0;}
				   }
				  }
				  else
				  {
				   if(DKF==2&&Tkcn[0]>2) DKF=0;
				   
				   if((DKF==2)||(DKF==0))	//////////05.11.10	
				   {
					if((SRmax[0]-SRmaxo[1])>20) 	  //10
					  { Zp0[0]=1;Zp0[1]=1; Zp0[2]=1;Zp0[3]=1;Zp[0]=1;Zp[1]=1;DKF=2;}
					else
					 if((SRmax[0]-SRmaxo[1])>10)	   //10
					  { DKF=2;Zp0[0]=1;Zp0[1]=1;Zp[0]=1;Zp[1]=1;}
					else {DKF=0;}
				   }
				  } 
			   }
			   else 
				{DKF=0;}
	
				 if(i==1)
				 {
				  if(DKF==1) 
				  { Jbcz[i]=4;
					Zp[0]=Zp[1]; 
	
					if(Tkc[i]==0&&Tslipk<13&&Tslip<35) 
					   {Tabk=(Tjj[1]+3)>>2; Tabo=Tabk+(Tjj[1]+4)>>1;}
					  else Tabo=Tjj[1]>>1;
					
					Jbcz[0]=Tabo;	 
				   // if(Tkcn[1]>2&&Jbcz[0]>6) Jbcz[0]=6;								 ////2006.1.11
	
					   if(st[0]==3)   //2006.2.13				
						{
						 Jb[0]=1;
						 if(T3[0]<11) {if(Jbcz[0]>5) Jbcz[0]=5;}
						}
	
				  }
				  
				  if(DKF==0)		//2006.2.19
				  {
					Jbcz[i]=4; 
					DKF=1;
					//if(st[0]==3&&Tjj[1]>3&&Tkcn[0]>0) 
					if(st[0]==3)
					 {
					  Jbcz[0]=(Tjj[1])>>1; 
					  if(Tjj[1]>3&&T3[0]>10)	
					  {
					   if(Awr[0]<-460&&SR[0]>-1) Jb[0]=1; else {Jbcz[0]=(Tjj[1]-1)>>1; Jb[0]=1;}
					   if(Tslipk>16) {if(Jbcz[0]>15) Jbcz[0]=15;} else {Jbcz[0]=Jbcz[0]+1; if(Jbcz[0]>18) Jbcz[0]=19;} 
					   //if(Tkcn[0]<2&&Jbcz[0]>7) Jbcz[0]=7;
					  }
					  else
					  {Jb[0]=1; if(Jbcz[0]>5) Jbcz[0]=5;}
					 }
				  }
				  
				  if(DKF==2)		//2006.2.19
				  {
					Jbcz[i]=4; 
					DKF=1;
					if(st[0]==3)
					{
					 Jbcz[0]=(Tjj[1])>>1;  
					 if(Tjj[1]>3&&Tkcn[0]>0) 
					 {
					  if(Awr[0]<-460&&SR[0]>-1) Jb[0]=1; else {Jbcz[0]=(Tjj[1]-1)>>1; Jb[0]=1;}
					  if(Tslipk>16) {if(Jbcz[0]>15) Jbcz[0]=15;} else {Jbcz[0]=Jbcz[0]+1; if(Jbcz[0]>18) Jbcz[0]=19;} 
					  //if(Tkcn[0]<2&&Jbcz[0]>7) Jbcz[0]=7;
					 }
				 else
					 {Jb[0]=1; if(Jbcz[0]>5) Jbcz[0]=5;}
	
					} 
				  }
				  
				 } 
				 
				 if(i==0)
				 {
				  if(DKF==2)
				  { Jbcz[i]=4;
					Zp[1]=Zp[0];
	
					//Tabo=Tjj[0]>>1;
					if(Tkc[i]==0&&Tslipk<13&&Tslip<35) 
					  {Tabk=(Tjj[0]+3)>>2; Tabo=Tabk+(Tjj[0]+4)>>1;}
					 else Tabo=Tjj[0]>>1;
					 
					Jbcz[1]=Tabo;									////2006.1.11
				   // if(Tkcn[0]>2&&Jbcz[1]>6) Jbcz[1]=6; 
				   
					  if(st[1]==3)	 //2006.2.13
					   {
						Jb[1]=1;
						if(T3[1]<11) {if(Jbcz[1]>5) Jbcz[1]=5;}
					   }
	
				  }
				  
				  if(DKF==0)	 //2006/2/19
				  { 
					DKF=2;
					if(st[1]==3)
					 {
					  Jbcz[1]=(Tjj[0])>>1; 
					  if(Tjj[0]>3&&T3[1]>10)
					  { 
					   if(Awr[1]<-460&&SR[1]>-1) { Jb[1]=1;} else {Jbcz[1]=(Tjj[0]-1)>>1; Jb[1]=1;}
					   //if(Jbcz[1]>15) Jbcz[1]=15; 	   //2006.2.19
					   if(Tslipk>16) {if(Jbcz[1]>15) Jbcz[1]=15; } else  {Jbcz[1]=Jbcz[1]+1; if(Jbcz[1]>18) Jbcz[1]=19; }
					   //if(Tkcn[1]<2&&Jbcz[1]>7) Jbcz[1]=7;
					  }
					  else
					  { Jb[1]=1; if(Jbcz[1]>5) Jbcz[1]=5;}
					   
					 }
				  }
				  
				  if(DKF==1)	 //2006/2/19
				  { 
					DKF=2;
					if(st[1]==3)
					{
					 if(Tjj[0]>3&&Tkcn[1]>0)
					//if(st[1]==3&&Tjj[0]>3&&T3[1]>10) 
					 {
					  Jbcz[1]=(Tjj[0])>>1; 
					  if(Awr[1]<-460&&SR[1]>-1) { Jb[1]=1;} else {Jbcz[1]=(Tjj[0]-1)>>1; Jb[1]=1;}
					  if(Tslipk>16) {if(Jbcz[1]>15) Jbcz[1]=15; } else	{Jbcz[1]=Jbcz[1]+1; if(Jbcz[1]>18) Jbcz[1]=19; }
					  //if(Tkcn[1]<2&&Jbcz[1]>7) Jbcz[1]=7;
					 }
					 else
					  { Jb[1]=1; if(Jbcz[1]>5) Jbcz[1]=5;}
					  
					}
				  }
				 }
				 
			 } // if(Tkg_DKF[i]==1)
	//////////////////////////////////// 2006.2.18 e
	
					if(DI[i]==3) Tjj[i]++;
				
					if(Vs<5000&&Kv>15)
					{
					  if(Tjj[i]>50)
					  {
						 if((i==0)&&(SR[1]<6)) DI[i]=1;
						 if((i==1)&&(SR[0]<6)) DI[i]=1;
						}
					  
					  if((Tjj[i]>36)&&(Kv>20))
					  {
					   if((i==0)&&(SR[1]<6)) {DI[i]=1;if(T1[i]>53) Nocontrols[i]=6;}
					   if((i==1)&&(SR[0]<6)) {DI[i]=1;if(T1[i]>53) Nocontrols[i]=6;}
						}
					}
	
					if(step[i]>250) step[i]=250; 
					
		  break;
	}	  
			case 4: 	///////////////// �Կ� ǰ���ѹ-->��ѹ�ж�
	{
			  if(Aw[i]>(BB0-80)) {T2[i]++; }
			  st[i]=2; DI[i]=1;
			break;
	}		   
			case 2: 	//////��ѹ-С������ѹ
	{
			  Vsq=Vstq;  //2008.6.5
			  Jbk[i]=0;
			  T5[i]=0;
			  Tkg_DKF[i]=0;
			  
			  if(Aw[i]>(BB0-20))
				 T2[i]++;
			  if(T2[i]>20) T2[i]=20; 
			  if(T1[i]<60) T1[i]++;
	
			  if(Tvnold[i]<-10) DI[i] = 3;
				if(Tvnold[i]>0) DI[i] = 1;
	
			 //if((Tzz[i]>25&&Aw[i]<300&&SR[i]>20)||(Aw[i]<AA1))  2006.4.8
			 Tgro=0;
	
			 if(Tslipk<15&&T1[i]>25&&Aw[i]<-1000&&SR[i]>20) Tgro=1; //2009.10 300 ->-300 ,2009.11 -300 ->-200 
			  else 
			  if(Tslipk>14&&T1[i]>29&&Aw[i]<-1100&&SR[i]>20) Tgro=1;  //2009.10 -300
			   else 
			   if(Tdk<21&&Aw[i]<AA1&&SR[i]>18) Tgro=1;	//2009.9.22  add Tdk<21 2009.10  -1000 ->AA1
				else 
				if(T1[i]>50&&Aw[i]<-700&&SR[i]>35) Tgro=1;	//2010.1.14 add
					
			 if((T130[i]>20)&&(SR[i]<21)) {Tgro=0;Tzz[i]=3;}  //2009.12.2 add
			 if((Kv>22)&&(SR[i]<3)) {Tgro=0;Tzz[i]=3;}	//2009.12.29 add
				
			 if(Tzz[i]>2) {Tgro=0;}  //2009.12.2 add
				
			 if(Tgro>0)
			 {st[i] = 1;
				if(Tvnold[i]<-5) DI[i] = 3;
			  step[i] = 0;
			  T1[i]=0;
			  Tzz[i]++;   //2009.12.3 modify 0->++
			  if(Vs>1500) 
				 Tbb[i]++;
			  else 
				 Tbb[i]=Tbb0;
			 } 
	
			  if(Awr[i]>BB[i]) BB[i]=Awr[i];
			  
			  Vsr=BB[i]>>1; 
			  
			  if(BB[i]>800)
			  {
			   if(Kv<20) {Vsr=Vsr-200;}
				else if(Kv<23) { Vsr=Vsr-150;}
			  }
			  
			 Tgro=0;
	
				 if(Vw[i]>(Vwo+100))
				  {
				  if(Vw[i]>Awb[i]&&Awr[i]>100) 
				   {
					 Awb[i]=Vw[i];
				   }
				   else
					{
					 if(Tvnold[i]<-1) T30[i]++;
					 //if(T30[i]>20) {if(SR[i]<3) Tbb[i]=Tbb0+1;} //2009.12.18 cut
					 if(T30[i]>3) 
						{if(SR[i]<6) {Tbb[i]=Tbb0+1;Tgro=1;} else {if(T30[i]>7) {Tbb[i]=Tbb0+1;Tgro=1;}}} //2013.06.30 
					}
				  }  
	
			 if(Tslipk>15&&Aw[i]<Vsr&&T2[i]>2&&SR[i]<SRZY) {Tgro=1;}
			  else
			  if(BB[i]>2000&&Aw[i]<(Vsr-300)&&T2[i]>2&&SR[i]<SRZY) {Tgro=1;}
			   else
			   if(Awr[i]>4200&&T1[i]>1&&SR[i]<SRZY) 
				{ if(Tslip<35&&Tslipk<20) 
				   {if(Awr[i]<(BB[i]-230)) Tgro=3;} 
				  else {Tgro=3;}
				}
				else
				 if(T1[i]>1&&SR[i]<1) 
				 { if(Tslip>25&&Tslipk>15) 
				   {
					if(Awr[i]<(BB[i]-300))
					 {
					  if(Vs>8000) {if(BB[i]>1700) Tgro=3;}
					   else 
					   if(Vs>6000) { if(BB[i]>1000) Tgro=3;}
						else
						if(Vs>3000) { if(BB[i]>500) Tgro=3;}
					 }	
				   } 
				 }
	
			  if(Aw[i]<(BB0-380)&&T2[i]>2&&SR[i]<(SRZY+5)) {Tgro=1;}
			   else
			   if(Aw[i]<250&&T1[i]>6&&SR[i]<SRZY) {Tgro=2;}
				else
				if(Jb_bak[i]==1&&(SR[i]<6||Awr[i]>-500)) {Tgro=2;}
				 else
				 if(Awr[i]>-150&&Awr[i]<100&&Tbb[i]>1&&T1[i]>8&&SR[i]<(SRZY+5))  {Tgro=4;}
				  else
				  if((Tbb[i]>=Tbb0&&T1[i]>2)||(Tbb[i]>1&&Vs<3000&&T1[i]>3))  {Tgro=5;}
	
			 if(SR[i]<20)		//2009.12.14 modify
				{if((Tvnold[i]>1)||(Aw[i]>2000)||(T2[i]>2))
				 {if(Tanold[i]==0) Tanold[i]=1;}
				}
			 if(SR[i]<5&&Tanold[i]==1) Tanold[i]=2;
			 if(Tvnold[i]<-1&&Tanold[i]>1) 
				{if((i==0)&&(Tanold[1]>0)) Tgro=1;
				 if((i==1)&&(Tanold[0]>0)) Tgro=1;
				 if(Kv>20) Tgro=1;
				}
	
			   if(SR[i]<21)
				{
				  if(i==0) 
					{if(T130[i]>30) {Tbb[i]=Tbb0+1;Tgro=5;} 
					  else if((T130[i]>20)&&(T130[1]>25)) {Tbb[i]=Tbb0+1;Tgro=5;}}//2009.12.1
				  if(i==1) 
					{if(T130[i]>30) {Tbb[i]=Tbb0+1;Tgro=5;}
					  else if((T130[i]>20)&&(T130[0]>25)) {Tbb[i]=Tbb0+1;Tgro=5;}} //2009.12.1
				}
	
			 //if(Tvnold[i]>20) Tgro=0; //2012.3.12
				if(Tvnold[i]>6) Tgro=0; //2013.3.16
				
			  if(Tgro>0) {st[i]=5; Tbb[i]=Tbb0+1;DI[i]=1;GV_st5_Enter_group[i]=1;}	   //2006.4.4; changed at 2016.10.16 by sz &&&&&&&&
			  
			 if(Nocontrols[0]>2||Nocontrols[1]>2)	  //2007.11.28
			 {
			   Jbk[1]=0;Jb[1]=0;Jbk[0]=0;Jb[0]=0;
			 }
			 else
			 {	
			  if(i==0)			   //2006.3.25
			   {
				 if(st[1]!=5) { Tgro=0;}
			   }
			   else 
			   {
				 if(st[0]!=5) { Tgro=0;}
			   }
			 }
	
			 if( Tgro>0)
			 { 
			  
			  if(Tkcn[0]<1&&Tkcn[1]<1) Jk_gd=0;
			  if(Zp[0]<Zp[1]) Zp[1]=Zp[0]; else Zp[0]=Zp[1]; //2009.12.13 add		   
			  
			  for(ijs=0;ijs<2;ijs++)
			  {
				if(i==1&&ijs==0) {T3[0]=1; T3[1]=1;}
				if(i==0&&ijs==0) {T3[0]=1; T3[1]=1;}			 
	
				if(Nocontrols[0]>2||Nocontrols[1]>2) {ij=i; ijs=2;}  //2007.11.28
				else
				 {ij=ijs;}
				 
				Tbb[ij]=0;
				BB[ij]=0;			  
				Dzbz[ij]=0;Jbk[ij]=0;Tzzo[ij]=0;Jbcz[ij]=3;
	
					  st[ij]=3;
					  DI[ij]=0;
					  step[ij] = 0;
					  ZY3[ij]=0;
					  //T3[ij]=0; //2006.3.26
					  T2[ij]=0;
					  T1[ij]=0;
					  Tzz[ij]=0;
					  Jb_bak[ij]=0;
					  //Jb[ij]=0;  //2006.3.24
					  
				Tkc[ij]++; if(Tkc[ij]>3) Tkc[ij]=4;
	
				 Tjj[ij]=0; 
				 Tdkbz=0;
		
		if(Tslip>20&&Tslipk>8&&(Tslipk+7)>=Tkcqh)
		{
		 Tgro=0;
		 if(Tkc[ij]==1&&Jb[ij]==0)	   //2006.3.25
		 {
		  if(Taw200[ij]<6) {if(Kvs[ij]<28) {Kvq=28;Tgro=1;}}	
		   else if(Taw200[ij]<9) {if(Kvs[ij]<25) {Kvq=25;Tgro=1;}}
			else if(Taw200[ij]<12) {if(Kvs[ij]<22) {Kvq=22;Tgro=1;}}
			 else if(Taw200[ij]<15) {if(Kvs[ij]<18) {Kvq=18;Tgro=1;}}
		  
		   if(Tgro==1) {for(j=0;j<SS;j++) {Kvs[j]=Kvq;} Kvq_old=Kvq; Kvh=Kvq; Kvh_old=Kvh;}
		 }	
		}
				  Taw200[ij]=0;
				  Jb[ij]=0;  //2006.3.25
				  
					   if(ij==0) 
					   {
						 Tgro=Tgr[0]+Tgr[1];
	
						 if(Tgro==0)
						 {
							if(Tslipk<15&&Kv<20&&SRmax[0]>15&&SRmax[1]>15) 
							 { if(Vstq>Vmax2[1]) Vsmin=Vmax2[1]; else Vsmin=Vst_max;}
							 else
							 {
							  if(SRmax[0]>13&&SRmax[1]>13)
							   { if(Vst_max<Vs2) Vsmin=(Vmax2[0]+Vst_max)>>1; 
								   else Vsmin=(Vmax2[0]+Vs2)>>1;
							   }
							   else
							   {if(Vmax2[0]<Vs2) Vsmin=Vmax2[0]; else Vsmin=Vs2;}
							 }
						 }
						 else
						 if(Tgro==1)
						  { 
							 if(Vmax2[0]<Vsth) Vsmin=Vmax2[0];else Vsmin=Vsth;
						  }
						  else Vsmin=Vstq-30;
	
	
						Vsr=Vsmin;
	
						if(SRmax[0]<12&&SRmax[1]<12)
						  {Vsmin=(Vsmin+Vstq)>>1;}
						  else if(SRmax[0]>20&&SRmax[1]>20)
							{
							  if(Vsr>Vs) 
							   { if(Kv<18) {Vsmin=(Vsr+Vs)>>1; Vsmin=(Vsmin+Vsr)>>1;} else Vsmin=(Vsr+Vs)>>1;}
							   else
								{ if(Kv<20) Vsmin=(Vsmin+Vs)>>1; else Vsmin=Vsr;} 
							  
							}
							else 
							if((SRmax[0]<6&&SRmax[1]>15)||(SRmax[1]<6&&SRmax[0]>15))
							  { if(Kv<20) {Vsmin=(Vsr+Vs)>>1; Vsmin=(Vsmin+Vsr)>>1; }
								else Vsmin=(Vsr+Vs)>>1;
							  }  
							  else
							  {Vsmin=(Vsr+Vs)>>1;}
						 if(Ttjs<9) Ttjs++; //2009.10
					   } //ij=0 2006.4.9
	
					  if(Tqjs[ij]<238) Tqjs[ij]++;
	
					  if(Tkcn[ij]>2&&Kv<13&&Vsmin<Vs) {Vsmin=(Vsr+Vsmin)>>1;}
					  
					  // Tkcn[ij]=0; //2006.4=3.31
	
	
						 if(Vw1[ij]>360) Vw1[ij]=360;
						 if(Vw1[ij]<0) Vw1[ij]=0;
						 Vsr=Vw1[ij]>>1;
	
						 if(Tslipk>13) Vstq=Vsmin-Vsr; else Vstq=Vsmin;
						 Vsr=(Vmax2[ij]+Vsmin)>>1; 
	
						 if(Tslip<10&&Tnum[ij]<20&&Tkc[ij]<2&&Tslipk<10)
						 {
							Vsr=Vsr-Vw1[ij];
						 }
						 else
						 { 
						   Vsr=(Vsr-Vw1[ij]); //2006.4.2
						   
						   if(WheelBadFlag[ij]==0&&Tgr[ij]==0) 
						   {
							if(Vsmin>Vs_old[ij])
							{
							  if(Tslip>16) Kvs[ij]=(Kvs[ij]+15)>>1; else Kvs[ij]=(Kvs[ij]+10)>>1;
							  if(Tvnold[ij]>20) Kvq=(Kvq+Kvs[ij])>>1;
							}
							else
							if(Vsmin<Vs_old[ij])
							{
							  Kvq = (unsigned int)((Vs_old[ij]-Vsr)/Tnum[ij]);
							}  //10.27
						   }	
						   else Kvs[ij]=Kv+1;
						   
						   if(Tvnold[ij]>18)
							{ 
							 if(Kvq_old>19) {Kvs[ij]=Kvq; Kvq_old=Kvq;} //2009.5.22
							}
							
							  if(Tkcn[ij]>2&&Vsmin<Vs)	Kvs[ij] = (Kvs[ij]+Kvq+5)>>1;
								else  Kvs[ij] = (Kvs[ij]+Kvq)>>1;
								
							  if(Tkc[ij]<2)
							  {  
								//if(Vw[2]<1200&&Vw[3]<1200&&Kvs[ij]>13) Kvs[ij]=13;
								if(ij==0&&SR[1]<20&&SR[2]<20&&SR[3]<20&&Kv<20) {Kvs[ij]=(Kvs[ij]+20)>>1; Kv=Kvs[ij];}
								if(ij==1&&SR[0]<20&&SR[2]<20&&SR[3]<20&&Kv<20) {Kvs[ij]=(Kvs[ij]+20)>>1; Kv=Kvs[ij];}	 
							  }
								
							  if(Kvs[ij]>45)
								 Kvs[ij]=45;
							  if(Kvs[ij]<1)
								 Kvs[ij]=1;
							  
							  Kvq=(unsigned int)((Kvs[0]+Kvs[1])>>1); 
							  Kvq=(unsigned int)((Kvq+Kvq_old)>>1);//((Kv+Kvq_old)>>1);
							  Kvq_old=Kvq;
	
							 // Kvmin=Kvq;
							  
							  if(Tgr[ij]==0)
							  {
							   if(Vs_st[ij]>Vsck[ij]) Vsck[ij]=Vs_st[ij]; 
								else 
								{ 
								 if(Jk_gd>1)
								  {Vsck[ij]=(Vs_st[ij]+Vsck[ij])>>1;Vsck[ij]=(Vs_st[ij]+Vsck[ij])>>1;}
								  else
								  if(Jk_gd==0)
								   { Vsck_b=(Vs_st[ij]+Vsck[ij])>>1; Vsck[ij]=( Vsck_b+Vsck[ij])>>1;}
								   else 
								   Vsck[ij]=(Vs_st[ij]+Vsck[ij])>>1; 
								} 
							  }
							  else
							  {Vsck[ij]=((unsigned int)(Vs2)+Vsck[ij])>>1;}
							  
							  Tnum[ij]=0;
							  Vs_old[ij]=Vsmin;
							  Tanold[ij]=0;Tvnold[ij]=0;
	
				if(Kvq>30)
					{
					   StepNum1=14;
					   StepNum10=16;
					}
					else if(Kvq>23)
					{
					   StepNum1=16;
					   StepNum10=18;
					}
					else if(Kvq>16)
					{  
					   StepNum1=19;
					   StepNum10=21;
					}
					else if(Kvq>10)
					{  
					   StepNum1=22;
					   StepNum10=24;
					}
					else 
					{
					   StepNum1=25;
					   StepNum10=27;
					}
	
				   //if(DKF!=0) { Zp[0]=1;Zp[1]=1; Zp[2]=1;Zp[3]=1;StepNum1=25;StepNum2=25; StepNum10=14;StepNum20=14; }
						 }
						 
						Tkcn[ij]=0; 
						TStepNum[ij]=StepNum1;
						
						if(Tcjy[1]>Tcjy[0])  
						 { if(Kvs[ij]>25)
							 if(Vsq<Vstq) 
							  {
								//Kvs[ij]=(Kvs[ij]>>1)+Kvs[ij]; //2008.6.19
								Kvs[ij]=(Kvs[ij]>>1);
							  }
						 }	 //2008.6.5
			   Tzy[ij]=0;
				} //ijs
				
					 Tcjy[0]=0; Tcjy[1]=0; 
					 if(Tqjs[0]>Tqjs[1]) {Tqjs[1]=Tqjs[0];} else {Tqjs[0]=Tqjs[1]; } //2009.10
			 } //Tgro>0 
		  break;
	}
		case 5: 	///////////////// Nop 
	{
		  DI[i]=1; 
		  if(T5[i]<10) T5[i]++;
		  
		  if(T5[i]>0)
		  {
			  if(T5[i]<100)   // 10->100; changed at 2016.10.15 by sz
			  {
						if(SR[i]>70&&GV_st5_Enter_group[i]>0)	{	T5[i]++;} // added at 2016.10.15
						else  { if(T5[i]>10) T5[i]=10;}; // added at 2016.10.15
			}
	//*/
					if(T5[i]>15&&Vs>10000) // added at 2016.10.15; changed 4000->10000 at 2016.10.16
					{
						st[i]=1;DI[i]=3; // added at 2016.10.15
						GV_st5_Enter_group[i]=0;  // added at 2016.10.15
					}
	//*/
		  if(i==0&&st[1]==3) {Jb[1]=1;Jbk[1]=0;}
		  if(i==1&&st[0]==3) {Jb[0]=1;Jbk[0]=0;}
	
			  if(Diagnose_Data[i+1]!=0) 
				{st[i]=5; if(i==0) {Jbk[1]=0;Jb[1]=0;} else {Jbk[0]=0;Jb[0]=0;}}  ////////////////// Add at 2006.3.26
			  if(Diagnose_Data[i+7]!=0) 
				{st[i]=5; if(i==0) {Jbk[1]=0;Jb[1]=0;} else {Jbk[0]=0;Jb[0]=0;}}  ////////////////// Add at 2006.3.26
			  if(Nocontrols[i]>2)	   
				{st[i]=5; DI[i]=0; if(i==0) {Jbk[1]=0;Jb[1]=0;} else {Jbk[0]=0;Jb[0]=0;}}  ////////////////// Add at 2006.3.26
	
		  }
		  
		  if(st[0]==5&&st[1]==5) {st[1]=2;Tbb[1]=Tbb0+1;T1[1]=10;T2[1]=10;Tzz[1]=10;}
	
		  if(Tst5==0)
		  {
		   Tst5=1;	
		   if(i==1&&DKF==1&&st[0]==1) 
				{
				 if(Tslip<35&&Tslipk<15)
				 {
				  //if(Jbcz[0]>5) {Tabo=(Jbcz[0])>>1; Jbcz[0]=Tabo+2;}
				  if(Jbcz[0]>25) {Jbcz[0]=25;}
				 }
				 else
				 {
				  if(Jbcz[0]>16) Jbcz[0]=9;
				   else if(Jbcz[0]>13) Jbcz[0]=7;
					else if(Jbcz[0]>9) Jbcz[0]=5;
					 else if(Jbcz[0]>5) Jbcz[0]=3;
					  else if(Jbcz[0]>3) Jbcz[0]=2;
					   else Jbcz[0]=1;
				 }
				}
				
			   if(i==0&&DKF==2&&st[1]==1) 
				{
				 if(Tslip<35&&Tslipk<15)
				 {
				  //if(Jbcz[1]>5) {Tabo=(Jbcz[1])>>1; Jbcz[1]=Tabo+2;}
				  if(Jbcz[1]>25) {Jbcz[1]=25;}
				 }
				 else
				 {
				  if(Jbcz[1]>16) Jbcz[1]=9;
				   else if(Jbcz[1]>13) Jbcz[1]=7;
					else if(Jbcz[1]>9) Jbcz[1]=5;
					 else if(Jbcz[1]>5) Jbcz[1]=3;
					  else if(Jbcz[1]>3) Jbcz[1]=2;
					   else Jbcz[1]=1;
				 }
				}
		  } 
		  
		  break;
	}	   
		case 3: 	///////////////// С������ѹ-->��ѹ 
	{ 
			   Vs_mid=Vmax>>2; Vsr=Vmax>>1;
					 Vs_mid=Vs_mid+Vsr;
					 
			   if(T3[i]<2000) T3[i]++;
			   
			   if(Tst5>1)
			   {
		   if(i==1&&DKF==1&&st[0]==1) 
				{
				 if(Tslip<35&&Tslipk<15)
				 {
				  if(Jbcz[0]>5) {Tabo=(Jbcz[0])>>1; Jbcz[0]=Tabo+2;}
				  //if(Jbcz[0]>25) {Jbcz[0]=25;}
				 }
				 else
				 {
				  if(Jbcz[0]>16) Jbcz[0]=9;
				   else if(Jbcz[0]>13) Jbcz[0]=7;
					else if(Jbcz[0]>9) Jbcz[0]=5;
					 else if(Jbcz[0]>5) Jbcz[0]=3;
					  else if(Jbcz[0]>3) Jbcz[0]=2;
					   else Jbcz[0]=1;
				 }
				}
				
			   if(i==0&&DKF==2&&st[1]==1) 
				{
				 if(Tslip<35&&Tslipk<15)
				 {
				  if(Jbcz[1]>5) {Tabo=(Jbcz[1])>>1; Jbcz[1]=Tabo+2;}
				  //if(Jbcz[1]>25) {Jbcz[1]=25;}
				 }
				 else
				 {
				  if(Jbcz[1]>16) Jbcz[1]=9;
				   else if(Jbcz[1]>13) Jbcz[1]=7;
					else if(Jbcz[1]>9) Jbcz[1]=5;
					 else if(Jbcz[1]>5) Jbcz[1]=3;
					  else if(Jbcz[1]>3) Jbcz[1]=2;
					   else Jbcz[1]=1;
				 }
				}
			   }
	
			   Tst5=2;
						  
			  Tgro=0;
			  if(Tkc[i]<4&&Aw[i]<(AA1-2200)&&SR[i]>1) //2009.12.10 SR[i]>-2
				{
				 Tgro=1; 
				 if(Tslip>35||Tslipk>20) { if(SR[i]<1) Tgro=0;}
				}
			  
			  if(Kv>18)
			   {if((Aw[i]<(AA1-260)&&SR[i]>(SRJY+10))||(Awr[i]<AAr&&SR[i]>(SRJY+10))) Tgro=1;	//2013.06.30	+6
			   }
			   else
			   {if((Aw[i]<AA1&&SR[i]>30)||(Awr[i]<AAr&&SR[i]>25)) Tgro=1;	//2013.06.30	+6
			   }
			  if((Aw[i]<(AA1-1800)&&SR[i]>15&&Vs>5500)||(Vs<5501&&SR[i]>17&&Awr[i]<(AA1-2300))) Tgro=2; //2013.06.30	+5
			  if((Awr[i]<AA1&&SR[i]>16&&Dzbz[i]>0)||(Jb[i]>0)) Tgro=3;		 //2013.06.30	 +5
			  if(Switch_Flag==0xa5&&GV_OffRoad_Mode_Flag<2) // added at 2014.2.23
			  {
				Tgro=0; // added at 2014.2.23
				if((Aw[i]<AA1&&SR[i]>30)||(Awr[i]<AAr&&SR[i]>25)) Tgro=1; // added at 2014.2.23
				if((SR[i]>60)&&(Awr[i]<-100)) Tgro=2; // added at 2014.3.10
			  }
			  if(Tgro>0&&T3[i]>3)	//13.06.30	4->3
			  {
				  Awb[i]=Vw[i];
				  Tdjk=0; Tdk=0; Tvn=0;  //2009.9.17
				  
				  //if(Tcjy[i]>(T3[i]+T3[i])) { if(Kvs[i]>25) Kvs[i]=Kvs[i]>>1;} //2008.6.5
	
				  SRmaxo[i]=SRmax[i];
				  SRmax[i]=0;
				   
				  //Tcjy[i]=0; //2008.6.5
				  Dzbz[i]=0;
				  if(Jb[i]==2) Jb[i]=0;
				  
				  Jb_bak[i]=Jb[i];
				 // Jb[i]=0;   //2006.3.24
				  Jbk[i]=0;
	
			//		if(i==1&&DKF==1&&st[0]!=3) {Jb[0]=1;}
			//		if(i==0&&DKF==2&&st[1]!=3) {Jb[1]=1;}
	
				  T1[i]=0;
				  T2[i]=0;
				  T3[i]=0; 
				  Tzz[i]=0; 	   
				  st[i] = 1;
					  LV_OffRoad_ByteCount[i]=0; // added at 2014.2.25
				  
			  DI[i] = 3;
				  step[i] = 0;
				  Vw1[i]=0;
				  Vw0[i]=Aw[i];
				  T30[i]=0;
				  T130[i]=0; 
				  Tst5=0;
				  
				  if((ZY3[i]>1)||((ZY3[0]>0)&&(ZY3[1]>0)))
				  {
					 if(Zp0[i]<3)  Zp0[i]=3;
					  else Zp0[i]++;										
				  }
				  else if((ZY3[i]==1)||(ZNum[i]>60))  //// else if(ZNum[i]==4)
				  { 
					 if(Zp0[i]<2)	Zp0[i]=2;
					  else Zp0[i]++;					   
				  }
				  else if(ZNum[i]>45)
				  { 
					 Zp0[i]++;
					 if(Zp0[i]>3)	Zp0[i]=3;
				  }
				  else if(ZNum[i]>30)
				  { 
					 Zp0[i]=2;
				  }
				  else if(ZNum[i]<17)
				  {
					Zp0[i]--;
					 if(Zp0[i]<1) Zp0[i]=1;  //2009.12.3 : 2-->1
				  }
	
					if(Zp0[i]>4)   Zp0[i]=4;
					Zp[i]=Zp0[i];
				  
				  if(DKF!=0)	/////// Add at 2004.8.26
				  {
					 if(Zp[i]>2)
					 { Zp[i]=2;
					   Zp0[i]=2;
					 }
					 if(Jb[i]==1&&Tkcn[i]<2) { Zp[i]=1; Zp0[i]=1;}
				  }
				  
				  
				  if(DKF==1)  {Zp[0]=Zp[1]; Zp0[0]=Zp0[1];}
				  if(DKF==2)  {Zp[1]=Zp[0]; Zp0[1]=Zp0[0];}
	
				  ZY3[i]=0;
				  ZNum[i]=0;
			  }
			  else if(Dzbz[i]==0)
			  {
			  if(ZNum[i]==0) {if(Vs1>Vw[i]) Vw0[i]=Vs1; else Vw0[i]=Vs2;}
				
				if(ZNum[i]>90)	//2009.12.13 5->6
				{  ZY3[i]++;	 if(ZY3[i]>9) ZY3[i]=10;
				  if(Ttjs<9) Ttjs=Ttjs+4; //2009.10
					if(Tqjs[i]<238) Tqjs[i]++;
				  //Tkcn[i]=Tkcn[i]+5;
				  SRmaxo[i]=SRmax[i];
				  SRmax[i]=0;
	
				  if(i==1&&DKF==1) DKF=0;
				   if(i==0&&DKF==1&&(ZY3[1]>0||ZNum[1]>47)) DKF=0;
				  
				  if(i==0&&DKF==2) DKF=0;
				   if(i==1&&DKF==2&&(ZY3[0]>0||ZNum[0]>47)) DKF=0;
	
	
				  if(DKF!=0&&ZY3[i]>1&&SR[i]>6) ZY3[i]=0;
				  if(DKF==0&&ZY3[i]>0)
				  {
					if(Vs>1200)  //7000
					 {
					  if(Kv>12)
					   {
						if(SR[i]>10) Zp[i]=1;else if(SR[i]>4) Zp[i]=2;else if(SR[i]>-2) Zp[i]=2;else  Zp[i]=3;
					   } 
					  if(Kv<13)
					   {
					   if(SR[i]>10) Zp[i]=1;else if(SR[i]>7) Zp[i]=2;else if(SR[i]>4) Zp[i]=3;else if(SR[i]>-1) Zp[i]=4;else  Zp[i]=4;
					   }
					 }
					 else 
					  Zp[i]=2;
	
					 Zp0[i]=Zp[i]; 
				  }
	
	//				 if(Vs1>Vw[i]) Vsmin=Vs1;  else Vsmin=Vs2; //2012.3.18
					 if(Vw[i]<Vs1) {Vsmin=Vs1; }
						else 
					   {
						if(Vw[i]>Vs2) {Vsmin=Vs2; } 
						 else
						  {if(Vs_old[i]>Vw[i]) Vsmin=Vw[i]-1000;  
							  else Vsmin=Vs1;
						  }
					   }
	
				  if((WheelBadFlag[i]==0)&&(Tgr[i]==0))
				  {
				   Kvs[i] = (unsigned int)((Vs_old[i]-Vsmin)/Tnum[i]);
				  }
				  else Kvs[i]=Kv+1;
	
				   ZNum[i]=0;Vw0[i]=Vsmin;
				   Kvs[i]=Kvs[i]+6; 				
				   
				   if(Kvs[i]>48)
					  Kvs[i]=48;
				   if(Kvs[i]<10)
					  Kvs[i]=10;
	
				   if(Tkcn[i+2]<2) 
					{Kvs[i+2]=(Kvs[i]+Kvs[i+2])>>1;
					 //Kvs[i+2]=(Kvs[i]+Kvs[i+2])>>1;
					 Kvh=(unsigned int)((Kvs[2]+Kvs[3])>>1); 
					 Kvh_old=Kvh;
					}
				   
				   Kvq=(unsigned int)((Kvs[0]+Kvs[1])>>1); 
				   //Kv=Kvs[i]; 
				 //  Kvmin=Kvq;
				 //  Kvmin=min(Kvs[0],Kvs[1],Kvs[2],Kvs[3]);
				   Kvq=(unsigned int)((Kvq+Kvq_old)>>1);
				   Kvq_old=Kvq;
	
	
			if(Kvq>30)
			{
			   StepNum1=16;
			   StepNum10=18;
			}
			else if(Kvq>23)
			{
			   StepNum1=18;
			   StepNum10=20;
			}
			else if(Kvq>16)
			{  
			   StepNum1=20;
			   StepNum10=22;
			}
			else if(Kvq>9)
			{  
			   StepNum1=23;
			   StepNum10=25;
			}
			else 
			{
			   StepNum1=25;
			   StepNum10=27;
			}
	
					 if(ZY3[i]==1||ZY3[i]==2||(ZY3[i]==3)||ZY3[i]==5||ZY3[i]==7)
					 { 
						if(Tgr[i]==0)
						{
						 if(Vs_st[i]>Vsck[i]) Vsck[i]=Vs_st[i]; 
						  else 
						  { 
								 if(Jk_gd>1)
								  {Vsck[i]=(Vs_st[i]+Vsck[i])>>1;Vsck[i]=(Vs_st[i]+Vsck[i])>>1;}
								  else
								  if(Jk_gd==0)
								   { Vsck_b=(Vs_st[i]+Vsck[i])>>1; Vsck[i]=( Vsck_b+Vsck[i])>>1;}
								   else 
								   Vsck[i]=(Vs_st[i]+Vsck[i])>>1; 
						  }
					}
					else
						 {Vsck[i]=((unsigned int)(Vs2)+Vsck[i])>>1;}
						 
						if(Tgr[i]==0) Vsmin=(long int)(Vs_st[i]);
						 else Vsmin=(Vstq+Vmax2[0])>>1;
						  
						Vsr=Vsmin;	
	
						if(SRmax[0]<12&&SRmax[1]<12)
						  {  
							   if(Vsmin>Vs) 
								{ if(SR[i]<8) {Vsmin=(Vsmin+Vs)>>1;Vsmin=(Vsmin+Vsr)>>1;}
								   else {Vsmin=(Vsmin+Vs)>>1;}
								}
								else
								 { if(SR[i]<8) Vsmin=(Vsmin+Vs)>>1; 
									 else {Vsmin=(Vsmin+Vs)>>1;Vsmin=(Vsmin+Vsr)>>1;}
								 } 
						  }
						  else 
						  if(SRmax[i]==0)
						   {
							 if((i==0&&SR[1]>10)||(i==1&&SR[0]>10)) Vsmin=(Vsmin+Vs)>>1;
							  else
							  {Vsmin=(Vsmin+Vs)>>1; Vsmin=(Vsmin+Vsr)>>1;}
						   }
						   else  
						   {Vsmin=(Vsmin+Vs)>>1;}
	
						  Vstq=Vsmin;	  
	
					 }
				}
				
				if(Zp[i]<2) Zp[i]=1; //2009.12.3 2->1
					if(Zp[i]>4) Zp[i]=4; //2009.12.3 add
				if(st[i]==3)
				{
				  Tgro=0;
				  ZNum[i]++;
				  if(Tslipk>30)
				   {if(T3[i]<25&&SR[i]<2&&Aw[0]>-380&&Aw[0]<380&&Aw[1]>-380&&Aw[1]<380) Tgro=1;}
				   else
				   {if(T3[i]<15&&SR[i]<2&&Aw[0]>-200&&Aw[0]<260&&Aw[1]>-200&&Aw[1]<260) Tgro=1;}
				  
				 if((Tgro==1)&&(Tzz[i]==0)) {Tzz[i]=1;TStepNum[i]=TStepNum[i]>>1;} //2009.12.14 add
				   
			if((step[i]<=Zp[i]))		//2009.12.8 modify: if((step[i]<=Zp[i])||(Tgro==1))
			  { 
			   step[i]++;
			   //if(Tgro==1&&step[i]>=Zp[i]) step[i]=Zp[i]; //2009.12.14 cut
			   
			   if(step[i]>250) step[i]=250;
			   
				   DI[i] = 0; 
					 
				   if(i==0)
					{
					  if(st[1]==3)	{ DI[0] = 0; Jbk[0]=0;}
					  else 
					  {
					   if(Jbk[0]==2)
						 DI[0] = 1; 
						else  DI[0] = 0;
					  }
					}
				  
					  if(i==1)
					{
					  if(st[0]==3)	{ DI[1] = 0; Jbk[1]=0;}
					  else 
					  {
					   if(Jbk[1]==2)
						DI[1] = 1; 
						 else  DI[1] = 0; 
					  }
					}
	
			  }
			  else
			  {  
			   if(step[i]<TStepNum[i])	   //20
				 {
					step[i]++; if(step[i]>250) step[i]=250;
					DI[i] = 1;
						
				Jbk[i]=2;
				 }
				 else
				 {	TStepNum[i]=StepNum10;
				  step[i] = 0;	   //2	
				  //ZNum[i]++; //2009.12.13 cut
				  Tzz[i]=0; //2009.12.13 add
				  Tkcn[i]++;
						if(SR[i]<-10&&DKF==0) {  Zp[i]++; }
						 else 
						  Zp[i]--;
	
						 if(Zp[i]<2||Kv<15||DKF!=0) //// if(Zp[i]<=1||Kv<15) 
							Zp[i]=1;
	
						if((WheelBadFlag[i]==0)&&(Tgr[i]==0)) 
						{
						 if(Tkcn[i]>3) 
						  { if(Tslipk<36) {Tslipk=36; if(Tkcn[0]>3&&Tkcn[1]>3) Zp[i]=3;}
							if(Kvs[i]<18) Kvs[i]=18;
							if(Kvs[i+2]<15) Kvs[i+2]=15;
						  }
						  else if(Tkcn[i]>2) { if(Kvs[i]<15) Kvs[i]=15;if(Kvs[i+2]<12) Kvs[i+2]=12;}
						   else if(Tkcn[i]>1) { if(Kvs[i]<12) Kvs[i]=12;if(Kvs[i+2]<9) Kvs[i+2]=9;}
	
						 Kvq=(Kvs[0]+Kvs[1])>>1;
						 Kvh=(Kvs[2]+Kvs[3])>>1;
						 
						 if(Tkcn[i]>2&&Tkcn[i]!=5) 
						  {
						  if(Aw[0]>-600&&Aw[0]<600&&Aw[1]>-600&&Aw[1]<600) 
							{
							 if(Vmax2[1]<Vs) 
							   {Vsmin=(Vstq+Vmax2[1])>>1;Vstq=(Vstq+Vsmin)>>1;}
							   else 
							   if(Vmax2[0]>Vs) 
								{Vsmin=(Vstq+Vmax2[0])>>1;Vstq=(Vstq+Vsmin)>>1;}  
								else {Vsmin=(Vmax2[1]+Vmax2[0])>>1;Vstq=(Vstq+Vsmin)>>1;}  
							} 
						  }
	
						 if(Tkcn[i]>5)
						 { 
						  if(Vw[i]<Vs_old[i])
							{ 
							  //if(Vw[i]<Vs1) Vsmin=Vs1; else Vsmin=Vs2; //2012.3.18
	
							if(Vw[i]<Vs1) {Vsmin=Vs1; }
								else 
							  {
							   if(Vw[i]>Vs2) {Vsmin=Vs2; } 
							   else
							   {if(Vs_old[i]>Vw[i]) Vsmin=Vw[i]-1000;  
									  else Vsmin=Vs1;
							   }
							  }
	
	
	
							  Kvq = (unsigned int)((Vs_old[i]-Vsmin)/Tnum[i]);
	
							  if(Kvq>Kvs[i]) {Kvs[i]=(Kvs[i]+Kvq)>>1;}
						 
							  Kvq=(unsigned int)((Kvs[0]+Kvs[1]+8)>>1); 
							  Kvq=(unsigned int)((Kvq+Kvq_old)>>1);
							  Kvq_old=Kvq;
							}  //10.27
						 }
						} 
							
				 }
			  }
				 }	//st[i]=3
			  }
	
			  if(ZY3[i]>1&&SR[i]<5&&DKF==0)    // 9 -- > 5 2006.1.21
			  {
				DI[i]=0;			
				Dzbz[i]++;
				ZNum[i]=0; 
				
					   if(Tgr[i]==0)
						{
						 if(Vs_st[i]>Vsck[i]) Vsck[i]=Vs_st[i]-20; else Vsck[i]=(Vs_st[i]+Vsck[i])>>1;
					}
					else
						{Vsck[i]=((unsigned int)(Vs2)+Vsck[i])>>1;}
			  }
			  else if(Dzbz[i]>0) Dzbz[i]++; 
			  
			  if(Dzbz[i]>60) {Dzbz[i]=0;ZNum[i]=0;ZY3[i]=1;}
			   
			  if(Dzbz[i]>0&&ZY3[i]>1&&SR[i]>8) {  Dzbz[i]=0;DI[i]=1;ZNum[i]=0;ZY3[i]=1;}	   // 14 -- > 8 2006.1.21
	
			  if(DI[i]==0) {if(Tzy[i]<200) Tzy[i]++;} //2009.12.12
	
				  
			   // }
		  break;
	}
		  }//end switch
	
		  if(Vs>800)
		  {  if(SR[i]>80)  //2007.11.22
				WheelBadNum[i]++;
			 else WheelBadNum[i]=0;
		  }
	
		  if(WheelBadFlag[i]==1&&SR[i]<15&&Tawj[i]<100) WheelBadFlag[i]=0;
	
		  Tgro=Tgr[0]+Tgr[1]+Tgr[2]+Tgr[3];
	
		  if(WheelBadNum[i]>300)
		  {  
			 if(Nocontrols[i]<3) Nocontrols[i]=3;
			 Kvs[i]=Kv;
			 if(WheelBadNum[i]>650)  WheelBadNum[i]=650;
			 if(WheelBadNum[i]<480)
			 {
			 WheelBadFlag[i]=1;
			 DI[i]=1;
			 }
	//*
			 else
			 if(WheelBadNum[i]<580&&Vwzo[i]<1&&Tgro<4&&Vmax>2000)
			 {
			  if((i==0&&WheelBadFlag[1]==0)||(i==1&&WheelBadFlag[0]==0))
			  {
			   WheelBadNum[i]=300;
			   DI[i]=3;
			   st[i]=1;
			   step[i] = 0;
			   T1[i] = 0;
			   Vwzo[i]++;
			  }
			 }
	//*/
			 else 
			 {
			  WheelBadFlag[i]=1;
			  DI[i]=0;
			  if(Nocontrols[i]<4) Nocontrols[i]=4;
			 } 
		  }
		} //if(st[i]!=0)
	   }//end for  QZ
	
		if(Tslipk<250&&Tkc[0]==0&&Tkc[1]==0) { if(st[0]==3&&st[1]==3) Tslipk++;}
	 
		for(i=0;i<2;i++)
		{
		 if(st[0]==0||st[1]==0) 			   //2006.6.28
		 {		
		  Tgro==0;
		  if(SR[i]>5&&Awr[i]<AAr) Tgro=1;
		  
		  if(SR[i]>4&&Aw[i]<-3000) Tgro=1;
		  if(SR[i]>5&&Aw[i]<-2200) Tgro=1;
		  if(SR[i]>6&&Aw[i]<-1700) Tgro=1;	 //2009.12	  6->7,-1500->-1700
		  if(SR[0]>5&&Aw[0]<AAr&&SR[1]>5&&Aw[1]<AAr) Tgro=1;   //2009.10	-1000->AA1
	
		  if(Aw[0]<-950&&SR[0]>3&&SR[1]>6&&Aw[1]<-1300) Tgro=1; //2009.10	2->3, -800->-950,  -1200->-1300
		  if(SR[0]>6&&Aw[0]<-1300&&Aw[1]<-950&&SR[1]>3) Tgro=1;   //2009.10  2->3, -800->950,  -1200->-1300
	
		  if(SR[0]<4&&SR[1]<4) Tgro=0; //2009.10  4->5
	
		  if((T0>0)||(Tgro==1))
		  {
		   step[i]=Zp[i];
		   st[i] = 3;
		   if(SR[i]>4) {DI[i]=1;step[i]=Zp[i]+1;} else DI[i]=0; 
		   T3[i]=4;T0=1;
		   if(Tkc[i]==0&&(Tslipk>20||Tslip>50)) Jbcz[i]=2; else Jbcz[i]=3;
	
		  }
		  else { if(Tslipk<250&&i==1) Tslipk++;}
		  
		  //if(i==0) {if(SR[i]<5&&SR[1]<5) {DI[i]=0;}}	 //2006.6.28
		  //if(i==1) {if(SR[i]<5&&SR[0]<5) {DI[i]=0;}}
		  if(SR[0]<5&&SR[1]<5) {DI[i]=0;}	//2009.12.10
	
		  if(SR[0]<-3&&SR[1]<-3) 
		  {
		   if(T0==0)
		   { Tgro=Tgr[0]+Tgr[1];
			if(Tgro==0) {Vsr=(Vw[0]+Vw[1])>>1;Vstq=(Vstq+Vsr)>>1;}
			 else 
			 {if(Tgr[0]==0) {Vstq=(Vstq+Vw[0])>>1;Vsr=Vw[0];} 
			   else if(Tgr[1]==0) {Vstq=(Vstq+Vw[1])>>1;Vsr=Vw[1];}
					 else {Vstq=Vsth;Vsr=Vsth;}
			 }
			 
			 if(Tnum[i]>40)
			 { //Tslip=1; 
			  if(Vs_old[i]>Vsr) 
				Kvq = (unsigned int)((Vs_old[i]+100-Vsr)/Tnum[i]);
			   else Kvq=0;
			   
			   Kvs[i] = (Kvs[i]+Kvq+1)>>1;
							  if(Kvs[i]>45)
								 Kvs[i]=45;
							  if(Kvs[i]<1)
								 Kvs[i]=1;
				  Kvq=(unsigned int)((Kvs[0]+Kvs[1])>>1); 
				  
			   if(Kvs[i]<5&&Tslipk>30) 
				{
	
				  //if(Tgr[i]==0) Jbcz[i]++; 
				  if(Tslipk>50) {Tawj1[i]=111; Jbcz[i]=3;}
	
				  if((Tawj1[0]+Tawj1[1])>200) Tawj1[2]=110;
				}
	
				  Kvq=(unsigned int)((Kvq+Kvq_old)>>1);//((Kv+Kvq_old)>>1);
				  Kvq_old=Kvq;
								 
			 }
		   }
		  }  
		 }
	
		 if(Nocontrols[0]>3&&Nocontrols[1]>3) {DI[i]=0; WheelBadFlag[i]=1;} //2007.11.29
	
		 if(Switch_Flag==0xa5) // added at 2014.2.25
		 {
			if(st[i]==1&&LV_OffRoad_ControlFlag[i]==0) LV_OffRoad_ControlFlag[i]=1; // added at 2014.3.7
			if(LV_OffRoad_ControlFlag[i]==1) // added at 2014.2.27
			{
				if(LV_OffRoad_ByteCount[i]<100) LV_OffRoad_ByteCount[i]++; // added at 2014.2.25
				if(GV_OffRoad_Mode_Flag<2) // added at 2014.2.25,changed at 2014.3.9
				{
					if(LV_OffRoad_ByteCount[i]<40) // added at 2014.2.25
					{
						if((SR[i]<100&&LV_OffRoad_ByteCount[i]<16)||(LV_OffRoad_ByteCount[i]<10)) // added at 2014.2.25
						{
							DI[i]=0; // added at 2014.2.25
							LV_OffRoad_st1_Count[i]=0; // added at 2014.2.25
						}
						else // added at 2014.2.25
						{
							if(LV_OffRoad_st1_Count[i]<100) LV_OffRoad_st1_Count[i]++; // added at 2014.2.25
	//						if(LV_OffRoad_st1_Count[i]>3) // added at 2014.2.25
							{
							DI[i]=1; // added at 2014.2.25
							}
						}
						st[i]=1; // added at 2014.2.27
					}
					else // added at 2014.2.25
					{
						DI[i]=3; // added at 2014.2.25
						LV_OffRoad_ControlFlag[i]=2; // added at 2014.2.25
					}
				}
			  }
	//		  if(st[i]!=1&&SR[i]<20) LV_OffRoad_ByteCount[i]=0; // added at 2014.2.27
			  if(st[i]==3) // added at 2014.2.27
			  {
				if(SR[i]<5) // added at 2014.2.27
				{
					if(Zp[i]<4) Zp[i]=4; // added at 2014.2.27
					LV_OffRoad_ControlFlag[i]=0; // added at 2014.3.7
					LV_OffRoad_ByteCount[i]=0; // added at 2014.3.7
				}
			  }
			  else // added at 2014.2.28
			  {
				if(SR[i]<2&&Vw[i]>2000) // added at 2014.2.28
				{
					if(st[i]>4) st[i]=3; // added at 2014.2.28
				}
				if(Vw[i]<1200||SR[i]>95) // added at 2014.3.9
				{
					st[i]=1; // added at 2014.3.9
					DI[i]=3; // added at 2014.3.9
					if(Tjj[i]<110) Nocontrols[i]=0; // added at 2014.3.10
				}
			  }
		 }
		 Val_Act(i);
		 
		}
	 
	//	WDI=~WDI;  // WDI==1, TLE4271-2G hardware watchdog (30ms)
	
	   Vs=(Vstq+Vsth)>>1;
		
	   for(i=2;i<4;i++) 					   ///// ����1	  
	   { 
	
	   //	Vs=Vsh;
		  Tnum[i]++;
		  
		if(st[i]!=0)   //2006.1.8
		{
	   //	  if(DKF!=0) Zp0[i]=1;	
			if(DKF==1)	{Zp[2]=Zp[3];Zp0[2]=Zp0[3];}
			if(DKF==2)	{Zp[3]=Zp[2];Zp0[3]=Zp0[2]; }
	
			//if(DKF==1)  {Zp[2]=Zp[1];Zp[3]=Zp[1];} 
			//if(DKF==2)  {Zp[3]=Zp[0];Zp[2]=Zp[0];} 
	
		  if(Vw[i]<(Vwo+20)&&WheelBadFlag[i]==0)
		  {  if(TVw[i]<210) TVw[i]++;
		  }
		  else TVw[i]=0;
	
		 if((st[i]>1)&&(Vs>2700))
		 {
		  if((SR[i]>70&&Aw[i]<-1000)||(TVw[i]>20&&TVw[i]<200))
		  {
			 Zp[i]=2;  Zp0[i]=2;
			 TVw[i]=0;
			 Vw1[i]=0;
			 Vw0[i]=Aw[i];
			 if(st[i]==3) Tjj[i]=0;
			 
			if(st[i]!=3)
			   {
				st[i] = 1;
				DI[i] = 3;
	
				T1[i]=0;
				T2[i]=0;
				Tzz[i]=0;
				Tbb[i]=0;
				T3[i]=0;
				step[i] = 0; 
				Jb_bak[i]=0;
			  if(st[i]==3) Tjj[i]=0;
			  ZY3[i]=0;
			  
			  Dzbz[i]=0;
			  Jb[i]=0;
			 }
			 else
			  Jb[i]=1; 
	
		  }
		 }
		 
		  if(st[i]!=3)
		  {  Tst[i]++;
			 if(Tst[i]>550) 
				Tst[i]=550; 
		  }   
		  else Tst[i]=0;
		  if((Tst[i]>400&&Aw[i]<11)||(Tst[i]>500&&Aw[i]>50))
		  {  Tst[i]=0;
			 st[i]=3;
			 DI[i]=0;  
			 Zp0[i]=6;
			 Zp[i]=6;
			 step[i] = 0;
			 ZY3[i]=1;
			 ZNum[i]=0;
	
			 step[i] = 0;
			 T1[i] = 0;
	
		 T2[i]=0;  
		 Tzz[i]=0;
		 Tbb[i]=0;
		 T3[i]=0;
		 Jb_bak[i]=0;
			 Tjj[i]=0;
			 Dzbz[i]=0;
			 Vw1[i]=0;
			 Vw0[i]=Vw[i];
		  }
	
		  if(Tqjs[0]>Tqjs[1]) {Tqjs_k=Tqjs[0];} else {Tqjs_k=Tqjs[1]; } //2009.10
	
		  switch(st[i])
		  { 
			case 1: 			   ////��ѹ-��ѹ
			  //Tzz[i]=0;
			  T1[i]++;
			  //Tjj[i]++; 
	
			  if(Vw[i]<Vsmax_st)
				{
				 if((Tvnold[i]<-1)&&(Aw[i]<-1500)&&(SR[i]>17)) {if(DI[i]==1) DI[i]=3;}
				 //if((Tvnold[i]<-1)&&(SR[i]>50)) {if(DI[i]==1) DI[i]=3;} //2009.12.9
				}
	
			  Tgro=0;
			  if((T1[i]>(T10+20)&&Tbb[i]==0)||(T1[i]>(T11+10)&&Tbb[i]>0)) {Tgro=10;}
			   else 
			   if(T1[i]>2)	//2013.06.30 5->2
				{
				  if(Awr[i]>200&&SR[i]<(SRHF+3)) {Tgro=2;}
				   else
				   if(Awr[i]>150&&SR[i]<20&&Tbb[i]>0) {Tgro=3;}
					else
					if(Awr[i]>300&&SR[i]<(SRby1+2)) {Tgro=4;}		  //SR<(SRby1+3) 2006.3.27
					 else
					 if(Awr[i]>230&&Awr[i]<300&&SR[i]<(SRby1)) {Tgro=5;}
					  else
					  if(Awr[i]>150&&Awr[i]<240&&SR[i]<(SRby1-2)) {Tgro=6;}
					
				  if(Vs>15000&&Awr[i]>-1&&SR[i]<10) {Tgro=7;}  //SR<15 2006.3.27
				   else 
				   if(Vs>13000&&Awr[i]>-1&&SR[i]<11) {Tgro=7;}	//SR<15 2006.3.27
					else 
					if(Vs>11000&&Awr[i]>-1&&SR[i]<12) {Tgro=7;}  //SR<15 2006.3.27
					 else 
					 if(Vs>10000&&Awr[i]>-1&&SR[i]<13) {Tgro=7;}  //SR<15 2006.3.27
					  else 
					  if(Awr[i]>-1&&SR[i]<14) {Tgro=7;}  //SR<15 2006.3.27
					
					if(DI[i]==1&&SR[i]<1) {Tgro=7;}  //2009.12.14 add	
					if(Tqjs_k>(Tqjs[i]+1)) {if(SR[i]<5) Tgro=2;} //2010.1.2
						
				  if((T130[i]>20)&&(SR[i]<16)) {Tgro=7;Tzz[i]=3;} //2009.12.2	
					if((Tzz[i]>0)&&(SR[i]<5)) {Tgro=7;}
	
	//				if(Switch_Flag==0xa5&&GV_OffRoad_Mode_Flag<2) // added at 2014.2.23
	//				{
	//					Tgro=0; // added at 2014.2.23
	//					if(Awr[i]>100&&SR[i]<(SRHF+30)) // added at 2014.2.23
	//					{
	//						Tgro=8; // added at 2014.2.23
	//					}
	//				}
				}
				if(Switch_Flag==0xa5) // added at 2014.3.9
				{
					if(Tgro>0&&SR[i]>95) Tgro=0; // added at 2014.3.9
					if(Tgro>0&&Vw[i]<1200) Tgro=0; // added at 2014.3.9
				}
			  if(Tjj[i]>200) {Tgro=7;Tjj[i]=200;} //2009.12.3 add
				
			  if(Tgro>0)
			  {
				  st[i] = 2;
				  DI[i] = 1;
				  step[i] = 0;
				  T1[i] = 0; 
				  //AwFlag[i]=0;
			  }
		  else
			  {
				
			   if(Awr[i]<-200&&Vw[i]<Awb[i]&&Tbb[i]==0) Awb[i]=Vw[i];
			   
			   if(Vw[i]>(Vwo+100)&&Tbb[i]>0)
				{
				if(Vw[i]>Awb[i]&&Awr[i]>100) 
				 {
				   Awb[i]=Vw[i];
				 }
	/*			   else  //2009.12.15 cut
				  {
				   if(Tvnold[i]<-1) T30[i]++;
				   if(T30[i]>69) T30[i]=69; //2009.6.4 if(T30[i]>70) Tbb[i]=Tbb0;
				  }
	*/
				}  
				
				  
	//			 Awb[i]=Awr[i];
	//			 if(Tkc[i]>=0) //2006.2.12
			   { 
				  //if(Aw[i]>120) step[i]=step[i]+1; else { step[i]=0; DI[i] = 3;} //2006.6.26
				  
				  if(Aw[i]>120) step[i]=step[i]+1; 
				   else 
					{ 
					 if(Tslip>20&&Tslipk>10&&Tbb[i]==0&&Vw[i]>(Awb[i]+120)) {step[i]=step[i]+1;} 
					  else { step[i]=0;if(Aw[i]<-900) DI[i]=3;}
					}
	
	
				  Tkgo=0;
				  if(Aw[i]>2000&&Tslipk>19) Tkgo=1;
	
				  if(Tslip<36&&Tslipk<20)
				   {
					if(Tkc[i]==0&&step[i]>(Jb0[i]+1))  Tkgo=1;
					 else if(Tkc[i]>0)
					  {
						if(Tslipk>12)
						{ 
						 if(Kv>25&&step[i]>1)  Tkgo=1;
						  else if(Kv>20&&step[i]>2)  Tkgo=1;
						   else if(Kv>15&&step[i]>3)  Tkgo=1;
							else if(step[i]>4)	 Tkgo=1;  
						}
						else if(step[i]>5)	 Tkgo=1;  
					  }
				   }
				  else 
				   {if(SR[i]<6&&Tslipk>20&&Aw[i]>-100&&Tjj[i]>Jb0[i]) Tkgo=1;  // add 2006.4.4	
					 if(Kv>25&&step[i]>1)  Tkgo=1;
					  else if(Kv>20&&step[i]>2)  Tkgo=1;
					   else if(Kv>15&&step[i]>3)  Tkgo=1;
						else if(step[i]>4)	 Tkgo=1;  
				   } 
					
				  if(Tkgo==1&&DI[i]==3)
					{	
	
					  DI[i] = 1;
					  step[i]=Jb0[i]+1;
				  }
			   }
	
				  if(SR[i]<-3) DI[i]=1;  //2006.7.14
	
			  }
	
			  if(Tvnold[i]>0) DI[i]=1;
	
				  if(DI[i]==3) Tjj[i]++; 
				  
				  //if(Tjj[i]>250) {st[i]=2;DI[i]=1;Tjj[i]=251;} // DI[i]=1 ---> 0 2006.2.18 
				  
				  if(step[i]>250) step[i]=250; 
			  
		  break;
			case 2: 	//////��ѹ-С������ѹ
			   
				 if(Aw[i]>(BB1-50))
				 T2[i]++;
	
			  if(T2[i]>40) T2[i]=40;
	
			  if(T1[i]<60) T1[i]++;
	
			  
			  //if((Tzz[i]>26&&Aw[i]<300&&SR[i]>20)||(Aw[i]<AA1))
	
			 Tgro=0;													
			 if(Tslipk<15&&T1[i]>26&&Aw[i]<-900&&SR[i]>20) Tgro=1;	  //2009.11 300 -> -200 
			  else 
			  if(Tslipk>14&&T1[i]>30&&Aw[i]<-1000&&SR[i]>20) Tgro=1;	  //2009.10 -300 -> -700 
			   else 
			   if(Aw[i]<AAr&&SR[i]>15) Tgro=1;	//2009.10  -1000->AAr
				else 
				if(T1[i]>50&&Aw[i]<-700&&SR[i]>35) Tgro=1;	//2010.1.14 add
					
			 if((T130[i]>20)&&(SR[i]<21)) {Tgro=0;Tzz[i]=3;} //2009.12.2
				if(Tzz[i]>2)  {Tgro=0;} //2009.12.3 add
			 
			 if(Tqjs_k>(Tqjs[i]+1)) {if((Aw[i]<-300)&&(SR[i]>25)) Tgro=2;} //2010.1.2
			 
			 if(Tgro>0)
			  {st[i] = 1;
				 if(Tvnold[i]<1) DI[i] = 3;
			   step[i] = 0;
			   T1[i]=0;
			   Tzz[i]++;
			  if(Vs>1800) 
				 Tbb[i]++;
			  else 
				 Tbb[i]=Tbb0+1;
			  }
			 
			  if(Awr[i]>BB[i]) BB[i]=Awr[i];   
	
			  Vsr=BB[i]>>1; 
			  if(BB[i]>800)
			  {
			   if(Kv<20) {Vsr=Vsr-250;}
				else if(Kv<23) { Vsr=Vsr-200;}
			  }
	
			 Tgro=0;
	
			   if(Vw[i]>(Vwo+100))
				{
				if(Vw[i]>Awb[i]&&Awr[i]>100) 
				 {
				   Awb[i]=Vw[i];
				 }
				 else
				  {
				   if(Tvnold[i]<-1) T30[i]++;
				   if(T30[i]>10) {if(SR[i]<3) {Tbb[i]=Tbb0+1;Tgro=1;} else {if(T30[i]>20) {Tbb[i]=Tbb0+1;Tgro=1;}}}
				  }
				}  
	
			 if(Tslipk>15&&Aw[i]<Vsr&&T2[i]>2&&SR[i]<SRZY) {Tgro=1;}
			  else
			  if(BB[i]>2000&&Aw[i]<(Vsr-300)&&T2[i]>2&&SR[i]<SRZY) {Tgro=1;}
			   else
			   if(Awr[i]>4000&&T1[i]>1&&SR[i]<SRZY1) 
				{ if(Tslip<35&&Tslipk<20) 
				   {if(Awr[i]<(BB[i]-300)) Tgro=3;} 
				  else {Tgro=3;}
				}
				else
				if(Kv>22&&Aw[i]<(BB1-230)&&T2[i]>1&&SR[i]<(SRZY1+6)) {Tgro=1;}	//+6 -- >+3 2006.4.3
				 else
				 if(Kv>13&&Aw[i]<(BB1-290)&&T2[i]>2&&SR[i]<(SRZY1+3)) {Tgro=1;}  //+6 -- >+3 2006.4.3
				  else
				  if(Kv<16&&Aw[i]<(BB1-380)&&T2[i]>3&&SR[i]<(SRZY1+2)) {Tgro=1;}  //+6 -- >+3 2006.4.3
				
			 if(Aw[i]<250&&T1[i]>6&&SR[i]<(SRZY1)) {Tgro=2;}
			  else
			  if(Awr[i]>-150&&Awr[i]<100&&Tbb[i]>1&&T1[i]>5&&SR[i]<(SRZY+5))  {Tgro=4;}
			   else
				if((Tbb[i]>Tbb0&&T1[i]>3)||(Tbb[i]>2&&Vs<3000&&T1[i]>3))  Tgro=5; 
	
			 if(SR[i]<20)		//2009.12.14 modify
				{if((Tvnold[i]>1)||(Aw[i]>2000))
				 {if(Tanold[i]==0) Tanold[i]=1;}
				}
			 if(SR[i]<5&&Tanold[i]==1) Tanold[i]=2;
			 if(Tvnold[i]<-1&&Tanold[i]>1) {Tbb[i]=Tbb0+1;Tgro=5;}
	
			  if(SR[i]<20)
				{
				  if(i==2) 
					{if(T130[i]>30) {Tbb[i]=Tbb0+1;Tgro=5;} 
					  else if((T130[i]>20)&&(T130[3]>25)) {Tbb[i]=Tbb0+1;Tgro=5;}}//2009.12.1
				  if(i==3) 
					{if(T130[i]>30) {Tbb[i]=Tbb0+1;Tgro=5;}
					  else if((T130[i]>20)&&(T130[2]>25)) {Tbb[i]=Tbb0+1;Tgro=5;}} //2009.12.1
				}
	
			  if(Tqjs_k>(Tqjs[i]+1)) {if(SR[i]>13) Tgro=0;} //2010.1.2
			  //if(Tvnold[i]>20) Tgro=0; //2012.3.12
				if(Tvnold[i]>6) Tgro=0; //2012.3.12
		
			  if( Tgro>0)
			  {   BB[i]=0;
					T1[i]=0;T2[i]=0;  Tzz[i]=0;Tbb[i]=0;T3[i]=0;step[i] = 0; Jb_bak[i]=0;
				  Tjj[i]=0;Dzbz[i]=0;ZY3[i]=0; Tzzo[i]=0;
				  //if(Zp[2]<Zp[3]) Zp[3]=Zp[2]; else Zp[2]=Zp[3]; //2009.12.13 add
				  if(Tkc[i]<4) Tkc[i]++;
				  
				  if(Tqjs[i]<230) Tqjs[i]++;		  //2009.10. 2009.11
				  if(Tqjs_k>(Tqjs[i]+1)) {Tqjs[i]=Tqjs_k;}	 //2009.10
					
		if(Tslip>20&&Tkcqh>(Tslipk+7))
		{
		 Tgro=0;
		 if(Tkc[i]==1)
		 {
		  if(Taw200[i]<6) {if(Kvs[i]<30) {Kvh=30;Tgro=1;}}	
		   else if(Taw200[i]<9) {if(Kvs[i]<26) {Kvh=26;Tgro=1;}}
			else if(Taw200[i]<12) {if(Kvs[i]<22) {Kvh=22;Tgro=1;}}
			 else if(Taw200[i]<15) {if(Kvs[i]<18) {Kvh=18;Tgro=1;}}
		  
		   if(Tgro==1) {for(j=0;j<SS;j++) {Kvs[j]=Kvh;} Kvh_old=Kvh; Kvq=Kvh; Kvq_old=Kvq;}
			
		 }	
		}
	
		 
			 Taw200[i]=0;
	
	
				 {
					st[i]=3;
					DI[i]=0;
					
	
					step[i] = 0; 
	
				 //   if(Vs>500)
					  {
						Vsr=Vs2;
	
						 Tgro=Tgr[0]+Tgr[1]+Tgr[2]+Tgr[3];
	
						 if(Tgro<4)
						  {
						   if(Vsr<=Vmax) Vsmin=Vmax;
							else 
							 Vsmin=(Vsr+Vmax)>>1;  
						  }
						  else Vsmin=Vs;
	
	
						Vsr=Vsmin;
	
						if(SRmax[2]<12&&SRmax[3]<12)
						  {if(T30[i]<8) Vsmin=(Vsmin+Vsth)>>1;}
						  else if(SRmax[2]>20&&SRmax[3]>20)
							{
							 if(Vsth<Vs)
							  {if(Vsmin>Vs&&Kv<15) Vsmin=Vsr; else {if(T30[i]<8) Vsmin=(Vsr+Vs)>>1;}}
								else {if(Vsmin>Vsth&&Kv<15) Vsmin=Vsr; else {if(T30[i]<8) Vsmin=(Vsr+Vsth)>>1;}}
							}
							else if(Kv<15&&(SRmax[2]==0||SRmax[3]==0))
								   {Vsmin=(Vsr+Vs)>>1; Vsmin=(Vsmin+Vs)>>1; }
								  else
								   {if(T30[i]<8) Vsmin=(Vsr+Vs)>>1;}
	
						if(Tkcn[i]>2&&Kv<13&&Vsmin<Vs) {Vsmin=(Vsr+Vsmin)>>1;}
	
						SRmax[i]=0;
						//Tkcn[i]=0;
						
						  //Vsmin=(Vsmin+Vs)>>1;   //2005.12.21
						
						 if(Vw1[i]>300) Vw1[i]=300;
						 if(Vw1[i]<0) Vw1[i]=0;
						 
						 //Vsth=(Vsmin-Vw1[i]);  //2006.4.2
	
						 if(Tslip<10&&Tnum[i]<20&&Tkc[i]<2&&Tslipk<10)
						 {
							Vsth=(Vsmin-Vw1[i]);	
						 }
						 else
						 {
						  Vsth=(Vsmin-Vw1[i]);	//2006.4.2
						 
						  if(WheelBadFlag[i]==0&&Tgr[i]==0) 
						   {
							if(Vsmin>Vs_old[i])
							{
							  if(Tslip>16) Kvs[i]=(Kvs[i]+15)>>1; else Kvs[i]=(Kvs[i]+10)>>1;
							  if(Tvnold[i]>25) Kvh=(Kvh+Kvs[i])>>1;
							}
							else
							if(Vsmin<Vs_old[i])
							{
							  Kvh = (unsigned int)((Vs_old[i]-Vsth)/Tnum[i]);
							}  //10.27
						   }else Kvs[i]=Kv+1;	 
	
						   if(Tvnold[i]>20) 
							{
							 if(Kvh_old>18){Kvs[i]=Kvh; Kvh_old=Kvh;}//2009.5.22
							}
							
							  //Kvs[i] = (Kvs[i]+Kvh)>>1;
							  if(Tkcn[i]>2&&Vsmin<Vs)  Kvs[i] = (Kvs[i]+Kvh+5)>>1;
								else  Kvs[i] = (Kvs[i]+Kvh)>>1;
							   
							  if(Tkc[i]<2)
							  {  
								//if(Vw[0]<1200&&Vw[1]<1200&&Kvs[i]>13) Kvs[i]=13;
								if(i==2&&SR[3]<20&&SR[0]<20&&SR[1]<20&&Kv<20) {Kvs[i]=(Kvs[i]+20)>>1; Kv=Kvs[i];}
								if(i==3&&SR[2]<20&&SR[0]<20&&SR[1]<20&&Kv<20) {Kvs[i]=(Kvs[i]+20)>>1; Kv=Kvs[i];}	 
							  }
							  
							   
							  if(Kvs[i]>48)
								 Kvs[i]=48;
							  if(Kvs[i]<1)
								 Kvs[i]=1;
							  
							  Kvh=(unsigned int)((Kvs[2]+Kvs[3])>>1); 
							  
							 // Kvmin=min(Kvs[0],Kvs[1],Kvs[2],Kvs[3]);
							 
							  Kvh=(unsigned int)((Kvh+Kvh_old)>>1);//((Kv+Kvh_old)>>1);
							  Kvh_old=Kv;
							 // Kvh=Kvh;
							  Tnum[i]=0;
							  Tanold[i]=0;Tvnold[i]=0;
							  
							  if(Tgr[i]==0)
							   {
								 if(Vs_st[i]>Vsck[i]) Vsck[i]=Vs_st[i]; 
								  else 
								  { 
								   if(Jk_gd>1)
									{Vsck[i]=(Vs_st[i]+Vsck[i])>>1;Vsck[i]=(Vs_st[i]+Vsck[i])>>1;}
									else
									if(Jk_gd==0)
									 { Vsck_b=(Vs_st[i]+Vsck[i])>>1; Vsck[i]=( Vsck_b+Vsck[i])>>1;}
									 else 
									 Vsck[i]=(Vs_st[i]+Vsck[i])>>1; 
								  }
						   }
						   else
							   {Vsck[i]=((unsigned int)(Vs2)+Vsck[i])>>1;}
							  
							  
							  Vs_old[i]=Vsmin;
					
				if(Kvh>30)
					{
					   StepNum2=16;
					   StepNum20=18;
					}
					else if(Kvh>23)
					{
					   StepNum2=18;
					   StepNum20=20;
					}
					else if(Kvh>16)
					{  
					   StepNum2=21;
					   StepNum20=23;
					}
					else if(Kvh>9)
					{  
					   StepNum2=24;
					   StepNum20=26;
					}
					else 
					{
					   StepNum2=27;
					   StepNum20=29;
					}
						 }
						 
						 Tkcn[i]=0;
					  }
				 }	
	
			  // if(DKF!=0) {StepNum1=30;StepNum2=40; StepNum10=25;StepNum20=25;}  //2006.1.16
	
			   TStepNum[i]=StepNum2;
			   Tzy[i]=0; //2009.12.12
			   }
		   break;
		   
		case 3:    ///////////////// С������ѹ-->��ѹ
	
			  T3[i]++;
			  if(T3[i]>200) T3[i]=200;
	
			  Tgro=0;
			  if(Tkc[i]<4&&Aw[i]<(AA1-2300)&&SR[i]>1) Tgro=1;  //2009.10 -2600
	
			  if((Aw[i]<AA1&&SR[i]>(SRJY+4))||(Awr[i]<AAr&&SR[i]>(SRJY+4))) Tgro=1;
			  if(Aw[i]<(AA1-1500)&&SR[i]>(SRJY+1)) Tgro=1;
			  if((Aw[i]<(AA1-2000)&&SR[i]>10&&Vs>6000)||(Vs<6001&&SR[i]>11&&Awr[i]<(AA1-2800))) Tgro=1; //2009.10 -2000,-3000
			  if((Awr[i]<AAr&&SR[i]>12&&Dzbz[i]>0)||(Jb[i]==1)) Tgro=1;    //2009.10  -1500 ->AAr
			  if(Switch_Flag==0xa5&&GV_OffRoad_Mode_Flag<2) // added at 2014.2.23
			  {
				Tgro=0; // added at 2014.2.23
				if((Aw[i]<AA1&&SR[i]>30)||(Awr[i]<AAr&&SR[i]>25)) Tgro=1; // added at 2014.2.23
				if((SR[i]>60)&&(Awr[i]<-100)) Tgro=2; // added at 2014.3.10
			  }
			  if(Tgro>0&&T3[i]>2)	//04.01.01			 ////////// 05.11.6
			  {
				  Awb[i]=Vw[i];
				  //Tkcn[i]=Tkcn[i]+ZNum[i];
	   
				  Dzbz[i]=0;  
				  Jb_bak[i]=Jb[i];
				  Jb[i]=0;
				  T3[i] = 0; 
				  T1[i] = 0; 
				  T2[i]=0;
				  Tzz[i]=0;
				  st[i] = 1;
					  LV_OffRoad_ByteCount[i]=0; // added at 2014.2.25
				  DI[i] = 3;
				  step[i] = 0;
				  Vw1[i]=0;
				  Vw0[i]=Aw[i];
				  T30[i]=0;
				  T130[i]=0;
				  
				  if((ZY3[i]>1)||((ZY3[2]>0)&&(ZY3[3]>0)))
				  {
					 if(Zp0[i]<3)  Zp0[i]=3;
					  else Zp0[i]++;											 
				  }
				  else if((ZY3[i]==1)||(ZNum[i]>3))  //// else if(ZNum[i]==4)
				  { 
					 if(Zp0[i]<2)	Zp0[i]=2;
					  else Zp0[i]++;										   
				  }
				  else if(ZNum[i]==3)
				  { 
					 Zp0[i]++;
					 if(Zp0[i]>3)	Zp0[i]=3;
				  }
				  else if(ZNum[i]==2)
				  { 
					 Zp0[i]=2;
				  }
				  else if(ZNum[i]<2)
				  {
					Zp0[i]--;
					 if(Zp0[i]<1) Zp0[i]=1;  //2009.12.3 : 2-->1
				  }
	
					if(Zp0[i]>4)   Zp0[i]=4;
					Zp[i]=Zp0[i];
				  
				  if(DKF!=0)	//// Add at 2004.8.26
				  {
					 if(Zp[i]>2)
					 { Zp[i]=2;
					   Zp0[i]=2;
					 }	   
				  }
				  if(DKF==1)  {Zp[2]=Zp[3];Zp0[2]=Zp0[3];}
				  if(DKF==2)  {Zp[3]=Zp[2];Zp0[3]=Zp0[2]; }
	
				  ZY3[i]=0;
				  ZNum[i]=0;
			  }
			  else if(Dzbz[i]==0)
			  { //ZY4[i]=0; 
				if(ZNum[i]==0)
				   if(Vs1>Vw[i]) Vw0[i]=Vs1;  else Vw0[i]=Vs2;
				   
				if(ZNum[i]==6) //2009.12.13 5->6
				{  ZY3[i]++;	if(ZY3[i]>10) ZY3[i]=10;
				   if(Tqjs[i]<230) Tqjs[i]++;
				  //Tkcn[i]=Tkcn[i]+5;
	
				  if(i==3&&DKF==1&&ZY3[1]>0) DKF=0;
				   if(i==2&&DKF==1&&(ZY3[1]>0||ZNum[1]>3)) DKF=0;
				  
				  if(i==2&&DKF==2&&ZY3[0]>0) DKF=0;
				   if(i==3&&DKF==2&&(ZY3[0]>0||ZNum[0]>3)) DKF=0;
	
	
				  if(DKF!=0&&ZY3[i]>2&&SR[i]>6) ZY3[i]=1;
	
				  if(DKF==0&&ZY3[i]>0)
				   {
					if(Vs>1200)
					 {
					  if(SR[i]>10) Zp[i]=1;else if(SR[i]>4) Zp[i]=2;else if(SR[i]>-2) Zp[i]=2;else	Zp[i]=3;
	//					if(SR[i]>10) Zp[i]=2;else if(SR[i]>4) Zp[i]=2;else if(SR[i]>-2) Zp[i]=3;else  Zp[i]=4;
					 }
					 else 
					  Zp[i]=2;
	
			   //	   Zp[i]=2;
					 Zp0[i]=Zp[i]; 
				   }
	
	//				 if(Vs1>Vw[i]) Vsmin=Vs1;  else Vsmin=Vs2; //2012.3.18
	
					 if(Vw[i]<Vs1) {Vsmin=Vs1; }
						else 
					   {
						if(Vw[i]>Vs2) {Vsmin=Vs2; } 
						 else
						  {if(Vs_old[i]>Vw[i]) Vsmin=Vw[i]-1000;  
							  else Vsmin=Vs1;
						  }
					   }
	
				  ZNum[i]=0;
				  if((WheelBadFlag[i]==0)&&(Tgr[i]==0)) 
				  {
				   Kvs[i] = (unsigned int)((Vs_old[i]-Vsmin)/Tnum[i]);
				  } else Kvs[i]=Kv+1;
	
				   Kvs[i]=Kvs[i]+6;
				   Vw0[i]=Vsmin;
				   if(Kvs[i]>48)
					  Kvs[i]=48;
				   if(Kvs[i]<10)
					  Kvs[i]=10;
				   Kvh=(unsigned int)((Kvs[2]+Kvs[3])>>1); 
				   //Kv=Kvs[i]; 
				   //Kvh=Kvh;
				   //Kvh=min(Kvs[0],Kvs[1],Kvs[2],Kvs[3]);
				   Kvh=(unsigned int)((Kvh+Kvh_old)>>1);
				   Kvh_old=Kvh;
	 
			if(Kvh>30)
			{
			   StepNum2=17;
			   StepNum20=19;
			}
			else if(Kvh>23)
			{
			   StepNum2=19;
			   StepNum20=21;
			}
			else if(Kvh>16)
			{  
			   StepNum2=22;
			   StepNum20=24;
			}
			else if(Kvh>9)
			{  
			   StepNum2=25;
			   StepNum20=27;
			}
			else 
			{
			   StepNum2=29;
			   StepNum20=31;
			}
	
					/////if(Vs<4200) ZYhz=1;	   //// Modify at 2004.8.26
					if((((ZY3[i]==1)||ZY3[i]==2||(ZY3[i]==3)||ZY3[i]==4||ZY3[i]==6)&&ZYhz==0)||((ZY3[i]==1||ZY3[i]==3||ZY3[i]==5)&&ZYhz==1))
					{
					   if(Tgr[i]==0)
						{
						 if(Vs_st[i]>Vsck[i]) Vsck[i]=Vs_st[i]-20; 
						  else 
						  { 
								 if(Jk_gd>1)
								  {Vsck[i]=(Vs_st[i]+Vsck[i])>>1;Vsck[i]=(Vs_st[i]+Vsck[i])>>1;}
								  else
								  if(Jk_gd==0)
								   { Vsck_b=(Vs_st[i]+Vsck[i])>>1; Vsck[i]=( Vsck_b+Vsck[i])>>1;}
								   else 
								   Vsck[i]=(Vs_st[i]+Vsck[i])>>1; 
						  }
					}
					else
						 {Vsck[i]=((unsigned int)(Vs2)+Vsck[i])>>1;}	
											
						Vsr=Vs2;
	
						 Tgro=Tgr[0]+Tgr[1]+Tgr[2]+Tgr[3];
	
						 if(Tgro<4)
						  {
						   if(Vsr<=Vmax) {Vsmin=(Vmax+Vs2)>>1;Vsmin=(Vmax+Vsmin)>>1;}
							else 
							 Vsmin=(Vsr+Vmax)>>1;  
						  }
						  else Vsmin=Vs;
	
						if(ZY3[i]==2||ZY3[i]==4||ZY3[i]==6) Vsmin=(Vsmin+Vsmin+Vsmin+Vs)>>2; 
						  else 
						  {
							if(SR[i]<20) Vsmin=(Vsmin+Vsmin+Vs)/3; 
							  else	Vsmin=(Vsmin+Vs)>>1; 
						  }
	
						Vsth=Vsmin;
					}
			  }
	
			 // 	  if(DKF!=0) Zp0[i]=1;	
			 
				if(Zp[i]<2) Zp[i]=1;  //2009.12.3 2->1
					if(Zp[i]>4) Zp[i]=4; //2009.12.3 add
				if(st[i]==3)
				{
				 if(step[i]<=Zp[i]) 	   ///// ��  1,8   ��	1 ��15
			  {
				 step[i]++; if(step[i]>250) step[i]=250;
				DI[i] = 0;	
				if(Tzz[i]==0) {Tzz[i]=1;ZNum[i]++;} //2009.12.13 add
		   
			  }
			  else
			  {  
					 if(step[i]<TStepNum[i])		///20  
				 {	
						
				step[i]++; if(step[i]>250) step[i]=250;
				DI[i] = 1;
	
				  //	 if(SR[i]<1||Aw[i]>-600)
				  //	   DI[i] = 0;
	
				 }
				 else
				 {	TStepNum[i]=StepNum20;
					  step[i] = 0;	   //2	
						//ZNum[i]++;  //2009.12.13 cut
						Tzz[i]=0;  //2009.12.13 add
						Tkcn[i]++;
	
						if(SR[i]<-10&&DKF==0) Zp[i]++;
						 else 
						  Zp[i]--;
	
						 if(Zp[i]<2||Kv<17||DKF!=0) //// if(Zp[i]<=1||Kv<17) 
							Zp[i]=1;
	
					   
						if((WheelBadFlag[i]==0)&&(Tgr[i]==0))
						{
						 if(Tkcn[i]>3) 
						  {
							if(Kvs[i]<18) Kvs[i]=18; 
							if(Tslipk<36) Tslipk=36;
							if(Kv<18) {Kvq=18;Kvh=18; Kvq_old=18;Kvh_old=18;}
						  }
						  else if(Tkcn[i]>2) { if(Kvs[i]<14) Kvs[i]=14;}
						   else if(Tkcn[i]>1) { if(Kvs[i]<10) Kvs[i]=10;}
							
						 Kvh=(Kvs[2]+Kvs[3])>>1;
						 
						 if(Tkcn[i]>2&&Tkcn[i]!=5) 
						  {
						   if(Aw[0]>-600&&Aw[0]<600&&Aw[1]>-600&&Aw[1]<600) 
							{
							 if(Vmax<Vs) 
							   {Vsmin=(Vstq+Vmax)>>1;Vstq=(Vstq+Vsmin)>>1;}
							   else 
							   if(Vs2>Vs) 
								{Vsmin=(Vstq+Vs2)>>1;Vstq=(Vstq+Vsmin)>>1;}  
								else {Vsmin=(Vmax+Vs2)>>1;Vstq=(Vstq+Vsmin)>>1;}  
							} 
						  }
										
						 if(Tkcn[i]>5)
						 { 
												
						  if(Vw[i]<Vs_old[i])
							{ 
							  //if(Vw[i]<Vs1) Vsmin=Vs1; else Vsmin=Vs2; //2012.3.18
	
							if(Vw[i]<Vs1) {Vsmin=Vs1; }
								else 
							  {
							   if(Vw[i]>Vs2) {Vsmin=Vs2; } 
							   else
							   {if(Vs_old[i]>Vw[i]) Vsmin=Vw[i]-1000;  
									  else Vsmin=Vs1;
							   }
							  }
	
	
							  Kvh = (unsigned int)((Vs_old[i]-Vsmin)/Tnum[i]);
	
							  if(Kvh>Kvs[i]) 
							  {Kvs[i]=(Kvs[i]+Kvh)>>1;}
							  
							   Kvh=(unsigned int)((Kvs[0]+Kvs[1]+7)>>1); 
							   Kvh=(unsigned int)((Kvh+Kvh_old)>>1);
							   Kvh_old=Kvh;
							  
							}  //10.27
						 }
						} 
	
						//Zp[i]=1; 
				 }
			  }
				} //st=3	  
			  }
	
			  if(ZY3[i]>2&&SR[i]<5&&DKF==0)   // 8 -- > 5 2006.1.21
			  {
				DI[i]=0;			
				Dzbz[i]++;
				ZNum[i]=0; 
				
				if(Tgr[i]==0)
				 {
				   if(Vs_st[i]>Vsck[i]) Vsck[i]=Vs_st[i]-20; else Vsck[i]=(Vs_st[i]+Vsck[i])>>1;
			 }
			 else
				  {Vsck[i]=((unsigned int)(Vs2)+Vsck[i])>>1;}
			  }else if(Dzbz[i]>0) Dzbz[i]++; 
			  
			  if(Dzbz[i]>60) {Dzbz[i]=0;ZNum[i]=0;ZY3[i]=2;}
			   
			  if(Dzbz[i]>0&&ZY3[i]>2&&SR[i]>8) {  Dzbz[i]=0;DI[i]=1;ZNum[i]=0;ZY3[i]=2;}   // 14 -- > 8 2006.1.21
			  
			  if(DI[i]==0) {if(Tzy[i]<200) Tzy[i]++;} //2009.12.12 
		  break;
		  }//end switch
	
		  if(Vs>800)
		  {  if(SR[i]>90)//2007.11.22
				WheelBadNum[i]++;
			 else WheelBadNum[i]=0;
		  }
	
		  if(WheelBadFlag[i]==1&&SR[i]<15&&Tawj[i]<100) WheelBadFlag[i]=0;
	
		  Tgro=Tgr[0]+Tgr[1]+Tgr[2]+Tgr[3];
	
		  if(WheelBadNum[i]>260)
		  {  
	
			 Kvs[i]=Kv;
			 if(WheelBadNum[i]>650)  WheelBadNum[i]=650;
			 if(WheelBadNum[i]<420)
			 {
			 WheelBadFlag[i]=1;
			 DI[i]=1;
			 }
	//*
			 else
			 if(WheelBadNum[i]<480&&Vwzo[i]<1&&Tgro<4&&Vmax>2000)
			 {
			  if((i==2&&WheelBadFlag[3]==0)||(i==3&&WheelBadFlag[2]==0))
			  {
			   WheelBadNum[i]=340;
			   DI[i]=3;
			   st[i]=1;
			   step[i] = 0;
			   T1[i] = 0;
			   Vwzo[i]++;
			  }
			 }
	//*/
			 else 
			 {
			  WheelBadFlag[i]=1;
			  DI[i]=0;
			  if(Nocontrols[i]<4) Nocontrols[i]=4;	//2008.6.16
			 } 
		  }
	
		}  // if(st[i]!=0)
	   }//end for HZ
	
	
	   for(i=2;i<4;i++)
		{
		  if(st[2]==0||st[3]==0)				/////��ѹ-��ѹ
		  {
	
		   if(SR[2]<-3&&SR[3]<-3) 
		   { Tgro=Tgr[2]+Tgr[3];
			if(Tgro==0) {Vsmin=(Vw[2]+Vw[3])>>1;Vsth=(Vsth+Vsmin)>>1;}
			 else {if(Tgr[2]==0) Vsth=(Vsth+Vw[2])>>1; else if(Tgr[3]==0) {Vsth=(Vsth+Vw[3])>>1;}}
			if(Tho==0&&Tnum[i]>20)
			{  
			  Kvh = (unsigned int)((Vs_old[i]-Vsth)/Tnum[i]);
			  Kvs[i] = (Kvs[i]+Kvh+1)>>1;
							  if(Kvs[i]>45)
								 Kvs[i]=45;
							  if(Kvs[i]<1)
								 Kvs[i]=1;
				 Kvh=(unsigned int)((Kvs[2]+Kvs[3])>>1); 
				 Kvh=(unsigned int)((Kvh+Kvh_old)>>1);//((Kv+Kvh_old)>>1);
				 Kvh_old=Kvh;
			}
		   }
	
		  if(SR[i]>5&&Awr[i]<AAr) Tho=1;	 //2009.10	  AAr->AA1	   
		  if(SR[i]>4&&Aw[i]<-2000) Tho=1;	 //2009.10	  -2000->-2300
		  if(SR[i]>6&&Aw[i]<-1650) Tho=1;  ////2009.10	 -1300,-1500
		  if(SR[i]>8&&Aw[i]<-1300) Tho=1;  ////2009.10	 -800,-1300
		  if(SR[i]>15&&Aw[i]<-1000) Tho=1;	////2009.10   -800,-1300
		  if(SR[i]>20&&Aw[i]<-800) Tho=1;  ////2009.10	 -800,-1300
			
		 if(SR[2]>5&&Aw[2]<-1000&&SR[3]>5&&Aw[3]<-1000) Tho=1;	////2009.10   -500,-1000
		 
		   if(Tho==1)				 /////��ѹ-��ѹ
		   {
			 Tho=1;
	//		 if(SR[i]>2) /////// 05.11.8	
			 if(Aw[i]<AA1&&SR[i]>4) /////// 05.11.25	
			 { st[i] = 1;  
			   DI[i] = 3;
			   if(Kvs[i]>25) Kvs[i]=25; 	 //2005.11.25
			 }
			 else
			 if(Aw[2]>-750&&Aw[3]>-750)  ///5	-1500	/////// 05.11.8
			 {
				   st[i] = 3;	 T3[i]=3;
				   if(SR[i]>5) {DI[i]=1;step[i]=Zp[i]+1;} else {DI[i]=0; step[i]=Zp[i];}
				   if(Kvs[i]<25) Kvs[i]=25; 	 //2005.11.25
			  }
			  else 
			  if(Aw[i]>-1300)  
			  {
			   st[i]=3;   T3[i]=3;
			   if(SR[i]>6) {DI[i]=1;step[i]=Zp[i]+1;} else {DI[i]=0; step[i]=Zp[i];}
			   if(Kvs[i]<20) Kvs[i]=20; 	 //2005.11.25
			  }
			  else
			  { st[i] = 3;	T3[i]=3;
				DI[i] = 0;
				if(Kvs[i]<20) Kvs[i]=20;	  //2005.11.25
			  }
		   }else { if(Tkcqh<250&&i==1) Tkcqh++;}
	
		   if(i==2) {if(SR[i]<3&&SR[3]<6) {DI[i]=0;}}
		   if(i==3) {if(SR[i]<3&&SR[2]<6) {DI[i]=0;}}
			
		  }
	
		 if(Switch_Flag==0xa5) // added at 2014.2.25
		 {
			if(st[i]==1&&LV_OffRoad_ControlFlag[i]==0) LV_OffRoad_ControlFlag[i]=1; // added at 2014.3.7
			if(LV_OffRoad_ControlFlag[i]==1) // added at 2014.2.27
			{
				if(LV_OffRoad_ByteCount[i]<100) LV_OffRoad_ByteCount[i]++; // added at 2014.2.25
				if(GV_OffRoad_Mode_Flag<2) // added at 2014.2.25,changed at 2014.3.9
				{
					if(LV_OffRoad_ByteCount[i]<40) // added at 2014.2.25
					{
						if((SR[i]<100&&LV_OffRoad_ByteCount[i]<16)||(LV_OffRoad_ByteCount[i]<10)) // added at 2014.2.25
						{
							DI[i]=0; // added at 2014.2.25
							LV_OffRoad_st1_Count[i]=0; // added at 2014.2.25
						}
						else // added at 2014.2.25
						{
							if(LV_OffRoad_st1_Count[i]<100) LV_OffRoad_st1_Count[i]++; // added at 2014.2.25
	//						if(LV_OffRoad_st1_Count[i]>3) // added at 2014.2.25
							{
							DI[i]=1; // added at 2014.2.25
							}
						}
						st[i]=1; // added at 2014.2.27
					}
					else // added at 2014.2.25
					{
						DI[i]=3; // added at 2014.2.25
						LV_OffRoad_ControlFlag[i]=2; // added at 2014.2.25
					}
				}
			  }
	//		  if(st[i]!=1&&SR[i]<20) LV_OffRoad_ByteCount[i]=0; // added at 2014.2.27
			  if(st[i]==3) // added at 2014.2.27
			  {
				if(SR[i]<5) // added at 2014.2.27
				{
					if(Zp[i]<4) Zp[i]=4; // added at 2014.2.27
					LV_OffRoad_ControlFlag[i]=0; // added at 2014.3.7
					LV_OffRoad_ByteCount[i]=0; // added at 2014.3.7
				}
			  }
			  else // added at 2014.2.28
			  {
				if(SR[i]<2&&Vw[i]>2000) // added at 2014.2.28
				{
					if(st[i]>4) st[i]=3; // added at 2014.2.28
				}
				if(Vw[i]<1200||SR[i]>95) // added at 2014.3.9
				{
					st[i]=1; // added at 2014.3.9
					DI[i]=3; // added at 2014.3.9
					if(Tjj[i]<120) Nocontrols[i]=0; // added at 2014.3.10
				}
			  }
		 }
		 Val_Act(i);
	
		}
	
	  }///end else 
	}




/////////////////////////////////////////////////////////////////  wheel

/////////////////////////////////////////
      //wheel speed filter  subroutine
 ////////////////////////////////////////
 
void wheel(void)
{
  char i=0; 

  for(i=0;i<4;i++)
  {
    filter(i);

    if(Vw[i]<=Vwo)      ////////////////////////////// Add at 2004.11.19
    { Vw[i]=Vwo; }
    ////////////////////////////////////////////////// Add at 2004.11.18
    if(ctrlflag==0&&End_Break==0)
    {
     if(Vw[i]<=Vwo){ Wheel_Value[i]=0; } 
     else
     { //Wheel_Value[i]=(unsigned int)(Vw[i]/3); // Vw[i]*3384/10000 ~= Vw[i]/3
       Wheel_Value[i]=(unsigned int)(Vw[i]*5/14); // 07.8.30
       if(Wheel_Value[i]<400){ Wheel_Value[i]=Wheel_Value[i]+38; }   // 07.9.8
     }
    }
  }
}  

void filter(unsigned char No)
{
	 long int filte[3]={0,0,0},Vnew=958;
	 long int count_now[4]={0,0,0,0};				 
	 unsigned char i=0,Ktss=11,zeroflag=0;
	 unsigned int Vnewf=958,Vnewfo=958;
	 signed int SRst=0;
	
	 for(i=0;i<3;i++)
	 {
	   count_now[i]=count[No][i];
	   if(count_now[i]!=count[No][i])
		 count_now[i]=count[No][i];
	   if(count_now[i]==0) { count_now[i]=65535; }
	 }
	  
	 if(count_now[0]<count_now[1])
	 { filte[0]=count_now[1]; }
	 else
	 { filte[0]=count_now[0]; }
	 if(filte[0]<count_now[2])
	 { filte[0]=count_now[2]; }//取大值
	 //if(filte[0]<count_now[3])
	 //{ filte[0]=count_now[3]; } 
	
	 if(count_now[0]>count_now[1])
	 { filte[1]=count_now[1]; filte[2]=count_now[0];}//filte[1]取小值，filte[2]取大值
	 else
	 { filte[1]=count_now[0]; filte[2]=count_now[1];}
	 if(filte[1]>count_now[2])
	 { filte[2]=filte[1];filte[1]=count_now[2]; }
	 else 
	 {
	  if(filte[2]>count_now[2]) { filte[2]=count_now[2]; }
	 }
	 if(filte[0]==filte[2]) {filte[2]=(filte[2]+filte[1])>>1;}
	 if(filte[1]==filte[2]) {filte[2]=(filte[2]+filte[0])>>1;}
	 
	 //if(filte[1]>count_now[3])
	 //{ filte[1]=count_now[3]; } 
	
	 if(ABS_Valves_Flag!=0&&ctrlflag==0)  //07.10.10;08.3.31
	 {
	  if(filte[0]<41000) 
	  {
	   if(filte[0]<count_now[3]) {filte[0]=count_now[3];}
	  }
	 
	
	  zeroflag=0;
	  for(i=0;i<4;i++)
	  {
	   if(count_now[i]>65150)
	   { zeroflag++; }
	  }
	
	  if(zeroflag>0)
	  {////////////////////////2007.9.15
	   if(filte[0]<41000) { filte[0]=41000; } //41000=1531;65146=964; 65212=963;   65348=961; //65416=960;
	   if(filte[0]>65150) { if(ABS_Valves_Flag!=0){ filte[0]=65150; } }//08.3.31
	   if(filte[2]<41000) { filte[2]=41000; }
	   if(filte[1]<41000) { filte[1]=41000; }
	  }
	 }
	 
	 zeroflag=0;
	 for(i=0;i<4;i++)
	 {
	   if(count_now[i]==65535)
	   { zeroflag++; }
	 }
	
	 //if(zeroflag>1) {filte[0]=65535;filte[1]=65535;filte[2]=65535;} // 65536=>958=>Vwo;	  2007.9.15
	 if(zeroflag>1) {filte[0]=65535;} // 65536=>958=>Vwo;	  2007.9.15
	 //filte[1]=min(count[No][0],count[No][1],count[No][2],count[No][3]);
	
	////////////////////////////////////////////////////
	
	 if(ABS_Valves_Flag!=0&&ctrlflag==0)  //07.9.14;08.3.31 
	 {
	   if((filte[1]*13)>(filte[0]*10))
		{ counter[No]=(filte[2]+filte[0])>>1; } 
		else 
		{ counter[No]=filte[0]; } 
	 }
	 else
	 { counter[No]=filte[0]; }
	 
	/////////////////////////////////////////////////////
	  
	 if(counter[No]==65535)
	 {	
	   Vnew=(long int)(XS/counter[No]);  //Vw[No]=1000*(PI*D*F)/(Z*counter[No]);   Vnew=Vwo;
	   Vwo=Vnew;
	   Vw[No]=Vnew;
	   Vnewf=(unsigned int)(Vnew);
	//	 if(Diagnose_Data[No+1]!=0) Tgr[No]=1;	////////////////// Add at 2005.1.18
	 }
	 else
	 { 
	//	 Vnew=(long int)(10000*XS/counter[No]);  //Vw[No]=(PI*D*F)/(Z*counter[No]);
	   if(count[No][3]<count_base)
		{Vnew=(long int)(XS/counter[No]);}	//Vw[No]=1000*(PI*D*F)/(Z*counter[No]);
		else {Vnew=(long int)(XS_ds/count[No][3]);}
	   
	   //if(No==0&&Vnew<4200) Vnew=Vwo;
	   //if(No==0&&Vnew<5200) {if(Tjxc<121) Tjxc++; if(Tjxc==120) Tjxc=0; if(Tjxc>60) Vnew=Vwo;} //miss 
			
	   ///////////////////////////////////////////Add on 08.7.5
	   if(Tsliph>39)
	   {//if(Vnew>(Vw[No]<<1)||(Vnew<Vw[No]>>1))
		if(ctrlflag==0)///////////08.7.5-2
		{if(Vnew>(Vw[No]<<1))
		 { counter_Time[No]++;
		   if(counter_Time[No]>9)//9:50ms,08.7.5
		   { counter_Time[No]=0;
			 //Vw[No]=(Vnew+Vw[No])>>1;////Move on 08.7.5
		   }
		   else
		   { Vnew=Vw[No]; }
		 }
		 else{ counter_Time[No]=0; } 
		}
		else///////////Add on 08.7.5-3;2->1,3->8,08.10.20
		{if(Vnew>(Vw[No]<<2))	//2009.10.23  1 ->2
		 { counter_Time[No]++;
		   if(counter_Time[No]>0)//3:20ms
		   { counter_Time[No]=0;
			 //Vw[No]=(Vnew+Vw[No])>>1;////Move on 08.7.18
		   }
		   else
		   { Vnew=Vw[No]; }
		 }
		 else{ counter_Time[No]=0; } 
		}
	   }
	   ////////////////////////////////////////////
	
	   if(Vnew>64000) Vnew=64000;
	   if(Vnew<Vwo) Vnew=Vwo;
	
	   
	   Vnewf=(unsigned int)(Vnew);
	
	   if(Tsliph<40) 
		{ Tgr[No]=0;
		  Vw[No]=Vnew; 
		  Vnold[No]=Vnew; 
		  Vs_st[No]=Vnewf;
		  Vsm_st[No]=Vnewf;
		  Vsck[No]=Vs_st[No]; 
		}
	 
	   
		if(ctrlflag==0)
		{
		  
		  if((Vnew-Vnold[No])<7001)  ///// 2800-->5800 ,05.11.6 
			{
			  Vw[No]=(Vw[No]+Vnew)>>1;
			  Vnold[No]=Vnew; 
			  Tgr[No]=0;
	
			}
		   else if((Vnew-Vnold[No])>7000)  ///// 2800-->5800 ,05.11.6 
		   {
			Tgr[No]=1;
			
			  if(Vnew<=Vs1)
			  {
			   Vw[No]=(Vnew+Vw[No])>>1;
			   Vnold[No]=(Vnold[No]+Vw[No])>>1;
			  }
			  else 
			   {
				if(Vnew>Vw[No]) 
				 Vw[No]=Vw[No]+70;
				 else  Vw[No]=(Vnew+Vw[No])>>1;
				Vnold[No]=(Vnold[No]+40);  //=Vs1  2005.1.2
			   }
			if(Vw[No]<Vwo) Vw[No]=Vwo; 
		   }
		 //if(Vw[No]>((long int)(Vs_st[No])+1000)) Vw[No]=(long int)(Vs_st[No])+1000;	//2005.12.5
		}
		else
		{
		   if((Vnew-Vnold[No])<7001)  ///// 2800-->5800 ,05.11.6 
		   {
			Tgr[No]=0;
			 {
			  Vw[No]=(Vw[No]+Vnew)>>1;
			  Vnold[No]=Vnew; 
			  if(Vw[No]<Vwo) Vw[No]=Vwo; 
			 } 
		   }
		   else if(((Vnew-Vnold[No])>7000)&&(Vnew<(Vsao+5800)))  ///// 2800-->5800 ,05.11.6 
		   {
			Tgr[No]=1;
	
			if(Vw[No]<Vs2) 
			 {
			   Vw[No]=Vw[No]+70;  //=Vs1  2005.1.2
			   Vnold[No]=(Vnold[No]+50);
			 }
			 else
			 {
			   Vw[No]=Vw[No]+80;
			   Vnold[No]=(Vnold[No]+60);
			 }
			if(Vw[No]<Vwo) Vw[No]=Vwo; 
		   }
		   else 
		   {
			Tgr[No]=1;
	
			Vw[No]=Vw[No]+70;
			Vnold[No]=Vnold[No]+60;
	
			if(Vw[No]<Vwo) Vw[No]=Vwo; 
		   }
		}
	
		//if(Tawj[No]>99) Tgr[No]=1;   //2007.1.27
	
	 }
	
	   Vnewfo=Vs_st[No];
	   if((ctrlflag==1)||(End_Break==1))
	   {
		if(End_Break==0)
		{
		 if(st[No]==3&&Tgr1[No]==1) 
		  {
			Tgr1[No]=0;
			Ktss=6;
			if(Vnewf<(Vssou+800))
			{
			 if(Vw[No]<Vnew) Vsm_st[No]=(unsigned int)(Vw[No]+300); 
			  else Vsm_st[No]=Vnewf+300;
			}
			else  Vsm_st[No]=Vssou+300; 
		  }
		 if(st[No]!=3) Tgr1[No]=1;
		}
		else
		 {
		  if(Tgr1[No]==1) 
		   {
			Tgr1[No]=0;
			Ktss=6;
			if(Vnewf<(Vssou+800))
			{
			 if(Vw[No]<Vnew) Vsm_st[No]=(unsigned int)(Vw[No]+300); 
			  else Vsm_st[No]=Vnewf+300;
			}
			else  Vsm_st[No]=Vssou+300; 
		  }
		 }
	
			 
		if(Vnewf<=Vssou) 
		  {Ts_st[No]=0;if(Vnewf>(Vs_st[No]+200))  Vs_st[No]=Vnewf;}
		 else 
		 {	
			Ts_st[No]++;
			if(Vnewf<Vsm_st[No]) 
			{ if(Vnewf<(Vssou+300)) Vs_st[No]=Vnewf; }
		 } 
	
	
	//////////////////////////// 2006.2.17
		 Ktss=6;
		 if(Vnewf>(Vssou+2300)&&Ktss<14) Ktss=14;
		   else if(Vnewf>(Vssou+1800)&&Ktss<13) Ktss=13;
			else if(Vnewf>(Vssou+1300)&&Ktss<12) Ktss=12;
			 else if(Vnewf>(Vssou+800)&&Ktss<11) Ktss=11;
			  else if(Vnewf>(Vssou+300)&&Ktss<10) Ktss=10;
			   else if(Vnewf>(Vssou)&&Ktss<8) Ktss=8;
	
	
	   if(Vssou>20000) {Vs_w=420;Ktss=Ktss-6;}
		else if(Vssou>18000) {Vs_w=390;Ktss=Ktss-6;}
		 else if(Vssou>16000) {Vs_w=360;Ktss=Ktss-5;}
		  else if(Vssou>14000) {Vs_w=320;Ktss=Ktss-3;}
		   else if(Vssou>12000) {Vs_w=280;Ktss=Ktss-2;}
			else if(Vssou>10000) {Vs_w=240;Ktss=Ktss-1;}
			 else if(Vssou>8000) {Vs_w=200;}
			  else if(Vssou>6000) {Vs_w=165;}
			   else if(Vssou>4500) {Vs_w=135;}			
				else if(Vssou>3500) {Vs_w=110;}
				 else if(Vssou>2500) {Vs_w=90;}
				  else Vs_w=70;
							  
		 if(Vnewf<Vs_st[No]) 
		  {
			 if(Vnew>Vw[No]) 
			 { 
			  Vnewf=(unsigned int)(Vw[No]);
			 } 
	
			 Vnewfo=Vs_st[No];
			 Vs_st[No]=Vnewf;
			 //Vsm_st[No]=Vnewf;
			 Ts_st[No]=0;
		  }
		  else if(Ts_st[No]>Ktss) 
		  {
			 if(Vnewf>(Vssou+Vs_w)) 	 //300-->Vs_w  2006.2.19
			 { 
			  Vnewf=Vssou+Vs_w;
			 } 
	
			 Vnewfo=Vs_st[No];
			 Vs_st[No]=Vnewf;
			 Vsm_st[No]=Vnewf;
			 Ts_st[No]=0;
		  }
	
	
		if(End_Break==1) Vsck[No]=Vs_st[No];	   //2006.1.7
		 
	   }
	   else
	   {
		if(Vsm_st[No]>Vnewf) 
		  { if((Vsm_st[No]-Vnewf)>800) {Ts_st[No]=0; Vsm_st[No]=Vs_st[No];}}
	
		if(Vnewf>(Vs_st[No]+300)) 
		 {
			Ts_st[No]++; 
			if(Vnewf>Vsm_st[No]) {Vsm_st[No]=Vnewf; }
			 else 
			  { 
			   if((Vsm_st[No]-Vnewf)>700) {Ts_st[No]=0;Vsm_st[No]=Vs_st[No];}
			  }
		 }
		 else {Ts_st[No]=0;Vsm_st[No]=Vs_st[No];}		//2005.11.30
	
		 Vsck[No]=Vs_st[No];	   //2005.12.4
		 
		if(Ts_st[No]>15||Vnewf<Vs_st[No]||Vnew<Vs) 
		 {
			if(Vnew>Vw[No]) Vs_st[No]=(unsigned int)(Vw[No]); else Vs_st[No]=Vnewf;
			 Vsm_st[No]=Vs_st[No];
			 Ts_st[No]=0;
		 } 
	
	   }
	   
	
	
			 if(Vnewfo==Vs_st[No]) 
			  {
				if(Kgvw[No]<100) {Kgvw[No]++; if(Kgvw[No]>98) Kgvw[No]=98;} else Kgvw[No]=1;
			  }
			  else	if(Vs_st[No]<Vnewfo) 
			   {
				if(Kgvw[No]>100&&Kgvw[No]!=250) {Kgvw[No]++; if(Kgvw[No]>200) Kgvw[No]=200;} else Kgvw[No]=101;
			   }
			   else 
				 Kgvw[No]=250;
	   
		 if(Diagnose_Data[No+1]!=0) {Tgr[No]=1;Nocontrols[No]=16;}	///// Add at 2005.6.4;60->30,07.9.10
	
		//if(Diagnose_Data[No+7]!=0) { Tgr[No]=1;Nocontrols[No]=1;}  ////////////////// Add at 2005.2.18
		
		if(Nocontrols[No]>10) Tgr[No]=1; 
	 
	 /*=====================================*/
	   // wheel circle deformity diagnose
	/*=====================================*/
	 /////////////////////////////////////////////////////////// Move on 07.10.16
	 /*
	 if(Vs1>8000&&ctrlflag==0&&End_Break==0&&unctrl==0)
	 {
	  if(filte[1]>=count_max)
	  {
		if(count[No][1]>6*count[No][2]||6*count[No][1]<count[No][2])
		{  
		   circle_badflag[No]++;
			if(circle_badflag[No]>=100) circle_badflag[No]=101;  //// Modify at 05.2.25
	 
		   if(circle_Tflag>=200)  // (200: 1s(20:1/10))  // Modify at 2004.11.26
		   {  
			  circle_Tflag=0; 
			  if(circle_badflag[No]>=100)	//// (40:1/5) 50:1/4 100:1/2  //// Modify at 05.2.25
	//			   wheel_bad[No]=1; 		 //wheel circle bad flag
			   {  wheel_bad[No]=1; Nocontrols[No]=30;}		   //wheel circle bad flag;60->30,07.9.10
			  else
				 wheel_bad[No]=0;		   //Add at 2005.1.12
			  circle_badflag[No]=0;
		   }
		}					
	  }
	 }
				 
	 if(wheel_bad[0]==1) //wheel circle bad flag (foreside axis left) 
	 {	 
	   Diagnose_Write(1,0x0400);
	   Error_Flash[4]=0x0062;		  // Flash Code
	   Error_Flash[0]=0x0062;		  // Current Error Flash Code
	   Diagnose_Data[0]=0x0107;   // Current Error
	
	 }
	 if(wheel_bad[1]==1)  //wheel circle bad flag (foreside axis right)   
	 { 
	   Diagnose_Write(2,0x0400);
	   Error_Flash[8]=0x0061;		  // Flash Code
	   Error_Flash[0]=0x0061;		  // Current Error Flash Code
	   Diagnose_Data[0]=0x0207;   // Current Error
	
	 }
	 if(wheel_bad[2]==1) //wheel circle bad flag (behind axis left)   
	 { 
	   Diagnose_Write(3,0x0400);
	   Error_Flash[12]=0x0064;		   // Flash Code
	   Error_Flash[0]=0x0064;		  // Current Error Flash Code
	   Diagnose_Data[0]=0x0307;   // Current Error
	
	 }
	 if(wheel_bad[3]==1) //wheel circle bad flag (behind axis right)   
	 {
	   Diagnose_Write(4,0x0400);
	   Error_Flash[16]=0x0063;		   // Flash Code
	   Error_Flash[0]=0x0063;		  // Current Error Flash Code
	   Diagnose_Data[0]=0x0407;   // Current Error
	
	 } 
	 */ 
	
}




/******************************************
         bt_int()  interrupt subroutine
*******************************************/
void bt_int(void)    // M30290 have Add on 07.9.28
{ unsigned char j=0;
	count_base_flag++;
  if(count_base_flag==1)  
  	{count_base_flag=0;count_base=count_base_q;count_base_q=0;}

  for(j=0;j<4;j++)
 {
    if(count[j][3]>count_base) //////Add on 08.5.6
    { count_temp[0]=0;
    	count[j][2]=0;
      count[j][1]=0;
      count[j][0]=0;
    }
 }
}
#if 0
/******************************************
         icoc0_int()  interrupt subroutine
*******************************************/
void icoc0_int(void) //////////////////// M30290 have Add on 07.9.28
{ if(g1ir0==1) ///////////////////////// ch-0
  { g1ie0&=0xf7;
    No_WheelandOver_Flag[0]=1;
    g1ir0=0;   // clear interrupt flag
    old_tr[0]=new_tr[0];
    new_tr[0]=g1tm0;
    //new_tr[0]=(unsigned int)(g1tm0);//Modify on 08.3.3
    T_tire[0]++;

    if(count[0][3]<count_base) //////Add on 08.5.6
      { if(new_tr[0]>old_tr[0])////Add on 08.4.22
    	  { count_temp[0]=new_tr[0]-old_tr[0]; }
      	else
        if(new_tr[0]<old_tr[0])////Add on 08.5.6
    	   { count_temp[0]=0xffff-old_tr[0];
           count_temp[0]=count_temp[0]+new_tr[0];
         }
         else {count_temp[0]=65535;}
      }
    else if(count[0][3]==count_base)///////////// Base Timer over flow
     {
      if(new_tr[0]<old_tr[0])////Add on 08.5.6
    	   { count_temp[0]=0xffff-old_tr[0];
           count_temp[0]=count_temp[0]+new_tr[0];
         }
         else {count_temp[0]=65535;}
     }
     else
     { count_temp[0]=0;
       count[0][2]=0;
       count[0][1]=0;
       count[0][0]=0; 
     }

    count[0][3]=0;
    
    if(count_temp[0]>count_max)//&&count_temp[]<62800,Add on 08.3.6;Modify on 08.4.22
    {   //count[0][3]=count[0][2];
        count[0][2]=count[0][1];
        count[0][1]=count[0][0];
        count[0][0]=count_temp[0];
    }
    
    if(ctrlflag==0&&End_Break==0&&count_temp[0]>count_max)
    { 
      Wheel_Time_Count[0]=max(count[0][0],count[0][1],count[0][2],count_temp[0]);
      //Wheel_Time_Count[0]=(unsigned int)((count[0][0]+count[0][1]+count[0][2]+count[0][3])>>2); 
      //if(Wheel_Time_Count[0]<=10000)  // 50Hz(ZZ==100,f==0.5MHz)
      if(Wheel_Time_Count[0]<40000&&Vw[0]>1300)  // 40Hz(ZZ==100,f==2MHz)
      {
        m1_tb[0]++;
        if(m1_tb[0]>ZZ){ m1_tb[0]=0; }
      }
      else
      { m1_tb[0]=0;  
        Sensor_Value[0]=0;
      }
    }
    else 
    { m1_tb[0]=0; }
         
    if(count[0][0]>65534){ m1_tb[0]=0; Sensor_Value[0]=0; }  
    
    g1ie0|=0x08;
  }
  
  if(g1ir3==1) ///////////////////////// ch-3
  { g1ie0&=0xfe;
    No_WheelandOver_Flag[3]=1;
    g1ir3=0;   // clear interrupt flag
    old_tr[3]=new_tr[3];
    new_tr[3]=g1tm3;
    //new_tr[3]=(unsigned int)(g1tm3);//Modify on 08.3.3
    T_tire[3]++;
    if(count[3][3]<count_base) //////Add on 08.5.6
      { if(new_tr[3]>old_tr[3])////Add on 08.4.22
    	 { count_temp[3]=new_tr[3]-old_tr[3]; }
      	else
        if(new_tr[3]<old_tr[3])////Add on 08.5.6
    	   { count_temp[3]=0xffff-old_tr[3];
           count_temp[3]=count_temp[3]+new_tr[3];
         }
         else {count_temp[3]=65535;}
      }
    else if(count[3][3]==count_base)///////////// Base Timer over flow
     {
      if(new_tr[3]<old_tr[3])////Add on 08.5.6
    	   { count_temp[3]=0xffff-old_tr[3];
           count_temp[3]=count_temp[3]+new_tr[3];
         }
         else {count_temp[3]=65535;}
     }
     else
     { count_temp[3]=0;
    	 count[3][2]=0;
       count[3][1]=0;
       count[3][0]=0;
     }

    count[3][3]=0;
           
    if(count_temp[3]>count_max)//&&count_temp[]<62800,Add on 08.3.6;Modify on 08.4.22
    {   //count[3][3]=count[3][2];
        count[3][2]=count[3][1];
        count[3][1]=count[3][0];
        count[3][0]=count_temp[3];
    }
    
    if(ctrlflag==0&&End_Break==0&&count_temp[3]>count_max)
    { 
      Wheel_Time_Count[3]=max(count[3][0],count[3][1],count[3][2],count_temp[3]);
      //Wheel_Time_Count[3]=(unsigned int)((count[3][0]+count[3][1]+count[3][2]+count[3][3])>>2); 
      //if(Wheel_Time_Count[3]<=10000)  // 50Hz(ZZ==100,f==0.5MHz)
      if(Wheel_Time_Count[3]<40000&&Vw[3]>1300)  // 40Hz(ZZ==100,f==2MHz)
      {
        m1_tb[3]++;
        if(m1_tb[3]>ZZ){ m1_tb[3]=0; }
      }
      else
      { m1_tb[3]=0;  
        Sensor_Value[3]=0;
      }
    }
    else 
    { m1_tb[3]=0; }
         
    if(count[3][0]>65534){ m1_tb[3]=0; Sensor_Value[3]=0; }  
    
    g1ie0|=0x01;
  }
  
}

/******************************************
         icoc1_int()  interrupt subroutine
*******************************************/
void icoc1_int(void) //////////////////// M30290 have Add on 07.9.28
{ 
  if(g1ir1==1) ///////////////////////// ch-1
  { g1ie1&=0xfb;
    No_WheelandOver_Flag[1]=1;
    g1ir1=0;   // clear interrupt flag
    old_tr[1]=new_tr[1];
    new_tr[1]=g1tm1;
    //new_tr[1]=(unsigned int)(g1tm1);//Modify on 08.3.3
    T_tire[1]++;

    if(count[1][3]<count_base) //////Add on 08.5.6
      { if(new_tr[1]>old_tr[1])////Add on 08.4.22
    	  { count_temp[1]=new_tr[1]-old_tr[1]; }
      	else
        if(new_tr[1]<old_tr[1])////Add on 08.5.6
    	   { count_temp[1]=0xffff-old_tr[1];
           count_temp[1]=count_temp[1]+new_tr[1];
         }
         else {count_temp[1]=65535;}
      }
    else if(count[1][3]==count_base)///////////// Base Timer over flow
     {
      if(new_tr[1]<old_tr[1])////Add on 08.5.6
    	   { count_temp[1]=0xffff-old_tr[1];
           count_temp[1]=count_temp[1]+new_tr[1];
         }
         else {count_temp[1]=65535;}
     }
     else
     { count_temp[1]=0;
       count[1][2]=0;
       count[1][1]=0;
       count[1][0]=0;
     }

    count[1][3]=0;
    
    if(count_temp[1]>count_max)//&&count_temp[]<62800,Add on 08.3.6;Modify on 08.4.22
    {   //count[1][3]=count[1][2];
        count[1][2]=count[1][1];
        count[1][1]=count[1][0];
        count[1][0]=count_temp[1];
    }
    
    if(ctrlflag==0&&End_Break==0&&count_temp[1]>count_max)
    { 
      Wheel_Time_Count[1]=max(count[1][0],count[1][1],count[1][2],count_temp[1]);
      //Wheel_Time_Count[1]=(unsigned int)((count[1][0]+count[1][1]+count[1][2]+count[1][3])>>2); 
      //if(Wheel_Time_Count[1]<=10000)  // 50Hz(ZZ==100,f==0.5MHz)
      if(Wheel_Time_Count[1]<40000&&Vw[1]>1300)  // 40Hz(ZZ==100,f==2MHz)
      {
        m1_tb[1]++;
        if(m1_tb[1]>=(ZZ+1)){ m1_tb[1]=0; }
      }
      else
      { m1_tb[1]=0;  
        Sensor_Value[1]=0;
      }
    }
    else 
    { m1_tb[1]=0; }
         
    if(count[1][0]>65534){ m1_tb[1]=0; Sensor_Value[1]=0; }  
    
    g1ie1|=0x04;
  }

  if(g1ir2==1) ///////////////////////// ch-2
  { g1ie1&=0xfd;
    No_WheelandOver_Flag[2]=1;
    g1ir2=0;   // clear interrupt flag
    old_tr[2]=new_tr[2];
    new_tr[2]=g1tm2;
    //new_tr[2]=(unsigned int)(g1tm2);//Modify on 08.3.3
    T_tire[2]++;
    if(count[2][3]<count_base) //////Add on 08.5.6
      { if(new_tr[2]>old_tr[2])////Add on 08.4.22
    	  { count_temp[2]=new_tr[2]-old_tr[2]; }
      	else
        if(new_tr[2]<old_tr[2])////Add on 08.5.6
    	   { count_temp[2]=0xffff-old_tr[2];
           count_temp[2]=count_temp[2]+new_tr[2];
         }
         else {count_temp[2]=65535;}
      }
    else if(count[2][3]==count_base)///////////// Base Timer over flow
     {
      if(new_tr[2]<old_tr[2])////Add on 08.5.6
    	   { count_temp[2]=0xffff-old_tr[2];
           count_temp[2]=count_temp[2]+new_tr[2];
         }
         else {count_temp[2]=65535;}
     }
     else
      { count_temp[2]=0;
        count[2][2]=0;
        count[2][1]=0;
        count[2][0]=0;
      }

    count[2][3]=0;
    
    if(count_temp[2]>count_max)//&&count_temp[]<62800,Add on 08.3.6;Modify on 08.4.22
    {   //count[2][3]=count[2][2];
        count[2][2]=count[2][1];
        count[2][1]=count[2][0];
        count[2][0]=count_temp[2];
    }
    
    if(ctrlflag==0&&End_Break==0&&count_temp[2]>count_max)
    { 
      Wheel_Time_Count[2]=max(count[2][0],count[2][1],count[2][2],count_temp[2]);
      //Wheel_Time_Count[2]=(unsigned int)((count[2][0]+count[2][1]+count[2][2]+count[2][3])>>2); 
      //if(Wheel_Time_Count[2]<=10000)  // 50Hz(ZZ==100,f==0.5MHz)
      if(Wheel_Time_Count[2]<40000&&Vw[2]>1300)  // 40Hz(ZZ==100,f==2MHz)
      {
        m1_tb[2]++;
        if(m1_tb[2]>=(ZZ+1)){ m1_tb[2]=0; }
      }
      else
      { m1_tb[2]=0;  
        Sensor_Value[2]=0;
      }
    }
    else 
    { m1_tb[2]=0; }
         
    if(count[2][0]>65534){ m1_tb[2]=0; Sensor_Value[2]=0; }  
    
    g1ie1|=0x02;
  }
} 
#else

/***********************************************************************************************************************
* Function Name: r_Config_TAUD0_5_interrupt
* Description  : This function is TAUD05 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma interrupt r_Config_TAUD0_5_interrupt(enable=manual)
void r_Config_TAUD0_5_interrupt(void)
{
    //if (TAUD0.CSR5 & _TAUD_OVERFLOW_OCCURS)    /* overflow occurs */
    //{
    //    g_taud0_ch5_width = (uint32_t)(TAUD0.CDR5 + 1U) + _TAUD_OVERFLOW_VALUE;
    //}
    //else
    //{
    //    g_taud0_ch5_width = (uint32_t)(TAUD0.CDR5 + 1U);
    //}
	
    /* Start user code for r_Config_TAUD0_5_interrupt. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
	
	//if(g1ir3==1) ///////////////////////// ch-3
  //{ g1ie0&=0xfe;
    No_WheelandOver_Flag[3]=1;
  //  g1ir3=0;   // clear interrupt flag
    old_tr[3]=new_tr[3];
    new_tr[3]=TAUD0.CDR5;
    //new_tr[3]=(unsigned int)(g1tm3);//Modify on 08.3.3
    T_tire[3]++;
    if(count[3][3]<count_base) //////Add on 08.5.6
      { if(new_tr[3]>old_tr[3])////Add on 08.4.22
    	 { count_temp[3]=new_tr[3]-old_tr[3]; }
      	else
        if(new_tr[3]<old_tr[3])////Add on 08.5.6
    	   { count_temp[3]=0xffff-old_tr[3];
           count_temp[3]=count_temp[3]+new_tr[3];
         }
         else {count_temp[3]=65535;}
      }
    else if(count[3][3]==count_base)///////////// Base Timer over flow
     {
      if(new_tr[3]<old_tr[3])////Add on 08.5.6
    	   { count_temp[3]=0xffff-old_tr[3];
           count_temp[3]=count_temp[3]+new_tr[3];
         }
         else {count_temp[3]=65535;}
     }
     else
     { count_temp[3]=0;
    	 count[3][2]=0;
       count[3][1]=0;
       count[3][0]=0;
     }

    count[3][3]=0;
           
    if(count_temp[3]>count_max)//&&count_temp[]<62800,Add on 08.3.6;Modify on 08.4.22
    {   //count[3][3]=count[3][2];
        count[3][2]=count[3][1];
        count[3][1]=count[3][0];
        count[3][0]=count_temp[3];
    }
    
    if(ctrlflag==0&&End_Break==0&&count_temp[3]>count_max)
    { 
      Wheel_Time_Count[3]=max(count[3][0],count[3][1],count[3][2],count_temp[3]);
      //Wheel_Time_Count[3]=(unsigned int)((count[3][0]+count[3][1]+count[3][2]+count[3][3])>>2); 
      //if(Wheel_Time_Count[3]<=10000)  // 50Hz(ZZ==100,f==0.5MHz)
      if(Wheel_Time_Count[3]<40000&&Vw[3]>1300)  // 40Hz(ZZ==100,f==2MHz)
      {
        m1_tb[3]++;
        if(m1_tb[3]>ZZ){ m1_tb[3]=0; }
      }
      else
      { m1_tb[3]=0;  
        Sensor_Value[3]=0;
      }
    }
    else 
    { m1_tb[3]=0; }
         
    if(count[3][0]>65534){ m1_tb[3]=0; Sensor_Value[3]=0; }  
    
  //  g1ie0|=0x01;
  //}
}

/***********************************************************************************************************************
* Function Name: r_Config_TAUD0_7_interrupt
* Description  : This function is TAUD07 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma interrupt r_Config_TAUD0_7_interrupt(enable=manual)
void r_Config_TAUD0_7_interrupt(void)
{
    //if (TAUD0.CSR7 & _TAUD_OVERFLOW_OCCURS)    /* overflow occurs */
    //{
    //    g_taud0_ch7_width = (uint32_t)(TAUD0.CDR7 + 1U) + _TAUD_OVERFLOW_VALUE;
    //}
    //else
    //{
    //    g_taud0_ch7_width = (uint32_t)(TAUD0.CDR7 + 1U);
    //}
    /* Start user code for r_Config_TAUD0_7_interrupt. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */

	//if(g1ir0==1) ///////////////////////// ch-0
	//{ 
	//	g1ie0&=0xf7;
		No_WheelandOver_Flag[0]=1;
	//	g1ir0=0;   // clear interrupt flag
		old_tr[0]=new_tr[0];
		new_tr[0]=TAUD0.CDR7;
		//new_tr[0]=(unsigned int)(g1tm0);//Modify on 08.3.3
		T_tire[0]++;

		if(count[0][3]<count_base) //////Add on 08.5.6
		{ 
			if(new_tr[0]>old_tr[0])////Add on 08.4.22
				{ count_temp[0]=new_tr[0]-old_tr[0]; }
			else if(new_tr[0]<old_tr[0])////Add on 08.5.6
				{ 
				count_temp[0]=0xffff-old_tr[0];
				count_temp[0]=count_temp[0]+new_tr[0];
				}
			else {count_temp[0]=65535;}
		}
		else if(count[0][3]==count_base)///////////// Base Timer over flow
		{
			if(new_tr[0]<old_tr[0])////Add on 08.5.6
			{ 
				count_temp[0]=0xffff-old_tr[0];
				count_temp[0]=count_temp[0]+new_tr[0];
			}
			else {count_temp[0]=65535;}
		}
		else
		{ 
			count_temp[0]=0;
			count[0][2]=0;
			count[0][1]=0;
			count[0][0]=0; 
		}

		count[0][3]=0;

		if(count_temp[0]>count_max)//&&count_temp[]<62800,Add on 08.3.6;Modify on 08.4.22
		{   //count[0][3]=count[0][2];
			count[0][2]=count[0][1];
			count[0][1]=count[0][0];
			count[0][0]=count_temp[0];
		}

		if(ctrlflag==0&&End_Break==0&&count_temp[0]>count_max)
		{ 
			Wheel_Time_Count[0]=max(count[0][0],count[0][1],count[0][2],count_temp[0]);
		//Wheel_Time_Count[0]=(unsigned int)((count[0][0]+count[0][1]+count[0][2]+count[0][3])>>2); 
		//if(Wheel_Time_Count[0]<=10000)  // 50Hz(ZZ==100,f==0.5MHz)
		if(Wheel_Time_Count[0]<40000&&Vw[0]>1300)  // 40Hz(ZZ==100,f==2MHz)
		{
		m1_tb[0]++;
		if(m1_tb[0]>ZZ){ m1_tb[0]=0; }
		}
		else
		{ m1_tb[0]=0;  
		Sensor_Value[0]=0;
		}
		}
		else 
		{ 
			m1_tb[0]=0; 
		}

		if(count[0][0]>65534)
			{ m1_tb[0]=0; Sensor_Value[0]=0; }  

	//	g1ie0|=0x08;
	//}
	
}

/***********************************************************************************************************************
* Function Name: r_Config_TAUD0_9_interrupt
* Description  : This function is TAUD09 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma interrupt r_Config_TAUD0_9_interrupt(enable=manual)
void r_Config_TAUD0_9_interrupt(void)
{
	unsigned char i=0;
	
    //if (TAUD0.CSR9 & _TAUD_OVERFLOW_OCCURS)    /* overflow occurs */
    //{
    //    g_taud0_ch9_width = (uint32_t)(TAUD0.CDR9 + 1U) + _TAUD_OVERFLOW_VALUE;
    //}
    //else
    //{
    //    g_taud0_ch9_width = (uint32_t)(TAUD0.CDR9 + 1U);
    //}
    /* Start user code for r_Config_TAUD0_9_interrupt. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */

	//TAUD0.TT |=0x0200;
	//if(g1ir2==1) ///////////////////////// ch-2
  //{ g1ie1&=0xfd;
	No_WheelandOver_Flag[2]=1;
	//   g1ir2=0;   // clear interrupt flag
	old_tr[2]=new_tr[2];
	new_tr[2]=TAUD0.CDR9;


	//new_tr[2]=(unsigned int)(g1tm2);//Modify on 08.3.3
	T_tire[2]++;

	if(count[2][3]<count_base) //////Add on 08.5.6
	{ 
		if(new_tr[2]>old_tr[2])////Add on 08.4.22
		{ 
			count_temp[2]=new_tr[2]-old_tr[2]; 
		}
		else if(new_tr[2]<old_tr[2])////Add on 08.5.6
		{ 
			count_temp[2]=0xffff-old_tr[2];
			count_temp[2]=count_temp[2]+new_tr[2];
		}
		else 
		{
			count_temp[2]=65535;
		}
	}
    else if(count[2][3]==count_base)///////////// Base Timer over flow
	{
		if(new_tr[2]<old_tr[2])////Add on 08.5.6
		{ 
			count_temp[2]=0xffff-old_tr[2];
			count_temp[2]=count_temp[2]+new_tr[2];
		}
		else 
		{
			count_temp[2]=65535;
		}
	}
    else
	{ 
		count_temp[2]=0;
		count[2][2]=0;
		count[2][1]=0;
		count[2][0]=0;
	}

    count[2][3]=0;
    
    if(count_temp[2]>count_max)//&&count_temp[]<62800,Add on 08.3.6;Modify on 08.4.22
    {   //count[2][3]=count[2][2];
        count[2][2]=count[2][1];
        count[2][1]=count[2][0];
        count[2][0]=count_temp[2];
    }
    
    if(ctrlflag==0&&End_Break==0&&count_temp[2]>count_max)
	{ 
		Wheel_Time_Count[2]=max(count[2][0],count[2][1],count[2][2],count_temp[2]);
		//Wheel_Time_Count[2]=(unsigned int)((count[2][0]+count[2][1]+count[2][2]+count[2][3])>>2); 
		//if(Wheel_Time_Count[2]<=10000)  // 50Hz(ZZ==100,f==0.5MHz)
		if(Wheel_Time_Count[2]<40000&&Vw[2]>1300)  // 40Hz(ZZ==100,f==2MHz)
		{
			m1_tb[2]++;
			if(m1_tb[2]>=(ZZ+1))
				{ m1_tb[2]=0; }
		}
		else
		{ 
			m1_tb[2]=0;  
			Sensor_Value[2]=0;
		}
	}
    else 
    { m1_tb[2]=0; }
         
    if(count[2][0]>65534){ m1_tb[2]=0; Sensor_Value[2]=0; }  
    
   // g1ie1|=0x02;
  //}
   //TAUD0.TT &=0xfdff;
}

/***********************************************************************************************************************
* Function Name: r_Config_TAUD0_11_interrupt
* Description  : This function is TAUD011 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma interrupt r_Config_TAUD0_11_interrupt(enable=manual)
void r_Config_TAUD0_11_interrupt(void)
{
    //if (TAUD0.CSR11 & _TAUD_OVERFLOW_OCCURS)    /* overflow occurs */
    //{
    //    g_taud0_ch11_width = (uint32_t)(TAUD0.CDR11 + 1U) + _TAUD_OVERFLOW_VALUE;
    //}
    //else
    //{
    //    g_taud0_ch11_width = (uint32_t)(TAUD0.CDR11 + 1U);
    //}
    /* Start user code for r_Config_TAUD0_11_interrupt. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
	//R_Config_TAUD0_11_Start();

	//if(g1ir1==1) ///////////////////////// ch-1
  	//{ 
  	//g1ie1&=0xfb;
	
    No_WheelandOver_Flag[1]=1;
    //g1ir1=0;   // clear interrupt flag
    old_tr[1]=new_tr[1];
    new_tr[1]=TAUD0.CDR11;
    //new_tr[1]=(unsigned int)(g1tm1);//Modify on 08.3.3
    T_tire[1]++;

	if(count[1][3]<count_base) //////Add on 08.5.6
	{ 
		if(new_tr[1]>old_tr[1])////Add on 08.4.22
		{ 
			count_temp[1]=new_tr[1]-old_tr[1]; 
		}
		else if(new_tr[1]<old_tr[1])////Add on 08.5.6
		{ 
			count_temp[1]=0xffff-old_tr[1];
			count_temp[1]=count_temp[1]+new_tr[1];
		}
		else 
		{
			count_temp[1]=65535;
		}
	}
    else if(count[1][3]==count_base)///////////// Base Timer over flow
	{
		if(new_tr[1]<old_tr[1])////Add on 08.5.6
		{ 
			count_temp[1]=0xffff-old_tr[1];
			count_temp[1]=count_temp[1]+new_tr[1];
		}
		else
		{
			count_temp[1]=65535;
		}
	}
	else
    { 
		count_temp[1]=0;
		count[1][2]=0;
		count[1][1]=0;
		count[1][0]=0;
	}

    count[1][3]=0;
    
    if(count_temp[1]>count_max)//&&count_temp[]<62800,Add on 08.3.6;Modify on 08.4.22
    {   //count[1][3]=count[1][2];
        count[1][2]=count[1][1];
        count[1][1]=count[1][0];
        count[1][0]=count_temp[1];
    }
    
    if(ctrlflag==0&&End_Break==0&&count_temp[1]>count_max)
	{ 
		Wheel_Time_Count[1]=max(count[1][0],count[1][1],count[1][2],count_temp[1]);
		//Wheel_Time_Count[1]=(unsigned int)((count[1][0]+count[1][1]+count[1][2]+count[1][3])>>2); 
		//if(Wheel_Time_Count[1]<=10000)  // 50Hz(ZZ==100,f==0.5MHz)
		if(Wheel_Time_Count[1]<40000&&Vw[1]>1300)  // 40Hz(ZZ==100,f==2MHz)
		{
			m1_tb[1]++;
			if(m1_tb[1]>=(ZZ+1))
			{
				m1_tb[1]=0; 
			}
		}
		else
		{
			m1_tb[1]=0;  
			Sensor_Value[1]=0;
		}
	}
    else 
    { 
    	m1_tb[1]=0; 
	}
         
    if(count[1][0]>65534)
		{ m1_tb[1]=0; Sensor_Value[1]=0; }  
    
    //g1ie1|=0x04;
  //}


}


#endif

unsigned short max(unsigned short a,unsigned short b,unsigned short c,unsigned short d)
{
  if(a<b)
     a=b;
  if(a<c)
     a=c;
  if(a<d)
     a=d;
  return a; 
 }   

 unsigned short min(unsigned short a,unsigned short b,unsigned short c,unsigned short d)
 {
  if(a>b)
     a=b;
  if(a>c)
     a=c;
  if(a>d)
     a=d;
  return a; 
 } 

void Tire_press(signed int total_val)
{unsigned char j=0;
		unsigned int Z_tire0=0,Z_tire1=0;
		
	 if(ctrlflag==0)
	 {	
		 
		 Z_tire0=T_tire[0]-T_tire_k[0];
	   Z_tire1=T_tire[1]-T_tire_k[1];
	   
	   Tgro=0;
		 if((Z_tire0>200)||(Z_tire1>200))
			{
		 T_tire_k[0]=T_tire[0];
		 T_tire_k[1]=T_tire[1];
		 total_4=Z_tire1-Z_tire0;
		 
		 if(Z_tire1>Z_tire0) {total_q=(signed int)(Z_tire1-Z_tire0); total_q=total_q-total_5;}
			else {total_q=(signed int)(Z_tire0-Z_tire1);total_q=total_q+total_5;}
		 
		 if(total_q>23) {Tgro=3;}
			else
			 {
				if(total_q>15) {Tgro=2;}
				else 
					{if(total_q>10) {Tgro=1;} }
			 }
	
		 if(Tgro==3) 
			{	if(T_tijs<100) //2010.4.9
					{ T_tijs++;
						T_tire[0]=0;T_tire[1]=0;T_tire[2]=0;T_tire[3]=0;T_tire_k[0]=0;T_tire_k[1]=0;
					}
			}
		 if(Tgro==2)
			{	if(T_tijs<80)  //2010.4.9
					{ T_tijs++;
						T_tire[0]=0;T_tire[1]=0;T_tire[2]=0;T_tire[3]=0;T_tire_k[0]=0;T_tire_k[1]=0;
					}
			}
		  if(Tgro==1)
			{	if(T_tijs<60)  //2010.4.9
					{ T_tijs++;
						T_tire[0]=0;T_tire[1]=0;T_tire[2]=0;T_tire[3]=0;T_tire_k[0]=0;T_tire_k[1]=0;
					}
			}
		}
	
	  Z_tire0=T_tire[0]+T_tire[3];Z_tire1=T_tire[1]+T_tire[2];
	
		if((Z_tire0>60000)||(Z_tire1>60000)) 
	  { 
		Tgro=0;
		
		total_0=Z_tire1-Z_tire0;
		
		if(Z_tire1>Z_tire0) {total_a=(signed int)(Z_tire1-Z_tire0); total_a=total_a-total_1;}
			 else {total_a=(signed int)(Z_tire0-Z_tire1);total_a=total_a+total_1;}
		
			if(total_a>total_val) 
			{
			if(Tj_total_a<9) Tj_total_a++;
				if(Tj_total_a>3) { Tgro=1;Tj_total_a=0;T_tijs=0;}
			}
			else {T_tijs=0;Tj_total_a=0;}
		 
		Z_tire0=min(T_tire[0],T_tire[1],T_tire[2],T_tire[3]);
		Z_tire1=max(T_tire[0],T_tire[1],T_tire[2],T_tire[3]);
		total_2=Z_tire1-Z_tire0;	
		   
		   if(Z_tire1>Z_tire0) {total_b=(signed int)(Z_tire1-Z_tire0); total_b=total_b-total_3;}
				else {total_b=(signed int)(Z_tire0-Z_tire1);total_b=total_b+total_3;}	 
		  
		if(T_tijs>20) total_val=total_val+360;	//2010.4.9
		if(total_b>total_val)
			{
			if(Tj_total_b<9) Tj_total_b++;
				if(Tj_total_b>5) { Tgro=2;Tj_total_b=0;T_tijs=0;}
			}
			else {T_tijs=0;Tj_total_b=0;}
			 
	  if(ABS_Special_Function_Flag==0x01)
	   {
			if(Tgro>0)
		 {
		   if(ASRLamp_DynamicError_Flag==0&&GV_Funciton_Setup_Init_Flag==0)
			{
			 ASRLamp_DynamicError_Flag=0x01;
			 Diagnose_Write(29,0x0010);
			 Diagnose_Write(55,ASRLamp_DynamicError_Flag);
			 Error_Flash[43]=0x78;
			 Diagnose_Data[0]=0x04f01;
			}
		 }
	   }
	
		T_tire[0]=0;T_tire[1]=0;
		T_tire[2]=0;T_tire[3]=0; 
		T_tire_k[0]=0;T_tire_k[1]=0;
		}
	 }
	 
	 if((ctrlflag!=0)&&(ctrlflag!=1)) 
		{T_tire[0]=0;T_tire[1]=0;T_tire[2]=0;T_tire[3]=0;
			T_tire_k[0]=0;T_tire_k[1]=0;Tj_total_a=0;Tj_total_b=0;} 
	}

