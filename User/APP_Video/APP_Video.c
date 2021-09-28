//视频播放器应用

#include "sys.h"
#include "systick.h"
#include "tim.h"

#include "key.h"
#include "lcd.h"
#include "led.h"

#include "GameEngine.h"
#include "SD.h"

#include "APP_Video.h"

/**************************************** 私有定义 ****************************************/

typedef struct
{
    double fps;
    uint32_t frame_count;
} VideoInfo; //视频信息定义

/*****************************************************************************************/

/**************************************** 全局变量 ****************************************/

FIL video_file;          //视频文件
char video_filename[32]; //视频文件名
char video_filepath[32]; //视频文件路径

VideoInfo video_info;

volatile uint32_t current_frame = 0;
volatile uint8_t is_to_play = 0;
volatile uint8_t is_disp_progress = 0;

/*****************************************************************************************/

/**************************************** 私有函数 ****************************************/

void APP_Video_Play(void);
uint8_t APP_Video_Menu(void);
void APP_Video_Msg(uint8_t *head, uint8_t *content);

/*****************************************************************************************/

/**
  * @brief  启动视频播放器
  */
void APP_Video_Launcher(void)
{
    //GUI 选取文件
    uint8_t ret;
    if ((ret = SD_SelectFile(video_filename, "32v")) != SD_OK)
    {
        if (ret == SD_ERROR)
            APP_Video_Msg("警告", "选取文件出错！\n\n请按任意键返回");

        return;
    }

    SD_GetPath(video_filepath, video_filename);

    //打开文件
    uint8_t res;
    if (res = f_open(&video_file, video_filepath, FA_OPEN_EXISTING | FA_READ) != FR_OK)
    {
        char temp[10];
        sprintf(temp, "%d", res);
        APP_Video_Msg("警告", "打开文件出错！\n\n请按任意键返回");
        APP_Video_Msg("警告", temp);

        return;
    }

    //读取视频信息
    uint32_t br;
    if (f_read(&video_file, &video_info, sizeof(video_info), &br) != FR_OK)
    {
        APP_Video_Msg("警告", "读取视频信息错误！\n\n请按任意键返回");
        return;
    }

    if (video_info.fps <= 0.0 || video_info.fps > 45.0)
    {
        APP_Video_Msg("警告", "不支持的视频格式！\n\n请按任意键返回");
        return;
    }

    //设置定时器
    TIM_Set(TIM2, video_info.fps, 0, 0);

    f_lseek(&video_file, 153600);
    APP_Video_Play();
}

/**
  * @brief  播放视频
  * @retval 成功返回 0，失败返回 1 
  */
void APP_Video_Play(void)
{
    uint32_t br;

    while (1)
    {
        if (f_read(&video_file, GE_Draw_VRam, sizeof(GE_Draw_VRam), &br) != FR_OK)
        {
            APP_Video_Msg("警告", "视频播放出错！\n\n请按任意键返回");
            return;
        }

        current_frame++;

        if (is_disp_progress != 0)
        {
            GE_Draw_Fill(8, 228, 304, 8, WHITE);
            GE_Draw_Rectangle(10, 230, 300, 4, GREEN);
            GE_Draw_Rectangle(11, 231, (float)current_frame / (float)video_info.frame_count * 298.0, 2, BLUE);

            is_disp_progress++; //溢出到 0 时进度条消失
        }

        switch (KEY_GetKey())
        {
        case JOY_R_UP: //进度条
        {
            is_disp_progress = !is_disp_progress;
        }
        break;

        case JOY_L_UP: //菜单
        {
            //关闭定时器
            TIM_Set(TIM2, 0, 0, 0);

            if (APP_Video_Menu() == 1)
                return;

            //打开定时器
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
  * @brief  播放视频
  * @retval 返回播放器返回 0，结束播放返回 1
  */
uint8_t APP_Video_Menu(void)
{
    uint8_t content[3][GE_GUI_MENUBOX_CONTENT_LEN] = {"视频信息", "退出播放器", "返回"};
    while (1)
    {
        GE_Draw_Fill(50, 50, 220, 140, WHITE);
        switch (GE_GUI_MenuBox(50, 50, 220, 140, "菜单", 3, content, NULL))
        {
        case 0: //返回
        {
            KEY_ClearKey();
            return 0;
        }
        break;

        case 1: //视频信息
        {
            char str_info[50];
            sprintf(str_info, "视频帧率: %.2lf\n视频时长: %.2lfs", video_info.fps, (double)video_info.frame_count / video_info.fps);
            GE_Draw_Fill(50, 50, 220, 140, WHITE);
            GE_GUI_MsgBox(50, 50, 220, 140, "视频信息", str_info, NULL);

            KEY_WaitKey(JOY_L);
        }
        break;

        case 2: //退出播放器
        {
            f_close(&video_file);
            return 1;
        }
        break;

        case 3: //返回
        {
            return 0;
        }
        }
    }
}

/**
  * @brief  消息框，任意键按下后退出
  * @param  head: 标题
  * @param  content: 内容
  */
void APP_Video_Msg(uint8_t *head, uint8_t *content)
{
    GE_Draw_Fill(60, 75, 200, 90, WHITE);
    GE_GUI_MsgBox(60, 75, 200, 90, head, content, NULL);
    KEY_WaitKey(JOY_L);
}

/**
  * @brief  TIM2 中断处理
  */
void TIM2_IRQHandler(void)
{
    if ((TIM2->SR & TIM_FLAG_UPDATE) != RESET)
    {
        //清除更新标志
        TIM2->SR = ~TIM_FLAG_UPDATE;

        is_to_play = 1;
    }
}
