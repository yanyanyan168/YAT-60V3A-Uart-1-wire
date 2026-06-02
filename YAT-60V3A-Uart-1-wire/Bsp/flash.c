#include "flash.h"

static bit flash_user_range_ok(u8 addr, u16 len)
{
    if(len == 0U)
    {
        return 0;
    }

    /* 当前只允许访问校准区：0x3F00~0x3F7F。 */
    if(((u16)addr + len) > FLASH_CAL_MAX_SIZE)
    {
        return 0;
    }

    return 1;
}

/**
  * @brief  flash erase sector
  * @param  addr : sector address in flash
  * @retval None
  */
void flash_ferase_sector(u8 addr)
{
    if((addr & (FLASH_EEPROM_SECTOR_SIZE - 1U)) != 0U)
    {
        return;
    }

    if(!flash_user_range_ok(addr, FLASH_EEPROM_SECTOR_SIZE))
    {
        return;
    }

    IE_EA = 0;
    
    CLK_CON6       = (CLK_CON6 & 0xC0) | 0x4;
    FLASH_ADDR     = FLASH_EEPROM_PAGE_H;
    FLASH_ADDR     = addr;
    FLASH_PASSWORD = FLASH_PASSWORD(0xB9);          // 写入操作密码
    FLASH_CON      = FLASH_SER_TRG(0x1);            // 触发扇区擦除
    
    while(!(FLASH_STA & FLASH_SER_FLG(0x1)));       // 等待扇区擦除空闲
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
    if(!flash_user_range_ok(addr, len))
    {
        return;
    }

    IE_EA = 0;
    
    FLASH_ADDR = FLASH_EEPROM_PAGE_H;
    FLASH_ADDR = addr;
    
    while(len >= 1) {
        while(!(FLASH_STA & FLASH_PROG_FLG(0x1)));  // 等待烧录空闲
        FLASH_DATA     = *(p_data++);
        FLASH_PASSWORD = FLASH_PASSWORD(0xB9);      // 写入操作密码
        FLASH_CON      = FLASH_PROG_TRG(0x1);       // 触发烧录
        
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
    if(!flash_user_range_ok(addr, len))
    {
        return;
    }

    IE_EA = 0;
    
    FLASH_TRIM |= 0xC;
    while(len != 0) {
        *(p_data++) = *((u8 code *)(FLASH_EEPROM_PAGE_START + addr++));
        len--;
    }
    FLASH_TRIM = (FLASH_TRIM & 0xF3) | 0x4;
    
    IE_EA = 1;
}
