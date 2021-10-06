//ϵͳ����Ӧ��

#include "string.h"

#include "sys.h"
#include "systick.h"

#include "key.h"
#include "lcd.h"
#include "hc25.h"

#include "GameEngine.h"
#include "WLAN.h"

#include "APP_Setting.h"

/**************************************** ˽�ж��� ****************************************/

/*****************************************************************************************/

/**************************************** ȫ�ֱ��� ****************************************/

/*****************************************************************************************/

/**************************************** ˽�к��� ****************************************/

void APP_Setting_Msg(uint8_t *head, uint8_t *content);

/*****************************************************************************************/

/**
  * @brief  ��������
  */
void APP_Setting_Launcher(void)
{
    uint8_t content[4][GE_GUI_MENUBOX_CONTENT_LEN] = {"��������", "ҹ��ģʽ", "����", "�˳�����"};

    while (1)
    {
    label_menu_1:
        GE_Draw_ClrAll(WHITE);
        switch (GE_GUI_MenuBox(5, 5, 310, 230, "ϵͳ����", 4, content, NULL))
        {
        case 0: //�˳�����
        {
            return;
        }
        break;

        case 1: //��������
        {
            uint8_t content[1][GE_GUI_MENUBOX_CONTENT_LEN] = {"������Ϣ"};

            while (1)
            {
                GE_Draw_Fill(5, 5, 310, 230, WHITE);
                switch (GE_GUI_MenuBox(5, 5, 310, 230, "��������", 1, content, NULL))
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
                            strcpy(temp_ipaddr_str, "��ѯʧ��");
                        }

                        sprintf(temp_str, "����������\n\nIP��ַ�������أ�\n%s", temp_ipaddr_str);
                    }
                    else
                    {
                        strcpy(temp_str, "����δ����");
                    }

                    GE_GUI_MsgBox(5, 5, 310, 230, "��������", temp_str, NULL);

                    KEY_WaitKey(JOY_L);
                }
                }
            }
        }
        break;

        case 2: //ҹ��ģʽ
        {
            uint8_t content[2][GE_GUI_MENUBOX_CONTENT_LEN] = {"����", "�ر�"};

            GE_Draw_Fill(5, 5, 310, 230, WHITE);
            switch (GE_GUI_MenuBox(5, 5, 310, 230, "ҹ��ģʽ", 2, content, NULL))
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

        case 3: //����
        {
            GE_Draw_Fill(5, 5, 310, 230, WHITE);
            GE_GUI_MsgBox(5, 5, 310, 230, "����", "STM32Player v0.2\nPowered By StopPointTeam.\nAll Rights Reversed.\n\n��Ȩ���У�����ؾ�", NULL);

            KEY_WaitKey(JOY_L);
        }
        break;

        case 4: //�˳�����
        {
            return;
        }
        }
    }
}

/**
  * @brief  ��Ϣ����������º��˳�
  * @param  head: ����
  * @param  content: ����
  */
void APP_Setting_Msg(uint8_t *head, uint8_t *content)
{
    GE_Draw_Fill(60, 75, 200, 90, WHITE);
    GE_GUI_MsgBox(60, 75, 200, 90, head, content, NULL);
    KEY_WaitKey(JOY_L);
}
