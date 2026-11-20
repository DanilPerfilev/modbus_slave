#include "mb_slave.h"
#include "xprintf.h"

/*Временная заглушка  */
//uint8_t Func_Write_Multiple_Registers(uint8_t* ptr_frame, size_t num_frame, uint8_t* TX_buf, uint8_t* TX_buf_size){ /* TODO */ };
uint8_t Func_Write_Multiple_Coils(uint8_t* ptr_frame, size_t num_frame, uint8_t* TX_buf, uint8_t* TX_buf_size){ /* TODO */ };
//uint8_t func_write_single_register(uint8_t* ptr_frame, size_t num_frame, uint8_t* TX_buf, uint8_t* TX_buf_size){ /* TODO */ };
//void Func_Write_Single_Coil(uint8_t* ptr_frame, size_t num_frame){ /* TODO */ };
//void Func_read_coil(uint8_t* ptr_frame, size_t num_frame){ /* TODO */ };
//void Func_Read_Discrete_Inputs(uint8_t* ptr_frame, size_t num_frame){ /* TODO */ };
//void Func_Read_Holding_Registers(uint8_t* ptr_frame, size_t num_frame){ /* TODO */ };
//void func_read_Input_Registers(uint8_t* ptr_frame, size_t num_frame){ /* TODO */ };

typedef enum {
	MB_ENOERR,	/* no error. */
	MB_ENOREG,	/* illegal register address. */
	MB_EINVAL,	/* illegal argument. */
	MB_EPORTERR,	/* porting layer error. */
	MB_ENORES,	/* insufficient resources. */
	MB_EIO,		/* I/O error. */
	MB_EILLSTATE,	/* protocol stack in illegal state. */
	MB_ETIMEDOUT,	/* timeout error occurred. */
	MB_ECRCERR,	/* CRC mismatch */
	MB_ENOTSUP	/* function not supported */
} eMBErrorCode;

eMBErrorCode parsing_frame(uint8_t* ptr_frame, size_t num_frame);

typedef struct
{
	uint8_t num_code_func;
	uint8_t (*prt_code_func_handler)(uint8_t*, size_t, uint8_t*, uint8_t*);
}CommandHandler;

/*include buf_registers*/
#if Discrete_input > 0
	uint8_t buf_Discrete_input [mbRegisters_buf] = {1,0,1,0};
#endif
#if Coil > 0
	uint8_t buf_Coil [mbRegisters_buf] = {0,1,0,1};
#endif
#if Input_Register > 0
	uint16_t buf_Input_registers[mbRegisters_buf] = {100, 200, 300, 400};
#endif
#if Holding_registers > 0
	uint16_t buf_Holding_registers[mbRegisters_buf] = {500,600,700,800};
#endif

/*include code_function and registers  */
static CommandHandler xFuncHandlers[MB_FUNC_HANDLERS_MAX] = {
#if Read_coil_enabled > 0
	{Read_Coil, func_read_coil},
#endif
#if Read_Discrete_Inputs_enabled > 0
	{Read_Discrete_Inputs, func_read_discrete_inputs},
#endif
#if Read_Holding_Registers_enabled > 0
	{Read_Holding_Registers, func_read_holding_registers},
#endif
#if Read_Input_Registers_enabled > 0
	{Read_Input_Registers, func_read_input_registers},
#endif
#if Write_Single_Register_enabled > 0
	{Write_Single_Register, func_write_single_register},
#endif
#if Write_Multiple_Coils_enabled > 0
	{Write_Multiple_Coils, Func_Write_Multiple_Coils},
#endif
#if Write_Multiple_Registers_enabled > 0
	{Write_Multiple_Registers, func_write_multiple_registers},
#endif
#if Write_Single_Coil_enabled > 0  
	{Write_Single_Coil, func_write_single_coil},
#endif
}; 

uint8_t
check_frame_crc (uint8_t* ptr_frame, size_t num_frame);

