#pragma once

//
// This file contains all IDs of game assets 


#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_ENEMY1 30
#define ID_TEX_ENEMY2 40
#define ID_TEX_BACKGROUND 50


#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPA_RED	3
#define OBJECT_TYPE_COIN 4
#define OBJECT_TYPE_PLATFORM 5
#define OBJECT_TYPE_FIREBALL	7
#define OBJECT_TYPE_PLANT	8
#define OBJECT_TYPE_PIPE	9
#define OBJECT_TYPE_COLOR_BLOCK	12
#define OBJECT_TYPE_HEADBUSH	10
#define OBJECT_TYPE_KOOPA_GREEN	14
#define OBJECT_TYPE_QUESTION_BLOCK 15
#define OBJECT_TYPE_MUSHROOM	16
#define OBJECT_TYPE_PORTAL	50


#pragma region MARIO 

#define ID_SPRITE_MARIO 10000
#define ID_SPRITE_MARIO_BIG (ID_SPRITE_MARIO + 1000)
#define ID_SPRITE_MARIO_BIG_IDLE (ID_SPRITE_MARIO_BIG + 100)
#define ID_SPRITE_MARIO_BIG_IDLE_LEFT (ID_SPRITE_MARIO_BIG_IDLE + 10)
#define ID_SPRITE_MARIO_BIG_IDLE_RIGHT (ID_SPRITE_MARIO_BIG_IDLE + 20)

#define ID_SPRITE_MARIO_BIG_WALKING (ID_SPRITE_MARIO_BIG + 200)
#define ID_SPRITE_MARIO_BIG_WALKING_LEFT (ID_SPRITE_MARIO_BIG_WALKING + 10)
#define ID_SPRITE_MARIO_BIG_WALKING_RIGHT (ID_SPRITE_MARIO_BIG_WALKING + 20)
#define ID_SPRITE_MARIO_BIG_RUNNING (ID_SPRITE_MARIO_BIG + 300)
#define ID_SPRITE_MARIO_BIG_RUNNING_LEFT (ID_SPRITE_MARIO_BIG_RUNNING + 10)
#define ID_SPRITE_MARIO_BIG_RUNNING_RIGHT (ID_SPRITE_MARIO_BIG_RUNNING + 20)
#define ID_SPRITE_MARIO_BIG_JUMP (ID_SPRITE_MARIO_BIG + 400)
#define ID_SPRITE_MARIO_BIG_JUMP_WALK (ID_SPRITE_MARIO_BIG_JUMP + 10)
#define ID_SPRITE_MARIO_BIG_JUMP_WALK_LEFT (ID_SPRITE_MARIO_BIG_JUMP_WALK + 2)
#define ID_SPRITE_MARIO_BIG_JUMP_WALK_RIGHT (ID_SPRITE_MARIO_BIG_JUMP_WALK + 6)
#define ID_SPRITE_MARIO_BIG_JUMP_RUN (ID_SPRITE_MARIO_BIG_JUMP + 20)
#define ID_SPRITE_MARIO_BIG_JUMP_RUN_LEFT (ID_SPRITE_MARIO_BIG_JUMP_RUN + 2)
#define ID_SPRITE_MARIO_BIG_JUMP_RUN_RIGHT (ID_SPRITE_MARIO_BIG_JUMP_RUN + 6)
#define ID_SPRITE_MARIO_BIG_SIT (ID_SPRITE_MARIO_BIG + 500)
#define ID_SPRITE_MARIO_BIG_SIT_LEFT (ID_SPRITE_MARIO_BIG_SIT + 10)
#define ID_SPRITE_MARIO_BIG_SIT_RIGHT (ID_SPRITE_MARIO_BIG_SIT + 20)
#define ID_SPRITE_MARIO_BIG_BRACE (ID_SPRITE_MARIO_BIG + 600)
#define ID_SPRITE_MARIO_BIG_BRACE_LEFT (ID_SPRITE_MARIO_BIG_BRACE + 10)
#define ID_SPRITE_MARIO_BIG_BRACE_RIGHT (ID_SPRITE_MARIO_BIG_BRACE + 20)

