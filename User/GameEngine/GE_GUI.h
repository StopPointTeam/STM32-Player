//游戏引擎 GUI 库

#ifndef __GE_GUI_H
#define __GE_GUI_H

//回调函数定义
typedef void (*MsgBoxHandler)(void);
typedef void (*MenuBoxHandler)(uint8_t choice_num);

void GE_GUI_Init(void);

void GE_GUI_MsgBox(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *head, uint8_t *content, MsgBoxHandler func);

#define GE_GUI_MENUBOX_MAX_CONTENT_NUM 10 //适用参数 5, 5, 310, 230
#define GE_GUI_MENUBOX_CONTENT_LEN 37     //适用参数 5, 5, 310, 230
uint8_t GE_GUI_MenuBox(
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t height,
    uint8_t *head,
    uint8_t content_amount,
    uint8_t content[][GE_GUI_MENUBOX_CONTENT_LEN],
    MenuBoxHandler handler_func);

#endif
