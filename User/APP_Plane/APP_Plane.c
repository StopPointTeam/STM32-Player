//飞机大战游戏

#include "stdlib.h"

#include "sys.h"
#include "systick.h"

#include "key.h"
#include "led.h"
#include "lcd.h"

#include "GameEngine.h"

#include "fxlib.h"

#include "APP_Plane_Bitmap.h"

#include "APP_Plane.h"

/**************************************** 私有定义 ****************************************/

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define STATUS_BAR_WIDTH 8

#define SMALL_PLANE_MAX 32
#define MEDIUM_PLANE_MAX 16
#define LARGE_PLANE_MAX 2

#define BULLET_MAX 16

#define PLANE_TYPE_SMALL 0
#define PLANE_TYPE_MEDIUM 1
#define PLANE_TYPE_LARGE 2

#define PLANE_STATUS_NONEXISTENT (-14)
#define PLANE_STATUS_EXPLODING_2 (-13)
#define PLANE_STATUS_EXPLODING_1 (-6)
#define PLANE_STATUS_ALIVE 1

#define SP_HEALTH_VAL 2
#define MP_HEALTH_VAL 6
#define LP_HEALTH_VAL 10

#define BULLET_STATUS_NONEXISTENT 0
#define BULLET_STATUS_FLYING 1

#define DIRECTION_UP 0
#define DIRECTION_DOWN 1
#define DIRECTION_LEFT 2
#define DIRECTION_RIGHT 3

#define SP_GRAPHIC_WIDTH 7
#define SP_GRAPHIC_HEIGHT 7

#define MP_GRAPHIC_WIDTH 12
#define MP_GRAPHIC_HEIGHT 9

#define LP_GRAPHIC_WIDTH 33
#define LP_GRAPHIC_HEIGHT 22

#define RP_GRAPHIC_WIDTH 13
#define RP_GRAPHIC_HEIGHT 13

#define BULLET_GRAPHIC_WIDTH 5
#define BULLET_GRAPHIC_HEIGHT 3

#define SP_COLLISION_POS_X 1
#define SP_COLLISION_POS_Y 1
#define SP_COLLISION_WIDTH 5
#define SP_COLLISION_HEIGHT 5

#define MP_COLLISION_POS_X 1
#define MP_COLLISION_POS_Y 1
#define MP_COLLISION_WIDTH 10
#define MP_COLLISION_HEIGHT 7

#define LP_COLLISION_POS_X 2
#define LP_COLLISION_POS_Y 2
#define LP_COLLISION_WIDTH 27
#define LP_COLLISION_HEIGHT 18

#define RP_COLLISION_POS_X 2
#define RP_COLLISION_POS_Y 2
#define RP_COLLISION_WIDTH 8
#define RP_COLLISION_HEIGHT 9

#define BULLET_COLLISION_POS_X (-1)
#define BULLET_COLLISION_POS_Y 0
#define BULLET_COLLISION_WIDTH 5
#define BULLET_COLLISION_HEIGHT 5

#define COLLISION_ROLE_PLANE 0
#define NO_COLLISION_ROLE_PLANE 1

#define Random() rand()

typedef struct
{
    int x;
    int y;
    int status;
} Object;

/*****************************************************************************************/

/**************************************** 全局变量 ****************************************/

Object Small_Plane[SMALL_PLANE_MAX];
Object Medium_Plane[MEDIUM_PLANE_MAX];
Object Large_Plane[LARGE_PLANE_MAX];
Object Bullet[BULLET_MAX];
Object Role_Plane;

int SP_Generate_Interval_Max, SP_Generate_Interval_Min, MP_Generate_Interval_Max, MP_Generate_Interval_Min, LP_Generate_Interval_Max, LP_Generate_Interval_Min, Bullet_Generate_Interval;
int SP_Move_Interval, MP_Move_Interval, LP_Move_Interval, Bullet_Move_Interval;
int Get_Key_Interval, Turbo_Interval;
int SP_Destroyed, MP_Destroyed, LP_Destroyed, Score;

/*****************************************************************************************/

/**************************************** 私有函数 ****************************************/

void Menu(void);
void Game(void);
void Move_Plane(int type);
void New_Plane(int type);
void Move_Role_Plane(int direction);
void Move_Bullet(void);
void New_Bullet(void);
void Init(void);
void Draw_All(void);
void Game_Over(void);
int Collision_Detection(void);

