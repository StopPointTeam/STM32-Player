//SysTick �������ʱ����

#include "key.h"
#include "led.h"
#include "Clock.h"

#include "systick.h"

static volatile uint32_t s_uiDelayCount = 0;
static volatile uint8_t s_ucTimeOutFlag = 0;
static SOFT_TMR s_tTmr[TMR_COUNT] = {0};          //�����ʱ���ṹ�����
volatile uint32_t g_iRunTime = 0;                 //ȫ������ʱ�䣬��λ ms
static volatile uint8_t g_ucEnableSystickISR = 0; //�ȴ�������ʼ��

void SysTick_ISR(void);
static void SysTick_SoftTimerDec(SOFT_TMR *_tmr);

/**
  * @brief  ���� SysTick �жϣ�����ʼ�������ʱ��
  */
void SysTick_Init(void)
{
    //�������е������ʱ��
    for (uint8_t i = 0; i < TMR_COUNT; i++)
    {
        s_tTmr[i].Count = 0;
        s_tTmr[i].PreLoad = 0;
        s_tTmr[i].Flag = 0;
        s_tTmr[i].Mode = TMR_ONCE_MODE; //ȱʡ��һ���Թ���ģʽ
    }

    //���� SysTick �ж�����Ϊ 1ms�������� SysTick �ж�
    SysTick_Config(SystemCoreClock / 1000);

    g_ucEnableSystickISR = 1; //ִ��systick�ж�
}

void SysTick_ISR(void)
{
    static uint8_t s_count = 0;
    static uint16_t l_count = 0;

    //���� Delay_ms
    if (s_uiDelayCount > 0)
        if (--s_uiDelayCount == 0)
            s_ucTimeOutFlag = 1;

    //�����ʱ���ļ������� 1
    for (uint8_t i = 0; i < TMR_COUNT; i++)
        SysTick_SoftTimerDec(&s_tTmr[i]);

    //ȫ������ʱ���� 1
    g_iRunTime++;
    if (g_iRunTime == UINT32_MAX)
        g_iRunTime = 0;

    //ÿ�� 1ms ����һ��

    if (++s_count >= 10)
    {
        s_count = 0;

        //ÿ�� 10ms ����һ��
        KEY_Scan10ms();
    }

    if (++l_count >= 1000)
    {
        l_count = 0;

        //ÿ�� 1000ms ����һ��
        Clock_Handler();
    }
}

static void SysTick_SoftTimerDec(SOFT_TMR *_tmr)
{
    if (_tmr->Count > 0)
    {
        //�����ʱ���������� 1 �����ö�ʱ�������־
        if (--_tmr->Count == 0)
        {
            _tmr->Flag = 1;

            //������Զ�ģʽ�����Զ���װ������
            if (_tmr->Mode == TMR_AUTO_MODE)
                _tmr->Count = _tmr->PreLoad;
        }
    }
}

/**
  * @brief  ms ��ʱ
  * @param  n: ��ʱ��
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
  * @brief  us ��ʱ
  * @param  n: ��ʱ��
  */
void Delay_us(uint32_t n)
{
    uint32_t ticks;
    uint32_t told;
    uint32_t tnow;
    uint32_t tcnt = 0;
    uint32_t reload;

    reload = SysTick->LOAD;
    ticks = n * (SystemCoreClock / 1000000); //��Ҫ�Ľ�����

    tcnt = 0;
    told = SysTick->VAL; //�ս���ʱ�ļ�����ֵ

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

            //ʱ�䳬��������Ҫ�ӳٵ�ʱ�䣬���˳�
            if (tcnt >= ticks)
                break;
        }
    }
}

/**
  * @brief  ����һ����ʱ���������ö�ʱ����
  * @param  _id: ��ʱ�� ID
  * @param  _period: ��ʱ���ڣ���λ ms
  */
void SysTick_StartTimer(uint8_t _id, uint32_t _period)
{
    DISABLE_INT();

    s_tTmr[_id].Count = _period;      //ʵʱ��������ֵ
    s_tTmr[_id].PreLoad = _period;    //�������Զ���װֵ�����Զ�ģʽ������
    s_tTmr[_id].Flag = 0;             //��ʱʱ�䵽��־
    s_tTmr[_id].Mode = TMR_ONCE_MODE; //һ���Թ���ģʽ

    ENABLE_INT();
}

/**
  * @brief  ����һ���Զ���ʱ���������ö�ʱ����
  * @param  _id: ��ʱ�� ID
  * @param  _period: ��ʱ���ڣ���λ ms
  */
void SysTick_StartAutoTimer(uint8_t _id, uint32_t _period)
{
    DISABLE_INT();

    s_tTmr[_id].Count = _period;      //ʵʱ��������ֵ
    s_tTmr[_id].PreLoad = _period;    //�������Զ���װֵ�����Զ�ģʽ������
    s_tTmr[_id].Flag = 0;             //��ʱʱ�䵽��־
    s_tTmr[_id].Mode = TMR_AUTO_MODE; //�Զ�����ģʽ

    ENABLE_INT();
}

/**
  * @brief  ֹͣһ����ʱ��
  * @param  _id: ��ʱ�� ID
  */
void SysTick_StopTimer(uint8_t _id)
{
    DISABLE_INT();

    s_tTmr[_id].Count = 0;            //ʵʱ��������ֵ
    s_tTmr[_id].Flag = 0;             //��ʱʱ�䵽��־
    s_tTmr[_id].Mode = TMR_ONCE_MODE; //�Զ�����ģʽ

    ENABLE_INT();
}

/**
  * @brief  ��ⶨʱ���Ƿ�ʱ
  * @param  _id: ��ʱ�� ID
  * @param  _period: ��ʱ����
  * @retval ���� 0 ��ʾ��ʱδ����1 ��ʾ��ʱ��
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
  * @brief  ��ȡ����ʱ�䣬��λ ms������Ա�ʾ 24.85 ��
  * @retval ����ʱ��
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
  * @brief  ���㵱ǰ����ʱ��͸���ʱ��֮��Ĳ�ֵ�������˼�����ѭ��
  * @param  _LastTime: �ϸ�ʱ��
  * @retval ��ǰʱ��͹�ȥʱ��Ĳ�ֵ����λ ms
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
  * @brief  SysTick �жϷ������
  */
void SysTick_Handler(void)
{
    HAL_IncTick();

    if (g_ucEnableSystickISR == 0)
        return;

    SysTick_ISR();
}
