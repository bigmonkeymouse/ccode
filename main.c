/***********************************************************************/
/*                                                                     */
/*  FILE        :Main.c                                                */
/*  DATE        :Mon, Oct 24, 2016                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
#include "r_cg_userdefine.h"

/*==============================================
		main Routine
  ==============================================*/            
/*-----------------------------------------------*/
//#include "sfr29.h"
//#include "define_29.h"
#include "PORT_Define.h"
#include "ABS_ctrl_lib.h"
#include "ABS_adc_lib.h"
#include "ABS_eeprom_lib.h"
#include "ABS_zd_KWP_lib.h"
#include "ABS_uart_lib.h"
#include "ABS_canbus_lib.h"
#include "ABS_init_lib.h"
#include "ABS_det_lib.h"
#include "Config_UART0.h"
#include "r_rh850_can_drv.h"
#include "r_rh850_can_sfr.h"
#include "RH850_F1L_CAN.h"

//#include ""
#if 1
#include "Symple.h"

unsigned short SWOUT_PORT;
 unsigned char adcon0;
 unsigned char adcon1;
 unsigned char adcon2;

 unsigned char adic;
 //unsigned char vcut;

 unsigned char cclkr;

 unsigned char brp;
 unsigned char sam;
 unsigned char pts;
 unsigned char pbs1;
unsigned char pbs2;
unsigned char sjw;

unsigned char p3;
unsigned char g1ir0;
unsigned char g1ie0;
unsigned char g1tm0;
unsigned char g1ir1;
unsigned char g1ie1;
unsigned char g1tm1;
unsigned char g1ir2;
unsigned char g1ie2;
unsigned char g1tm2;
unsigned char g1ir3;
unsigned char g1ie3;
unsigned char g1tm3;

unsigned char wdts;
unsigned char cm22;
unsigned char cm23;

unsigned char prc1;
unsigned char pm03;

//unsigned char ta0mr;
//unsigned char ta0;
//unsigned char ta0ic;
//unsigned char ta0s;
//unsigned char bts_g1bcr1;
//unsigned char ta3s;
//unsigned char ta1s;

unsigned char u0rbl;
unsigned char u0tb;
unsigned char u1tb;
unsigned char u1tbl;
//unsigned char re_u1c1;
//unsigned char te_u1c1;
unsigned char crd_u1c0;
unsigned char ta4s;

unsigned char u1rbl;
unsigned char ti_u1c1;

unsigned char u1tb;
unsigned char u1tbl;
unsigned char u1mr;
unsigned char u1c0;
unsigned char ucon;
unsigned char u1brg;

unsigned char u1irs;
unsigned char s1tic;
unsigned char s1ric;
unsigned char u1brg;

 can_mctl_def  c0mctl[16];
can_slot_def_t c0slot[16];
can_mask_def_t c0gmr,c0lmar,c0lmbr;
slot_def c0idr_addr;
slot_def c0icr_addr;
byte_def c0trmic_addr;
byte_def c0recic_addr;
c0ctlr c0ctlr_addr;

unsigned char temp_use;
unsigned char temp_use2;
unsigned char temp_use5;
unsigned char temp_use11;
unsigned char temp_use14;
unsigned char temp_use3;


#endif
/*-----------------------------------------------*/

//#define XS 56520
//extern unsigned char GV_New_Lamp_Judge_Flag; // added at 2010.12.22
//extern unsigned char GV_New_Lamp_Control_Flag;  // added at 2010.12.22
//extern unsigned char GV_New_Lamp_Condition_Flag;  // added at 2010.12.22
//extern unsigned char GV_New_Lamp_Judge_Value;  // added at 2010.12.22
//extern unsigned int GV_New_Lamp_High_Counter,GV_New_Lamp_Low_Counter;  // added at 2010.12.22
//extern unsigned char GV_New_Lamp_Judge_Value;  // added at 2010.12.22


/*------------------------------------------------------------------------*/ ////Add at 2004.10.20
unsigned char System_Flash=2; // 4S/4M:2;6S/6M:1;4S/3M:3;6S/4M:4; /// Add at 2004.10.21
unsigned char Select_System=0x09; // Select the System xS/xM (0x09:4S/4M,0x12:6S/6M)
unsigned char Select_Control=0x00;  // Select the control data for System  //(0x10: have RET; 0x00: all no )
unsigned char Serial_Number1=0x29,Serial_Number2=0x01; // Serial number //0x44,0x01
unsigned char Parts_Number1=0x01,Parts_Number2=0x04,Parts_Number3=0x00; //parts serial nuber   0x04,0xd8,0x04
unsigned char Special_Number=0x89;  //special serial number   //0x87:2007,0x88:2008;08.5.26
unsigned char Product_Number=0x46;  // product number for ECU  //0x4e:14,0x42:1
unsigned char Software_Head=0x08;   //Software serial number head  //0x08
unsigned char Software_Type=0x01; //Software type ,refer to ECU Type (0x01:Basic-ABS 24V,0x02:CAB-ABS 24V,0x03:FRAM-ABS 24V)
unsigned char Software_Number1=0x03,Software_Number2=0x00; //Software number   //0x00,0x00
unsigned char Select_Second_System=0x08; // 0x08:Have ABS Switch;0x18:Have ABS Switch and ASR Switch,Add on 07.3.7
unsigned char ABSLampForEBC1 = 1;           //added by KXL on 14.3.31 for EBC1 byte6 - bit0��1
/*------------------------------------------------------------------------*/ ////Add at 2004.11.24

unsigned char DD = 100, ZZ = 100, SS = 4;    //////4S/4M (100cm)
//long int  XS = 62800000;            /////D=1.0m, Z=100,F=2MHz,M30290
long int  XS = 31400000;            /////D=1.0m, Z=100,F=1MHz,M30290
long int  count_max = 706;       /////D=1.0m, Z=100, // Vmax = 160 Km/h,M30290
long int AA0=-1200,AA1=-1200,AAr0=-1100,AAr=-1100;
//int AA0=-1600,AA1=-1500,AAr0=-1400,AAr=-1400;//08.10.20
//static int AA0=-1150,AA1=-1150,AAr0=-1000,AAr=-1000;

//unsigned int  PI = 3.14;
//unsigned char F  = 2000000;(2MHz,f/8) // Modify at 2004.11.18
//unsigned char F  = 500000;(0.5MHz,f/32) // Modify at 2004.11.18
//XS =10000*(PI*D*F)/(Z*10);    ////D:m
//Vw*1000 = XS/counter[No];
//count_max = 10*XS/Vmax;  // Vmax: m/s (160Km/h==160/3.6m.s==44.44m/s)

long int  XS_ds = 31415;            /////D=1.0m, Z=100,F=1000Hz,M30290
//unsigned int  PI = 3.1415;
//unsigned char F  = 1000;(1000Hz,T=1ms) // Modify at 2009.11.20
//XS =1000*(PI*D*F)/(Z);    ////D:m
//Vw*1000 = XS/counter[No];
//count_max = 10*XS/Vmax;  // Vmax: m/s (160Km/h==160/3.6m.s==44.44m/s)


/*-----------------------------------------------*/

//extern void T_ta0(void);
//extern void T_ta1(void);
//extern void T_ta3(void);

extern void I_watchdog(void);

//void w_tb0(void);
extern void bt_int(void);    // M30290 have Add on 07.9.28
extern void icoc0_int(void); // M30290 have 
extern void icoc1_int(void); // M30290 have 

extern void delay100(unsigned char tt);
//////////////////////////////////////// Add at 2004.10.2
extern void wheel(void);
extern void filter(unsigned char No);
extern void Checkfault(void);
extern void Check_Error_Static(void);

////////////////////////////////////////

extern unsigned short max(unsigned short a,unsigned short b,unsigned short c,unsigned short d);
extern unsigned short min(unsigned short a,unsigned short b,unsigned short c,unsigned short d);

//static void filter(unsigned char No);  ///////// Modify at 2004.10.2
//static void wheel(void);  ///////// Modify at 2004.10.2
extern void Vspeed(void);
extern void det_control(void);
extern void control(void);
extern void EndControl(void);
extern void EndControl1(void);
extern void Val_Act(char i);
extern void init_control();
extern void EBL_EBD(unsigned char T_ch);
extern void Tire_press(signed int total_val);

//static void FaultWrite(void);
//static void Dynamic_Write(void);

extern void Wheel_Sen(void);
//static void Sensor_Sen(void);  /////// Add on 07.11.30
extern void Sensor_Value_Send(void); ////07.12.20
void can_transmit(void);   // send data to CAN bus

//////////////////////////////////////////////////////////////////////////////////////

extern void init(void);  /* routine declare */
//////////////////////07.12.20
extern unsigned int READ(unsigned char address);
extern unsigned char WRITE(unsigned char address,unsigned int op_data);
//extern unsigned char ERASE(unsigned char address);
//extern unsigned char ERAL(void);
//extern unsigned char WRAL(unsigned int op_data);

