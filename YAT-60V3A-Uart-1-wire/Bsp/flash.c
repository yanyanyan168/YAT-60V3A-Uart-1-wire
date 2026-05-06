#include "flash.h"

/**
  * @brief  flash erase sector
  * @param  addr : sector address in flash
  * @retval None
  */
void flash_ferase_sector(u8 addr)
{
    IE_EA = 0;
    
    CLK_CON6       = (CLK_CON6 & 0xC0) | 0x4;
    FLASH_ADDR     = 0x3F;
    FLASH_ADDR     = addr;
    FLASH_PASSWORD = FLASH_PASSWORD(0xB9);          // Ð´Èë²Ù×÷ÃÜÂë
    FLASH_CON      = FLASH_SER_TRG(0x1);            // ´¥·¢ÉÈÇø²Á³ý
    
    while(!(FLASH_STA & FLASH_SER_FLG(0x1)));       // µÈ´ýÉÈÇø²Á³ý¿ÕÏÐ
    CLK_CON6 |= 0x2F;
    
    IE_EA = 1;
}

/**
  * @brief  flash program
  * @param  addr   : Write data address in flash
  * @param  p_data : Write data to flash
  * @param  len    : Data length
  * @retval None
  */
void flash_program(u8 addr, u8 *p_data, u8 len)
{
    IE_EA = 0;
    
    FLASH_ADDR = 0x3F;
    FLASH_ADDR = addr;
    
    while(len >= 1) {
        while(!(FLASH_STA & FLASH_PROG_FLG(0x1)));  // µÈ´ýÉÕÂ¼¿ÕÏÐ
        FLASH_DATA     = *(p_data++);
        FLASH_PASSWORD = FLASH_PASSWORD(0xB9);      // Ð´Èë²Ù×÷ÃÜÂë
        FLASH_CON      = FLASH_PROG_TRG(0x1);       // ´¥·¢ÉÕÂ¼
        
        len -= 1;
    }
    
    IE_EA = 1;
}

/**
  * @brief  flash program
  * @param  addr   : Read data address in flash
  * @param  p_data : Read data to flash
  * @param  len    : Data length
  * @retval None
  */
void flash_fread(u8 addr, u8 *p_data, u16 len)
{
    IE_EA = 0;
    
    FLASH_TRIM |= 0xC;
    while(len != 0) {
        *(p_data++) = *((u8 code *)(0x3F00 + addr++));
        len--;
    }
    FLASH_TRIM = (FLASH_TRIM & 0xF3) | 0x4;
    
    IE_EA = 1;
}
