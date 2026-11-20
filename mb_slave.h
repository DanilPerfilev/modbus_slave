#ifndef MB_SLAVE_H
#define MB_SLAVE_H

#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#define Read_Coil     0x01       
#define Read_Discrete_Inputs     0x02   
#define Read_Holding_Registers   0x03 
#define Read_Input_Registers     0x04     
#define Write_Single_Coil     0x05     
#define Write_Single_Register 0x06
#define Write_Multiple_Coils  0x0f
#define Write_Multiple_Registers 0x10

#define size_frame_for_send 8
#define size_max_frame 24
#define BUFFER_MODBUS_SIZE 128

#define transmit_function_not_supported 0x01
#define false_elemantary_address 0x02
#define false_dates 0x04
#define error_CRC 0x10
#define set_parametres_forbidden 0x20

/*include type dates (registers)  */
#define Discrete_input 1
#define Coil 1
#define Input_Register 1
#define Holding_registers 1

#define mbRegisters_buf 50 

#define ADDRESS_DEVICE 0x0c

#define MB_FUNC_HANDLERS_MAX                    ( 8 )

/*! \brief If the <em>Read Input Registers</em> function should be enabled. */
#define Read_coil_enabled              (  1 )

/*! \brief If the <em>Read Holding Registers</em> function should be enabled. */
#define Read_Discrete_Inputs_enabled            (  1 )

/*! \brief If the <em>Write Single Register</em> function should be enabled. */
#define Read_Holding_Registers_enabled           (  1 )

/*! \brief If the <em>Write Multiple registers</em> function should be enabled. */
#define Read_Input_Registers_enabled  (  1 )

/*! \brief If the <em>Read Coils</em> function should be enabled. */
#define Write_Single_Register_enabled              (  1 )

/*! \brief If the <em>Write Coils</em> function should be enabled. */
#define Write_Multiple_Coils_enabled              (  1 )

/*! \brief If the <em>Write Multiple Coils</em> function should be enabled. */
#define Write_Multiple_Registers_enabled    (  1 )

/*! \brief If the <em>Read Discrete Inputs</em> function should be enabled. */
#define Write_Single_Coil_enabled    (  1 )

/*include buf_registers*/
#if Discrete_input > 0
	extern uint8_t buf_Discrete_input [mbRegisters_buf];
#endif
#if Coil > 0
	extern uint8_t buf_Coil [mbRegisters_buf];
#endif
#if Input_Register > 0
	extern uint16_t buf_Input_registers[mbRegisters_buf];
#endif
#if Holding_registers > 0
	extern uint16_t buf_Holding_registers[mbRegisters_buf];
#endif

typedef struct
{
	uint8_t buf_frame[BUFFER_MODBUS_SIZE];
 	volatile size_t buf_size_frame;
 	volatile bool frame_process;
} bufFrame;

uint8_t modbus_creater_answer(uint8_t* ptr_mb_buf, size_t mb_buf_size, uint8_t *resp_buf, 
    uint8_t *resp_buf_size);
uint16_t mb_crc16(uint8_t* ptr_ADUFrame, size_t size_ADUFrame);

#include "union_holdings.h"


#endif
