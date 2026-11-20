#include "union_holdings.h"
#define SIZE_ADU_WR_S_REGISTER 9

#if Write_Single_Register_enabled > 0

uint8_t func_write_single_register(uint8_t* ptr_frame, size_t num_frame,  uint8_t* buf_TX, uint8_t* TX_buf_size)
{
	xprintf("start single register");
	uint16_t adrs_reg = 0;
	uint16_t value_register = 0;
	
	adrs_reg |= *(ptr_frame)<<8; /*High addres register*/
	adrs_reg |= *(ptr_frame+1); /*Low addres register  */
		
	value_register |= *(ptr_frame + 2)<<8; 
	value_register |= *(ptr_frame + 3); 

	buf_Holding_registers[adrs_reg] = value_register;

	size_t som =  SIZE_ADU_WR_S_REGISTER;

	ptr_frame = ptr_frame - 2;

	while(som--)
	{
		*buf_TX++ = *ptr_frame++; /*copy dates*/
	}

	*TX_buf_size = SIZE_ADU_WR_S_REGISTER;
	xprintf ("stop single register");
	return 0;
}
#endif
