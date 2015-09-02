/*******************************************************************************
 * Copyright (c) 2015, The LightCo
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are strictly prohibited without prior permission of The LightCo.
 * 
 * @file    std_type.h
 * @author  The LightCo
 * @version V1.0.0
 * @date    June-18-2015
 * @brief   This file contains expand of the std_type
 *
 ******************************************************************************/  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TYPES_H
#define __TYPES_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported define------------------------------------------------------------*/
#ifndef STD_ON
#define STD_ON  1
#endif

#ifndef STD_OFF
#define STD_OFF 0
#endif

#define ON                      0x01
#define OFF                     0x00

#define E_OK                    0
#define E_NOT_OK                1

#define BIT0                    (1<<0)
#define BIT1                    (1<<1)
#define BIT2                    (1<<2)
#define BIT3                    (1<<3)
#define BIT4                    (1<<4)
#define BIT5                    (1<<5)
#define BIT6                    (1<<6)
#define BIT7                    (1<<7)
#define BIT8                    (1<<8)  
#define BIT9                    (1<<9)  
#define BIT10                   (1<<10)
#define BIT11                   (1<<11)
#define BIT12                   (1<<12)
#define BIT13                   (1<<13)
#define BIT14                   (1<<14)
#define BIT15                   (1<<15)
#define BIT16                   (1<<16)
#define BIT17                   (1<<17)
#define BIT18                   (1<<18)  
#define BIT19                   (1<<19)
#define BIT20                   (1<<20)
#define BIT21                   (1<<21)
#define BIT22                   (1<<22)
#define BIT23                   (1<<23)
#define BIT24                   (1<<24)
#define BIT25                   (1<<25)
#define BIT26                   (1<<26)
#define BIT27                   (1<<27)
#define BIT28                   (1<<28)  
#define BIT29                   (1<<29)
#define BIT30                   (1<<30)
#define BIT31                   (1<<31)

#define SIZE_256                0x100

/* Exported typedef  -------------------------------------------------------- */
/**
 * @brief flags_t
 *
 */
typedef enum __flags {
    FLAG_TX_COMPLETED   = 1<<1,
    FLAG_RX_COMPLETED   = 1<<2,
    FLAG_READ_COMPLETED = 1<<3,
    FLAG_WRITE_COMPLETED = 1<<4,
}flags_t;


#include <stdbool.h>
typedef bool BOOL;
#ifndef FALSE
#define FALSE false
#define TRUE true
#endif

#endif /* __TYPES_H */

/*********** Portions COPYRIGHT 2015 Light. Co., Ltd.*****END OF FILE****/
