//系统设置应用

#include "string.h"

#include "sys.h"
#include "systick.h"

#include "key.h"
#include "lcd.h"
#include "hc25.h"

#include "GameEngine.h"
#include "WLAN.h"

#include "APP_Setting.h"

/**************************************** 私有定义 ****************************************/

/*****************************************************************************************/

/**************************************** 全局变量 ****************************************/

/*****************************************************************************************/

/**************************************** 私有函数 ****************************************/

void APP_Setting_Msg(uint8_t *head, uint8_t *content);

/*****************************************************************************************/

/**
  * @brief  启动设置
  */
void APP_Setting_Launcher(void)
{
    uint8_t content[4][GE_GUI_MENUBOX_CONTENT_LEN] = {"网络设置", "夜间模式", "关于", "退出设置"};

    while (1)
    {
    label_menu_1:
        GE_Draw_ClrAll(WHITE);
        switch (GE_GUI_MenuBox(5, 5, 310, 230, "系统设置", 4, content, NULL))
        {
        case 0: //退出设置
        {
            return;
        }
        break;

        case 1: //网络设置
        {
            uint8_t content[1][GE_GUI_MENUBOX_CONTENT_LEN] = {"网络信息"};

            while (1)
            {
                GE_Draw_Fill(5, 5, 310, 230, WHITE);
                switch (GE_GUI_MenuBox(5, 5, 310, 230, "网络设置", 1, content, NULL))
                {
                case 0:
                {
                    goto label_menu_1;
                }
                break;

                case 1:
                {
                    GE_Draw_Fill(5, 5, 310, 230, WHITE);

                    uint8_t temp_str[80];

                    if (WLAN_CheckNet())
                    {
                        uint8_t temp_ipaddr_str[60];

                        Delay_ms(2000);
                        if (WLAN_GetIPAddr(temp_ipaddr_str))
                        {
                            char *ret = strrchr(temp_ipaddr_str, '&');
                            *ret = '\n';
                        }
                        else
                        {
                            strcpy(temp_ipaddr_str, "查询失败");
                        }

                        sprintf(temp_str, "网络已连接\n\nIP地址及归属地：\n%s", temp_ipaddr_str);
                    }
                    else
                    {
                        strcpy(temp_str, "网络未连接");
                    }

                    GE_GUI_MsgBox(5, 5, 310, 230, "网络设置", temp_str, NULL);

                    KEY_WaitKey(JOY_L);
                }
                }
            }
        }
        break;

        case 2: //夜间模式
        {
            uint8_t content[2][GE_GUI_MENUBOX_CONTENT_LEN] = {"开启", "关闭"};

            GE_Draw_Fill(5, 5, 310, 230, WHITE);
            switch (GE_GUI_MenuBox(5, 5, 310, 230, "夜间模式", 2, content, NULL))
            {
            case 1:
            {
                LCD_SendCmd(LCD_CMD_DINVON);
            }
            break;

            case 2:
            {
                LCD_SendCmd(LCD_CMD_DINVOFF);
            }
            }
        }
        break;

        case 3: //关于
        {
            GE_Draw_Fill(5, 5, 310, 230, WHITE);
            GE_GUI_MsgBox(5, 5, 310, 230, "关于", "STM32Player v0.2\nPowered By StopPointTeam.\nAll Rights Reversed.\n\n版权所有，盗版必究", NULL);

            KEY_WaitKey(JOY_L);
        }
        break;

        case 4: //退出设置
        {
            return;
        }
        }
    }
}

/**
  * @brief  消息框，任意键按下后退出
  * @param  head: 标题
  * @param  content: 内容
  */
void APP_Setting_Msg(uint8_t *head, uint8_t *content)
{
    GE_Draw_Fill(60, 75, 200, 90, WHITE);
    GE_GUI_MsgBox(60, 75, 200, 90, head, content, NULL);
    KEY_WaitKey(JOY_L);
}