//extern void AD_Fad(void); 
extern void AD_Fad_Basic(unsigned char Fad_Times,unsigned char Fad_Value_GND,unsigned char Fad_Value_Low,unsigned int Fad_Value_High,unsigned int Fad_Value_Vbb);//07.12.20    
extern void Valves_Fout(unsigned char Fout_Times);
//extern void AD_V24(void);   ////// char-->int,06.5.18
extern void AD_V24(unsigned char V24_LTimes,unsigned char V24_HTimes,unsigned int V24_Value_Low,unsigned int V24_Value_High);//Modify on 07.12.20
extern void AD_Flash(void);

extern void Flash_ASRLamp(void);  ////// Add on 07.8.21

//extern void AD_Wheel(void);
extern void AD_Wheel(unsigned char Wad_Times,unsigned char Wad_Value_Low,unsigned int Wad_Value_Open,unsigned int Wad_Value_High);
/////////////////////////////////////////////////////////////////////////// Add at 2004.10.2
extern void RET_OpenorShort_Check(unsigned char RET_Error_Times); // Add at 2004.11.11
extern void AD_Wheel_Value(unsigned char Wad_Channel,unsigned char Sensor_Ratio_Times,unsigned char Sensor_Error_Times,unsigned char Sensor_Value_Ref);//07.12.13
extern void init_kword(unsigned char);
extern kword_main();

extern void ASR_DIF_Check(unsigned char DIFad_Times,unsigned char DIFad_Value_GND,unsigned char DIFad_Value_Low,unsigned int DIFad_Value_Have,unsigned int DIFad_Value_High,unsigned int DIFad_Value_Vbb);//ASR,07.12.20

extern send_byte(unsigned char); /////////// Add at 2004.12.22
extern unsigned char Mcutodata_flag; /////////// Add at 2004.12.22
/////////////////////////////////////////////////////////////////////////// 
extern void Diagnose_Write(unsigned char E_No,unsigned int E_Type);

extern void Clear_Error(void);////Modify on 08.3.31
extern void ProductData_Write(void);////Add on 08.3.31
extern void Auto_Configure(void);////Add on 08.3.31

extern void Check_Flag_Clear(void);  //////// Add on 07.2.6
///////////////////////////////////////////////////////////////////////////
//extern init_can20b(unsigned char in_baudrate);
extern init_can20b(unsigned char,unsigned char);

extern void ASR_Control(void);  ///////////////////////////////////////////////////// ASR----- //05.12.14          
//static void ASR_Engine_PID(unsigned char SR0,unsigned char KI,unsigned char KP,unsigned char KD);  /////// Add on 06.7.24
extern void ASR_Engine_PID(void);  /////// Modify on 06.9.12

