/**
  ******************************************************************************
  * @file    typedef.h
  * @author  HUGE-IC Application Team
  * @version V1.0.0
  * @date    01-05-2021
  * @brief   Main program body header file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2021 HUGE-IC</center></h2>
  *
  *
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
     
typedef unsigned char            uint8_t;
typedef unsigned short           uint16_t;
typedef unsigned long            uint32_t;
typedef signed char              int8_t;
typedef signed short             int16_t;
typedef signed long              int32_t;
     
typedef unsigned char            uint8;
typedef unsigned short           uint16;
typedef unsigned long            uint32;
typedef signed char              int8;
typedef signed short             int16;
typedef signed long              int32;
typedef signed long              long32;
typedef float                    float32;
typedef double                   float64;

typedef int32                    s32;
typedef int16                    s16;
typedef int8                     s8;

typedef const int32              sc32;
typedef const int16              sc16;
typedef const int8               sc8;

typedef uint32                   u32;
typedef uint16                   u16;
typedef uint8                    u8;

typedef const uint32             uc32;
typedef const uint16             uc16;
typedef const uint8              uc8;

/* Private define ------------------------------------------------------------*/
#define TRUE                     (1)
#define FALSE                    (0)
#define BIT8(n)                  ((uint8)1 << (n))
#define BIT16(n)                 ((uint16)1 << (n))
#define SET_BIT(reg, bit)        ((reg) |= (bit))
#define CLEAR_BIT(reg, bit)      ((reg) &= ~(bit))
#define READ_BIT(reg, bit)       ((reg) & (bit))
#define CLEAR_REG(reg)           ((reg) = (0x0))
#define WRITE_REG(reg, val)      ((reg) = (val))
#define READ_REG(reg)            ((reg))
#define NONSENSE(x)              ((void)(x))
    
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

    /* minimum values of exact-width signed integer types */
#define INT8_MIN                   -128
#define INT16_MIN                -32768
#define INT32_MIN          ((s32)(~0x7fffffff))   /* -2147483648 is unsigned */

    /* maximum values of exact-width signed integer types */
#define INT8_MAX                    127
#define INT16_MAX                 32767
#define INT32_MAX            ((s32)2147483647)


    /* maximum values of exact-width unsigned integer types */
#define UINT8_MAX                   255
#define UINT16_MAX                65535
#define UINT32_MAX           ((u32)4294967295)


#define time_after(a,b)    ((int)(b) - (int)(a) < 0)
    

extern u16 jiffies;


#ifdef __cplusplus
}
#endif

#endif //__TYPEDEF_H__

/**
  * @}
  */

/**
  * @}
  */

/*************************** (C) COPYRIGHT 2021 HUGE-IC ***** END OF FILE *****/

