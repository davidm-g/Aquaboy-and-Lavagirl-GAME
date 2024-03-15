#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define OBF BIT(0)
#define ENABLE_INTERRUPT BIT(0)
#define IBF BIT(1)
#define INH BIT(4)
#define AUX BIT(5)
#define TIM_ERR BIT(6)
#define PAR_ERR BIT(7)
#define ST_REGISTER 0x64
#define READ_CMD_BYTE 0x20
#define WRITE_CMD_BYTE 0x60
#define OUT_BUF 0x60
#define KBC_CMD_REG 0x64
#define ESC_BREAK 0x81

#define KEYBOARD_IRQ 1

#endif /* _LCOM_I8042_H_ */