#define ID_SPRITE_MARIO_DIE (ID_SPRITE_MARIO + 3000)

#define ID_SPRITE_MARIO_SMALL (ID_SPRITE_MARIO + 2000)
#define ID_SPRITE_MARIO_SMALL_IDLE (ID_SPRITE_MARIO_SMALL + 100)
#define ID_SPRITE_MARIO_SMALL_IDLE_LEFT (ID_SPRITE_MARIO_SMALL_IDLE + 10)
#define ID_SPRITE_MARIO_SMALL_IDLE_RIGHT (ID_SPRITE_MARIO_SMALL_IDLE + 20)

#define ID_SPRITE_MARIO_SMALL_WALKING (ID_SPRITE_MARIO_SMALL + 200)
#define ID_SPRITE_MARIO_SMALL_WALKING_LEFT (ID_SPRITE_MARIO_SMALL_WALKING + 10)
#define ID_SPRITE_MARIO_SMALL_WALKING_RIGHT (ID_SPRITE_MARIO_SMALL_WALKING + 20)
#define ID_SPRITE_MARIO_SMALL_RUNNING (ID_SPRITE_MARIO_SMALL + 300)
#define ID_SPRITE_MARIO_SMALL_RUNNING_LEFT (ID_SPRITE_MARIO_SMALL_RUNNING + 10)
#define ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT (ID_SPRITE_MARIO_SMALL_RUNNING + 20)
#define ID_SPRITE_MARIO_SMALL_JUMP (ID_SPRITE_MARIO_SMALL + 400)
#define ID_SPRITE_MARIO_SMALL_JUMP_WALK (ID_SPRITE_MARIO_SMALL_JUMP + 10)
#define ID_SPRITE_MARIO_SMALL_JUMP_WALK_LEFT (ID_SPRITE_MARIO_SMALL_JUMP_WALK + 2)
#define ID_SPRITE_MARIO_SMALL_JUMP_WALK_RIGHT (ID_SPRITE_MARIO_SMALL_JUMP_WALK + 6)
#define ID_SPRITE_MARIO_SMALL_JUMP_RUN (ID_SPRITE_MARIO_SMALL_JUMP + 20)
#define ID_SPRITE_MARIO_SMALL_JUMP_RUN_LEFT (ID_SPRITE_MARIO_SMALL_JUMP_RUN + 2)
#define ID_SPRITE_MARIO_SMALL_JUMP_RUN_RIGHT (ID_SPRITE_MARIO_SMALL_JUMP_RUN + 6)

#define ID_SPRITE_MARIO_SMALL_SIT (ID_SPRITE_MARIO_SMALL + 500)
#define ID_SPRITE_MARIO_SMALL_SIT_LEFT (ID_SPRITE_MARIO_SMALL_SIT + 10)
#define ID_SPRITE_MARIO_SMALL_SIT_RIGHT (ID_SPRITE_MARIO_SMALL_SIT + 20)

#define ID_SPRITE_MARIO_SMALL_BRACE (ID_SPRITE_MARIO_SMALL + 500)
#define ID_SPRITE_MARIO_SMALL_BRACE_LEFT (ID_SPRITE_MARIO_SMALL_BRACE + 10)
#define ID_SPRITE_MARIO_SMALL_BRACE_RIGHT (ID_SPRITE_MARIO_SMALL_BRACE + 20)

#pragma endregion 

#define ID_SPRITE_BRICK 20000

#define ID_SPRITE_GOOMBA 30000
#define ID_SPRITE_GOOMBA_WALK (ID_SPRITE_GOOMBA + 1000)
#define ID_SPRITE_GOOMBA_DIE (ID_SPRITE_GOOMBA + 2000)

#define ID_SPRITE_COIN 40000

