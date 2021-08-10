#include "stdio.h"

#include "sys.h"
#include "delay.h"

#include "key.h"
#include "lcd.h"

#include "GameEngine.h"
#include "SD.h"

#include "APP_Reader.h"

/**************************************** ˽�ж��� ****************************************/

typedef struct
{
    FSIZE_t page_ptr;
    uint32_t page_size;
} Page; //ҳ�ṹ����

typedef struct
{
    uint8_t font_size;
    uint32_t page_num;
    uint32_t page_amount;
} Save; //�浵�ṹ����

/*****************************************************************************************/

/**************************************** ȫ�ֱ��� ****************************************/

FIL file;          //�ļ�
char filename[32]; //�ļ���
char filepath[32]; //�ļ�·��

FIL page_file;          //��ҳ�ļ�
char page_filepath[32]; //��ҳ�ļ���
char page_filename[32]; //��ҳ�ļ�·��

FIL save_file;          //�浵�ļ�
char save_filepath[32]; //�浵�ļ���
char save_filename[32]; //�浵�ļ�·��

uint32_t page_amount = 1;            //ҳ��
uint8_t current_font_size = FONT_24; //��ǰ��������
uint32_t current_page = 1;           //��ǰҳ��

/*****************************************************************************************/

/**************************************** ˽�к��� ****************************************/

uint8_t APP_Reader_ReadPage(uint8_t font_size, uint32_t page_num);
uint8_t APP_Reader_SplitPages(uint8_t font_size);
uint8_t APP_Reader_Menu(void);
uint8_t APP_Reader_SaveWrite(void);
uint8_t APP_Reader_SaveRead(void);
void APP_Reader_Msg(uint8_t *head, uint8_t *content);

/*****************************************************************************************/

/**
  * @brief  �����Ķ���
  */
void APP_Reader_Launcher(void)
{
    //GUI ѡȡ�ļ�
    if (SD_SelectFile(filename, "txt") != SD_OK)
    {
        APP_Reader_Msg("����", "ѡȡ�ļ�����\n\n�밴���������");

        return;
    }

    SD_GetPath(filepath, filename);

    //���ļ�
    if (f_open(&file, filepath, FA_OPEN_EXISTING | FA_READ) != FR_OK)
    {
        APP_Reader_Msg("����", "���ļ�����\n\n�밴���������");

        return;
    }

    //���浵
    uint8_t is_saved = 0;
    if (APP_Reader_SaveRead() == 0)
        is_saved = 1;

    uint32_t save_page_amount = page_amount;

    //��ҳ
    if (APP_Reader_SplitPages(current_font_size) != 0)
    {
        APP_Reader_Msg("����", "��ҳ����\n\n�밴���������");

        return;
    }

    if (is_saved == 0 || save_page_amount != page_amount)
        current_page = 1;

    //��ʼ�Ķ�
    f_open(&page_file, page_filepath, FA_OPEN_EXISTING | FA_READ); //�򿪷�ҳ�ļ�
    while (1)
    {
        LCD_Disp_Off;
        GE_Draw_ClrAll(WHITE);

        if (APP_Reader_ReadPage(current_font_size, current_page) != 0)
        {
            GE_Draw_ClrAll(WHITE);
            LCD_Disp_On;
            APP_Reader_Msg("����", "��������\n\n�밴���������");

            return;
        }
        LCD_Disp_On;

        switch (KEY_GetKeyWait())
        {
        case KEY1: //���ò˵�
        {
            if (APP_Reader_Menu() != 0)
                return;
        }
        break;
        case KEY2: //���ҳ
        {
            if (current_page == page_amount)
                APP_Reader_Msg("��ʾ", "�ѵ����һҳ��");
            else
                current_page++;
        }
        break;
        case KEY3: //Ŀǰ�����ڴ˼�
        {
            if (current_page == 1)
                APP_Reader_Msg("��ʾ", "�ѵ���һҳ��");
            else
                current_page--;
        }
        break;
        case KEY4: //Ŀǰ�����ڴ˼�
        {
        }
        }
    }
}