/*****************************************************************************************/

/**
  * @brief  启动飞机大战游戏
  */
void APP_Plane_Launcher(void)
{
    Game();
}

void Game()
{
    srand(SysTick->VAL);

    int sp_generate_interval, mp_generate_interval, lp_generate_interval;
    int sp_move_timer = 0, mp_move_timer = 0, lp_move_timer = 0, bullet_move_timer = 0;
    int sp_generate_timer = 0, mp_generate_timer = 0, lp_generate_timer = 0, bullet_generate_timer = 0;
    int get_key_timer = 0, turbo_timer = 0;

    Init();

    sp_generate_interval = Random() % (SP_Generate_Interval_Max - SP_Generate_Interval_Min) + SP_Generate_Interval_Min;
    mp_generate_interval = Random() % (MP_Generate_Interval_Max - MP_Generate_Interval_Min) + MP_Generate_Interval_Min;
    lp_generate_interval = Random() % (LP_Generate_Interval_Max - LP_Generate_Interval_Min) + LP_Generate_Interval_Min;

    while (1)
    {
        sp_move_timer++;
        mp_move_timer++;
        lp_move_timer++;
        bullet_move_timer++;

        sp_generate_timer++;
        mp_generate_timer++;
        lp_generate_timer++;
        bullet_generate_timer++;

        get_key_timer++;
        turbo_timer++;

        if (sp_move_timer >= SP_Move_Interval)
        {
            Move_Plane(PLANE_TYPE_SMALL);
            sp_move_timer = 0;
        }

        if (sp_generate_timer >= sp_generate_interval)
        {
            New_Plane(PLANE_TYPE_SMALL);
            sp_generate_timer = 0;
            sp_generate_interval = Random() % (SP_Generate_Interval_Max - SP_Generate_Interval_Min) + SP_Generate_Interval_Min;
        }

        if (mp_move_timer >= MP_Move_Interval)
        {
            Move_Plane(PLANE_TYPE_MEDIUM);
            mp_move_timer = 0;
        }

        if (mp_generate_timer >= mp_generate_interval)
        {
            New_Plane(PLANE_TYPE_MEDIUM);
            mp_generate_timer = 0;
            mp_generate_interval = Random() % (MP_Generate_Interval_Max - MP_Generate_Interval_Min) + MP_Generate_Interval_Min;
        }

        if (lp_move_timer >= LP_Move_Interval)
        {
            Move_Plane(PLANE_TYPE_LARGE);
            lp_move_timer = 0;
        }

        if (lp_generate_timer >= lp_generate_interval)
        {
            New_Plane(PLANE_TYPE_LARGE);
            lp_generate_timer = 0;
            lp_generate_interval = Random() % (LP_Generate_Interval_Max - LP_Generate_Interval_Min) + LP_Generate_Interval_Min;
        }

        if (bullet_move_timer >= Bullet_Move_Interval)
        {
            Move_Bullet();
            bullet_move_timer = 0;
        }

        if (bullet_generate_timer >= Bullet_Generate_Interval)
        {
            New_Bullet();
            bullet_generate_timer = 0;
            Score++;
        }

        if (get_key_timer >= Get_Key_Interval)
        {
            get_key_timer = 0;
            switch (KEY_GetKey())
            {
            case JOY_U_DOWN:
                Move_Role_Plane(DIRECTION_UP);
                break;
            case JOY_D_DOWN:
                Move_Role_Plane(DIRECTION_DOWN);
                break;
            case JOY_L_DOWN:
                Move_Role_Plane(DIRECTION_LEFT);
                break;
            case JOY_R_DOWN:
                Move_Role_Plane(DIRECTION_RIGHT);
            }

            // if (key_down(K_EXIT))
            // {
            //     Game_Over();
            //     return;
            // }
        }

        if (turbo_timer >= Turbo_Interval)
        {
            if (SP_Generate_Interval_Max > 100)
                SP_Generate_Interval_Max -= 50;
            if (SP_Generate_Interval_Min > 50)
                SP_Generate_Interval_Min -= 25;
            if (SP_Move_Interval > 10)
                SP_Move_Interval--;

            if (MP_Generate_Interval_Max > 200)
                MP_Generate_Interval_Max -= 100;
            if (MP_Generate_Interval_Min > 100)
                MP_Generate_Interval_Min -= 50;
            if (MP_Move_Interval > 15)
                MP_Move_Interval -= 2;

            if (LP_Generate_Interval_Max > 2000)
                LP_Generate_Interval_Max -= 2000;
            if (LP_Generate_Interval_Min > 1000)
                LP_Generate_Interval_Min -= 1000;
            if (LP_Move_Interval > 30)
                LP_Move_Interval -= 3;

            if (Turbo_Interval > 1000)
                Turbo_Interval -= 1000;
            turbo_timer = 0;
        }

        if (Collision_Dection() == COLLISION_ROLE_PLANE)
        {
            Game_Over();
            return;
        }

        ML_clear_vram();
        Draw_All();
        ML_display_vram();
    }
}

