#include "union_holdings.h"
#define SIZE_ADU_WR_S_COIL 8
#if Write_Single_Coil_enabled  

uint8_t func_write_single_coil(uint8_t* ptr_frame, size_t num_frame,  uint8_t* buf_TX, uint8_t* TX_buf_size)
{
	xprintf("start single coil");
	uint16_t adrs_reg = 0;
	uint8_t value_coil = 0;
			
	adrs_reg |= *(ptr_frame)<<8; /*High addres register*/
	adrs_reg |= *(ptr_frame+1); /*Low addres register  */
		
	if ( *(ptr_frame + 2) == 0xFF)
	{
		value_coil = 1;
	}

	buf_Coil[adrs_reg] = value_coil;

	size_t som =  SIZE_ADU_WR_S_COIL;

	ptr_frame = ptr_frame - 2;

	while(som--)
	{
		*buf_TX++ = *ptr_frame++; /*copy dates*/
	}

	*TX_buf_size = SIZE_ADU_WR_S_COIL;
	xprintf ("stop single coil");
	return 0;
}
#endif
