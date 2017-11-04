/**
 * \file
 *         SHT7x.c
 * \description
 *         Driver of SHT71
 * \author
 *         XXXXXXXXXXXXXXX
 * \date
 *         2015-08-09 10:00
 * \copyright
 *         (c) RimeLink (www.rimelink.com) All Rights Reserved.
 */
#ifndef __SHT7x_H__
#define __SHT7x_H__

/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
enum {TEMP,HUMI};

typedef union 
{ 
    unsigned int    i;
    float    f;
} sht_value;


/* Private function prototypes -----------------------------------------------*/
void s_connectionreset(void);
char s_softreset(void);
char s_measure(unsigned char *p_value, unsigned char *p_checksum, unsigned char mode);
void calc_sth11(float *p_humidity ,float *p_temperature);
float calc_dewpoint(float h,float t);
void sht1x_Init(void);


#endif /*__SHT7x_H__*/