void Move_Role_Plane(int direction)
{
    if (direction == DIRECTION_UP && Role_Plane.y > 0)
        Role_Plane.y--;
    if (direction == DIRECTION_DOWN && Role_Plane.y + RP_GRAPHIC_HEIGHT < SCREEN_HEIGHT)
        Role_Plane.y++;
    if (direction == DIRECTION_LEFT && Role_Plane.x > 0)
        Role_Plane.x--;
    if (direction == DIRECTION_RIGHT && Role_Plane.x + RP_GRAPHIC_WIDTH < SCREEN_WIDTH - STATUS_BAR_WIDTH)
        Role_Plane.x++;
}

void New_Bullet()
{
    int i;

    for (i = 0; i < BULLET_MAX; i++)
    {
        if (Bullet[i].status == BULLET_STATUS_NONEXISTENT)
        {
            Bullet[i].status = BULLET_STATUS_FLYING;
            Bullet[i].x = Role_Plane.x + RP_GRAPHIC_WIDTH;
            Bullet[i].y = Role_Plane.y + (RP_GRAPHIC_HEIGHT >> 1) - 1;
            break;
        }
    }
}

void New_Plane(int type)
{
    int i;

    if (type == PLANE_TYPE_SMALL)
    {
        for (i = 0; i < SMALL_PLANE_MAX; i++)
        {
            if (Small_Plane[i].status == PLANE_STATUS_NONEXISTENT)
            {
                Small_Plane[i].status = SP_HEALTH_VAL;
                Small_Plane[i].x = SCREEN_WIDTH - STATUS_BAR_WIDTH;
                Small_Plane[i].y = Random() % (SCREEN_HEIGHT - SP_GRAPHIC_HEIGHT);
                return;
            }
        }
    }

    if (type == PLANE_TYPE_MEDIUM)
    {
        for (i = 0; i < MEDIUM_PLANE_MAX; i++)
        {
            if (Medium_Plane[i].status == PLANE_STATUS_NONEXISTENT)
            {
                Medium_Plane[i].status = MP_HEALTH_VAL;
                Medium_Plane[i].x = SCREEN_WIDTH - STATUS_BAR_WIDTH;
                Medium_Plane[i].y = Random() % (SCREEN_HEIGHT - MP_GRAPHIC_HEIGHT);
                return;
            }
        }
    }

    if (type == PLANE_TYPE_LARGE)
    {
        for (i = 0; i < LARGE_PLANE_MAX; i++)
        {
            if (Large_Plane[i].status == PLANE_STATUS_NONEXISTENT)
            {
                Large_Plane[i].status = LP_HEALTH_VAL;
                Large_Plane[i].x = SCREEN_WIDTH - STATUS_BAR_WIDTH;
                Large_Plane[i].y = Random() % (SCREEN_HEIGHT - LP_GRAPHIC_HEIGHT);
                return;
            }
        }
    }
}

