//���� FIFO ��

#include "adc.h"

#include "key.h"

#define DIGITAL_KEY_NUM 3 //�����źŰ�������
#define ADC_KEY_NUM 4     //ģ���źŰ�������
#define KEY_COUNT (DIGITAL_KEY_NUM + ADC_KEY_NUM)

//ʹ�� GPIO ʱ��
#define ALL_KEY_GPIO_CLK_ENABLE()     \
    {                                 \
        __HAL_RCC_GPIOE_CLK_ENABLE(); \
        __HAL_RCC_GPIOC_CLK_ENABLE(); \
        __HAL_RCC_GPIOD_CLK_ENABLE(); \
    };

typedef struct
{
    GPIO_TypeDef *gpio;
    uint16_t pin;
} X_GPIO_T;

static const X_GPIO_T s_gpio_list[DIGITAL_KEY_NUM] = {
    {GPIOE, GPIO_PIN_3}, //KEY1
    {GPIOC, GPIO_PIN_5}, //KEY2
    {GPIOD, GPIO_PIN_10} //JOY_OK
};

static volatile KEY_T s_tBtn[KEY_COUNT] = {0};
static volatile KEY_FIFO_T s_tKey; //���� FIFO ����
static volatile is_key_clear = 1;

static void InitKeyVar(void);
static void InitKeyDigital(void);
static void DetectKey(uint8_t i);

static uint8_t KeyPinActive(uint8_t _id)
{
    if (_id < DIGITAL_KEY_NUM)
    {
        uint8_t level;

        if ((s_gpio_list[_id].gpio->IDR & s_gpio_list[_id].pin) == 0)
            return 1;
        else
            return 0;
    }
    else if (_id < DIGITAL_KEY_NUM + ADC_KEY_NUM)
    {
        switch (_id)
        {
        case JOY_U:
        {
            if (ADC_Get_PC0() < 0.01)
                return 1;
            else
                return 0;
        }
        break;

        case JOY_D:
        {
            if (ADC_Get_PC0() > 3.20)
                return 1;
            else
                return 0;
        }
        break;

        case JOY_L:
        {
            if (ADC_Get_PC1() < 0.01)
                return 1;
            else
                return 0;
        }
        break;

        case JOY_R:
        {
            if (ADC_Get_PC1() > 3.20)
                return 1;
            else
                return 0;
        }
        }
    }
}

static uint8_t IsKeyDownFunc(uint8_t _id)
{
    uint8_t count = 0;
    uint8_t save = UINT8_MAX;

    //�ж��м���������
    for (uint8_t i = 0; i < KEY_COUNT; i++)
    {
        if (KeyPinActive(i))
        {
            count++;
            save = i;
        }
    }

    if (count == 1 && save == _id)
        return 1; //ֻ�е���������ʱ����Ч

    return 0;
}

/**
  * @brief  ��ʼ������
  */
void KEY_Init(void)
{
    InitKeyVar();     //��ʼ����������
    InitKeyDigital(); //��ʼ�����ְ���
}

static void InitKeyDigital(void)
{
    GPIO_InitTypeDef gpio_init;

    ALL_KEY_GPIO_CLK_ENABLE();

    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    for (uint8_t i = 0; i < DIGITAL_KEY_NUM; i++)
    {
        gpio_init.Pin = s_gpio_list[i].pin;
        HAL_GPIO_Init(s_gpio_list[i].gpio, &gpio_init);
    }
}

static void InitKeyVar(void)
{
    s_tKey.Read = 0;
    s_tKey.Write = 0;

    //ȱʡֵ
    for (uint8_t i = 0; i < KEY_COUNT; i++)
    {
        s_tBtn[i].LongTime = KEY_LONG_TIME;    //������Ϊ 0 ��ʾ����ⳤ�����¼�
        s_tBtn[i].Count = KEY_FILTER_TIME / 2; //����������Ϊ�˲�ʱ���һ��
        s_tBtn[i].State = 0;                   //����ȱʡ״̬��0 Ϊδ����
        s_tBtn[i].RepeatSpeed = 0;             //�����������ٶȣ�0 ��ʾ��֧������
        s_tBtn[i].RepeatCount = 0;             //����������
    }

    //ҡ���������ң�֧�ֳ��� 1 ����Զ�����
    KEY_SetKeyParam(JOY_U, 50, 6);
    KEY_SetKeyParam(JOY_D, 50, 6);
    KEY_SetKeyParam(JOY_L, 50, 6);
    KEY_SetKeyParam(JOY_R, 50, 6);
}

/**
  * @brief  ����ֵѹ�밴�� FIFO��������ģ��һ�������¼�
  * @param  _KeyCode: ��ֵ
  */
