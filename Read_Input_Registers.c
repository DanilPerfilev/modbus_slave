#include "union_holdings.h"
#include "xprintf.h"

/*buf_Input_registers*/

#if Read_Input_Registers_enabled > 0
	
	uint8_t 
	func_read_input_registers(uint8_t* ptr_frame, size_t num_frame, uint8_t* buf_TX, uint8_t* TX_buf_size)
	{
		xprintf("start input \r\n");
		uint8_t* copy_buf_TX = buf_TX;

		*buf_TX++ = ADDRESS_DEVICE;
		*buf_TX++ = Read_Input_Registers;

		uint16_t adrs_reg = 0;
		uint16_t num_reg = 0;
		adrs_reg |= *(ptr_frame)<<8; /*High addres register*/
		adrs_reg |= *(ptr_frame+1); /*Low addres register  */
		
		num_reg |= *(ptr_frame+2)<<8; /*High num register*/
		num_reg |= *(ptr_frame+3); /*Low num register  */	
	
		size_t size_dates = (sizeof(uint16_t) * num_reg);
		*buf_TX++ = size_dates;

		uint16_t* ptr_buf_Input_registers = &buf_Input_registers[adrs_reg];
		
		for(size_t i = 0; i < num_reg; i++)
		{
			*buf_TX++ = *(ptr_buf_Input_registers)>>8; /*High */
			*buf_TX++ = *(ptr_buf_Input_registers) & 0x00FF; /*Low*/
			ptr_buf_Input_registers++;
		}
		
		uint16_t resultCRC = mb_crc16 (copy_buf_TX, size_dates + 3);
		*buf_TX++ = resultCRC&0x00FF; //Low_CRC
		*buf_TX++ = resultCRC>>8; //High_CRC

		*TX_buf_size = size_dates + 5;	
		xprintf ("stop input \r\n");
		return 0;
	}
#endif