extern void RSC_Ctrl(void);////RSC,09.4.16
extern void G_acc_FullWD(unsigned char G_Times,unsigned int G_ad_Low,unsigned int G_ad_High);
extern void G_acc_Vsg(void);
/*--------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                     //////////////////////////////////////////////////////// ABS/ASR, CAN BUS, SAE J1939, Add on 06.8.18 
//extern unsigned char Auto_Configure_Flag;  ////////// Auto configure flag, Add on 06.10.16
extern unsigned int CAN_Eng_Speed;
extern unsigned char CAN_Status_EEC1,CAN_Driver_Torque,CAN_Eng_Torque,CAN_Control_Eng_Address; ////06.10.16
extern unsigned char TSC1_Control,Eng_Request_Torque,Status_EBC1,Status_EBC2,Status_EBC_State;
extern unsigned int Eng_Request_Speed;

extern unsigned char ASR_Close_Flag,chenxq;   //////// Add on 06.12.31

extern unsigned char ABS_LED_Condition;//////// Add on 07.9.11

/////////////////////////////////////////////////////// Product_Information,Add on 07.3.19
extern unsigned char Product_Information[4]; //the information of product from ISO9141 (date,worker & number)
//extern unsigned char Para_Write_Flag;//the flag of writing parameter:bit3=1-write product information
unsigned int Product_Inf_Data=0; 

signed int PIDE[3]={0,0,0},PID_out=0,PID_out_Old[3]={0,0,0};////PID,Add on 06.7.24;Old,07.1.30;Old[],08.2.19
long int Vs_ASR=1120;  ////////// Vs_ASR, Add on 06.7.24

long int Vs1=1120,Vwo=1120,Vw[6]={1120,1120,1120,1120,1120,1120};////07.12.20     
long int Vszo=1120,V30=1120,Vmax2[2],Vst_max;
long int Aw[4]={0,0,0,0},Awr[4]={0,0,0,0},BB[4],BB0,BB1;
long int Vs=1120,Vsq=1120,Vsh=1120,Vsr=1120,Vs_old[4]={1120,1120,1120,1120},Vsao=1120;
long int Vss=1120,Vss1=1120,Vss2=1120,Vstq=1120,Vsth=1120,Vnold[4]={1120,1120,1120,1120};
long int Vmax=1120,Vmin=1120,Vpmaxo=1120,Vpmaxo_2=1120,Vpmax[4]={3000,3000,3000,3000};
long int Vw0[4]={0,0,0,0},Vw1[4]={0,0,0,0},Vw2[4]={0,0,0,0},Vs_mid=0;
 int Vwm[4][7]={
                     0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0
                    }; 


long int Awb[4]={0,0,0,0},Vsso=0,Vso_max=0;
unsigned char Tkc[4]={0,0,0,0},Tkcn[4]={0,0,0,0},Tvswitch=0,Tgr[4]={0,0,0,0},Tckg=0,Kgvw[4]={0,0,0,0};
unsigned char Tgr1[4]={0,0,0,0},Tvso_max=0,Tgro_ret=0,Tgrotv=0,Tgrovw2[4]={0,0,0,0};
unsigned char Jbk[4]={0,0,0,0},Vwzo[4]={0,0,0,0},Tksjz=0,Tawj1[4]={0,0,0,0},Ts_st[4]={0,0,0,0},Tanold[4]={0,0,0,0},count_base_flag=0;

unsigned int  T_w_v=0,Tt_w_v=0,count_base_q=35,count_base=35; ////Tvs2=0,Move on 07.3.11
signed int  Tvs1=0,Tvnold[4]={0,0,0,0},Tvn=0,total_0=0,total_1=0,total_2=0,total_3=0,total_4=0,total_5=0,total_a=0,total_q=0,total_b=0;
unsigned int Tslip=0,Tslipk=0,Tvs1_Times=0,Tawj[4]={0,0,0,0},Tgro=0,Tquit=0,T_tire[4]={0,0,0,0},T_tire_k[2]={0,0};
unsigned char Ktcontrol=100,Tzzo[4]={0,0,0,0},Tckg1=0,Tckg2=0,Tckg3=0,Nocontrols[4]={0,0,0,0},Tvmaxsr=0,Tvmaxsr1=0;
unsigned char Tdjk=0,Tkg_DKF[4]={0,0,0,0},Tkgo=0,Tawj10[4]={0,0,0,0},Tvpmaxo=0,cxq0=0,T_tijs=0;////Tawj10,Add on 07.10.25
long int Vs2=1000,Vsmin=0;   ////////// Vsmin,Add on 06.12.31
//unsigned int Wait5_Times=0;   /////// Add at 2005.1.18 // when > ,break
unsigned int Jbcz[4]={8,8,8,8},Tho=0,Tkvcs=0; //in ECU2 Vmax;
signed int  Vmax_SR[4];
unsigned int Vs_st[4]={0,0,0,0},Vsm_st[4]={0,0,0,0},Vsck[4]={0,0,0,0},Vssou=0,Vsmax_st=1120;
unsigned int Vso_m2=0,Vso_zj=0,Vpmaxo_st=0,Vs_w=0;
unsigned int Tkcqh=0,Tjjc[4]={0,0,0,0};
unsigned int Tsliph=0;  ////////////////static,Modify on 07.3.15 
unsigned char Tdkbz=0,Jk_gd=1,T5[2],Tcpd=0,Ttawj[4]={0,0,0,0},Ttawj1[4]={0,0,0,0},Nocontrol_w[4]={0,0,0,0}; ////Ttawj,Add on 07.10.25
unsigned char Aw_single_Flag[6]={0,0,0,0,0,0},Thoq=0,Aw_single_ASR[6]={0,0,0,0,0,0};//Aw_single_,Thoq,Add on 07.3.11
signed int SRby=50, SRby1=50;

unsigned char StepNum1=22,StepNum2=22,BreakNum=0,BreakNum0=0,BreakNum1=0,Tdk=0,Tqjs[4]={0,0,0,0},Tqjs_k=0,Ttjs=0;
unsigned char TStepNum[4]={22,22,22,22},StepNum10=22,StepNum20=22,Tj_total_a=0,Tj_total_b=0;
unsigned char ctrlflag=0,T30[4]={0,0,0,0},T130[4]={0,0,0,0},Tjj[4]={0,0,0,0},Tzy[4]={0,0,0,0}; ////////static, Modify on 06.7.24
unsigned char Tflag=0,SRflag[4]={0,0,0,0},Tbreak=0,ZY3[4]={0,0,0,0},ZYhz=0,Tst5=0;
unsigned int numflag=0,Tsa1=0,Tbreak0=0;	
unsigned char ms100flag=0,Jb[4]={0,0,0,0},Jb_bak[4]={1,1,1,1},TVw[4]={0,0,0,0},Tsao=0;
unsigned int   T1[4],T2[4],T3[4]={0,0,0,0},T0=0,Tabo=0;
unsigned int   Tbb[4]={0,0,0,0},Tbb0=3,T10=40,T11=40,Tst[4]={0,0,0,0};  /////////Vw=Vw*100;  Aw=Aw*100;
unsigned char  st[4],step[4],DI[4],End_Break=0,End_Break1=0,EBj=0,EBk=5,EBj1=0,EBk1=5,Tstep=2,Tstep1=2;////07.12.20

signed char DKF=0,Taw200[4]={0,0,0,0};

//static signed int  SR[4],SRmax[4],SRmaxo[4],SRZY,SRJY,SRZY1,SRJY1,SRHF=10;
signed int SR[4]={0,0,0,0},SRmax[6]={0,0,0,0,0,0},SRmaxo[6]={0,0,0,0,0,0},SR_vf[4]={0,0,0,0},SR_sx=0,SR_xx=0; ////// 4-->6,06.7.24
unsigned char SRZY=0,SRJY=0,SRZY1=0,SRJY1=0,SRHF=10;//06.7.24

long int count[4][4]={
                      0,0,0,0,
                      0,0,0,0,
                      0,0,0,0,
                      0,0,0,0
                     };
long int counter[4]={65500,65500,65500,65500};     ///////counter_old[4];
unsigned char Dzbz[4]={0,0,0,0},Vflag2=0,counter_Time[4]={0,0,0,0};//num[4]={0,0,0,0};counter_Time,08.7.5

unsigned char cancelflag0=0,cancelflag1=0; ////////////no signal flag,07.12.20
unsigned char Zp[4]={1,1,1,1},Zp0[4]={1,1,1,1},ZNum[4]={0,0,0,0}; 
unsigned int  nosiglflag[4]={0,0,0,0};// Add at 05.1.23,07.12.20
//static unsigned char Th_end=0;////07.12.20
//static unsigned char ta1flag=0;////07.12.20
unsigned char Tzz[4]={0,0,0,0};////07.12.20
unsigned char WheelBadFlag[4]={0,0,0,0},VsFlag=0,VsFlag0=0;////07.12.20
unsigned int WheelBadNum[4]={0,0,0,0},Kvo=30;
unsigned int Tnum[4]={0,0,0,0},Tcjy[4]={0,0,0,0},Kvs[4]={5,5,5,5},Kv=5,Kvh=10,Kvq=10,Kvh_old=5,Kvq_old=5;
unsigned char over_tb0=0,over_tb1=0,over_tb2=0,over_tb3=0;
//static unsigned char m1_tb0=0,m1_tb1=0,m1_tb2=0,m1_tb3=0;

unsigned char Vflag=0,Switch_Flag=0,Switch_OFF=0,Switch_ON=0,Switch_LampON=0,Switch_LampOFF=0;////Add on 06.10.31
unsigned char OffRoad_40km_Flag=0; //// Add on 06.12.30
unsigned char GV_OffRoad_Mode_Flag=0; //// Added at 14.2.23;1->15km,2->40km,
unsigned char ASR_Switch_Flag=0,ASR_Switch_OFF=0,ASR_Switch_ON=0,ASR_Switch_LampON=0,ASR_Switch_LampOFF=0;//06.12.30

//static unsigned char wheel_bad[6]={0,0,0,0,0,0},No_Signal[6]={0,0,0,0,0,0}; // Modify at 05.1.11
unsigned char No_Signal[6]={0,0,0,0,0,0};  //// Modify at 05.1.11,07.12.20
unsigned int ABS_Check_Flag_Times=0; ////////////////// Add at 05.1.14
////////////////////////////////////////////////////////////////////////////
//static unsigned char FaultWrite_Flag=0xaa;       /////////////Add at 2004.10.12 // Move at 2004.10.18
unsigned char Wheel_DynamicError_Flag=0; ////Wheel Dynamic Error Flag, Add at 2004.10.18 int-->char, modify at 06.10.16
unsigned char ASRLamp_DynamicError_Flag; ////TPM Dynamic Error Flag, Added at 2010.5.26  bit0-TPM
unsigned char ABS_Special_Function_Flag=0; ////TPM Dynamic Error Flag, Added at 2010.5.26 
//static unsigned char Lamp_Numflag=0;               // Vs1>=1200, Lamp Error Flag  // Add at 2004.10.19

unsigned char W_Value_Data_Low[4]={0,0,0,0},Sensor_Value_Ratio_Times[4]={0,0,0,0}; // Modify at 2004.11.19
unsigned char m1_tb[4]={0,0,0,0},W_Value_Tflag[4]={0,0,0,0};// Wheel value check flag  // Add at 2004.11.11
unsigned int Wheel_Time_Count[4]={0,0,0,0},m1_tb_count[4]={0,0,0,0};   // Wheel value check count  // Add at 2004.11.11
extern unsigned char adcflag;     // Add at 2004.11.11
extern unsigned char Lampad_Low,Lampad_High; ///////// Add at 05.2.26

extern unsigned int Lampad;////Lamp test,08.11.17

extern unsigned char ASRLamp_Low,ASRLamp_High; ////////3550E-V3,Add on 07.8.21
////////////////////07.12.20
unsigned char Diagnose_Flag=0;                  ///////// Add at 05.2.26
unsigned int Dynamic_Check_Times=12000,Dynamic_Check_Times_ASR=60000;//06.7.3;3200->12000,08.4.29;_ASR,08.7.18
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////  ////Add at 2004.10.20
unsigned char No_Old_Error_Flag=0;
unsigned char No_WheelandOver_Flag[4]={0,0,0,0},Ecu_Error_Flag=0;  ////Add at 2004.11.18,07.12.20
/*------------------------------------------------------------------------*/
unsigned int READ_Code=0,Limit_Velocity=0xffa0;   // the velocity of limits for the vehicle
unsigned char ABS_Lamp_RET_Flag=0,ABS_Valves_Flag=0; //Add at 2004.10.2.9,07.12.20 
unsigned int Wheel_Value[6]={0,0,0,0,0,0};  // Vw[](Km/h)*94  //Add at 2004.10.9
unsigned int Sensor_Value[6]={0,0,0,0,0,0}; // Sensor voltage (V)*50  ( (V)/0.02 )  //Add at 2004.10.9
unsigned int Valve_Value=0;                 // Valve voltage  (V)*7  ( (V)/0.14 )  //Add at 2004.10.9
unsigned char Lamp_Condition=0xc0,Clear_Error_Flag=0;  //Add at 2004.10.9

unsigned char ABS_Check_Flag=0;    /////// Add at 2004.12.31 // When =0x55, TC check connect

unsigned int Wait_Times=0;   /////// Add at 2005.1.18 // when > ,break

unsigned char Error_Flash[48]= {
                                 0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                               }; 
unsigned int Diagnose_Data[32]={
                                 0,0,0,0,0,0,0,0,
                                 0,0,0,0,0,0,0,0,
                                 0,0,0,0,0,0,0,0,
                                 0,0,0,0,0,0,0,0
                               }; 

unsigned int Diagnose_Data_Old[32]={
                                     0,0,0,0,0,0,0,0,
                                     0,0,0,0,0,0,0,0,
                                     0,0,0,0,0,0,0,0,
                                     0,0,0,0,0,0,0,0
                                   }; 
unsigned int E_Writed_Flag[32]={
                                 0,0,0,0,0,0,0,0,
                                 0,0,0,0,0,0,0,0,
                                 0,0,0,0,0,0,0,0,
                                 0,0,0,0,0,0,0,0
                               }; 

//unsigned char TyrePDW[4]={0,0,0,0}; 

unsigned char F11openflag=0,F12openflag=0,F21openflag=0,F22openflag=0;
unsigned char F31openflag=0,F32openflag=0,F41openflag=0,F42openflag=0;
unsigned char F11shortflag=0,F12shortflag=0,F21shortflag=0,F22shortflag=0;
unsigned char F31shortflag=0,F32shortflag=0,F41shortflag=0,F42shortflag=0;
///////////////////////////////////////////////////////////////////////////////07.12.20
unsigned char F11Vbbflag=0,F12Vbbflag=0,F21Vbbflag=0,F22Vbbflag=0;
unsigned char F31Vbbflag=0,F32Vbbflag=0,F41Vbbflag=0,F42Vbbflag=0;
unsigned char Fout11=0,Fout12=0,Fout21=0,Fout22,Fout31=0,Fout32=0,Fout41=0,Fout42=0,JDQ_ST_Flag=0,DIFout=0,RETout=0;

///////////////////////////////////////RET,ASR,07.12.20;Modify on 08.4.29
unsigned char RET_HaveorNo_Flag=0,ASR_HaveorNo_Flag=0,DIFshortflag=0,DIFopenflag=0,DIFVbbflag=0;

