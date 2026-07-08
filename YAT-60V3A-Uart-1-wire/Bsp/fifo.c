/**
  ******************************************************************************
  * @file    fifo.c
  * @brief   DEBUG 和 COM 接收环形队列。
  *
  * 说明：
  * 1. 本文件实现两个独立的 FIFO 环形缓冲区：
  *    - DEBUG FIFO：用于 DEBUG 串口接收缓存
  *    - COM FIFO  ：用于 COM 串口接收缓存
  *
  * 2. FIFO 满时的处理策略：
  *    - 不拒收新数据
  *    - 丢弃最旧的 1 字节数据
  *    - 写入最新的 1 字节数据
  *    - overflow 标志置 1
  *
  * 3. 注意：
  *    - pop 函数内部没有判断 FIFO 是否为空
  *    - 调用 pop 前，外部必须先确认 fifo.cnt > 0
  ******************************************************************************
  */

#include "fifo.h"

/*
 * DEBUG 串口接收数据缓存区。
 *
 * xdata：
 * C51 存储类型，表示该数组放在外部 RAM 区。
 * FIFO 数据区通常比较大，放 xdata 可以节省内部 RAM。
 */
static u8 xdata s_ch_buf[DEBUG_FIFO_SIZE];

/*
 * COM 串口接收数据缓存区。
 */
static u8 xdata s_com_buf[COM_FIFO_SIZE];

/*
 * DEBUG FIFO 控制结构体。
 *
 * idata：
 * C51 存储类型，表示该变量放在内部 RAM 区。
 * FIFO 控制变量访问频繁，放 idata 访问速度较快。
 */
fifo_t idata s_ch_fifo;

/*
 * COM FIFO 控制结构体。
 */
fifo_t idata s_com_fifo;


/**
  * @brief  复位 FIFO 控制结构体。
  * @param  fifo: FIFO 控制结构体指针
  * @retval 无
  *
  * 说明：
  * 只复位读写指针、数据计数和溢出标志。
  * 不清空实际数据缓存区，因为 cnt 清零后旧数据已经无效。
  */
static void fifo_ctrl_reset(fifo_t *fifo)
{
    /*
     * rd：读指针，表示下一次读取的位置。
     */
    fifo->rd = 0U;

    /*
     * wr：写指针，表示下一次写入的位置。
     */
    fifo->wr = 0U;

    /*
     * cnt：当前 FIFO 中有效数据字节数。
     */
    fifo->cnt = 0U;
}


/**
  * @brief  初始化 FIFO 模块。
  * @param  无
  * @retval 无
  *
  * 说明：
  * 系统初始化时调用一次。
  * 同时初始化 DEBUG FIFO 和 COM FIFO。
  */
void fifo_module_init(void)
{
    fifo_ctrl_reset(&s_ch_fifo);
    fifo_ctrl_reset(&s_com_fifo);
}


/**
  * @brief  清空 DEBUG FIFO。
  * @param  无
  * @retval 无
  *
  * 说明：
  * 清空后，FIFO 中原有未读数据全部作废。
  */
void ch_fifo_clear(void)
{
    fifo_ctrl_reset(&s_ch_fifo);
}


/**
  * @brief  向 DEBUG FIFO 写入 1 字节数据。
  * @param  dat: 需要写入的数据
  * @retval 无
  *
  * 说明：
  * 1. 如果 FIFO 未满，直接写入。
  * 2. 如果 FIFO 已满：
  *    - overflow 置 1
  *    - rd 前移一格，丢弃最旧的 1 字节数据
  *    - cnt 减 1，为新数据腾出空间
  *    - 再写入新数据
  */
