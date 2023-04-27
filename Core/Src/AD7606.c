#include <main.h>
#include <stm32g4xx.h>
#include <AD7606.h>


void AD7606_SETOS(uint8_t osv)
{
	switch(osv)
	  {
	  case 0://000  200Kbps
	    AD7606OS0_L;
	    AD7606OS1_L;
	    AD7606OS2_L;
	    break;
	  case 1://001
	    AD7606OS0_H;
	    AD7606OS1_L;
	    AD7606OS2_L;
	    break;
	  case 2://010
	    AD7606OS0_L;
	    AD7606OS1_H;
	    AD7606OS2_L;
	    break;
	  case 3://011
	    AD7606OS0_H;
	    AD7606OS1_H;
	    AD7606OS2_L;
	    break;
	  case 4://100
	    AD7606OS0_L;
	    AD7606OS1_L;
	    AD7606OS2_H;
	    break;
	  case 5://101
	    AD7606OS0_H;
	    AD7606OS1_L;
	    AD7606OS2_H;
	    break;
	  case 6://110
	    AD7606OS0_L;
	    AD7606OS1_H;
	    AD7606OS2_H;
	    break;
	  }
}





void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}




void AD7606_RESET(void)
{
  AD7606_RESET_H;
  Delay(0xFF);
  AD7606_RESET_L;
}




uint16_t ad7606_ReadBytes(void)
{
  uint16_t usData = 0;
  for (uint8_t i = 0; i < 16; i++)
  {
    AD7606_SCLK_L;
    usData = usData << 1;
    if(AD7606_DOUTA)
    {
      usData |= 0x0001;
    }
    AD7606_SCLK_H;
  }
  return usData;
}

void AD7606_STARTCONV(void)
{
  AD7606_CONVST_A_L;
  AD7606_CONVST_B_L;
  Delay(0xF);
  AD7606_CONVST_A_H;
  AD7606_CONVST_B_H;
}


void AD7606_Init(uint8_t os )
{
	  AD7606_SETOS(os);//200Kbps
	  AD7606_RESET();
	  AD7606_CONVST_A_H;
	  AD7606_CONVST_B_H;
}

void AD7606_RANGE10(float vol_data[])
{
	uint16_t datatemp[8];//AD7606 data acquisition TEMP
	int data[8];
	if(!AD7606_BUSY)
	 {
	   AD7606_CS_L;
	   for(uint8_t i = 0;i < 8;i++)
	   {
		 datatemp[i]=ad7606_ReadBytes();//Read ADC data
		 if(datatemp[i]>32767)
		 {
		   datatemp[i]=65536-datatemp[i];
		   data[i]=datatemp[i];
		   data[i]=-data[i];
		 }
		 else
		 {
			 data[i]=datatemp[i];
		 }
		 vol_data[i]=data[i]/32768.0*10.0;
	   }
	   AD7606_CS_H;
	   AD7606_STARTCONV();
	   while(AD7606_BUSY);
	 }

}

