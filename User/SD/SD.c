//SD 卡操作库，具体读写等操作请使用 FatFs 中的函数完成

#include "stdio.h"
#include "string.h"

#include "key.h"
#include "lcd.h"

#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio_dma.h"

#include "GameEngine.h"

#include "SD.h"

/**************************************** 全局变量 ****************************************/

FATFS fs;
char DiskPath[4] = {0, 0, 0, 0};

/*****************************************************************************************/

/**************************************** 私有函数 ****************************************/

uint8_t SD_Mount(void);
uint8_t SD_DeMount(void);

/*****************************************************************************************/

/**
  * @brief  SD 卡初始化
  * @retval 成功返回 0，失败返回 1
  */
uint8_t SD_Init(void)
{
    FATFS_LinkDriver(&SD_Driver, DiskPath);
    return SD_Mount();
}

/**
  * @brief  SD 卡反初始化。在弹出 SD 卡前调用
  * @retval 成功返回 0，失败返回 1
  */
uint8_t SD_DeInit(void)
{
    return SD_DeMount();
}

/**
  * @brief  SD 卡挂载
  * @retval 成功返回 0，失败返回 1
  */
uint8_t SD_Mount(void)
{
    if (f_mount(&fs, DiskPath, 0) != FR_OK)
        return SD_ERROR;
    else
        return SD_OK;
}

/**
  * @brief  SD 卡卸载
  * @retval 成功返回 0，失败返回 1
  */
uint8_t SD_DeMount(void)
{
    if (f_mount(NULL, DiskPath, 0) != FR_OK)
        return SD_ERROR;
    else
        return SD_OK;
}

/**
  * @brief  SD 卡生成文件路径
  * @param  filepath: 返回储存文件路径
  * @param  filename: 文件名，含路径
  * @retval 成功返回 0，失败返回 1
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
  * @brief  SD 卡 GUI 文件选择器
  * @param  filepath: 返回储存文件路径
  * @param  filesuffix: 文件后缀名。NULL 时不限后缀
  * @retval 成功返回 0，失败返回 1
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
        if (f_readdir(&DirInf, &FileInf) != FR_OK || FileInf.fname[0] == 0) //读取结束
            break;

        if (FileInf.fname[0] == '.' || strcmp(FileInf.fname, "System Volume Information") == 0) //跳过文件和文件夹
            continue;

        //判断是文件还是子目录
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

    uint8_t choice = GE_GUI_MenuBox(5, 5, 310, 230, "请选择文件：", num, content, NULL);
    
    strcpy(filename, content[choice - 1]);
    return SD_OK;
}
