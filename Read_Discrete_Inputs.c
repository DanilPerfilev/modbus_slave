#include "union_holdings.h"
#include "xprintf.h"



#if Read_Discrete_Inputs_enabled  > 0

	uint8_t
	func_read_discrete_inputs (uint8_t* ptr_frame, size_t num_frame, uint8_t* buf_TX, uint8_t* TX_buf_size)
	{
		xprintf("start discrete");
		uint8_t* copy_buf_TX = buf_TX;
		*buf_TX++ = ADDRESS_DEVICE;
		*buf_TX++ = Read_Discrete_Inputs;
		
		uint16_t adrs_reg = 0;
		uint16_t num_reg = 0;
			
		adrs_reg |= *(ptr_frame)<<8; /*High addres register*/
		adrs_reg |= *(ptr_frame+1); /*Low addres register  */
		
		num_reg |= *(ptr_frame+2)<<8; /*High num register*/
		num_reg |= *(ptr_frame+3); /*Low num register  */

		size_t size_dates = (size_t)((num_reg + 7)/8);
		*buf_TX++ = size_dates; 
		*TX_buf_size = size_dates + 5;

		uint8_t* ptr_buf_discrete_registers = &buf_Discrete_input[adrs_reg];

		uint8_t answer_discrete [size_dates];
		for ( int c = 0; c <= size_dates; c++)
		{
			answer_discrete[c] = 0;
		}
		uint8_t* ptr_answer_discrete = answer_discrete;
		uint8_t index_buf_discrete = 0;
		uint8_t shift_ptr_discrete = 0;
		uint8_t save_buf_num = 0;
		for (uint8_t j = size_dates; j>0; j--)
		{
			for (int8_t i = 0; i<8; i++)
			{
				save_buf_num |= (*(ptr_buf_discrete_registers + shift_ptr_discrete)<<i);
				shift_ptr_discrete++;
			}
			shift_ptr_discrete = 0;
			answer_discrete[index_buf_discrete] = save_buf_num;
			save_buf_num = 0;
			index_buf_discrete++;
		}
			
		size_t som = size_dates;

		while(som--)
		{
			*buf_TX++ = *ptr_answer_discrete++; /*copy dates*/
		}
		
		uint16_t resultCRC = mb_crc16(copy_buf_TX, size_dates + 3);
		*buf_TX++ = resultCRC&0x00FF; //Low_CRC
		*buf_TX++ = resultCRC>>8; //High_CRC
		xprintf("stop discrete");
		return 0;
	}

#endif
