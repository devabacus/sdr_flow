#ifndef CORRECT_H__
#define CORRECT_H__


#include <stdio.h>
#include <string.h>
#include "nrf_drv_pwm.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "boards.h"
#include "bsp.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"
//#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "flash_storage.h"
#include "rgb_led.h"
#include "remote.h"

//#define NEW_CORRECT
//#define OLD_CORRECT
#define LAST_CORRECT


#ifdef LAST_CORRECT

#define CORR_KG_PLUS 9 
#define CORR_KG_MINUS 7
#define CORR_PERCENT 10

#endif

#ifdef NEW_CORRECT

#define CORR_KG_PLUS 30 
#define CORR_KG_MINUS 29
#define CORR_PERCENT 16

#endif

#ifdef OLD_CORRECT

#define CORR_KG_PLUS 27 
#define CORR_KG_MINUS 26
#define CORR_PERCENT 17

#endif



#define TOP_VALUE 1000


extern uint32_t corr_1_1; 
extern uint32_t corr_1_2; 
extern uint32_t corr_1_3; 

extern uint32_t corr_2_1; 
extern uint32_t corr_2_2; 
extern uint32_t corr_2_3; 

extern uint32_t corr_3_1; 
extern uint32_t corr_3_2; 
extern uint32_t corr_3_3; 



extern uint8_t reset_by_change_but_level;

extern uint32_t corr_counter;

void correct(uint32_t value, uint32_t value1, uint32_t value2);
void pwm_init_corr(void);

void corr_plus(uint32_t value);
void corr_minus(uint32_t value);
void corr_perc(uint32_t value);

void correct_value(uint32_t value);
void corr_counter_inc(void);


typedef enum {
	COR_SIMPLE,
	COR_OFFSET_KG,
	COR_AUTOZERO
	
} percent_cor_mode_t;

#define COR_MINUS_1 400
#define COR_MINUS_2 450
#define COR_MINUS_3 550

#define COR_PLUS_1 1400
#define COR_PLUS_2 1450
#define COR_PLUS_3 1550

#define COR_PERC_1 2020
#define COR_PERC_2 2100
#define COR_PERC_3 2150




#define COR_PLUS_START 1350
#define COR_PERC_START 2350






#endif
