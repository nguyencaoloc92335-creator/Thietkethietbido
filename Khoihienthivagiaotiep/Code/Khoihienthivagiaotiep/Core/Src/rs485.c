#include "rs485.h"
#include <string.h>

/* Hàm khởi tạo cấu hình ban đầu cho RS485 */
void RS485_Init(void)
{
    // Mặc định kéo chân DIR xuống LOW (Đặt MAX485 ở chế độ Nhận - Receiver)
    HAL_GPIO_WritePin(RS485_DIR_PORT, RS485_DIR_PIN, GPIO_PIN_RESET);
}

/* Hàm gửi một chuỗi ký tự qua đường truyền RS485 */
void RS485_SendString(char *str)
{
    // 1. Chuyển sang chế độ Phát (Transmitter) - Kéo chân DIR lên HIGH
    HAL_GPIO_WritePin(RS485_DIR_PORT, RS485_DIR_PIN, GPIO_PIN_SET);
    
    // 2. Gửi toàn bộ dữ liệu qua hàm HAL của UART (Bao gồm Timeout 1000ms)
    HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), 1000);
    
    // 3. Chuyển lại chế độ Nhận (Receiver) ngay sau khi gửi xong
    HAL_GPIO_WritePin(RS485_DIR_PORT, RS485_DIR_PIN, GPIO_PIN_RESET);
}