/**
  * @brief  ��ʾĳһҳ
  * @param  font_size: �����С
  * @param  page_num: ҳ�룬�� 1 ��ʼ
  * @retval �ɹ����� 0��ʧ�ܷ��� 1
  */
uint8_t APP_Reader_ReadPage(uint8_t font_size, uint32_t page_num)
{
    FRESULT f_res;
    uint32_t br;
    uint32_t page_br;
    Page page;

    //����ҳ��Ϣ
    f_res = f_lseek(&page_file, (page_num - 1) * sizeof(Page));
    f_res = f_read(&page_file, &page, sizeof(Page), &page_br);

    //��ȡ�ļ�
    uint8_t buffer[1500];

    f_res = f_lseek(&file, page.page_ptr);
    f_res = f_read(&file, buffer, page.page_size, &br);

    if (f_res != FR_OK)
        return 1;

    *((uint8_t *)buffer + page.page_size) = '\0';

    if (font_size == FONT_16)
        ge_font_print_set.font_size = FONT_16;
    else
        ge_font_print_set.font_size = FONT_24;

    GE_Font_Print_WithSet(0, 0, BORDER_MAX, BORDER_MAX, buffer);

    char temp_str[15];
    sprintf(temp_str, "ҳ��:%d/%d  %.1f%%", page_num, page_amount, (float)page_num / (float)page_amount * 100.0);
    GE_Font_Print(1, 223, BORDER_MAX, BORDER_MAX, FONT_16, BLUE, WHITE, TRUE, temp_str);

    return 0;
}

/**
  * @brief  ���ı��ļ���ҳ
  * @param  font_size: �����С
  * @retval �ɹ����� 0��ʧ�ܷ��� 1
  */
uint8_t APP_Reader_SplitPages(uint8_t font_size)
{
    uint32_t br;
    FRESULT f_res;
    char ch;
    uint8_t x, y;
    Page page;
    FSIZE_t fptr = 0;

    uint32_t page_bw;

    uint8_t x_max, y_max;
    if (font_size == FONT_16)
    {
        x_max = 40;
        y_max = 13;
    }
    else
    {
        x_max = 26;
        y_max = 9;
    }

    strcpy(page_filename, filename);
    strcat(page_filename, ".page");

    SD_GetPath(page_filepath, page_filename);

    f_res = f_open(&page_file, page_filepath, FA_CREATE_ALWAYS | FA_WRITE);

    page_amount = 1;
    f_lseek(&file, 0);
    page.page_size = 0;

    while (1)
    {
        x = 0;
        y = 0;

        while (1)
        {
            if (f_res != FR_OK)
                return 1;

            f_res = f_read(&file, &ch, 1, &br); //��һ���ַ�
            page.page_size++;

            if (br == 0) //��ҳ���
            {
                page.page_ptr = fptr;
                f_res = f_write(&page_file, &page, sizeof(Page), &page_bw);

                f_res = f_close(&page_file);

                if (f_res != FR_OK)
                    return 1;
                else
                    return 0;
            }

            if (ch <= 0x80) //ASCII �ַ�
            {
                if (ch == '\r' || ch == '\0') //���Ե��ַ�
                    continue;

                if ((x + 1 > x_max) || (ch == '\n'))
                {
                    x = 0;
                    y++;

                    if (y + 1 > y_max)
                    {
                        if (ch != '\n')
                            f_res = f_lseek(&file, file.fptr - 1);

                        page.page_size--;
                        break;
                    }
                }

                if (ch != '\n')
                    x++;
            }
            else //GBK �ַ�
            {
                f_res = f_lseek(&file, file.fptr + 1);
                page.page_size++;

                if (x + 2 > x_max)
                {
                    x = 0;
                    y++;

                    if (y + 1 > y_max)
                    {
                        f_res = f_lseek(&file, file.fptr - 2);
                        page.page_size -= 2;
                        break;
                    }
                }

                x += 2;
            }
        }

        page_amount++;                                              //ҳ������
        page.page_ptr = fptr;                                       //��ҳ��ʼָ��
        f_res = f_write(&page_file, &page, sizeof(Page), &page_bw); //д���ҳ�ļ�
        page.page_size = 0;                                         //ҳ��С����
        fptr = file.fptr;                                           //��ҳ��ʼָ��
    }
}

