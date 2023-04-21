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

