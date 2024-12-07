/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

// 표시할 색상 정의. 대충 맞춰 뒀는데, 취향껏 추가하거나 변경하기
#define COLOR_DEFAULT	15
#define COLOR_CURSOR	224
#define COLOR_RESOURCE  112
#define COLOR_USER_BASE 31
#define COLOR_AI_BASE 79
#define COLOR_PLATE 15
#define COLOR_ROCK 143
#define COLOR_USER_HAVESTOR 31
#define COLOR_AI_HAVESTOR 79
#define COLOR_SANDWORM 111
#define COLOR_DORMITORY 63
#define COLOR_GARAGE 95
#define COLOR_BARRACKS 47
#define COLOR_SHELTER 191
#define COLOR_USER_SOLDIER 31
#define COLOR_USER_FREMEN 31
#define COLOR_OTHER 112

// 지금은 자원, 맵, 커서만 표시
// 앞으로 화면에 표시할 내용들 여기에 추가하기
void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor,
	char ob_info[OB_INFO_HEIGHT][OB_INFO_WIDTH],
	char system_message[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH],
	char command[COMMAND_HEIGHT][COMMAND_WIDTH]
);
#endif