void Move_Plane(int type)
{
    int i;

    if (type == PLANE_TYPE_SMALL)
    {
        for (i = 0; i < SMALL_PLANE_MAX; i++)
        {
            if (Small_Plane[i].status >= PLANE_STATUS_ALIVE)
            {
                Small_Plane[i].x--;
                if (Small_Plane[i].x < -SP_GRAPHIC_WIDTH)
                    Small_Plane[i].status = PLANE_STATUS_NONEXISTENT;
            }
            else if (Small_Plane[i].status >= PLANE_STATUS_EXPLODING_1)
            {
                Small_Plane[i].status--;
            }
            else if (Small_Plane[i].status >= PLANE_STATUS_EXPLODING_2)
            {
                Small_Plane[i].status--;
            }
        }
    }

    if (type == PLANE_TYPE_MEDIUM)
    {
        for (i = 0; i < MEDIUM_PLANE_MAX; i++)
        {
            if (Medium_Plane[i].status >= PLANE_STATUS_ALIVE)
            {
                Medium_Plane[i].x--;
                if (Medium_Plane[i].x < -MP_GRAPHIC_WIDTH)
                    Medium_Plane[i].status = PLANE_STATUS_NONEXISTENT;
            }
            else if (Medium_Plane[i].status >= PLANE_STATUS_EXPLODING_1)
            {
                Medium_Plane[i].status--;
            }
            else if (Medium_Plane[i].status >= PLANE_STATUS_EXPLODING_2)
            {
                Medium_Plane[i].status--;
            }
        }
    }

    if (type == PLANE_TYPE_LARGE)
    {
        for (i = 0; i < LARGE_PLANE_MAX; i++)
        {
            if (Large_Plane[i].status >= PLANE_STATUS_ALIVE)
            {
                Large_Plane[i].x--;
                if (Large_Plane[i].x < -LP_GRAPHIC_WIDTH)
                    Large_Plane[i].status = PLANE_STATUS_NONEXISTENT;
            }
            else if (Large_Plane[i].status >= PLANE_STATUS_EXPLODING_1)
            {
                Large_Plane[i].status--;
            }
            else if (Large_Plane[i].status >= PLANE_STATUS_EXPLODING_2)
            {
                Large_Plane[i].status--;
            }
        }
    }
}

void Move_Bullet()
{
    int i;

    for (i = 0; i < BULLET_MAX; i++)
    {
        if (Bullet[i].status == BULLET_STATUS_FLYING)
        {
            Bullet[i].x++;
            if (Bullet[i].x > SCREEN_WIDTH - STATUS_BAR_WIDTH)
                Bullet[i].status = BULLET_STATUS_NONEXISTENT;
        }
    }
}

void Init()
{
    int i;

    for (i = 0; i < SMALL_PLANE_MAX; i++)
    {
        Small_Plane[i].status = PLANE_STATUS_NONEXISTENT;
    }

    for (i = 0; i < MEDIUM_PLANE_MAX; i++)
    {
        Medium_Plane[i].status = PLANE_STATUS_NONEXISTENT;
    }

    for (i = 0; i < LARGE_PLANE_MAX; i++)
    {
        Large_Plane[i].status = PLANE_STATUS_NONEXISTENT;
    }

    for (i = 0; i < BULLET_MAX; i++)
    {
        Bullet[i].status = BULLET_STATUS_NONEXISTENT;
    }

    Role_Plane.x = 0;
    Role_Plane.y = (SCREEN_HEIGHT - RP_GRAPHIC_HEIGHT) / 2;
    Role_Plane.status = PLANE_STATUS_ALIVE;

    SP_Generate_Interval_Max = 300; //600
    SP_Generate_Interval_Min = 150; //300
    SP_Move_Interval = 3;           //20

    MP_Generate_Interval_Max = 600; //1200
    MP_Generate_Interval_Min = 300; //600
    MP_Move_Interval = 5;           //30

    LP_Generate_Interval_Max = 6000; //12000
    LP_Generate_Interval_Min = 3000; //6000
    LP_Move_Interval = 10;           //60

    Bullet_Move_Interval = 1;      //3
    Bullet_Generate_Interval = 30; //100

    Get_Key_Interval = 1;
    Turbo_Interval = 10000;

    SP_Destroyed = 0;
    MP_Destroyed = 0;
    LP_Destroyed = 0;
    Score = 0;
}

