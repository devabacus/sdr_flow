#include "correct.h"

nrf_drv_pwm_t m_pwm_cor = NRF_DRV_PWM_INSTANCE(0);
nrf_pwm_values_individual_t seq_value;
nrf_pwm_sequence_t seq_corr;


uint32_t corr_1_1 = COR_MINUS_1; 
uint32_t corr_1_2 = COR_MINUS_2; 
uint32_t corr_1_3 = COR_MINUS_3; 
uint32_t corr_2_1 = COR_PLUS_1; 
uint32_t corr_2_2 = COR_PLUS_2; 
uint32_t corr_2_3 = COR_PLUS_3;  
uint32_t corr_3_1 = COR_PERC_1;
uint32_t corr_3_2 = COR_PERC_2;
uint32_t corr_3_3 = COR_PERC_3;

uint32_t corr_counter = 0;
uint8_t reset_by_change_but_level = 0;
percent_cor_mode_t percent_cor_mode = COR_OFFSET_KG;

void corr_counter_inc(void)
{
			corr_counter++;
			//SEGGER_RTT_printf(0, "corr_counter = %d\n\r", corr_counter);
			fds_update_value(&corr_counter, file_id_c, fds_rk_corr_counter);
}

void pwm_init_corr(void)

{
  
	nrf_drv_pwm_config_t const config0 =
	{
			.output_pins =
			{
					CORR_KG_PLUS, // channel 0
					CORR_KG_MINUS,             // channel 1
					CORR_PERCENT,             // channel 2
					NRF_DRV_PWM_PIN_NOT_USED,             // channel 3
			},
			.irq_priority = APP_IRQ_PRIORITY_LOW,
			.base_clock   = NRF_PWM_CLK_8MHz,
			.count_mode   = NRF_PWM_MODE_UP,
			.top_value    = TOP_VALUE,
			.load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
			.step_mode    = NRF_PWM_STEP_AUTO
	};
	
	nrf_drv_pwm_init(&m_pwm_cor, &config0, NULL);
	
	seq_value.channel_0 = TOP_VALUE;
	seq_value.channel_1 = TOP_VALUE;
	seq_value.channel_2 = TOP_VALUE;

	seq_corr.values.p_individual = &seq_value;
	seq_corr.length = NRF_PWM_VALUES_LENGTH(seq_value);
	seq_corr.repeats             = 0;
  seq_corr.end_delay           = 0;
	nrf_drv_pwm_simple_playback(&m_pwm_cor, &seq_corr, 0, NRF_DRV_PWM_FLAG_LOOP);
	
}

void update_seq(void)
{
	seq_corr.values.p_individual = &seq_value;
	seq_corr.length = NRF_PWM_VALUES_LENGTH(seq_value);
	nrf_drv_pwm_simple_playback(&m_pwm_cor, &seq_corr, 0, NRF_DRV_PWM_FLAG_LOOP);
}

void correct(uint32_t value, uint32_t value1, uint32_t value2)
	{
	
	seq_value.channel_0 = TOP_VALUE - value;
	seq_value.channel_1 = TOP_VALUE - value1;
	seq_value.channel_2 = TOP_VALUE - value2;
	seq_corr.values.p_individual = &seq_value;
	seq_corr.length = NRF_PWM_VALUES_LENGTH(seq_value);
	nrf_drv_pwm_simple_playback(&m_pwm_cor, &seq_corr, 0, NRF_DRV_PWM_FLAG_LOOP);
		
}
	
void corr_plus(uint32_t value)
{
	SEGGER_RTT_printf(0, "plus %d\r\n", value);
	seq_value.channel_0 = TOP_VALUE - value;
	update_seq();
}

void corr_minus(uint32_t value)
{
	SEGGER_RTT_printf(0, "min %d\r\n", value);
	seq_value.channel_1 = TOP_VALUE - value;
	update_seq();
}

void corr_perc(uint32_t value)
{
	SEGGER_RTT_printf(0, "perc %d\r\n", value);
	seq_value.channel_2 = TOP_VALUE - value;
	update_seq();
}

	

void correct_value(uint32_t value)
{
	if((activate_status >= DEMO)&&(!exp_subsriber))
	{
		if(0 < value && value <= 1000) // minus correct
		{
			//correct(0, value, 0); 
			corr_minus(value);
			corr_plus(0);
			if(percent_cor_mode == COR_SIMPLE)
			{
				corr_perc(0);
			}
			
		}
		else if (1000 < value && value <= 2000) // plus correct
		{
			value = value - 1000;
			//SEGGER_RTT_printf(0, "value in correct_value = %d\r\n", value);
			corr_plus(value); 
			corr_minus(0);
			
			if(percent_cor_mode == COR_SIMPLE)
			{
				corr_perc(0);
			}
		
		}
		
		//It needs to change this function for offset kg mode
		else if (2000 < value && value <= 3000)  // percent correct
		{
			value = value - 2000;
			corr_perc(value);
				
				if(percent_cor_mode == COR_OFFSET_KG)
				{
						offset_kg_cor_work();
				}
			
				else if(percent_cor_mode == COR_SIMPLE)
				{
						corr_plus(0);
						corr_minus(0);
				}
		}
	
	if((remote_mode == WORK_MODE)&&(!pin_in4_is_release)) // because we don't want to count while set up buttons
		{
			corr_counter_inc();			
		}
	}
	
	else if (!activate_status) // EXP_DEMO occured
	{
		if(!pin_in4_is_release) // blink 3 times only for correct buttons means that the demo mode is expired
		{
			rgb_set(50, 0, 0, 3, 500);
		}
		
	}
	else if (activate_attempts > ACTIVATE_ATTEMPTS_MAX)
	{
		rgb_set(0, 0, 50, 3, 500);
	}
	
		
}