uint16_t mb_crc16 (uint8_t* ptr_ADUFrame, size_t size_ADUFrame);

uint8_t selection_frame_PDU (uint8_t* ptr_frame);

uint8_t
modbus_creater_answer(uint8_t *ptr_mb_buf, size_t mb_buf_size,
		      uint8_t *TX_buf, uint8_t *TX_buf_size)
{
	//function UART_Reciever_IDLE transfer in this function
	//In UART_Reciever_IDLE necessery set flag of bufFrameRX.frame_process = true
	//further necessery transfer pointer of buffer dates and size of buffer dates
	eMBErrorCode state_err = MB_ENOERR;

	if (*ptr_mb_buf != ADDRESS_DEVICE)
		return MB_ENOERR;

	state_err = check_frame_crc(ptr_mb_buf, mb_buf_size);
	if (state_err != MB_ENOERR)
		return state_err;

//	uint8_t (*func_handler)(uint8_t *, size_t, uint8_t *, uint8_t *);
	uint8_t func_number = selection_frame_PDU(ptr_mb_buf + 1);
	xprintf("code_function: %d\n\r", func_number);
	if (func_number == MB_ENOTSUP) {
		#ifdef DEBUG
		xprintf("modbus_creater_answer: func_handler is NULL\r\n");
		#endif
		return MB_ENOTSUP;
	}
	xprintf("coil: %d\n\r", buf_Coil[0]);
	xprintf("discrete: %d\n\r", buf_Discrete_input[0] );
	xprintf("holdings: %d\n\r", buf_Holding_registers[0]);
	xprintf("input: %d\n\r", buf_Input_registers[0]);

	return xFuncHandlers[func_number].prt_code_func_handler (ptr_mb_buf + 2, mb_buf_size - 2, TX_buf, TX_buf_size);
}

uint16_t
mb_crc16 (uint8_t* ptr_ADUFrame, size_t size_ADUFrame)
{
  uint16_t rg16 = 0xFFFF; //1. load 16-bit register
  for(size_t ib = 0; ib < size_ADUFrame; ib++) //7. repeat all the bytes of data
  {
    rg16 = ptr_ADUFrame[ib] ^ rg16; //2. fisrt-byte data XOR Highbyte register  
  
    for(size_t i = 0; i<8; i++) // 6. repeat for 8
    {
      uint8_t carry = rg16&1;
	  rg16 = rg16>>1;
      if( carry == 1) //3. shift register in right
      {
        rg16 = rg16^(0xA001); //4. if True, then XOR 16byte XOR 0xA001
      }
	  //5. if False, then continue
    }
  }
  return rg16;
}

uint8_t 
selection_frame_PDU (uint8_t* ptr_frame) {	
	uint8_t func = *ptr_frame;
	xprintf("code_function: %d\n", func);
	/*checking existence code function  */
	/*call need function  */
	for(uint8_t i = 0; i < MB_FUNC_HANDLERS_MAX; i++)
	{
		xprintf("i: %d\n", i);
		xprintf("xFuncHandlers[].num_code_func: %d\r\n", xFuncHandlers[i].num_code_func);
		if(func == xFuncHandlers[i].num_code_func)
		{
			return i;
		//	return xFuncHandlers[i].prt_code_func_handler;  
		}
	}
	return MB_ENOTSUP;
}

uint8_t
check_frame_crc (uint8_t* ptr_frame, size_t num_frame){
	eMBErrorCode state_err = MB_ENOERR;
	
	/*selection CRC16  */
	uint16_t CRC_puc = *(ptr_frame + (num_frame-1))<<8;       /*lowCRC  */
	CRC_puc = CRC_puc | *(ptr_frame + (num_frame-2)); /*highCRC  */
	
	if(mb_crc16 (ptr_frame, num_frame-2) == CRC_puc ) //checking CRC16
		return MB_ENOERR;
	else
		return MB_ECRCERR;
}



//https://ipc2u.ru/articles/prostye-resheniya/modbus-rtu/