/////////////////////////////////////////////////////////////////////////////////// Add at 2004.10.19
unsigned char ZZflag=0,ZYflag=0,QZflag=0,QYflag=0;
unsigned char ZZV24flag=0,ZYV24flag=0,QZV24flag=0,QYV24flag=0;
unsigned char ZZOpenflag=0,ZYOpenflag=0,QZOpenflag=0,QYOpenflag=0;////07.12.20

unsigned char RET_Error_Flag=0;  // RET_Error_Flag, Add at 2004.11.11

unsigned char unctrl=0;  // with fault, disable ABS control flag


//unsigned char D_data=90,Z_data=100;
unsigned char V24_1Hflag=0,V24_1Lflag=0,V24_2Hflag=0,V24_2Lflag=0;

//extern unsigned int V24_1,V24_2;
//unsigned int V24_1avg=0,V24_2avg=0;
//unsigned char D_read=0,Z_read=0;

unsigned char ax,bx,cx,dx; ////aa,bb,cc,dd;M30290 do not use,Modify on 07.9.28  
const unsigned char ccc=0x55,ddd=0xaa,eee=0xaa,fff=0x55,ggg=0xff;

////////////////////////////////////////////////////////////////////////////////////////////////// ASR---------------
unsigned char case0_flag[4]={0,0,0,0};////Add on 08.6.30
unsigned char Brake_Switch=0,No_sigl_ASR=0,No_sigl_ASR_2=0,Error_ASRExit_Flag=0;//06.12.15;08.2.19;08.3.6
unsigned char ASR_Entry_Flag[6]={0,0,0,0,0,0},ASR_For1=0,ASR_For2=5,Drive_Num=0;//Drive_Num:0:4X2,1:4X4 
unsigned char ASR_Ch[6]={0,0,0,0,0,0},ASR_ANCh[6]={0,0,0,0,0,0},ASR_engine_Control=0; ////ASR_engine_Control,06.12.15
unsigned char Torque_Increase_Flag=0,Torque_Del_Flag=0,SR0=16,KI=12,KP=20,KD=0,PID_10msflag=0;//08.2.19
unsigned char T_Double_SR=0,T_Single_SR=0,Eng_Req_Torque_min=0x7d,Torque_max_Flag=0;////Add on 08.2.19
unsigned char HaveCANJ1939_Flag=0,NoCANJ1939_Flag=0,HaveorNoJ1939_Flag=0;//06.10.16;Diagnose_Data24_CAN,08.4.30
unsigned char CANJ1939_Read=0,Diagnose_Data24_CAN=0; ////Modify on 07.12.20;Diagnose_Data24_CAN,08.4.30
extern unsigned char Diagnose_Data23_Lamp;////Add on 08.4.30
signed int SRmin[6]={0,0,0,0,0,0},SRmino=0,SRref=16;////Add on 08.2.19
//extern unsigned char KWP_Eng_Torque,KWP_Control_Require;////KWP2000-Ctrl Eng,Add on 08.2.28;Modify on 08.5.6
unsigned char KWP_Eng_Torque=0x00;   // refer to the actual engine torque from KWP add at 2007.12.19  
unsigned char KWP_Control_Require=0x00;   // refer to the actual require command from KWP add at 2007.12.19 
unsigned char KWP_CAN_RET_Require=0x00;////Add on 08.5.6
////////////////////////////////////////////////////////////////Add on 07.11.30
unsigned char Sensor_No_Flag[6]={0,0,0,0,0,0},Sensor_Times[6]={0,0,0,0,0,0};
////////////////////////////////////////////////////////////////Add on 07.12.20
unsigned char selfcheck_flag=0,numflag100ms=0,numflag150ms=0,T_1msflag=0; 
unsigned int Valves100ms=0;
extern unsigned char Fadflag,V24flag,Wadflag,DIFflag,Vw_valueflag,Valves_Short_Flag; 
unsigned char Valves_Act_Flag[8]={0,0,0,0,0,0,0,0},DIF_Act_Flag=0,RET_Act_Flag=0;
////////////////////////////////////////////////////////////////07.12.20,test
extern unsigned int Fad1_1,Fad1_2,Fad2_1,Fad2_2,Fad3_1,Fad3_2,Fad4_1,Fad4_2,DIFad;
extern unsigned int Wheel_ZZ,Wheel_ZY,Wheel_QZ,Wheel_QY;

//////////////////////////////////////////////////////////////////////// M30290 Vw,Add on 07.9.28
long int new_tr[4]={0,0,0,0},old_tr[4]={0,0,0,0},count_temp[4]={0,0,0,0};////must be long int!!,08.4.28

extern volatile unsigned char GV_C_CCP_Running_Flag; // added at 2012.12.27 for debugging;
//extern _far void GF_CopyFlashCodeToRAM_JumpRAMExecute(void); // added at 2012.12.28
////////////////////////////////////////////////////////////G_acc,RSC,Add on 09.3.4
unsigned char G_RSC_flag=0,RSC_Exit_Flag=0,KI2=12,KP2=20,G_RSC0=0;
signed int PIDE2[3]={0,0,0},PID_out2=0;
extern unsigned int G_ad_data[4];
extern unsigned char G_HaveorNo_Flag;////Fearth:RSC,09.4.15 
extern signed int G_acc[2],G_acc_zero;

//////////////////////////////////////////////////////////////////////////////////////////////////////Mainroutines
    union{
      struct{
        char    d0:1;            /* Operation mode select bit */
        char    d1:1;            /* Operation mode select bit */
        char    d2:1;
        char    d3:1;
        char    d4:1;
        char    d5:1;
        char    D6:1;             /* Count source select bit */
        char    d7:1;             /* Count source select bit */
      }b;
      char    byte;
    } LV_Union_Judge_Flag; // added at 2010.5.26
unsigned char GV_Funciton_Setup_Init_Flag; // added at 2010.6.3
unsigned char GV_Valves_PowerOn_Self_Flag=0; // added at 2010.5.25
unsigned char LV_ASR_PowerOn_Counter=0; // added at 2010.5.25
unsigned char GV_First_Power_Flag=0; // added at 2010.5.27
unsigned int GV_Word5ms_Counter; // added at 2010.5.27
void LF_TPM_Work_Judge(void); // added at 2010.5.26
void CANJ1939_Identify(void); // added at 2014.3.31
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
long int temptemp;
can_frame_t Can_test_eight;
void delay(unsigned int z)
{
	unsigned int i;
	while(z--)
	{for(i=0;i<1228;i++);}
}

