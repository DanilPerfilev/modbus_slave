#ifndef UNION_HOLDINGS_H
#define UNION_HOLDINGS_H

#include "mb_slave.h"
uint8_t func_write_multiple_registers(uint8_t* ptr_frame, size_t num_frame, uint8_t* buf_TX, uint8_t* TX_buf_size);
//void Func_Write_Multiple_Coils(uint8_t* ptr_frame, size_t num_frame); 
uint8_t func_write_single_register(uint8_t* ptr_frame, size_t num_frame, uint8_t* buf_TX, uint8_t* TX_buf_size);
uint8_t func_write_single_coil(uint8_t* ptr_frame, size_t num_frame, uint8_t* buf_TX, uint8_t* TX_buf_size ); 

uint8_t func_read_coil(uint8_t* ptr_frame, size_t num_frame, uint8_t* buf_TX, uint8_t* TX_buf_size); 
uint8_t func_read_discrete_inputs(uint8_t* ptr_frame, size_t num_frame, uint8_t* buf_TX, uint8_t* TX_buf_size );
uint8_t func_read_holding_registers(uint8_t* ptr_frame, size_t num_frame, uint8_t* buf_TX, uint8_t* TX_buf_size);
uint8_t func_read_input_registers(uint8_t* ptr_frame, size_t num_frame, uint8_t* buf_TX, uint8_t* TX_buf_size);
#endif
