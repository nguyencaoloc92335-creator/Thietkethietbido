#ifndef RS485_H
#define RS485_H

#include "stm32f1xx_hal.h"

/* * Định nghĩa chân điều khiển chiều thu/phát cho MAX485 (DE/RE)
 * Ở ví dụ này giả sử dùng chân PA8 làm chân điều khiển DIR (Direction)
 */
#define RS485_DIR_PORT GPIOA
#define RS485_DIR_PIN  GPIO_PIN_8

/* Khai báo biến UART dùng cho RS485 (Mặc định CubeMX sinh ra là huart1) */
extern UART_HandleTypeDef huart1;

void RS485_Init(void);
void RS485_SendString(char *str);

#endif /* RS485_H */