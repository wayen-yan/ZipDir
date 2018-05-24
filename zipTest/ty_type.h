/******************************************************************************

  Copyright (C), 2016-2026, typonteq(beijing) Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ty_type.h
  Version       : V1.0
  Author        : typonteq software group
  Created       : 2017/12/20
  Description   :	

******************************************************************************/

#ifndef _TY_TYPE_H
#define _TY_TYPE_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef unsigned char 				TY_U8;
typedef unsigned short 				TY_U16;
typedef unsigned int            	TY_U32;
typedef unsigned long long			TY_U64;

typedef signed char             	TY_S8;
typedef short                   	TY_S16;
typedef int                     	TY_S32;
typedef long long					TY_S64;

/*float*/
typedef float               		TY_FLOAT;
/*double*/
typedef double                 		TY_DOUBLE;
typedef void						TY_VOID;
typedef char                    	TY_CHAR;

/*----------------------------------------------*
 * const defination                             *
 *----------------------------------------------*/
typedef enum {
    TY_FALSE = 0,
    TY_TRUE  = 1,
} TY_BOOL;

#ifndef NULL
    #define NULL    0L
#endif

#define TY_NULL     0L
#define TY_SUCCESS  0
#define TY_FAILURE  (-1)



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif


