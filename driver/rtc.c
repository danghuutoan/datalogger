#include "rtc.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_wwdg.h"
#include <time.h>
#include "std_type.h"

bool SYS_TIME_ADJUSTED = FALSE;
#define PL_RTC_Update               RTC_IRQHandler
void    _URTC_RTC_IRQHandler(void);
	
static bool PL_ConfigureRTC(u32 countval);
static uint32_t PL_RTC_WaitForSynchro(void);
static uint32_t PL_RTC_WaitForLastTask(void);


sytemdate_t sys_date;
bool        sys_nextday = FALSE;

/*******************************************************************************
* Function Name  : PL_RTC_GetTime
* Description    : Adjust the RTC time value
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void PL_RTC_GetTime(u8 *timebuffer)
{
  u32 acttime;
  struct tm* pLocalTime;
  	
	
	acttime = RTC_GetCounter();
	pLocalTime = localtime(&acttime);
	
	sys_date.hh     = (u8)pLocalTime->tm_hour;            /* hours since midnight (0,23)      */
	sys_date.mm     = (u8)pLocalTime->tm_min;             /* minutes after the hour (0,59)    */
	sys_date.ss     = (u8)pLocalTime->tm_sec;             /* seconds after the minute (0,61)  */
	sys_date.year   = (u16)pLocalTime->tm_year + 1900;    /* years since 1900                 */
	sys_date.month  = (u8)pLocalTime->tm_mon + 1;         /* months since January (0,11)      */
	sys_date.day    = (u8)pLocalTime->tm_mday;            /* day of the month (1,31)          */	

  timebuffer[0] = sys_date.hh;
  timebuffer[1] = sys_date.mm;
  timebuffer[2] = sys_date.ss;
  timebuffer[3] = (u8)(sys_date.year >> 8);
  timebuffer[4] = (u8)(sys_date.year);
  timebuffer[5] = sys_date.month;
  timebuffer[6] = sys_date.day;
}

/*******************************************************************************
* Function Name  : PL_RTC_SetTime
* Description    : Adjust the RTC time value
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
bool PL_RTC_SetTime(u8 *timebuffer)
{
  u32 acttime;
  struct tm *pLocalTime;
  
  acttime = RTC_GetCounter();
  pLocalTime = localtime(&acttime);
    
  pLocalTime->tm_hour = (int)(timebuffer[0]);     /* hours since midnight (0,23)      */
  pLocalTime->tm_min = (int)(timebuffer[1]);      /* minutes after the hour (0,59)    */
  pLocalTime->tm_sec = (int)(timebuffer[2]);      /* seconds after the minute (0,61)  */
  pLocalTime->tm_year = (int)((((int)(timebuffer[3]) << 8) | ((int)(timebuffer[4])))-1900); /* years since 1900 */
  pLocalTime->tm_mon = (int)(timebuffer[5] - 1);  /* months since January (0,11)      */ 
  pLocalTime->tm_mday = (int)(timebuffer[6]);     /* day of the month (1,31)          */
  pLocalTime->tm_wday  = (int)(timebuffer[7]);    /* days since Sunday (0,6)          */
  pLocalTime->tm_yday  = (int)(timebuffer[8]);    /* days since January 1 (0,365)     */
  pLocalTime->tm_isdst = (int)(timebuffer[9]);    /* Daylight Saving Time flag        */
  
  /* Update RTC counter */
  RTC_SetCounter(mktime(pLocalTime));
  
  /* Wait until last write operation on RTC registers has finished */
  if (PL_RTC_WaitForLastTask() == 0)
      return FALSE;
  
  /* Change the current time */
  sys_date.hh = timebuffer[0];
  sys_date.mm = timebuffer[1];
  sys_date.ss = timebuffer[2];
//  sys_date.year = (u16)(((u16)(timebuffer[3]) << 8) | ((u16)(timebuffer[4])));
//  sys_date.month = timebuffer[5];
//  sys_date.day = timebuffer[6];
//  sys_date.weekday = timebuffer[7];
//  sys_date.yearday = timebuffer[8];
//  sys_date.isdst = timebuffer[9];
  
  return TRUE;
}

