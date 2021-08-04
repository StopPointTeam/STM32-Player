//游戏引擎

#ifndef __GAMEENGINE_H
#define __GAMEENGINE_H

//在这里注释掉不需要的子库
#define EN_GE_DRAW
#define EN_GE_FONT //FONT 子库依赖于 DRAW

#ifdef EN_GE_DRAW
#include "GE_Draw.h"
#endif

#ifdef EN_GE_FONT
#include "GE_Font.h"
#endif

#define FALSE 0
#define TRUE 1

void GE_Init(void);

#endif