void main(void);
void main(void)
{
	unsigned char i=0;
	temptemp=1;
	R_MAIN_UserInit();
	R_Config_UART0_Start();
	g_uart0_rx_length =1;

	LV_Union_Judge_Flag.byte=0; // added at 2010.5.26
	init_can20b(4,1);
	delay(100);
	
 	if(ax!=0x55&&bx!=0xaa&&cx!=0x5a&&dx!=0xa5) ////aa,bb,cc,dd;M30290 do not use,Modify on 07.9.28
	{
		ax=0x55;
		bx=0xaa;
		cx=0x5a;
		dx=0xa5;

	///////////////////////////////////////////////////////////////////////////////// Add at 2005.2.6 
		if(Mcutodata_flag==0)
		{
			send_byte(0x2b);   // send "+" // send +(0x2b)+SS+D+Z
			Mcutodata_flag=1;
		}
	//////////////////////////////////////////////////  send S,D,Z

		ABSLamp_SetLow;       // LAMP_ctrl1==0(LAMP_ctrl2==1 or 0 !!!); warn LED light 
		Lamp_Condition &= 0x7f;   //// ABS Lamp Light, bit7==0;  // Add on 2007.5.14 

		ASRLamp_SetHigh;       // ABS Lamp Light,Add on 07.8.21
		Lamp_Condition &= 0xbf; //// ASR Lamp Light, bit6==0,Add on 07.8.21

	//////////////////////////////////ABS fully operational,Add on 08.4.11
		if(ABSLamp==0||Switch_Flag==0xa5)
		{ 
			Status_EBC_State &= 0xfc; 
		}
		
		if(ABSLamp!=0&&Switch_Flag==0)
		{ 
			Status_EBC_State &= 0xfd; 
			Status_EBC_State |= 0x01;
		}

		Fearth_SetHigh;        // Fearth==1
		F_ctrl_SetHigh;        // F_ctrl==1

		wheel();     //// Add at 2005.1.12

		for(i=1;i<32;i++)
		{
			Diagnose_Data_Old[i]=(unsigned int)(READ(i));
			if(Diagnose_Data_Old[i]==0xffff)
			{ 
				Diagnose_Data_Old[i]=0x0000; 
			}
			
			//Diagnose_Data[i] = Diagnose_Data_Old[i];  // Move at 2004.10.18
			if(Diagnose_Data_Old[i]!=0) 
			{ 
				No_Old_Error_Flag++; 
			}
		}
		
		Diagnose_Data[29]=Diagnose_Data_Old[29]; // added at 2010.5.27
		if(Diagnose_Data[29]>0) // added at 2010.5.27
		{
			Diagnose_Data[0]=0x04f01; // added at 2010.5.27
			Error_Flash[43]=0x78; // added at 2010.5.27
		}
	//////////////////////////////////////////////////////////// Add at 10.2.25
      total_1=(unsigned int)(READ(49));  //// int-->char:10.02.24 
		if(total_1==0xffff)  ////// 0xffff:10.02.24
		{ 
			total_1=0; 
		} ////// 10.02.24
		
		total_0=total_1;  ////// 10.02.24

      total_3=(unsigned int)(READ(50));  //// int-->char:10.02.24 
		if(total_3==0xffff)  ////// 0xffff:10.02.24
		{ 
			total_3=0; 
		} ////// 10.02.24
		
		total_2=total_3;  ////// 10.02.24

      total_5=(unsigned int)(READ(51));  //// int-->char:10.02.24 
		if(total_5==0xffff)  ////// 0xffff:10.02.24
		{ 
			total_5=0; 
		} ////// 10.02.24
		
		total_4=total_5;  ////// 10.02.24
	//////////////////////////////////////////////////////////// Add at 10.2.25
		if(total_0!=0) LV_Union_Judge_Flag.b.d0=1; // added at 2010.6.3
		if(total_2!=0) LV_Union_Judge_Flag.b.d1=1; // added at 2010.6.3
		if(total_4!=0) LV_Union_Judge_Flag.b.d2=1; // added at 2010.6.3
	//////////////////////////////////////////////////////////// Add at 10.6.3
		GV_Funciton_Setup_Init_Flag=(unsigned char)(READ(53));  //// Add at 10.6.3 
		if(GV_Funciton_Setup_Init_Flag==0xff)  ////// Add at 10.5.26
		{ 
			GV_Funciton_Setup_Init_Flag=0; 
		} //Add at 10.5.26
	//////////////////////////////////////////////////////////// Add at 10.6.3
	//////////////////////////////////////////////////////////// Add at 10.5.26
		ABS_Special_Function_Flag=(unsigned char)(READ(54));  //// Add at 10.5.26 
		if(ABS_Special_Function_Flag==0xff)  ////// Add at 10.5.26
		{ 
			ABS_Special_Function_Flag=0;
		} //Add at 10.5.26
	//////////////////////////////////////////////////////////// Add at 05.2.25
	//////////////////////////////////////////////////////////// Add at 10.5.26
		ASRLamp_DynamicError_Flag=(unsigned char)(READ(55));  //// Add at 10.5.26 
		if(ASRLamp_DynamicError_Flag==0xff)  ////// Add at 10.5.26
		{ 
			ASRLamp_DynamicError_Flag=0; 
		} //Add at 10.5.26
	//////////////////////////////////////////////////////////// Add at 05.2.25
	//////////////////////////////////////////////////////////// Add at 05.2.25
		Wheel_DynamicError_Flag=(unsigned char)(READ(63));  //// int-->char:06.10.16 
		if(Wheel_DynamicError_Flag==0xff)  ////// 0xffff-->0xff:06.10.16
		{ 
			Wheel_DynamicError_Flag=0; 
		}
	//////////////////////////////////////////////////////////// Add at 05.2.25
	//////////////////////////////////////////////////////////// Add at 05.12.21   
		RET_HaveorNo_Flag=(unsigned char)(READ(62));
		if(RET_HaveorNo_Flag==0x5a)//0x10->0x5a,08.4.29
		{ 
			Select_Control|=0x10; 
		} ////07.12.20;08.4.29
	//////////////////////////////////////////////////////////// Add at 06.1.2
		ASR_HaveorNo_Flag=(unsigned char)(READ(61)); 
		if(ASR_HaveorNo_Flag==0x5a) //////////////// Add on 06.10.16                  
		{ 
			Select_Control|=0x08; 
			Select_Second_System|=0x10; /////// Have ASR Swtich Function,Add on 07.3.7   
		}

		CANJ1939_Read=(unsigned char)(READ(59));     ///////// CANJ1939,Add on 06.10.16
		if(CANJ1939_Read==0xa5) //////////////// Add on 06.10.16                  
		{ 
			Select_Control|=0x04; 
		}

      Product_Inf_Data=(unsigned int)(READ(57));     ////////////////////Product Information,Add on 07.3.19 
		Product_Information[3]=(unsigned char)(Product_Inf_Data>>8);
		Product_Information[2]=(unsigned char)(Product_Inf_Data&0x00ff);
      Product_Inf_Data=(unsigned int)(READ(56));
		Product_Information[1]=(unsigned char)(Product_Inf_Data>>8);
		Product_Information[0]=(unsigned char)(Product_Inf_Data&0x00ff);
	}
 
 ///////////////////////////////////////////////////////////////////////08.2.11
	old_tr[0]=0;old_tr[1]=0;old_tr[2]=0;old_tr[3]=0;
	new_tr[0]=0;new_tr[1]=0;new_tr[2]=0;new_tr[3]=0;
	count_temp[0]=0;count_temp[1]=0;count_temp[2]=0;count_temp[3]=0;
 
 //bts_g1bcr1=1; // Base Timer Start,M30290 have,Add on 07.9.28
  
 //ta3s=1;        // timerA3 (5ms) start to count   // main routine start time 
 //ta3s=1;        // timerA3 (5ms) start to count   // main routine start time 
 //ta1s=1;        // timerA1 start to count//07.12.20
 	R_Config_TAUJ0_0_Create_UserInit();
 	R_Config_TAUJ0_1_Create_UserInit();
	Tflag=0; 
	
	while(1)
	{ 
		//if(GV_C_CCP_Running_Flag==1) // added for flash write
		//{
		//	GF_init_sys();
		//	GF_CopyFlashCodeToRAM_JumpRAMExecute();
		//}
  		///////////////////////////////////////////////07.12.20	

		//WDTA0WDTE =0xac;
		
  		if(numflag100ms<20){ numflag100ms++; }
		if(numflag100ms>19&&ctrlflag==0&&T_w_v>400)
		{ 
			if(ASRLamp!=0&&ABS_Lamp_RET_Flag==0&&selfcheck_flag>200&&Vs1<1121)
			{ ASRLamp_SetLow; }     //// ASR_Lamp die 
		} 
                 
		if(Tsliph<600){ Tsliph++; }   /////// 41->600,Modify on 07.3.15
  	  
  		wheel();  
  
  		Vspeed();
    
		/////////////////////////// //Modify at 2009.7.1   
		if(ctrlflag==0) {if(Tt_w_v<401) {Tt_w_v++;}}

		if(ctrlflag==1) Tt_w_v=0;
  
		///////////////////////////////////////////////////////////////////////////////////////////////////Check    
		if(ctrlflag==0&&T_w_v<401) //Modify at 2005.1.4
		{ 
			T_w_v++; 
			if(selfcheck_flag<200)
				{ T_w_v=401; }  ////07.12.20
		}
   		else if(ctrlflag!=0)   //// ==1 --> !=0,Modify on 07.1.25
		{ 
			T_w_v=0;
			if(ctrlflag==4) T_w_v=380;
		}
      
		if(ABS_Check_Flag==0x55)  // Check mode //Add at 2005.1.4
		{
			T_w_v=401;
			if(RET_HaveorNo_Flag==0x5a) //(0x10: RET ) //Add at 2006.4.5;08.4.29
			{
				if(Tsliph<600){ Tsliph++; }
				if(Tsliph<400){ H_ctrl_SetHigh; }    //// RET act open //Add at 2006.4.5 
				else { H_ctrl_Set_Low;}     //// RET act close //Add at 2006.4.5  
			} 
		}
		//////////////////////////Modify on 07.12.20
		//Sensor_Sen();  /////////Add on 07.11.30 
		Sensor_Value_Send();   
		Check_Error_Static(); 
		G_acc_Vsg();
            
		if(unctrl==1)  // function switch (SWOUT==0), ////////////////////// p1_1 ---> p1_4 (SWOUT)
		{
			if(ABS_Lamp_RET_Flag==0)   //// Add at 2005.1.11 
			{ 
				ABSLamp_SetLow;       // LAMP_ctrl1==0(LAMP_ctrl2==1 or 0 !!!); warn LED light  
				Lamp_Condition &= 0x7f;   //// ABS Lamp Light, bit7==0;  // Add at 2004.10.10
			}
			//////////////////////////////////////////////////////////////////////////////////07.12.20
			//if(ctrlflag==0&&ABS_Valves_Flag==0)////07.12.20 
			if(ABS_Check_Flag!=0x55&&ctrlflag==0&&selfcheck_flag>200)////08.2.11;selfcheck_flag,Add on 08.4.23
			{  
				Fearth_SetLow;        // Fearth==0
				F_ctrl_Set_Low;        // F_ctrl==0 
			}   
			if(ABS_Lamp_RET_Flag==0)
			{ 
				H_ctrl_Set_Low; 
			}

			if(ctrlflag!=0)
			{
				ctrlflag=0;
				//for(i=0;i<SS;i++){ DI[i]=0; Val_Act(i); } 
				//F4S4MCtrl_SetLow; // ABS Valves Ctrl == 0;
				F11Ctrl_SetLow;//F1-1 IN
				F12Ctrl_SetLow;//F1-2 IN
				F21Ctrl_SetLow;//F2-1 IN
				F22Ctrl_SetLow;//F2-2 IN
				F31Ctrl_SetLow;//F3-1 IN
				F32Ctrl_SetLow;//F3-2 IN
				F41Ctrl_SetLow;//F4-1 IN
				F42Ctrl_SetLow;//F4-2 IN
				
				if(SS==6){ F51Ctrl_SetLow;F52Ctrl_SetLow;F61Ctrl_SetLow;F62Ctrl_SetLow; } //ABS Valves Ctrl==0;6S/6M  ABS类型，6s5M
			}  
		} 

		//////////////////////////////////function switch(SWOUT==0),p1_1 ---> p1_4 (SWOUT);Switch_OFF>80,Add on 06.10.31

		SWOUT_PORT =SWOUT;
		if(SWOUT_PORT!=0) 
		{ 
			if(Switch_OFF<50)
			{ Switch_OFF=0; } 
			if(Switch_ON<80) //////////80*5=400ms,Modify at 2005.1.18
			{ Switch_ON++; }
			if(Switch_ON>60) 
			{ 
				Switch_ON=0;
				if(Switch_OFF>60) 
				{ 
					Switch_OFF=0;
					if(ctrlflag==0)
					{ 
						if(Switch_Flag!=0xa5) ////////// 0x5a (90<100),0xa5, Modify at 2005.7.12
							{ Switch_Flag=0xa5; }
						else
						{ Switch_Flag=0; }
					}
				}
			}
		} 
		else 
		{ 
			if(Switch_ON<50)
				{ Switch_ON=0; }
			if(Switch_OFF<80)  ////80*5==400ms 
				{ Switch_OFF++; } 
		}
        
		if(Switch_Flag==0xa5)
		{
			Lamp_Condition |= 0x10;  //// function switch close, bit4==1;  // Add at 2004.10.10
			if(Diagnose_Data[0]==0x0000)  ////// No Error
			{
				if(ABS_Lamp_RET_Flag==0)   //// Add at 2004.11.09
				{
					if(Switch_LampON<250)
					{
						ABSLamp_SetLow; //// ABS Lamp Light
						Lamp_Condition &= 0x7f;   //// ABS Lamp Light, bit7==0;  // Add at 2004.10.10  
						Switch_LampON++;
					} 
					
					if(Switch_LampON==250) ///// 200*5ms=1000ms
					{ 
						if(Lampad_Low==0)
						{ 
							ABSLamp_SetHigh; //// ABS Lamp Off
							Lamp_Condition |= 0x80;   //// ABS Lamp die, bit7==1; 
						}
						Switch_LampOFF++;
						if(Switch_LampOFF>250)
						{ 
							Switch_LampOFF=0;
							Switch_LampON=0;
						}
					}          
				}  
			} 
			
			if(ctrlflag==0) ///////////////////////////////////////// Add on 06.12.30
			{
				//////////////////////////////////////////// OffRoad_40km_Flag,Add on 06.12.30
				if(Vs>11000)
				{ OffRoad_40km_Flag=0; }
				else
				{ OffRoad_40km_Flag=0x5a; }
				/////////////////////////////MOve on 08.2.11
			}
			
			if(Vs>4200) // added at 2014.2.23
			{
				if(ctrlflag==0) // added at 2014.2.26
				{
					GV_OffRoad_Mode_Flag=1; // added at 2014.2.23
				}
				else // added at 2014.2.26
				{
					if(Vs<10000) // added at 2014.2.27
					{
						GV_OffRoad_Mode_Flag=1; // added at 2014.2.26
					}
				}
			}
			else
			{
				if(ctrlflag==0) // added at 2014.2.26
				{
					GV_OffRoad_Mode_Flag=0; // added at 2014.2.23
				}
				else // added at 2014.2.26
				{
					if(Vs<3000) // added at 2014.2.26;4000->3000;
					{
						GV_OffRoad_Mode_Flag=0; // added at 2014.2.26
					}
				}
			}
			
			if(Vs>11000) 
				GV_OffRoad_Mode_Flag=2; // added at 2014.2.23
		}
   		else
		{ 
			Switch_Flag=0;
			Lamp_Condition &= 0xef;   //// function switch disconnection, bit4==0;  // Add at 2004.10.10
		}   
   		///////////////////////////////////////////////////////////////////////////Switch_OFF,Switch_ON,Add on 06.10.31
   
		/////////////////////////////////////////////////////////////////////////////////p7_0, ASR Switch,Add on 06.12.30
		
   		if(ASR_SW!=0) //on
		{
			if(ASR_Switch_OFF<50)
				{ ASR_Switch_OFF=0; } 
			if(ASR_Switch_ON<80) //////////80*5=400ms,
				{ ASR_Switch_ON++; }
			if(ASR_Switch_ON>60) 
			{ 
				ASR_Switch_ON=0;
				if(ASR_Switch_OFF>60) 
				{ 
					ASR_Switch_OFF=0;
					if(ctrlflag==0)
					{ 
						if(ASR_Switch_Flag!=0xa5)
							{ ASR_Switch_Flag=0xa5; }
						else
							{ ASR_Switch_Flag=0; }
					}
				}
			}
			
			if(GV_First_Power_Flag==0x00) // added at 2010.5.28
			{
				if(GV_Valves_PowerOn_Self_Flag==0x00) GV_Valves_PowerOn_Self_Flag=0x01; // added at 2010.5.25
				if(GV_Valves_PowerOn_Self_Flag==0x01) // added at 2010.5.25
				{ 
					if(LV_ASR_PowerOn_Counter<200) LV_ASR_PowerOn_Counter++; // added at 2010.5.25
				}
			}
			else if(GV_First_Power_Flag==0x55) // added at 2010.5.28
			{
				if(GV_Valves_PowerOn_Self_Flag<0x10) // added at 2010.5.28
				{
					GV_Valves_PowerOn_Self_Flag=0x00; // added at 2010.5.28
					GV_First_Power_Flag=0x5a;	 // added at 2010.5.28
				}
				else if(GV_Valves_PowerOn_Self_Flag==0x55) // added at 2010.5.25
				{ 
					if(GV_Word5ms_Counter>1000) // quit TPM set when time longer // added at 2010.5.27
					{
						GV_First_Power_Flag=0x5a;	 // added at 2010.5.28
						GV_Valves_PowerOn_Self_Flag=0x00;
					}
				}
			}
		} 
		else //off
		{
			if(ASR_Switch_ON<50)
				{ ASR_Switch_ON=0; }
			if(ASR_Switch_OFF<80)  ////80*5==400ms 
				{ ASR_Switch_OFF++; } 
			if(GV_Valves_PowerOn_Self_Flag<0x10) // added at 2010.5.25
			{ 
				GV_Valves_PowerOn_Self_Flag=0; // added at 2010.5.25
				LV_ASR_PowerOn_Counter=0; // added at 2010.5.25
			}
			else if(GV_Valves_PowerOn_Self_Flag==0x55) // added at 2010.5.25
			{ 
				if(GV_Word5ms_Counter>600) GV_Valves_PowerOn_Self_Flag=0x00; // quit TPM set when time longer // added at 2010.5.27
				if(LV_ASR_PowerOn_Counter>198) GV_Valves_PowerOn_Self_Flag=0x5a; // added at 2010.5.25
			}
		}
		
		LF_TPM_Work_Judge(); // added at 2010.5.26
   		if(ASR_Switch_Flag==0xa5)
		{ 
			Lamp_Condition |= 0x20;  ////ASR function switch close, bit5==1,07.3.7

			///////////////////////////////////////////////////////////////////////No ASR Error,No ASR Close,Modify on 06.12.31
			if((Diagnose_Data[18]==0x0000&&Diagnose_Data[24]==0x0000)&&(ASR_Close_Flag!=0x55))
			{
				if(ABS_Lamp_RET_Flag==0)   
				{
					if(ASR_Switch_LampON<250)
					{ 
						ASRLamp_SetHigh; //// ASR Lamp Light
						Lamp_Condition &= 0xbf;   //// ASR Lamp Light, bit6==0;   
						ASR_Switch_LampON++;
					}
					
					if(ASR_Switch_LampON==250) ///// 200*5ms=1000ms
					{ 
						if(ASRLamp_Low==0)  //// Add on 07.8.21
						{
							ASRLamp_SetLow; //// ASR Lamp Off // changed at 2010.5.27
							Lamp_Condition |= 0x40;   //// ASR Lamp die, bit6==1; 
						}

						ASR_Switch_LampOFF++;
						if(ASR_Switch_LampOFF>250)
						{ 
							ASR_Switch_LampOFF=0;
							ASR_Switch_LampON=0;
						}
					}          
				}  
			} 
		}
		else
		{ 
			ASR_Switch_Flag=0;
			Lamp_Condition &= 0xdf;  //// ASR function switch disconnection, bit5==0,07.3.7
		}   
   		//////////////////////////////////////////////////////////////////////////////// ASR Switch,Add on 06.12.30
   		//////////////////////////////////////////////////////////////////////////////// ASR Close Function,Add on 06.12.31 
		//   if(ASR_Close_Flag==0x55||ASRLamp_DynamicError_Flag!=0) // changed at 2010.5.27 
		
   		if(ASR_Close_Flag==0x55) 
		{ 
			if(ABS_Lamp_RET_Flag==0)      //////////Diag, Add on 07.1.8  
			{
				ASRLamp_SetHigh; //// ASR Lamp Light
				Lamp_Condition &= 0xbf;   //// ASR Lamp Light, bit6==0;   
			}
		}
   		else
		{ ////////ASR_Switch_Flag,Add on 07.1.8;RSC:&&G_RSC_flag<4,09.4.20
			if((ctrlflag!=2&&ctrlflag!=3&&G_RSC_flag<4)&&
				(Diagnose_Data[18]==0x0000&&Diagnose_Data[24]==0x0000)&&(ASR_Switch_Flag==0&&ABS_Lamp_RET_Flag==0))
			{
				if(ASRLamp_Low==0&&selfcheck_flag>200&&ASRLamp_DynamicError_Flag==0)  ////Add on 07.8.21,07.12.20;changed at 2010.5.27
				{ 
					ASRLamp_SetLow; //// ASR Lamp Off
					Lamp_Condition |= 0x40;   //// ASR Lamp die, bit6==1,Add on 06.12.30 
				}
			} 
		}  	
   
//////////////////////////////////////////////// cut

//////////////////////////////////////////////////// Tire_press

   		Tire_press(200);  // shoud used later === noted at 2010.7.5

//////////////////////////////////////////////////// Tire_press
   		det_control();

   		can_transmit();    // CAN bus test 2005.2.7
 
    //if(ASR_Switch_Flag!=0xa5&&ASR_Close_Flag!=0x55&&unctrl==0)////ASR_HaveorNo_Flag,Add on 07.8.30,Move on 08.2.19
    //if(ASR_HaveorNo_Flag==0x5a&&Diagnose_Data[18]==0) ////Move on 08.2.19
   		if(ASR_Switch_Flag!=0xa5&&ASR_Close_Flag!=0x55&&ASR_HaveorNo_Flag==0x5a)////Add on 08.4.22
		{
			if(Tt_w_v>399)
			{
				ASR_Control();    /////////////////////////////// ASR Ctrl---------------- Add at 05.12.14 
				ASR_Engine_PID();    ////ASR_Engine_Control
			}
		}
		
		CANJ1939_Identify(); /////////// Add on 06.12.30
		deal_J1939_Engine_control_transmit(0);  /////////// ASR-Engine-Ctrl-SAE J1939,Add on 06.7.24
		
   		if(ASR_Switch_Flag!=0xa5&&G_HaveorNo_Flag==0x5a&&Diagnose_Data[22]==0)
		{ 
			RSC_Ctrl();//RSC,09.4.17
		}
		///////////////////////////////////////////////////////////////////////
   		Wait_Times=0;      //// Add at 2005.1.18 
   		//while(!Tflag)
   		while(Tflag==0&&Wait_Times<3800)  //// Modify at 2005.1.18  // 16MHz: 2500~=5ms
		{ 
			Wait_Times++;                   //// Add at 2005.1.18

			if(ctrlflag==0&&End_Break==0)
			{
				//Clear_Error();////Move on 08.3.31

				kword_main();    //////////////////// ISO9141 Add at 2004.10.10

			} 
			deal_J1939_Engine_control_transmit(0);  /////////// ASR-Engine-Ctrl-SAE J1939,Add on 06.7.24

		} 
   
		Tflag=0;
		Wait_Times=0;      //// Add at 2005.1.18
/////////////////////////////////////////////////////

	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////Subroutines

void LF_TPM_Work_Judge(void) // added at 2010.5.26
{
	unsigned int i;
	if(ABS_Special_Function_Flag!=0) ASR_Switch_Flag=0x00;  // for foton ouman debugging added at 2010.6.3
	if(Diagnose_Data[0]==0||Diagnose_Data[29]!=0) // added at 2010.6.3
	{
   if(GV_Valves_PowerOn_Self_Flag==0x5a) // added at 2010.5.25
   {
     for(i=0;i<3;i++)   // 3 times flight and die // added at 2010.5.25
     {
        ASRLamp_SetLow;        // ASR Lamp die // added at 2010.5.25
        Lamp_Condition |= 0x40;   //// ASR Lamp die, bit6==1,Add on 06.12.30 
        delay100(2);  // added at 2010.5.25
        ASRLamp_SetHigh;        // ASR Lamp Light // added at 2010.5.25
        Lamp_Condition &= 0xbf;   //// ASR Lamp Light, bit6==0;   
        delay100(2); // added at 2010.5.25
     }
     total_1=0; total_0=0; // added at 2010.5.26
     total_3=0; total_2=0; // added at 2010.5.26
     total_5=0; total_4=0; // added at 2010.5.26
      WRITE(49,total_0);       ///////// total_0; add at 2010.5.26
      WRITE(50,total_2);       ///////// total_2; add at 2010.5.26
      WRITE(51,total_4);       ///////// total_4; add at 2010.5.26
      if(ABS_Special_Function_Flag==0)  // added at 2010.5.26
      {
        ABS_Special_Function_Flag=0x01;  // added at 2010.5.26
        WRITE(54,ABS_Special_Function_Flag);       ///////// ; add at 2010.5.26
      }
      if(GV_Funciton_Setup_Init_Flag==0)  // added at 2010.6.7
      {
	 	 	 GV_Funciton_Setup_Init_Flag=0x01; // added at 2010.6.3
	     WRITE(53,GV_Funciton_Setup_Init_Flag);       ///////// ; add at 2010.6.3
	    }
      if(ASRLamp_DynamicError_Flag!=0)  // added at 2010.5.26
      {
        ASRLamp_DynamicError_Flag=0; // added at 2010.5.26
        WRITE(55,0);       ///////// ; add at 2010.5.26
      }
      if(Diagnose_Data_Old[29]!=0||Diagnose_Data[29]!=0)// added at 2010.5.26
      {
        Diagnose_Data_Old[29]=0; // added at 2010.5.26
        Diagnose_Data[29]=0; // added at 2010.5.26
        WRITE(29,0);       ////// add at 2010.5.26
      }
      ASRLamp_SetLow;        // ASR Lamp die // added at 2010.5.25
        Lamp_Condition |= 0x40;   //// ASR Lamp die, bit6==1,Add on 06.12.30 
   	 GV_Valves_PowerOn_Self_Flag=0xa5; // added at 2010.5.25
     LV_Union_Judge_Flag.byte=0x00; // added at 2010.6.3
   }
  }
//   if(GV_Valves_PowerOn_Self_Flag>0xa0) // added at 2010.5.26
   if(GV_Funciton_Setup_Init_Flag==1&&ABS_Special_Function_Flag==0x01) // added at 2010.5.26
   {
   	 if(LV_Union_Judge_Flag.b.d0==0) // added at 2010.5.26
   	 {
     	 if(total_0!=0) // added at 2010.5.26
     	 {
         WRITE(49,total_0);       ///////// total_0;  // added at 2010.5.26
         total_1=total_0;
      	 LV_Union_Judge_Flag.b.d0=1; // added at 2010.5.26
    	 }
     }
   	 if(LV_Union_Judge_Flag.b.d1==0) // added at 2010.5.26
   	 {
     	 if(total_2!=0) // added at 2010.5.26
     	 {
         WRITE(50,total_2);       ///////// total_2; // added at 2010.5.26
         total_3=total_2;
      	 LV_Union_Judge_Flag.b.d1=1; // added at 2010.5.26
     	 }
     }
   	 if(LV_Union_Judge_Flag.b.d2==0) // added at 2010.5.26
   	 {
     	 if(total_4!=0) // added at 2010.5.26
     	 {
         WRITE(51,total_4);       ///////// total_4; // added at 2010.5.26
         total_5=total_4;
      	 LV_Union_Judge_Flag.b.d2=1; // added at 2010.5.26
     	 }
     }
   	 if((LV_Union_Judge_Flag.byte & 0x07)==7) // added at 2010.5.26
   	 {
   	 	 GV_Valves_PowerOn_Self_Flag=0x00; // added at 2010.5.26
    	 GV_Funciton_Setup_Init_Flag=0x00; // added at 2010.6.3
  	   WRITE(53,GV_Funciton_Setup_Init_Flag);       ///////// ; add at 2010.6.3
   	 }
   }
//   if(Diagnose_Data[29]>0) //added at 2010.5.26
//   {
//    	 if(ASRLamp_DynamicError_Flag==0) // added at 2010.5.26
//   	 	 {
//   	  	 ABSLamp_SetHigh; // added at 2010.5.26
//   	 	   ASRLamp_DynamicError_Flag=0x01; //added at 2010.5.26
//   	 	   WRITE(55,ASRLamp_DynamicError_Flag); //added at 2010.5.26
//   	   }
//   }
}

///////////////////////////////////////////////////////////////////////////////ASR-----------------
void CANJ1939_Identify(void)
{
 ///////////////////////////////////////////////////////////////////////////////// ABS-ASR Status,Modify on 06.12.30
 if(ctrlflag==0){ Status_EBC1=0xc0; }
 else if(ctrlflag==1){ Status_EBC1=0xd0;ASR_Entry_Flag[1]=0; } /////// 06.12.8            
 else if(ctrlflag==2)
	{
		if(ASR_Entry_Flag[1]<6){ Status_EBC1=0xc4; }
  else
		{ 
			if(ASR_Entry_Flag[0]<6&&ASR_Entry_Flag[2]<6&&ASR_Entry_Flag[4]<6){ Status_EBC1=0xc1; }
    else{ Status_EBC1=0xc5; }
  }
 }
 else if(ctrlflag==3){ Status_EBC1=0xc1; }
 
 ////////////////////////////////////////////////////////////////////////Add on 08.4.11;Modify on 08.4.22
 if(Switch_Flag==0xa5)    //////////ABS off-road switch
 { Status_EBC1 |= 0x30; }
 if(ASR_Switch_Flag==0xa5||Diagnose_Data[1]!=0||Diagnose_Data[2]!=0||
    Diagnose_Data[3]!=0||Diagnose_Data[4]!=0||Diagnose_Data[5]!=0||Diagnose_Data[6]!=0)//ASR off-road switch,Sensors
 { Status_EBC1 |= 0x0f; }
 if(unctrl!=0)    
 { Status_EBC1 |= 0x3c; }
 if(Diagnose_Data[9]!=0||Diagnose_Data[10]!=0||Diagnose_Data[11]!=0||Diagnose_Data[12]!=0||Diagnose_Data[18]!=0)//Valves
 { Status_EBC1 |= 0x0c; }
 //////////////////////////////////////////////////////////////////////////////////
 
 if(Switch_Flag==0xa5)    ////////// ABS off-road switch
 //{ Status_EBC2=0xc1; }
 {Status_EBC2=0xf1;}        //change by KXL on 14.03.31 for the same to WABCO
 //else{ Status_EBC2=0xc0; }
 else{ Status_EBC2=0xf0; }  //change by KXL on 14.03.31 for the same to WABCO
 
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
	{ 
		Status_EBC_State &= 0xdf;
   Status_EBC_State |= 0x10; 
   //////////////////////////// Add on 07.12.7
   ABS_LED_Condition &= 0xf7; 
   ABS_LED_Condition |= 0x04;
 }
 if((Lamp_Condition&0x80)==0x80)  ////// ABS amber warning state,Lamp Off
	{ 
		Status_EBC_State &= 0xcf;
   ABS_LED_Condition &= 0xf3; /////// Add on 07.12.7
 }
 if((Lamp_Condition|0xbf)==0xbf)  ////// ASR amber warning state,Lamp On
	{ 
		Status_EBC_State &= 0x7f;
   Status_EBC_State |= 0x40;
 }
 if((Lamp_Condition&0x40)==0x40)  ////// ASR amber warning state,Lamp Off
 { Status_EBC_State &= 0x3f; }
 
 //////////////////////////////////ABS fully operational,Add on 08.4.11
 //if(ABSLamp==0||Switch_Flag==0xa5)
 if(ABSLampForEBC1==1||Switch_Flag==0xa5)   //changed on 14.03.31 by KXL for EBC1 Byte6 - bit2��1
 { Status_EBC_State &= 0xfc; }
 //if(ABSLamp==1&&Switch_Flag==0)
 if(ABSLampForEBC1==0&&Switch_Flag==0)      //changed on 14.03.31 by KXL for EBC1 Byte6 - bit2��1
	{ 
		Status_EBC_State &= 0xfd; 
   Status_EBC_State |= 0x01;
 }
 
	///////////////////////////////////////////////////////////////////// Have or No CAN SAE J1939,06.10.16
 if(HaveorNoJ1939_Flag<200)
 { HaveorNoJ1939_Flag++; }
 if(ctrlflag==0&&HaveorNoJ1939_Flag>80)  /////// 80*5ms==400ms
	{ CAN_Status_EEC1=0xff;CAN_Eng_Speed=0xfaff;HaveorNoJ1939_Flag=0;}
 if(CAN_Status_EEC1==0xff&&CAN_Eng_Speed==0xfaff)
	{ 
		if(HaveCANJ1939_Flag<40)
   { HaveCANJ1939_Flag=0; }
   if(NoCANJ1939_Flag<200)////140*5ms==700ms;100->140,08.2.19;140->200,08.6.25  
   { NoCANJ1939_Flag++; }
   if(NoCANJ1939_Flag==200&&(HaveCANJ1939_Flag==40||CANJ1939_Read==0xa5))////200,08.6.25
		{
			if(Diagnose_Data24_CAN==0x5a)
			{ Diagnose_Data[24]=Diagnose_Data[24]|0x0100;}
    else
			{ Diagnose_Write(24,0x0100);}
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
	{
		NoCANJ1939_Flag=0;
   if((CAN_Status_EEC1!=0xff&&CAN_Status_EEC1!=0xfc&&CAN_Status_EEC1!=0xfd)&&(CAN_Eng_Speed!=0xfaff))       
		{
			if(HaveCANJ1939_Flag<40)///////////40*5ms==200ms  
     { HaveCANJ1939_Flag++; }
     if(HaveCANJ1939_Flag==40)
     { 
       if(Error_Flash[35]==0x0075){ Error_Flash[35]=0; }
       //if(Error_Flash[0]==0x0075){ Error_Flash[0]=0x0011; };Move on 08.4.11
       if(Diagnose_Data[0]==0x1805){ Diagnose_Data[0]=0; }
       if(((Diagnose_Data[24]&0xfff0)&0x0100)==0x0100)
				{ 
					Diagnose_Data24_CAN=0x5a;
         Diagnose_Data[24]=Diagnose_Data[24]&0xfef0;   //////// 0xfeff:Have Error Times,0xfef0:No Error Times     
       }
             
       if(CANJ1939_Read!=0xa5)
				{
					WRITE(59,0xa5);
         CANJ1939_Read=0xa5;
         
         Select_Control|=0x04;   /////////// Add on 06.10.16
       }
     }     
   }
 }
}



/**************************************************************************/
/*------------------------------------------------------------------------*/
/* Description:      This program is used for can2.0B extern mode         */
/*------------------------------------------------------------------------*/
/* Input:                                                                 */
/* Output:   send data to CAN bus                                         */
/**************************************************************************/
extern unsigned char LV_OffRoad_ByteCount[4],LV_OffRoad_st1_Count[4],LV_OffRoad_ControlFlag[4]; // added at 2014.2.25
extern unsigned char LV_SwitchFlag_ByteCount,LV_Count_ij; // added at 2014.3.7

void CAN_TXD_Eignt(void)
{
	Can_test_eight.ID = 0x1fffc1Bf;
	Can_test_eight.DLC = 8;
	Can_test_eight.IDE = 1;
	Can_test_eight.THDSE = 1;

	Can_test_eight.DB[1] =st[0];
	Can_test_eight.DB[2] =st[0]>>8;
	Can_test_eight.DB[3] =st[1];
	Can_test_eight.DB[4] =st[1]>>8;
	Can_test_eight.DB[5] =Tvnold[0]; 
	Can_test_eight.DB[6] =Tvnold[0]>>8;; 
	Can_test_eight.DB[7] =Tvnold[1]; 
	Can_test_eight.DB[8] =Tvnold[1]>>8;; 
	
	R_CAN_TrmByTxBuf(0,8,&Can_test_eight);
}
void can_transmit(void)
{

   can_send(0,1,Vw[0],Vw[1],Vw[2],Vw[3]);  
   can_send(1,2,DI[0],DI[1],DI[2],DI[3]);
   can_send(2,3,Aw[0],Aw[1],Aw[2],Aw[3]); 
   can_send(3,4,SR[0],SR[1],Tjj[0],Tjj[1]); 
   can_send(4,5,ctrlflag,Kv,G_acc[0],Vs); 
   //can_send(5,6,st[0],st[1],Tvnold[0],Tvnold[1]); 
   CAN_TXD_Eignt();
   //can_send(0,1,Vw[0],Vw[1],Vw[2],Vw[3]);  
   //can_send(1,2,DI[0],DI[1],DI[2],DI[3]);
   //can_send(2,3,Aw[0],Aw[1],Aw[2],Aw[3]); 
   //can_send(3,4,SR[0],SR[1],Tjj[0],Tjj[1]);   
   //can_send(4,5,ctrlflag,Kv,G_acc[0],Vs); 
}


