/*
* raw(?) I/O
*/
#ifndef _IO_H_
#define _IO_H_

#define COLOR_RESET "\033[0m"          // ���� �ʱ�ȭ
#define TEXT_WHITE "\033[38;5;255m"    // ��� �ؽ�Ʈ
#define BACKGROUND_ORANGE "\033[48;5;208m" // ��Ȳ�� ���

#include "common.h"

KEY get_key(void);
void printc(POSITION pos, char ch, int color);
void printo(POSITION pos, char ch);
void gotoxy(POSITION pos);
void set_color(int color);

#endif

