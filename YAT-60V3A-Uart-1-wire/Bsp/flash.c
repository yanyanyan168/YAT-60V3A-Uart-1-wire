#include "flash.h"

/* 校准区范围检查：只允许访问 0x3F80~0x3FFF，完整 128 Bytes。 */
static bit flash_user_range_ok(u8 addr, u16 len)
{
    if(len == 0U)
    {
        return 0;
    }

    if(((u16)addr + len) > FLASH_CAL_MAX_SIZE)
    {
        return 0;
    }

    return 1;
}

/**
  * @brief  擦除校准扇区。
  * @param  addr  校准区内部偏移，目前只允许 0。
  * @note   实际擦除 0x3F80~0x3FFF。
  */
void flash_ferase_sector(u8 addr)
{
    u8 flash_addr_l;

    /* 只允许按 128 Bytes 扇区起始地址擦除。 */
    if((addr & (FLASH_USER_SECTOR_SIZE - 1U)) != 0U)
    {
        return;
    }

    /* 当前只允许擦除校准区所在扇区。 */
    if(!flash_user_range_ok(addr, FLASH_CAL_MAX_SIZE))
    {
        return;
    }

    flash_addr_l = FLASH_CAL_SECTOR_L + addr;

    IE_EA = 0;

    CLK_CON6       = (CLK_CON6 & 0xC0) | 0x4;
    FLASH_ADDR     = FLASH_USER_PAGE_H;
    FLASH_ADDR     = flash_addr_l;
    FLASH_PASSWORD = FLASH_PASSWORD(0xB9);
    FLASH_CON      = FLASH_SER_TRG(0x1);

    while(!(FLASH_STA & FLASH_SER_FLG(0x1)));

    CLK_CON6 |= 0x2F;

    IE_EA = 1;
}

/**
  * @brief  写入用户 Flash 校准数据。
  * @param  addr    校准区内部偏移，实际地址为 0x3F80 + addr。
  * @param  p_data  待写入数据指针。
  * @param  len     写入长度。
  * @note   当前允许写入完整校准扇区：0x3F80~0x3FFF。
  */
void flash_program(u8 addr, u8 *p_data, u8 len)
{
    if(!flash_user_range_ok(addr, len))
    {
        return;
    }

    IE_EA = 0;

    FLASH_ADDR = FLASH_USER_PAGE_H;
    FLASH_ADDR = FLASH_CAL_SECTOR_L + addr;

    while(len != 0U)
    {
        while(!(FLASH_STA & FLASH_PROG_FLG(0x1)));

        FLASH_DATA     = *(p_data++);
        FLASH_PASSWORD = FLASH_PASSWORD(0xB9);
        FLASH_CON      = FLASH_PROG_TRG(0x1);

        len--;
    }

    while(!(FLASH_STA & FLASH_PROG_FLG(0x1)));

    IE_EA = 1;
}

/**
  * @brief  读取用户 Flash 校准数据。
  * @param  addr    校准区内部偏移，实际地址为 0x3F80 + addr。
  * @param  p_data  读取数据保存指针。
  * @param  len     读取长度。
  */
void flash_fread(u8 addr, u8 *p_data, u16 len)
{
    if(!flash_user_range_ok(addr, len))
    {
        return;
    }

    IE_EA = 0;

    FLASH_TRIM |= 0xC;

    while(len != 0U)
    {
        *(p_data++) = *((u8 code *)(FLASH_USER_PAGE_START + FLASH_CAL_SECTOR_L + addr));
        addr++;
        len--;
    }

    FLASH_TRIM = (FLASH_TRIM & 0xF3) | 0x4;

    IE_EA = 1;
}