/*******************************************************************************
* Function Name  : PL_InitRTC
* Description    : Configure the internal RTC
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
bool PL_InitRTC(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  struct tm tLocalTime;
  
  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RTC_PREEMPTY_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = RTC_SUB_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);
  BKP_ClearFlag();

#ifdef DEVICE_USE_EXTOSC_32KHZ
  if (BKP_ReadBackupRegister(BKP_DR1) == 0x5A5A)
  {
    /* Check if the Power On Reset flag is set */
    if ((RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)||(RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET))
    {
      // Reset or power off occurred
    }

    /* Wait for RTC registers synchronization */
    if (PL_RTC_WaitForSynchro() != 0)
    {
      /* Enable the RTC Second */
      RTC_ITConfig(RTC_IT_SEC, ENABLE);
      /* Wait until last write operation on RTC registers has finished */
      if (PL_RTC_WaitForLastTask() != 0)
      {
        /* Clear reset flags */
        RCC_ClearFlag();
        return TRUE;
      }
    }
  }
#endif		
  /* Backup data register value is not correct or not yet programmed (when
     the first time the program is executed) */

  /* Date-Time Initialization: 16 December 2011 15:20:00 */  
  tLocalTime.tm_hour  = 11;   /* hours since midnight (0,23)      */
  tLocalTime.tm_min   = 15;   /* minutes after the hour (0,59)    */
  tLocalTime.tm_sec   = 0;    /* seconds after the minute (0,61)  */
  tLocalTime.tm_mday  = 22;   /* day of the month (1,31)          */
  tLocalTime.tm_mon   = 7;   /* months since January (0,11)      */ 
  tLocalTime.tm_year  = 115;  /* years since 1900                 */
  tLocalTime.tm_wday  = 6;    /* days since Sunday (0,6)          */
  tLocalTime.tm_yday  = 346;  /* days since January 1 (0,365)     */
  tLocalTime.tm_isdst = 0;    /* Daylight Saving Time flag        */
  //PL_RTC_SetTime(&tLocalTime);
  /* RTC Configuration */
  if (!PL_ConfigureRTC(mktime(&tLocalTime)))
    return FALSE;
  
  /* Clear reset flags */
  RCC_ClearFlag();
  return TRUE;
}

/**
  * @brief  Waits until last write operation on RTC registers has finished.
  * @note   This function must be called before any write to RTC registers.
  * @param  None
  * @retval None
  */
static uint32_t PL_RTC_WaitForLastTask(void)
{
  uint32_t del = 1000000;
  
  /* Loop until RTOFF flag is set */
  while (((RTC->CRL & RTC_FLAG_RTOFF) == (uint16_t)RESET) && del)
  {
    del--;
  }
  return del;
}

/**
  * @brief  Waits until the RTC registers (RTC_CNT, RTC_ALR and RTC_PRL)
  *   are synchronized with RTC APB clock.
  * @note   This function must be called before any read operation after an APB reset
  *   or an APB clock stop.
  * @param  None
  * @retval None
  */
static uint32_t PL_RTC_WaitForSynchro(void)
{
  uint32_t del = 1000000;
  
  /* Clear RSF flag */
  RTC->CRL &= (uint16_t)~RTC_FLAG_RSF;
  /* Loop until RSF flag is set */
  while (((RTC->CRL & RTC_FLAG_RSF) == (uint16_t)RESET) && del)
  {
    del--;
  }
  return del;
}
	
/*******************************************************************************
* Function Name  : PL_ConfigureRTC
* Description    : Configure the RTC if the backup registers are not initialized
* Input          : Request delay (1 unit = 100us)
* Output         : None
* Return         : None
*******************************************************************************/
static bool PL_ConfigureRTC(u32 countval)
{
  u32 nRTCdel;
  
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);
  /* Reset Backup Domain */
  BKP_DeInit();

  nRTCdel = 1000000;
#ifdef  DEVICE_USE_EXTOSC_32KHZ
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while ((RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) && nRTCdel)
    nRTCdel--;
#else
  /* Enable the LSI OSC */
  RCC_LSICmd(ENABLE);
  /* Wait till LSI is ready */
  while ((RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET) && nRTCdel)
    nRTCdel--;
