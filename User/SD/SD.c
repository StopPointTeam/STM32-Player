//SD �������⣬�����д�Ȳ�����ʹ�� FatFs �еĺ������

#include "stdio.h"
#include "string.h"

#include "key.h"
#include "lcd.h"

#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio_dma.h"

#include "GameEngine.h"

#include "SD.h"

/**************************************** ȫ�ֱ��� ****************************************/

FATFS fs;
char DiskPath[4] = {0, 0, 0, 0};

/*****************************************************************************************/

/**************************************** ˽�к��� ****************************************/

uint8_t SD_Mount(void);
uint8_t SD_DeMount(void);

/*****************************************************************************************/

/**
  * @brief  SD ����ʼ��
  * @retval �ɹ����� 0��ʧ�ܷ��� 1
  */
uint8_t SD_Init(void)
{
    FATFS_LinkDriver(&SD_Driver, DiskPath);
    return SD_Mount();
}

/**
  * @brief  SD ������ʼ�����ڵ��� SD ��ǰ����
  * @retval �ɹ����� 0��ʧ�ܷ��� 1
  */
uint8_t SD_DeInit(void)
{
    return SD_DeMount();
}

/**
  * @brief  SD ������
  * @retval �ɹ����� 0��ʧ�ܷ��� 1
  */
uint8_t SD_Mount(void)
{
    if (f_mount(&fs, DiskPath, 0) != FR_OK)
        return SD_ERROR;
    else
        return SD_OK;
}

/**
  * @brief  SD ��ж��
  * @retval �ɹ����� 0��ʧ�ܷ��� 1
  */
uint8_t SD_DeMount(void)
{
    if (f_mount(NULL, DiskPath, 0) != FR_OK)
        return SD_ERROR;
    else
        return SD_OK;
}

/**
  * @brief  SD �������ļ�·��
  * @param  filepath: ���ش����ļ�·��
  * @param  filename: �ļ�������·��
  * @retval �ɹ����� 0��ʧ�ܷ��� 1
  */
uint8_t SD_GetPath(uint8_t *filepath, uint8_t *filename)
{
    if (DiskPath[0] == 0)
        return SD_ERROR;
    else
        sprintf(filepath, "%s%s", DiskPath, filename);

    return SD_OK;
}

/**
  * @brief  SD �� GUI �ļ�ѡ����
  * @param  filepath: ���ش����ļ�·��
  * @param  filesuffix: �ļ���׺����NULL ʱ���޺�׺
  * @retval �ɹ����� 0��ʧ�ܷ��� 1
  */
uint8_t SD_SelectFile(uint8_t *filename, uint8_t *filesuffix)
{
    DIR DirInf;
    FILINFO FileInf;
    uint32_t num;
    uint8_t content[GE_GUI_MENUBOX_MAX_CONTENT_NUM][GE_GUI_MENUBOX_CONTENT_LEN];

    if (f_opendir(&DirInf, DiskPath) != FR_OK)
        return SD_ERROR;

    for (num = 0; num < GE_GUI_MENUBOX_MAX_CONTENT_NUM;)
    {
        if (f_readdir(&DirInf, &FileInf) != FR_OK || FileInf.fname[0] == 0) //��ȡ����
            break;

        if (FileInf.fname[0] == '.' || strcmp(FileInf.fname, "System Volume Information") == 0) //�����ļ����ļ���
            continue;

        //�ж����ļ�������Ŀ¼
        if (FileInf.fattrib & AM_DIR)
        {
            sprintf(content[num], "%s/", (char *)FileInf.fname);
        }
        else
        {
            if (filesuffix != NULL)
            {
                char *suffix = strrchr(FileInf.fname, '.');
                suffix++;
                if (strcmp(suffix, filesuffix) != 0)
                    continue;
            }

            sprintf(content[num], "%s", FileInf.fname);
        }

        num++;
    }

    uint8_t choice = GE_GUI_MenuBox(5, 5, 310, 230, "��ѡ���ļ���", num, content, NULL);
    
    strcpy(filename, content[choice - 1]);
    return SD_OK;
}
