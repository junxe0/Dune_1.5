/*
* raw(?) I/O
*/
#ifndef _IO_H_
#define _IO_H_

#define COLOR_RESET "\033[0m"          // 색상 초기화
#define TEXT_WHITE "\033[38;5;255m"    // 흰색 텍스트
#define BACKGROUND_ORANGE "\033[48;5;208m" // 주황색 배경

#include "common.h"

KEY get_key(void);
void printc(POSITION pos, char ch, int color);
void printo(POSITION pos, char ch);
void gotoxy(POSITION pos);
void set_color(int color);

#endif