void ch_fifo_push(u8 dat)
{
    /*
     * 判断 FIFO 是否已满。
     *
     * cnt >= DEBUG_FIFO_SIZE 表示当前缓存已经没有空位。
     */
    if(s_ch_fifo.cnt >= DEBUG_FIFO_SIZE)
    {
        /*
         * 读指针前移，相当于丢弃最旧的一个字节。
         */
        s_ch_fifo.rd++;

        /*
         * 环形缓冲区到尾部后，回到 0。
         */
        if(s_ch_fifo.rd >= DEBUG_FIFO_SIZE)
        {
            s_ch_fifo.rd = 0U;
        }

        /*
         * 已经丢弃了一个旧数据，所以有效数据数量减 1。
         * 这样后面可以正常写入新数据。
         */
        s_ch_fifo.cnt--;
    }

    /*
     * 将新数据写入当前写指针位置。
     */
    s_ch_buf[s_ch_fifo.wr] = dat;

    /*
     * 写指针后移。
     */
    s_ch_fifo.wr++;

    /*
     * 写指针到达尾部后，回到 0，实现环形队列。
     */
    if(s_ch_fifo.wr >= DEBUG_FIFO_SIZE)
    {
        s_ch_fifo.wr = 0U;
    }

    /*
     * FIFO 中有效数据数量加 1。
     */
    s_ch_fifo.cnt++;
}


/**
  * @brief  从 DEBUG FIFO 读取 1 字节数据。
  * @param  无
  * @retval 读取到的数据
  *
  * 注意：
  * 本函数不判断 FIFO 是否为空。
  * 调用本函数前，外部必须确认 s_ch_fifo.cnt > 0。
  */
u8 ch_fifo_pop(void)
{
    u8 dat;

    /*
     * 从当前读指针位置取出数据。
     */
    dat = s_ch_buf[s_ch_fifo.rd];
    
    /*
     * 读指针后移。
     */
    s_ch_fifo.rd++;

    /*
     * 读指针到达尾部后，回到 0。
     */
    if(s_ch_fifo.rd >= DEBUG_FIFO_SIZE)
    {
        s_ch_fifo.rd = 0U;
    }

    /*
     * 已读取 1 字节，有效数据数量减 1。
     */
    s_ch_fifo.cnt--;
    
    return dat;
}



/**
  * @brief  清空 COM FIFO。
  * @param  无
  * @retval 无
  *
  * 说明：
  * 清空后，FIFO 中原有未读数据全部作废。
  */
void com_fifo_clear(void)
{
    fifo_ctrl_reset(&s_com_fifo);
}


/**
  * @brief  向 COM FIFO 写入 1 字节数据。
  * @param  dat: 需要写入的数据
  * @retval 无
  *
  * 说明：
  * 1. 如果 FIFO 未满，直接写入。
  * 2. 如果 FIFO 已满：
  *    - overflow 置 1
  *    - rd 前移一格，丢弃最旧的 1 字节数据
  *    - cnt 减 1，为新数据腾出空间
  *    - 再写入新数据
  */
void com_fifo_push(u8 dat)
{
    /*
     * 判断 COM FIFO 是否已满。
     */
    if(s_com_fifo.cnt >= COM_FIFO_SIZE)
    {
        /*
         * 丢弃最旧的数据：
         * 读指针前移一格。
         */
        s_com_fifo.rd++;

        /*
         * 环形回绕。
         */
        if(s_com_fifo.rd >= COM_FIFO_SIZE)
        {
            s_com_fifo.rd = 0U;
        }

        /*
         * 丢弃一个旧数据后，有效数据数量减 1。
         */
        s_com_fifo.cnt--;
    }

    /*
     * 将新数据写入当前写指针位置。
     */
    s_com_buf[s_com_fifo.wr] = dat;

    /*
     * 写指针后移。
     */
    s_com_fifo.wr++;

    /*
     * 写指针环形回绕。
     */
    if(s_com_fifo.wr >= COM_FIFO_SIZE)
    {
        s_com_fifo.wr = 0U;
    }

    /*
     * 有效数据数量加 1。
     */
    s_com_fifo.cnt++;
}


/**
  * @brief  从 COM FIFO 读取 1 字节数据。
  * @param  无
  * @retval 读取到的数据
  *
  * 注意：
  * 本函数不判断 FIFO 是否为空。
  * 调用本函数前，外部必须确认 s_com_fifo.cnt > 0。
  */
u8 com_fifo_pop(void)
{
    u8 dat;
    
    /*
     * 从当前读指针位置取出数据。
     */
    dat = s_com_buf[s_com_fifo.rd];

    /*
     * 读指针后移。
     */
    s_com_fifo.rd++;

    /*
     * 读指针环形回绕。
     */
    if(s_com_fifo.rd >= COM_FIFO_SIZE)
    {
        s_com_fifo.rd = 0U;
    }

    /*
     * 有效数据数量减 1。
     */
    s_com_fifo.cnt--;
    
    return dat;
}

