#include "lcd20x4.h"

/* Hàm tạo trễ micro giây (tương đối) dùng cho mạch thật 
   (Giả sử chip chạy ở thạch anh 72MHz) */
void delay_us(uint16_t us)
{
    // Ở mạch thật, vòng lặp này có tốc độ chạy chính xác hơn rất nhiều
    for (volatile uint32_t i = 0; i < (us * (72/4)); i++) 
    {
        __NOP();
    }
}

/* Hàm gửi 4 bit dữ liệu ra các chân GPIO */
void LCD_Send_4Bit(uint8_t data)
{
    HAL_GPIO_WritePin(LCD_PORT, D4_PIN, ((data >> 0) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_PORT, D5_PIN, ((data >> 1) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_PORT, D6_PIN, ((data >> 2) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_PORT, D7_PIN, ((data >> 3) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* Hàm gửi lệnh (Command) tới LCD */
void LCD_Command(uint8_t cmd)
{
    // Chọn thanh ghi lệnh (RS = 0)
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_RESET);
    
    // Gửi 4 bit cao
    LCD_Send_4Bit(cmd >> 4);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_SET);
    delay_us(50);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_RESET);
    delay_us(50);
    
    // Gửi 4 bit thấp
    LCD_Send_4Bit(cmd & 0x0F);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_SET);
    delay_us(50);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_RESET);
    
    // Mạch thật dùng HAL_Delay rất chính xác
    HAL_Delay(2); 
}

/* Hàm gửi dữ liệu (Data/Ký tự) tới LCD */
void LCD_Data(uint8_t data)
{
    // Chọn thanh ghi dữ liệu (RS = 1)
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_SET);
    
    // Gửi 4 bit cao
    LCD_Send_4Bit(data >> 4);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_SET);
    delay_us(50);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_RESET);
    delay_us(50);
    
    // Gửi 4 bit thấp
    LCD_Send_4Bit(data & 0x0F);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_SET);
    delay_us(50);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_RESET);
    
    delay_us(100);
}

/* Hàm khởi tạo LCD ở chế độ 4-bit */
void LCD_Init(void)
{
    HAL_Delay(50);  // Mạch thật: Đợi 50ms cho điện áp màn hình LCD ổn định
    
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_RESET);
    
    // Trình tự khởi tạo 4-bit theo datasheet HD44780
    LCD_Send_4Bit(0x03);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_SET); delay_us(50); HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_RESET);
    HAL_Delay(5); 
    
    LCD_Send_4Bit(0x03);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_SET); delay_us(50); HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_RESET);
    HAL_Delay(1); 
    
    LCD_Send_4Bit(0x03);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_SET); delay_us(50); HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_RESET);
    HAL_Delay(1); 
    
    LCD_Send_4Bit(0x02); // Chuyển sang chế độ 4-bit
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_SET); delay_us(50); HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_RESET);
    HAL_Delay(2); 
    
    // Cấu hình các thông số
    LCD_Command(0x28); // 4-bit mode, 2 dòng (hỗ trợ cho 20x4), 5x8 font
    LCD_Command(0x0C); // Bật hiển thị, tắt con trỏ
    LCD_Command(0x06); // Tự động tăng con trỏ
    LCD_Command(0x01); // Xóa màn hình
    HAL_Delay(2); 
}

/* Hàm xóa màn hình */
void LCD_Clear(void)
{
    LCD_Command(0x01);
    HAL_Delay(2); 
}

/* Hàm đặt vị trí con trỏ cho LCD 20x4 */
void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t address = 0;
    switch (row)
    {
        case 0: address = 0x80; break; // Dòng 1
        case 1: address = 0xC0; break; // Dòng 2
        case 2: address = 0x94; break; // Dòng 3
        case 3: address = 0xD4; break; // Dòng 4
    }
    address += col;
    LCD_Command(address);
}

/* Hàm in chuỗi ký tự */
void LCD_String(char *str)
{
    while (*str)
    {
        LCD_Data(*str++);
    }
}