#define ID_SPRITE_CLOUD 50000
#define ID_SPRITE_CLOUD_BEGIN (ID_SPRITE_CLOUD+1000)
#define ID_SPRITE_CLOUD_MIDDLE (ID_SPRITE_CLOUD+2000)
#define ID_SPRITE_CLOUD_END (ID_SPRITE_CLOUD+3000)

#define ID_SPRITE_KOOPA_RED 1000
#define ID_SPRITE_KOOPA_RED_WALK_LEFT (ID_SPRITE_KOOPA_RED+10)
#define ID_SPRITE_KOOPA_RED_WALK_RIGHT (ID_SPRITE_KOOPA_RED+15)
#define ID_SPRITE_KOOPA_RED_DIE (ID_SPRITE_KOOPA_RED+20)
#define ID_SPRITE_KOOPA_RED_SHELL_IDLE (ID_SPRITE_KOOPA_RED+30)
#define ID_SPRITE_KOOPA_RED_SHELL_MOVING (ID_SPRITE_KOOPA_RED+40)
#define ID_SPRITE_KOOPA_RED_SHELL_FLICKERING (ID_SPRITE_KOOPA_RED+50)
#define ID_SPRITE_KOOPA_RED_WING (ID_SPRITE_KOOPA_RED+60)

#define ID_SPRITE_KOOPA_GREEN 1100
#define ID_SPRITE_KOOPA_GREEN_WALK_LEFT (ID_SPRITE_KOOPA_GREEN+10)
#define ID_SPRITE_KOOPA_GREEN_WALK_RIGHT (ID_SPRITE_KOOPA_GREEN+15)
#define ID_SPRITE_KOOPA_GREEN_DIE (ID_SPRITE_KOOPA_GREEN+20)
#define ID_SPRITE_KOOPA_GREEN_SHELL_IDLE (ID_SPRITE_KOOPA_GREEN+30)
#define ID_SPRITE_KOOPA_GREEN_SHELL_MOVING (ID_SPRITE_KOOPA_GREEN+40)
#define ID_SPRITE_KOOPA_GREEN_SHELL_FLICKERING (ID_SPRITE_KOOPA_GREEN+50)
#define ID_SPRITE_KOOPA_GREEN_WING (ID_SPRITE_KOOPA_GREEN+60)

#define ID_SPRITE_COLOR_BLOCK 1200
// White blocks
#define ID_SPRITE_COLOR_BLOCK_WHITE_1 (ID_SPRITE_COLOR_BLOCK + 1)
#define ID_SPRITE_COLOR_BLOCK_WHITE_2 (ID_SPRITE_COLOR_BLOCK + 2)
#define ID_SPRITE_COLOR_BLOCK_WHITE_3 (ID_SPRITE_COLOR_BLOCK + 3)
#define ID_SPRITE_COLOR_BLOCK_WHITE_4 (ID_SPRITE_COLOR_BLOCK + 4)
#define ID_SPRITE_COLOR_BLOCK_WHITE_5 (ID_SPRITE_COLOR_BLOCK + 5)
#define ID_SPRITE_COLOR_BLOCK_WHITE_6 (ID_SPRITE_COLOR_BLOCK + 6)
#define ID_SPRITE_COLOR_BLOCK_WHITE_7 (ID_SPRITE_COLOR_BLOCK + 7)
#define ID_SPRITE_COLOR_BLOCK_WHITE_8 (ID_SPRITE_COLOR_BLOCK + 8)
#define ID_SPRITE_COLOR_BLOCK_WHITE_9 (ID_SPRITE_COLOR_BLOCK + 9)