#endif
  
  if (nRTCdel)
  {
#ifdef  DEVICE_USE_EXTOSC_32KHZ
    /* Select LSE as RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
#else
    /* Select LSI as RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		//RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);
#endif
    /* Enable RTC Clock */
    RCC_RTCCLKCmd(ENABLE);
    /* Wait for RTC registers synchronization */
    if (PL_RTC_WaitForSynchro() == 0)
      return FALSE;
    /* Wait until last write operation on RTC registers has finished */
    if (PL_RTC_WaitForLastTask() == 0)
      return FALSE;
    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
    if (PL_RTC_WaitForLastTask() == 0)
      return FALSE;
    /* Set RTC prescaler: set RTC period to 1sec */
#ifdef  DEVICE_USE_EXTOSC_32KHZ
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
#else
    RTC_SetPrescaler(40000);
		//RTC_SetPrescaler(62500);
#endif
    /* Wait until last write operation on RTC registers has finished */
    if (PL_RTC_WaitForLastTask() == 0)
      return FALSE;
    /* Set time registers to 00:00:00; configuration done via gui */
		  if (BKP_ReadBackupRegister(BKP_DR1) != 0x5A5A)
  {
			RTC_SetCounter(countval);
	}
    
    /* Wait until last write operation on RTC registers has finished */
    if (PL_RTC_WaitForLastTask() == 0)
      return FALSE;
    /* Write RTC flag in backup register */
    BKP_WriteBackupRegister(BKP_DR1, 0x5A5A);
    return TRUE;
  }
  return FALSE;
}

/*******************************************************************************
* Function Name  : PL_RTC_LimitCheck
* Description    : Verify RTC registers limits
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void PL_RTC_Update(void)
{
  u32 acttime;
  struct tm* pLocalTime;
  
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    acttime = RTC_GetCounter();
    pLocalTime = localtime(&acttime);
    
    sys_date.hh     = (u8)pLocalTime->tm_hour;            /* hours since midnight (0,23)      */
    sys_date.mm     = (u8)pLocalTime->tm_min;             /* minutes after the hour (0,59)    */
    sys_date.ss     = (u8)pLocalTime->tm_sec;             /* seconds after the minute (0,61)  */
    sys_date.year   = (u16)pLocalTime->tm_year + 1900;    /* years since 1900                 */
    sys_date.month  = (u8)pLocalTime->tm_mon + 1;         /* months since January (0,11)      */
    sys_date.day    = (u8)pLocalTime->tm_mday;            /* day of the month (1,31)          */
    
    if ((sys_date.hh == 0)&&(sys_date.mm == 0)&&(sys_date.ss == 0))
      sys_nextday = TRUE;
    
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);   
  }
}
/*******************************************************************************
* Function Name  : GetSysTime
* Description    : Return the system time
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GetSysTime(uint8_t *timebuffer)
{
  uint8_t buffer[TIME_BUFFER_LEN];
  
  PL_RTC_GetTime(buffer);
  
  /* Get only hh, mm, ss*/
  timebuffer[0] = buffer[0]; // hh
  timebuffer[1] = buffer[1]; // mm
  timebuffer[2] = buffer[2]; // ss
	timebuffer[3] = buffer[3]; // yr (MSB)
  timebuffer[4] = buffer[4]; // yr (LSB)
  timebuffer[5] = buffer[5]; // mth
	timebuffer[6] = buffer[6]; // day
}

/*******************************************************************************
* Function Name  : DH_SetSysTime
* Description    : Update the system time
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool SetSysTime(uint8_t *timebuffer)
{
//  uint32_t acttime;
//  struct tm *pLocalTime;
  uint8_t buffer[TIME_BUFFER_LEN];

//  acttime = RTC_GetCounter();
//  pLocalTime = localtime(&acttime);

  /* Set only hh, mm, ss*/
  buffer[0] = timebuffer[0]; // hh
  buffer[1] = timebuffer[1]; // mm
  buffer[2] = timebuffer[2]; // ss
//	buffer[3] = timebuffer[3]; // hh
//  buffer[4] = timebuffer[4]; // mm
//  buffer[5] = timebuffer[5]; // ss
//	buffer[6] = timebuffer[6]; // hh
//  buffer[7] = timebuffer[7]; // mm
//  buffer[3] = (uint8_t)(pLocalTime->tm_year >> 8);
//  buffer[4] = (uint8_t)pLocalTime->tm_year;
//  buffer[5] = pLocalTime->tm_mon;
//  buffer[6] = pLocalTime->tm_mday;
//  buffer[7] = pLocalTime->tm_wday;
//  buffer[8] = pLocalTime->tm_yday;
//  buffer[9] = pLocalTime->tm_isdst;
  
  if (PL_RTC_SetTime(buffer))
  {
    SYS_TIME_ADJUSTED = TRUE;
    return TRUE;
  }
  return FALSE;
}