void Draw_All()
{
    int i, j, digit, remain;
    unsigned char tmp;

    for (i = 0; i < LARGE_PLANE_MAX; i++)
    {
        if (Large_Plane[i].status >= PLANE_STATUS_ALIVE)
        {
            ML_bmp_or_cl(Bitmap_LP_CoverArea, Large_Plane[i].x, Large_Plane[i].y, LP_GRAPHIC_WIDTH, LP_GRAPHIC_HEIGHT);
            ML_bmp_xor_cl(Bitmap_LP_ClearArea, Large_Plane[i].x, Large_Plane[i].y, LP_GRAPHIC_WIDTH, LP_GRAPHIC_HEIGHT);
        }
        else if (Large_Plane[i].status >= PLANE_STATUS_EXPLODING_1)
        {
            ML_bmp_or_cl(Bitmap_LP_CoverArea, Large_Plane[i].x, Large_Plane[i].y, LP_GRAPHIC_WIDTH, LP_GRAPHIC_HEIGHT);
            ML_bmp_xor_cl(Bitmap_LP_ClearArea, Large_Plane[i].x, Large_Plane[i].y, LP_GRAPHIC_WIDTH, LP_GRAPHIC_HEIGHT);
            ML_bmp_or_cl(Bitmap_Explosion_Large_1, Large_Plane[i].x, Large_Plane[i].y, 16, 16);
            ML_bmp_or_cl(Bitmap_Explosion_Large_1, Large_Plane[i].x + LP_GRAPHIC_WIDTH - 16, Large_Plane[i].y + LP_GRAPHIC_HEIGHT - 16, 16, 16);
        }
        else if (Large_Plane[i].status >= PLANE_STATUS_EXPLODING_2)
        {
            ML_bmp_or_cl(Bitmap_LP_CoverArea, Large_Plane[i].x, Large_Plane[i].y, LP_GRAPHIC_WIDTH, LP_GRAPHIC_HEIGHT);
            ML_bmp_xor_cl(Bitmap_LP_ClearArea, Large_Plane[i].x, Large_Plane[i].y, LP_GRAPHIC_WIDTH, LP_GRAPHIC_HEIGHT);
            ML_bmp_or_cl(Bitmap_Explosion_Large_2, Large_Plane[i].x, Large_Plane[i].y, 16, 16);
            ML_bmp_or_cl(Bitmap_Explosion_Large_2, Large_Plane[i].x + LP_GRAPHIC_WIDTH - 16, Large_Plane[i].y + LP_GRAPHIC_HEIGHT - 16, 16, 16);
        }
    }

    for (i = 0; i < MEDIUM_PLANE_MAX; i++)
    {
        if (Medium_Plane[i].status >= PLANE_STATUS_ALIVE)
        {
            ML_bmp_or_cl(Bitmap_MP_CoverArea, Medium_Plane[i].x, Medium_Plane[i].y, MP_GRAPHIC_WIDTH, MP_GRAPHIC_HEIGHT);
            ML_bmp_xor_cl(Bitmap_MP_ClearArea, Medium_Plane[i].x, Medium_Plane[i].y, MP_GRAPHIC_WIDTH, MP_GRAPHIC_HEIGHT);
        }
        else if (Medium_Plane[i].status >= PLANE_STATUS_EXPLODING_1)
        {
            ML_bmp_or_cl(Bitmap_MP_CoverArea, Medium_Plane[i].x, Medium_Plane[i].y, MP_GRAPHIC_WIDTH, MP_GRAPHIC_HEIGHT);
            ML_bmp_xor_cl(Bitmap_MP_ClearArea, Medium_Plane[i].x, Medium_Plane[i].y, MP_GRAPHIC_WIDTH, MP_GRAPHIC_HEIGHT);
            ML_bmp_or_cl(Bitmap_Explosion_1, Medium_Plane[i].x + 2, Medium_Plane[i].y, 8, 8);
        }
        else if (Medium_Plane[i].status >= PLANE_STATUS_EXPLODING_2)
        {
            ML_bmp_or_cl(Bitmap_MP_CoverArea, Medium_Plane[i].x, Medium_Plane[i].y, MP_GRAPHIC_WIDTH, MP_GRAPHIC_HEIGHT);
            ML_bmp_xor_cl(Bitmap_MP_ClearArea, Medium_Plane[i].x, Medium_Plane[i].y, MP_GRAPHIC_WIDTH, MP_GRAPHIC_HEIGHT);
            ML_bmp_or_cl(Bitmap_Explosion_2, Medium_Plane[i].x + 2, Medium_Plane[i].y, 8, 8);
        }
    }

    for (i = 0; i < SMALL_PLANE_MAX; i++)
    {
        if (Small_Plane[i].status >= PLANE_STATUS_ALIVE)
        {
            ML_bmp_or_cl(Bitmap_SP_CoverArea, Small_Plane[i].x, Small_Plane[i].y, SP_GRAPHIC_WIDTH, SP_GRAPHIC_HEIGHT);
            ML_bmp_xor_cl(Bitmap_SP_ClearArea, Small_Plane[i].x, Small_Plane[i].y, SP_GRAPHIC_WIDTH, SP_GRAPHIC_HEIGHT);
        }
        else if (Small_Plane[i].status >= PLANE_STATUS_EXPLODING_1)
        {
            ML_bmp_or_cl(Bitmap_SP_CoverArea, Small_Plane[i].x, Small_Plane[i].y, SP_GRAPHIC_WIDTH, SP_GRAPHIC_HEIGHT);
            ML_bmp_xor_cl(Bitmap_SP_ClearArea, Small_Plane[i].x, Small_Plane[i].y, SP_GRAPHIC_WIDTH, SP_GRAPHIC_HEIGHT);
            ML_bmp_or_cl(Bitmap_Explosion_1, Small_Plane[i].x, Small_Plane[i].y - 1, 8, 8);
        }
        else if (Small_Plane[i].status >= PLANE_STATUS_EXPLODING_2)
        {
            ML_bmp_or_cl(Bitmap_SP_CoverArea, Small_Plane[i].x, Small_Plane[i].y, SP_GRAPHIC_WIDTH, SP_GRAPHIC_HEIGHT);
            ML_bmp_xor_cl(Bitmap_SP_ClearArea, Small_Plane[i].x, Small_Plane[i].y, SP_GRAPHIC_WIDTH, SP_GRAPHIC_HEIGHT);
            ML_bmp_or_cl(Bitmap_Explosion_2, Small_Plane[i].x, Small_Plane[i].y - 1, 8, 8);
        }
    }

    for (i = 0; i < BULLET_MAX; i++)
    {
        if (Bullet[i].status == BULLET_STATUS_FLYING)
            ML_bmp_or_cl(Bitmap_Bullet, Bullet[i].x, Bullet[i].y, BULLET_GRAPHIC_WIDTH, BULLET_GRAPHIC_HEIGHT);
    }

    if (Role_Plane.status == PLANE_STATUS_ALIVE)
    {
        ML_bmp_or_cl(Bitmap_RP_CoverArea, Role_Plane.x, Role_Plane.y, RP_GRAPHIC_WIDTH, RP_GRAPHIC_HEIGHT);
        ML_bmp_xor_cl(Bitmap_RP_ClearArea, Role_Plane.x, Role_Plane.y, RP_GRAPHIC_WIDTH, RP_GRAPHIC_HEIGHT);
    }

    j = 10000000;
    remain = Score;
    for (i = 0; i < 8; i++)
    {
        digit = remain / j;
        ML_bmp_or_cl(Bitmap_Numbers[digit], SCREEN_WIDTH - STATUS_BAR_WIDTH, i * 4, 7, 3);
        remain -= digit * j;
        j /= 10;
    }

    GE_Draw_Rectangle(32, 56, 257, 129, BLACK);
}

