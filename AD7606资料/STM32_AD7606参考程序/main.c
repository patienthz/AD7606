#include"stm32f10x_conf.h"
#define AD7606OS0_H GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define AD7606OS0_L GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define AD7606OS1_H GPIO_SetBits(GPIOC,GPIO_Pin_14)
#define AD7606OS1_L GPIO_ResetBits(GPIOC,GPIO_Pin_14)
#define AD7606OS2_H GPIO_SetBits(GPIOC,GPIO_Pin_15)
#define AD7606OS2_L GPIO_ResetBits(GPIOC,GPIO_Pin_15)

#define AD7606_CONVST_A_H GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define AD7606_CONVST_A_L GPIO_ResetBits(GPIOA,GPIO_Pin_11)
#define AD7606_CONVST_B_H GPIO_SetBits(GPIOA,GPIO_Pin_12)
#define AD7606_CONVST_B_L GPIO_ResetBits(GPIOA,GPIO_Pin_12)

#define AD7606_SCLK_H GPIO_SetBits(GPIOA,GPIO_Pin_0)
#define AD7606_SCLK_L GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define AD7606_RESET_H GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define AD7606_RESET_L GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define AD7606_CS_H GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define AD7606_CS_L GPIO_ResetBits(GPIOA,GPIO_Pin_2)

#define AD7606_BUSY GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define AD7606_DOUTA GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define AD7606_DOUTB GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)

void RCC_Configuration(void); 
void GPIO_Configuration(void);
void NVIC_Configuration(void); 
void Delay(vu32 nCount);
void AD7606_SETOS(uint8_t osv);
void AD7606_RESET(void);
uint16_t ad7606_ReadBytes(void);
void AD7606_STARTCONV(void);



uint16_t datatemp[8];
int main(void) 
{ 

  RCC_Configuration();
  GPIO_Configuration();
  NVIC_Configuration();

  AD7606_SETOS(0x00);
  AD7606_RESET();
  AD7606_CONVST_A_H;
  AD7606_CONVST_B_H;
  
  
  while (1) 
  {
    if(!AD7606_BUSY)
    {
      AD7606_CS_L;
      for(uint8_t i = 0;i < 8;i++)
      {
        datatemp[i]=ad7606_ReadBytes();
      }
      AD7606_CS_H;
      AD7606_STARTCONV();
      while(AD7606_BUSY);
    }
  } 
} 

void AD7606_STARTCONV(void)
{
  AD7606_CONVST_A_L;
  AD7606_CONVST_B_L;
  Delay(0xF);
  AD7606_CONVST_A_H;
  AD7606_CONVST_B_H;
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

void AD7606_RESET(void)
{
  AD7606_RESET_H;
  Delay(0xFF);
  AD7606_RESET_L;
}

void AD7606_SETOS(uint8_t osv)
{
  switch(osv)
  {
  case 0://000
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

void RCC_Configuration(void) 
{    
  ErrorStatus HSEStartUpStatus;
  RCC_DeInit(); 
  RCC_HSEConfig(RCC_HSE_ON); 
  HSEStartUpStatus = RCC_WaitForHSEStartUp(); 
  if(HSEStartUpStatus == SUCCESS) 
  { 
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); 
    FLASH_SetLatency(FLASH_Latency_2); 
    RCC_HCLKConfig(RCC_SYSCLK_Div1);  
    RCC_PCLK2Config(RCC_HCLK_Div1);  
    RCC_PCLK1Config(RCC_HCLK_Div2); 
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 
    RCC_PLLCmd(ENABLE); 
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) { } 
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 
    while(RCC_GetSYSCLKSource() != 0x08) { } 
  } 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOA, ENABLE); 
} 

void GPIO_Configuration(void) 
{ 
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_11|GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
} 


void NVIC_Configuration(void) 
{ 
#ifdef  VECT_TAB_RAM   
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  
#else
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);    
#endif 
} 

void Delay(vu32 nCount) 
{ 
  for(; nCount != 0; nCount--); 
} 