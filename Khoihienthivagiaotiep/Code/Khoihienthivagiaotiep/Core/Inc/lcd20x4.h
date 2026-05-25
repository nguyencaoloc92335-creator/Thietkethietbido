#ifndef LCD20X4_H
#define LCD20X4_H

#include <stm32f1xx_hal.h>

/* Định nghĩa các chân kết nối LCD (Có thể thay đổi theo cấu hình CubeMX của bạn) */
#define LCD_PORT GPIOA

#define RS_PIN GPIO_PIN_1
#define EN_PIN GPIO_PIN_2
// Chân RW nối thẳng xuống GND (0V) để tiết kiệm chân IO

#define D4_PIN GPIO_PIN_3
#define D5_PIN GPIO_PIN_4
#define D6_PIN GPIO_PIN_5
#define D7_PIN GPIO_PIN_6

/* Khai báo các hàm điều khiển LCD */
void LCD_Init(void);
void LCD_Command(uint8_t cmd);
void LCD_Data(uint8_t data);
void LCD_String(char *str);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Clear(void);

#endif /* LCD20X4_H */