// Green blocks
#define ID_SPRITE_COLOR_BLOCK_GREEN_1 (ID_SPRITE_COLOR_BLOCK + 10)
#define ID_SPRITE_COLOR_BLOCK_GREEN_2 (ID_SPRITE_COLOR_BLOCK + 11)
#define ID_SPRITE_COLOR_BLOCK_GREEN_3 (ID_SPRITE_COLOR_BLOCK + 12)
#define ID_SPRITE_COLOR_BLOCK_GREEN_4 (ID_SPRITE_COLOR_BLOCK + 13)
#define ID_SPRITE_COLOR_BLOCK_GREEN_5 (ID_SPRITE_COLOR_BLOCK + 14)
#define ID_SPRITE_COLOR_BLOCK_GREEN_6 (ID_SPRITE_COLOR_BLOCK + 15)
#define ID_SPRITE_COLOR_BLOCK_GREEN_7 (ID_SPRITE_COLOR_BLOCK + 16)
#define ID_SPRITE_COLOR_BLOCK_GREEN_8 (ID_SPRITE_COLOR_BLOCK + 17)
#define ID_SPRITE_COLOR_BLOCK_GREEN_9 (ID_SPRITE_COLOR_BLOCK + 18)

// Red blocks
#define ID_SPRITE_COLOR_BLOCK_RED_1   (ID_SPRITE_COLOR_BLOCK + 19)
#define ID_SPRITE_COLOR_BLOCK_RED_2   (ID_SPRITE_COLOR_BLOCK + 20)
#define ID_SPRITE_COLOR_BLOCK_RED_3   (ID_SPRITE_COLOR_BLOCK + 21)
#define ID_SPRITE_COLOR_BLOCK_RED_4   (ID_SPRITE_COLOR_BLOCK + 22)
#define ID_SPRITE_COLOR_BLOCK_RED_5   (ID_SPRITE_COLOR_BLOCK + 23)
#define ID_SPRITE_COLOR_BLOCK_RED_6   (ID_SPRITE_COLOR_BLOCK + 24)
#define ID_SPRITE_COLOR_BLOCK_RED_7   (ID_SPRITE_COLOR_BLOCK + 25)
#define ID_SPRITE_COLOR_BLOCK_RED_8   (ID_SPRITE_COLOR_BLOCK + 26)
#define ID_SPRITE_COLOR_BLOCK_RED_9   (ID_SPRITE_COLOR_BLOCK + 27)

// Blue blocks
#define ID_SPRITE_COLOR_BLOCK_BLUE_1  (ID_SPRITE_COLOR_BLOCK + 28)
#define ID_SPRITE_COLOR_BLOCK_BLUE_2  (ID_SPRITE_COLOR_BLOCK + 29)
#define ID_SPRITE_COLOR_BLOCK_BLUE_3  (ID_SPRITE_COLOR_BLOCK + 30)
#define ID_SPRITE_COLOR_BLOCK_BLUE_4  (ID_SPRITE_COLOR_BLOCK + 31)
#define ID_SPRITE_COLOR_BLOCK_BLUE_5  (ID_SPRITE_COLOR_BLOCK + 32)
#define ID_SPRITE_COLOR_BLOCK_BLUE_6  (ID_SPRITE_COLOR_BLOCK + 33)
#define ID_SPRITE_COLOR_BLOCK_BLUE_7  (ID_SPRITE_COLOR_BLOCK + 34)
#define ID_SPRITE_COLOR_BLOCK_BLUE_8  (ID_SPRITE_COLOR_BLOCK + 35)
#define ID_SPRITE_COLOR_BLOCK_BLUE_9  (ID_SPRITE_COLOR_BLOCK + 36)

#define ID_SPRITE_COLOR_BLOCK_SHADOW 1240
#define ID_SPRITE_COLOR_BLOCK_SHADOW_1 (ID_SPRITE_COLOR_BLOCK_SHADOW + 1)
#define ID_SPRITE_COLOR_BLOCK_SHADOW_2 (ID_SPRITE_COLOR_BLOCK_SHADOW + 2)
#define ID_SPRITE_COLOR_BLOCK_SHADOW_3 (ID_SPRITE_COLOR_BLOCK_SHADOW + 3)
#define ID_SPRITE_COLOR_BLOCK_SHADOW_4 (ID_SPRITE_COLOR_BLOCK_SHADOW + 4)
#define ID_SPRITE_COLOR_BLOCK_SHADOW_5 (ID_SPRITE_COLOR_BLOCK_SHADOW + 5)