int Collision_Dection()
{
    int i, j;
    int x1, y1, w1, h1, x2, y2, w2, h2, x3, y3, w3, h3;

    x1 = Role_Plane.x + RP_COLLISION_POS_X;
    y1 = Role_Plane.y + RP_COLLISION_POS_Y;
    w1 = RP_COLLISION_WIDTH;
    h1 = RP_COLLISION_HEIGHT;

    w2 = SP_COLLISION_WIDTH;
    h2 = SP_COLLISION_HEIGHT;
    w3 = BULLET_COLLISION_WIDTH;
    h3 = BULLET_COLLISION_HEIGHT;
    for (i = 0; i < SMALL_PLANE_MAX; i++)
    {
        if (Small_Plane[i].status >= PLANE_STATUS_ALIVE)
        {
            x2 = Small_Plane[i].x + SP_COLLISION_POS_X;
            y2 = Small_Plane[i].y + SP_COLLISION_POS_Y;
            if (x1 - x2 < w2 && x2 - x1 < w1 && y1 - y2 < h2 && y2 - y1 < h1)
                return COLLISION_ROLE_PLANE;
            for (j = 0; j < BULLET_MAX; j++)
            {
                if (Bullet[j].status == BULLET_STATUS_FLYING)
                {
                    x3 = Bullet[j].x + BULLET_COLLISION_POS_X;
                    y3 = Bullet[j].y + BULLET_COLLISION_POS_Y;
                    if (x3 - x2 < w2 && x2 - x3 < w3 && y3 - y2 < h2 && y2 - y3 < h3)
                    {
                        Small_Plane[i].status--;
                        if (Small_Plane[i].status == PLANE_STATUS_ALIVE - 1)
                        {
                            SP_Destroyed++;
                            Score += 1000;
                        }
                        Bullet[j].status = BULLET_STATUS_NONEXISTENT;
                    }
                }
            }
        }
    }

    w2 = MP_COLLISION_WIDTH;
    h2 = MP_COLLISION_HEIGHT;
    w3 = BULLET_COLLISION_WIDTH;
    h3 = BULLET_COLLISION_HEIGHT;
    for (i = 0; i < MEDIUM_PLANE_MAX; i++)
    {
        if (Medium_Plane[i].status >= PLANE_STATUS_ALIVE)
        {
            x2 = Medium_Plane[i].x + MP_COLLISION_POS_X;
            y2 = Medium_Plane[i].y + MP_COLLISION_POS_Y;
            if (x1 - x2 < w2 && x2 - x1 < w1 && y1 - y2 < h2 && y2 - y1 < h1)
                return COLLISION_ROLE_PLANE;
            for (j = 0; j < BULLET_MAX; j++)
            {
                if (Bullet[j].status == BULLET_STATUS_FLYING)
                {
                    x3 = Bullet[j].x + BULLET_COLLISION_POS_X;
                    y3 = Bullet[j].y + BULLET_COLLISION_POS_Y;
                    if (x3 - x2 < w2 && x2 - x3 < w3 && y3 - y2 < h2 && y2 - y3 < h3)
                    {
                        Medium_Plane[i].status--;
                        if (Medium_Plane[i].status == PLANE_STATUS_ALIVE - 1)
                        {
                            MP_Destroyed++;
                            Score += 6000;
                        }
                        Bullet[j].status = BULLET_STATUS_NONEXISTENT;
                    }
                }
            }
        }
    }

    w2 = LP_COLLISION_WIDTH;
    h2 = LP_COLLISION_HEIGHT;
    w3 = BULLET_COLLISION_WIDTH;
    h3 = BULLET_COLLISION_HEIGHT;
    for (i = 0; i < LARGE_PLANE_MAX; i++)
    {
        if (Large_Plane[i].status >= PLANE_STATUS_ALIVE)
        {
            x2 = Large_Plane[i].x + LP_COLLISION_POS_X;
            y2 = Large_Plane[i].y + LP_COLLISION_POS_Y;
            if (x1 - x2 < w2 && x2 - x1 < w1 && y1 - y2 < h2 && y2 - y1 < h1)
                return COLLISION_ROLE_PLANE;
            for (j = 0; j < BULLET_MAX; j++)
            {
                if (Bullet[j].status == BULLET_STATUS_FLYING)
                {
                    x3 = Bullet[j].x + BULLET_COLLISION_POS_X;
                    y3 = Bullet[j].y + BULLET_COLLISION_POS_Y;
                    if (x3 - x2 < w2 && x2 - x3 < w3 && y3 - y2 < h2 && y2 - y3 < h3)
                    {
                        Large_Plane[i].status--;
                        if (Large_Plane[i].status == PLANE_STATUS_ALIVE - 1)
                        {
                            LP_Destroyed++;
                            Score += 10000;
                        }
                        Bullet[j].status = BULLET_STATUS_NONEXISTENT;
                    }
                }
            }
        }
    }

    return NO_COLLISION_ROLE_PLANE;
}

void Game_Over()
{
    int i;

    for (i = 0; i < BULLET_MAX; i++)
    {
        Bullet[i].status = BULLET_STATUS_NONEXISTENT;
    }

    ML_clear_vram();
    Draw_All();
    ML_display_vram();
    Sleep(750);
    ML_bmp_or_cl(Bitmap_Explosion_Large_1, Role_Plane.x - 2, Role_Plane.y - 2, 16, 16);
    ML_display_vram();
    Sleep(750);
    ML_clear_vram();
    Draw_All();
    ML_bmp_or_cl(Bitmap_Explosion_Large_2, Role_Plane.x - 2, Role_Plane.y - 2, 16, 16);
    ML_display_vram();
    Sleep(1500);
}