/**
  * @brief  ���ò˵�
  * @retval �����Ķ������� 0�������Ķ����� 1
  */
uint8_t APP_Reader_Menu(void)
{
    uint8_t content[2][GE_GUI_MENUBOX_CONTENT_LEN] = {"��������", "�˳�"};

    GE_Draw_Fill(50, 50, 220, 140, WHITE);
    switch (GE_GUI_MenuBox(50, 50, 220, 140, "�˵�", 2, content, NULL))
    {
    case 1:
    {
        GE_Draw_Fill(50, 50, 220, 140, WHITE);

        uint8_t content[2][GE_GUI_MENUBOX_CONTENT_LEN] = {"�������", "С������"};
        if (GE_GUI_MenuBox(50, 50, 220, 140, "��������", 2, content, NULL) == 1)
        {
            if (current_font_size == FONT_24)
            {
                APP_Reader_Msg("��ʾ", "������ʹ�ô�����壡");
                current_page--;
            }
            else
            {
                float read_pos = (float)current_page / (float)page_amount;
                current_font_size = FONT_24;
                f_close(&page_file);
                APP_Reader_SplitPages(current_font_size);
                f_open(&page_file, page_filepath, FA_OPEN_EXISTING | FA_READ);
                current_page = (float)page_amount * read_pos;
            }
        }
        else
        {
            if (current_font_size == FONT_16)
            {
                APP_Reader_Msg("��ʾ", "������ʹ��С�����壡");
                current_page--;
            }
            else
            {
                float read_pos = (float)current_page / (float)page_amount;
                current_font_size = FONT_16;
                f_close(&page_file);
                APP_Reader_SplitPages(current_font_size);
                f_open(&page_file, page_filepath, FA_OPEN_EXISTING | FA_READ);
                current_page = (float)page_amount * read_pos;
            }
        }
    }
    break;

    case 2:
    {
        //�ر��ļ�
        f_close(&page_file);
        f_close(&file);

        APP_Reader_SaveWrite();

        return 1;
    }
    break;
    }

    return 0;
}

/**
  * @brief  ���浵
  * @retval �д浵���� 0���޴浵���� 1
  */
uint8_t APP_Reader_SaveRead(void)
{
    uint32_t save_br;
    Save save;

    strcpy(save_filename, filename);
    strcat(save_filename, ".save");

    SD_GetPath(save_filepath, save_filename);

    if (f_open(&save_file, save_filepath, FA_OPEN_EXISTING | FA_READ) != FR_OK ||
        f_read(&save_file, &save, sizeof(Save), &save_br) != FR_OK)
    {
        f_close(&save_file);

        return 1;
    }

    f_close(&save_file);

    if (save.font_size == FONT_16)
        current_font_size = FONT_16;
    else if (save.font_size == FONT_24)
        current_font_size = FONT_24;
    else
        return 1;

    current_page = save.page_num;
    page_amount = save.page_amount;

    return 0;
}

/**
  * @brief  д�浵
  * @retval �ɹ����� 0��ʧ�ܷ��� 1
  */
uint8_t APP_Reader_SaveWrite(void)
{
    uint32_t save_bw;
    Save save;

    save.font_size = current_font_size;
    save.page_num = current_page;
    save.page_amount = page_amount;

    strcpy(save_filename, filename);
    strcat(save_filename, ".save");

    SD_GetPath(save_filepath, save_filename);
    f_open(&save_file, save_filepath, FA_CREATE_ALWAYS | FA_WRITE);
    f_write(&save_file, &save, sizeof(Save), &save_bw);
    f_close(&save_file);

    return 0;
}

/**
  * @brief  ��Ϣ����������º��˳�
  * @param  head: ����
  * @param  content: ����
  */
void APP_Reader_Msg(uint8_t *head, uint8_t *content)
{
    GE_Draw_Fill(60, 75, 200, 90, WHITE);
    GE_GUI_MsgBox(60, 75, 200, 90, head, content, NULL);
    KEY_GetKeyWait();
}
