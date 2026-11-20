#include "union_holdings.h"
#define SIZE_ANSWER_WR_M_REGISTERS 6

#if Write_Single_Register_enabled > 0

uint8_t func_write_multiple_registers (uint8_t* ptr_frame, size_t num_frame,  uint8_t* buf_TX, uint8_t* TX_buf_size)
{
	xprintf("start MULTI register\r\n");
	uint16_t adrs_reg = 0;
	uint16_t num_reg = 0;
	uint8_t  num_reg_date = 0;
	uint16_t value_register = 0;
	uint8_t* copy_buf_TX = buf_TX;

	adrs_reg |= *(ptr_frame)<<8; /*High addres register*/
	adrs_reg |= *(ptr_frame+1); /*Low addres register  */

	num_reg |= *(ptr_frame+2)<<8; /*High num register*/
	num_reg |= *(ptr_frame+3); /*Low num register  */	
		
	num_reg_date |= *(ptr_frame + 4);
	
	ptr_frame = ptr_frame - 2;
	size_t som =  SIZE_ANSWER_WR_M_REGISTERS;

	while(som--)
	{	
		xprintf ("ANSWER: %x \r\n", *ptr_frame);
		*buf_TX++ = *ptr_frame++; /*copy dates*/
	}

	uint16_t resultCRC = mb_crc16 (copy_buf_TX, SIZE_ANSWER_WR_M_REGISTERS);
	*buf_TX++ = resultCRC&0x00FF; //Low_CRC
	*buf_TX++ = resultCRC>>8; //High_CRC



	uint8_t num_loop = num_reg;
	ptr_frame += 1;
	while (num_loop--)
	{
		value_register |= *(ptr_frame)<<8; 
		value_register |= *(ptr_frame + 1);
		buf_Holding_registers[adrs_reg] = value_register;
		value_register = 0;
		ptr_frame += 2;
		adrs_reg += 1;
	}

	*TX_buf_size = SIZE_ANSWER_WR_M_REGISTERS + 2;
	xprintf ("stop multi register \r\n");
	return 0;
}
#endif
