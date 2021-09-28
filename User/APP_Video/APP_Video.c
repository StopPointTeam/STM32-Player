//��Ƶ������Ӧ��

#include "sys.h"
#include "systick.h"
#include "tim.h"

#include "key.h"
#include "lcd.h"
#include "led.h"

#include "GameEngine.h"
#include "SD.h"

#include "APP_Video.h"

/**************************************** ˽�ж��� ****************************************/

typedef struct
{
    double fps;
    uint32_t frame_count;
} VideoInfo; //��Ƶ��Ϣ����

/*****************************************************************************************/

/**************************************** ȫ�ֱ��� ****************************************/

FIL video_file;          //��Ƶ�ļ�
char video_filename[32]; //��Ƶ�ļ���
char video_filepath[32]; //��Ƶ�ļ�·��

VideoInfo video_info;

volatile uint32_t current_frame = 0;
volatile uint8_t is_to_play = 0;
volatile uint8_t is_disp_progress = 0;

/*****************************************************************************************/

/**************************************** ˽�к��� ****************************************/

void APP_Video_Play(void);
uint8_t APP_Video_Menu(void);
void APP_Video_Msg(uint8_t *head, uint8_t *content);

/*****************************************************************************************/

/**
  * @brief  ������Ƶ������
  */
void APP_Video_Launcher(void)
{
    //GUI ѡȡ�ļ�
    uint8_t ret;
    if ((ret = SD_SelectFile(video_filename, "32v")) != SD_OK)
    {
        if (ret == SD_ERROR)
            APP_Video_Msg("����", "ѡȡ�ļ�����\n\n�밴���������");

        return;
    }

    SD_GetPath(video_filepath, video_filename);

    //���ļ�
    uint8_t res;
    if (res = f_open(&video_file, video_filepath, FA_OPEN_EXISTING | FA_READ) != FR_OK)
    {
        char temp[10];
        sprintf(temp, "%d", res);
        APP_Video_Msg("����", "���ļ�����\n\n�밴���������");
        APP_Video_Msg("����", temp);

        return;
    }

    //��ȡ��Ƶ��Ϣ
    uint32_t br;
    if (f_read(&video_file, &video_info, sizeof(video_info), &br) != FR_OK)
    {
        APP_Video_Msg("����", "��ȡ��Ƶ��Ϣ����\n\n�밴���������");
        return;
    }

    if (video_info.fps <= 0.0 || video_info.fps > 45.0)
    {
        APP_Video_Msg("����", "��֧�ֵ���Ƶ��ʽ��\n\n�밴���������");
        return;
    }

    //���ö�ʱ��
    TIM_Set(TIM2, video_info.fps, 0, 0);

    f_lseek(&video_file, 153600);
    APP_Video_Play();
}

/**
  * @brief  ������Ƶ
  * @retval �ɹ����� 0��ʧ�ܷ��� 1 
  */
void APP_Video_Play(void)
{
    uint32_t br;

    while (1)
    {
        if (f_read(&video_file, GE_Draw_VRam, sizeof(GE_Draw_VRam), &br) != FR_OK)
        {
            APP_Video_Msg("����", "��Ƶ���ų���\n\n�밴���������");
            return;
        }

        current_frame++;

        if (is_disp_progress != 0)
        {
            GE_Draw_Fill(8, 228, 304, 8, WHITE);
            GE_Draw_Rectangle(10, 230, 300, 4, GREEN);
            GE_Draw_Rectangle(11, 231, (float)current_frame / (float)video_info.frame_count * 298.0, 2, BLUE);

            is_disp_progress++; //����� 0 ʱ��������ʧ
        }

        switch (KEY_GetKey())
        {
        case JOY_R_UP: //������
        {
            is_disp_progress = !is_disp_progress;
        }
        break;

        case JOY_L_UP: //�˵�
        {
            //�رն�ʱ��
            TIM_Set(TIM2, 0, 0, 0);

            if (APP_Video_Menu() == 1)
                return;

            //�򿪶�ʱ��
            TIM_Set(TIM2, video_info.fps, 0, 0);
        }
        }

        while (is_to_play != 1)
            ;

        GE_Draw_Disp();
        is_to_play = 0;

        if (br == 0)
        {
            f_lseek(&video_file, 153600);
            current_frame = 0;
        }
    }
}

/**
  * @brief  ������Ƶ
  * @retval ���ز��������� 0���������ŷ��� 1
  */
uint8_t APP_Video_Menu(void)
{
    uint8_t content[3][GE_GUI_MENUBOX_CONTENT_LEN] = {"��Ƶ��Ϣ", "�˳�������", "����"};
    while (1)
    {
        GE_Draw_Fill(50, 50, 220, 140, WHITE);
        switch (GE_GUI_MenuBox(50, 50, 220, 140, "�˵�", 3, content, NULL))
        {
        case 0: //����
        {
            KEY_ClearKey();
            return 0;
        }
        break;

        case 1: //��Ƶ��Ϣ
        {
            char str_info[50];
            sprintf(str_info, "��Ƶ֡��: %.2lf\n��Ƶʱ��: %.2lfs", video_info.fps, (double)video_info.frame_count / video_info.fps);
            GE_Draw_Fill(50, 50, 220, 140, WHITE);
            GE_GUI_MsgBox(50, 50, 220, 140, "��Ƶ��Ϣ", str_info, NULL);

            KEY_WaitKey(JOY_L);
        }
        break;

        case 2: //�˳�������
        {
            f_close(&video_file);
            return 1;
        }
        break;

        case 3: //����
        {
            return 0;
        }
        }
    }
}

/**
  * @brief  ��Ϣ����������º��˳�
  * @param  head: ����
  * @param  content: ����
  */
void APP_Video_Msg(uint8_t *head, uint8_t *content)
{
    GE_Draw_Fill(60, 75, 200, 90, WHITE);
    GE_GUI_MsgBox(60, 75, 200, 90, head, content, NULL);
    KEY_WaitKey(JOY_L);
}

/**
  * @brief  TIM2 �жϴ���
  */
void TIM2_IRQHandler(void)
{
    if ((TIM2->SR & TIM_FLAG_UPDATE) != RESET)
    {
        //������±�־
        TIM2->SR = ~TIM_FLAG_UPDATE;

        is_to_play = 1;
    }
}
