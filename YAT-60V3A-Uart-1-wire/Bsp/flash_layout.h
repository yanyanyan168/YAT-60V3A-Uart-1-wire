#ifndef __FLASH_LAYOUT_H__
#define __FLASH_LAYOUT_H__

/*
 * TX8C126x Flash 布局约定（EEPROM + Rolling Code 模式）：
 *
 * 1. Keil IROM1 Size 按 FAQ 配置为 0x3EF4。
 * 2. 本工程当前只使用校准数据区，不使用“后续参数/备份区”。
 * 3. 校准数据固定放在 0x3F00~0x3F7F，共 128 Bytes。
 * 4. 0x3F80~0x3FFB 当前未使用，程序不主动擦写，避免误伤保留区。
 * 5. 0x3FFC~0x3FFF 为 Code CRC 区，禁止业务代码写入。
 */

#define FLASH_CODE_SIZE_EEPROM_RC       (0x3EF4U)

#define FLASH_EEPROM_PAGE_H             (0x3FU)
#define FLASH_EEPROM_PAGE_START         (0x3F00U)
#define FLASH_EEPROM_SECTOR_SIZE        (0x80U)

#define FLASH_CAL_ADDR                  (0x00U)    /* 实际地址：0x3F00 + 0x00 */
#define FLASH_CAL_MAX_SIZE              (0x80U)    /* 0x3F00~0x3F7F，128 Bytes */

#endif
