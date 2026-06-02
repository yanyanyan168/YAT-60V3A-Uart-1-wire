#ifndef __FLASH_LAYOUT_H__
#define __FLASH_LAYOUT_H__

/*
 * Flash 布局：
 *
 * 0x0000 ~ 0x3F7F : Code 区
 * 0x3F80 ~ 0x3FFF : 校准数据区，最后 128 Bytes 扇区
 *
 * 注意：
 * 本项目当前不使用 Code CRC 做量产校验。
 * 因此 0x3FFC~0x3FFF 不再特殊保留，校准区可完整使用 128 Bytes。
 */

#define FLASH_USER_PAGE_H          (0x3FU)
#define FLASH_USER_PAGE_START      (0x3F00U)

#define FLASH_USER_SECTOR_SIZE     (0x80U)
#define FLASH_CAL_SECTOR_L         (0x80U)

#define FLASH_CAL_ADDR             (0x00U)
#define FLASH_CAL_MAX_SIZE         (0x80U)

#endif