void KEY_PutKey(uint8_t _KeyCode)
{
    s_tKey.Buf[s_tKey.Write] = _KeyCode;

    if (++s_tKey.Write >= KEY_FIFO_SIZE)
    {
        s_tKey.Write = 0;
    }
}

/**
  * @brief  �Ӱ��� FIFO ��ȡһ����ֵ
  * @retval ��ֵ
  */
uint8_t KEY_GetKey(void)
{
    if (s_tKey.Read == s_tKey.Write)
    {
        return KEY_NONE;
    }
    else
    {
        uint8_t ret = s_tKey.Buf[s_tKey.Read];

        if (++s_tKey.Read >= KEY_FIFO_SIZE)
            s_tKey.Read = 0;

        if (
            ret == KEY1_UP ||
            ret == KEY2_UP ||
            ret == JOY_OK_UP ||
            ret == JOY_U_UP ||
            ret == JOY_D_UP ||
            ret == JOY_L_UP ||
            ret == JOY_R_UP)
        {
            if (is_key_clear)
            {
                is_key_clear = 0;
                return KEY_NONE;
            }
        }

        is_key_clear = 0;
        return ret;
    }
}

/**
  * @brief  �Ӱ��� FIFO ��ȡһ����ֵ����ֵʱ����
  * @retval ��ֵ
  */
uint8_t KEY_GetKeyWait(void)
{
    uint8_t key = KEY_NONE;

    while (key == KEY_NONE)
        key = KEY_GetKey();

    return key;
}

/**
  * @brief  ��ȡĳ��״̬
  * @param  _ucKeyID: ���� ID
  * @retval 1 ��ʾ���£�0 ��ʾδ����
  */
uint8_t KEY_GetKeyState(KEY_ID_E _ucKeyID)
{
    return s_tBtn[_ucKeyID].State;
}

/**
  * @brief  �ȴ��ض�������
  * @param  _ucKeyID: ���� ID
  */
void KEY_WaitKey(KEY_ID_E _ucKeyID)
{
    while (KEY_GetKeyState(_ucKeyID) != 1)
        ;
}

/**
  * @brief  ���ð�������
  * @param  _ucKeyID: ���� ID
  * @param  _LongTime: �����¼�ʱ��
  * @param  _RepeatSpeed: �����ٶ�
  */
void KEY_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t _RepeatSpeed)
{
    s_tBtn[_ucKeyID].LongTime = _LongTime;       //����ʱ�� 0 ��ʾ����ⳤ�����¼�
    s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed; //�����������ٶȣ�0 ��ʾ��֧������
    s_tBtn[_ucKeyID].RepeatCount = 0;
}

/**
  * @brief  ��հ��� FIFO
  */
void KEY_ClearKey(void)
{
    s_tKey.Read = s_tKey.Write;
    is_key_clear = 1;
}

static void DetectKey(uint8_t i)
{
    KEY_T *pBtn;

    pBtn = &s_tBtn[i];
    if (IsKeyDownFunc(i))
    {
        if (pBtn->Count < KEY_FILTER_TIME)
        {
            pBtn->Count = KEY_FILTER_TIME;
        }
        else if (pBtn->Count < 2 * KEY_FILTER_TIME)
        {
            pBtn->Count++;
        }
        else
        {
            if (pBtn->State == 0)
            {
                pBtn->State = 1;

                KEY_PutKey((uint8_t)(3 * i + 1));
            }

            if (pBtn->LongTime > 0)
            {
                if (pBtn->LongCount < pBtn->LongTime)
                {
                    if (++pBtn->LongCount == pBtn->LongTime)
                    {
                        KEY_PutKey((uint8_t)(3 * i + 3));
                    }
                }
                else
                {
                    if (pBtn->RepeatSpeed > 0)
                    {
                        if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
                        {
                            pBtn->RepeatCount = 0;
                            KEY_PutKey((uint8_t)(3 * i + 1));
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (pBtn->Count > KEY_FILTER_TIME)
        {
            pBtn->Count = KEY_FILTER_TIME;
        }
        else if (pBtn->Count != 0)
        {
            pBtn->Count--;
        }
        else
        {
            if (pBtn->State == 1)
            {
                pBtn->State = 0;

                KEY_PutKey((uint8_t)(3 * i + 2));
            }
        }

        pBtn->LongCount = 0;
        pBtn->RepeatCount = 0;
    }
}

/**
  * @brief  ɨ�����а�������������Ӧ�� SysTick �ж������Եĵ��ã�10ms һ��
  */
void KEY_Scan10ms(void)
{
    for (uint8_t i = 0; i < KEY_COUNT; i++)
        DetectKey(i);
}
