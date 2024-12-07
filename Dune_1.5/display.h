/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

// ǥ���� ���� ����. ���� ���� �״µ�, ���ⲯ �߰��ϰų� �����ϱ�
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

// ������ �ڿ�, ��, Ŀ���� ǥ��
// ������ ȭ�鿡 ǥ���� ����� ���⿡ �߰��ϱ�
void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor,
	char ob_info[OB_INFO_HEIGHT][OB_INFO_WIDTH],
	char system_message[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH],
	char command[COMMAND_HEIGHT][COMMAND_WIDTH]
);
#endif