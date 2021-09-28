//SysTick 及软件定时器库

#include "key.h"
#include "led.h"
#include "Clock.h"

#include "systick.h"

static volatile uint32_t s_uiDelayCount = 0;
static volatile uint8_t s_ucTimeOutFlag = 0;
static SOFT_TMR s_tTmr[TMR_COUNT] = {0};          //软件定时器结构体变量
volatile uint32_t g_iRunTime = 0;                 //全局运行时间，单位 ms
static volatile uint8_t g_ucEnableSystickISR = 0; //等待变量初始化

void SysTick_ISR(void);
static void SysTick_SoftTimerDec(SOFT_TMR *_tmr);

/**
  * @brief  配置 SysTick 中断，并初始化软件定时器
  */
void SysTick_Init(void)
{
    //清零所有的软件定时器
    for (uint8_t i = 0; i < TMR_COUNT; i++)
    {
        s_tTmr[i].Count = 0;
        s_tTmr[i].PreLoad = 0;
        s_tTmr[i].Flag = 0;
        s_tTmr[i].Mode = TMR_ONCE_MODE; //缺省是一次性工作模式
    }

    //配置 SysTick 中断周期为 1ms，并启动 SysTick 中断
    SysTick_Config(SystemCoreClock / 1000);

    g_ucEnableSystickISR = 1; //执行systick中断
}

void SysTick_ISR(void)
{
    static uint8_t s_count = 0;
    static uint16_t l_count = 0;

    //用于 Delay_ms
    if (s_uiDelayCount > 0)
        if (--s_uiDelayCount == 0)
            s_ucTimeOutFlag = 1;

    //软件定时器的计数器减 1
    for (uint8_t i = 0; i < TMR_COUNT; i++)
        SysTick_SoftTimerDec(&s_tTmr[i]);

    //全局运行时间增 1
    g_iRunTime++;
    if (g_iRunTime == UINT32_MAX)
        g_iRunTime = 0;

    //每隔 1ms 调用一次

    if (++s_count >= 10)
    {
        s_count = 0;

        //每隔 10ms 调用一次
        KEY_Scan10ms();
    }

    if (++l_count >= 1000)
    {
        l_count = 0;

        //每隔 1000ms 调用一次
        Clock_Handler();
    }
}

static void SysTick_SoftTimerDec(SOFT_TMR *_tmr)
{
    if (_tmr->Count > 0)
    {
        //如果定时器变量减到 1 则设置定时器到达标志
        if (--_tmr->Count == 0)
        {
            _tmr->Flag = 1;

            //如果是自动模式，则自动重装计数器
            if (_tmr->Mode == TMR_AUTO_MODE)
                _tmr->Count = _tmr->PreLoad;
        }
    }
}

/**
  * @brief  ms 延时
  * @param  n: 延时数
  */
void Delay_ms(uint32_t n)
{
    if (n == 0)
        return;
    else if (n == 1)
        n = 2;

    DISABLE_INT();

    s_uiDelayCount = n;
    s_ucTimeOutFlag = 0;

    ENABLE_INT();

    while (1)
    {
        if (s_ucTimeOutFlag == 1)
            break;
    }
}

/**
  * @brief  us 延时
  * @param  n: 延时数
  */
void Delay_us(uint32_t n)
{
    uint32_t ticks;
    uint32_t told;
    uint32_t tnow;
    uint32_t tcnt = 0;
    uint32_t reload;

    reload = SysTick->LOAD;
    ticks = n * (SystemCoreClock / 1000000); //需要的节拍数

    tcnt = 0;
    told = SysTick->VAL; //刚进入时的计数器值

    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += reload - tnow + told;
            told = tnow;

            //时间超过、等于要延迟的时间，则退出
            if (tcnt >= ticks)
                break;
        }
    }
}

/**
  * @brief  启动一个定时器，并设置定时周期
  * @param  _id: 定时器 ID
  * @param  _period: 定时周期，单位 ms
  */
void SysTick_StartTimer(uint8_t _id, uint32_t _period)
{
    DISABLE_INT();

    s_tTmr[_id].Count = _period;      //实时计数器初值
    s_tTmr[_id].PreLoad = _period;    //计数器自动重装值，仅自动模式起作用
    s_tTmr[_id].Flag = 0;             //定时时间到标志
    s_tTmr[_id].Mode = TMR_ONCE_MODE; //一次性工作模式

    ENABLE_INT();
}

/**
  * @brief  启动一个自动定时器，并设置定时周期
  * @param  _id: 定时器 ID
  * @param  _period: 定时周期，单位 ms
  */
void SysTick_StartAutoTimer(uint8_t _id, uint32_t _period)
{
    DISABLE_INT();

    s_tTmr[_id].Count = _period;      //实时计数器初值
    s_tTmr[_id].PreLoad = _period;    //计数器自动重装值，仅自动模式起作用
    s_tTmr[_id].Flag = 0;             //定时时间到标志
    s_tTmr[_id].Mode = TMR_AUTO_MODE; //自动工作模式

    ENABLE_INT();
}

/**
  * @brief  停止一个定时器
  * @param  _id: 定时器 ID
  */
void SysTick_StopTimer(uint8_t _id)
{
    DISABLE_INT();

    s_tTmr[_id].Count = 0;            //实时计数器初值
    s_tTmr[_id].Flag = 0;             //定时时间到标志
    s_tTmr[_id].Mode = TMR_ONCE_MODE; //自动工作模式

    ENABLE_INT();
}

/**
  * @brief  检测定时器是否超时
  * @param  _id: 定时器 ID
  * @param  _period: 定时周期
  * @retval 返回 0 表示定时未到，1 表示定时到
  */
uint8_t SysTick_CheckTimer(uint8_t _id)
{
    if (_id >= TMR_COUNT)
        return 0;

    if (s_tTmr[_id].Flag == 1)
    {
        s_tTmr[_id].Flag = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
  * @brief  获取运行时间，单位 ms。最长可以表示 24.85 天
  * @retval 运行时间
  */
int32_t SysTick_GetRunTime(void)
{
    int32_t runtime;

    DISABLE_INT();

    runtime = g_iRunTime;

    ENABLE_INT();

    return runtime;
}

/**
  * @brief  计算当前运行时间和给定时刻之间的差值。处理了计数器循环
  * @param  _LastTime: 上个时刻
  * @retval 当前时间和过去时间的差值，单位 ms
  */
int32_t SysTick_CheckRunTime(int32_t _LastTime)
{
    int32_t now_time;
    int32_t time_diff;

    DISABLE_INT();

    now_time = g_iRunTime;

    ENABLE_INT();

    if (now_time >= _LastTime)
        time_diff = now_time - _LastTime;
    else
        time_diff = INT32_MAX - _LastTime + now_time;

    return time_diff;
}

/**
  * @brief  SysTick 中断服务程序
  */
void SysTick_Handler(void)
{
    HAL_IncTick();

    if (g_ucEnableSystickISR == 0)
        return;

    SysTick_ISR();
}
