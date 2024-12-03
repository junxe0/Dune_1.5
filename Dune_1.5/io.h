/*
* raw(?) I/O
*/
#ifndef _IO_H_
#define _IO_H_

/* ================= 아스키 색상 =================== */
#define COLOR_RESET "\033[0m"          // 색상 초기화
#define TEXT_WHITE "\033[38;5;255m"    // 흰색 텍스트
#define BACKGROUND_ORANGE "\033[48;5;208m" // 주황색 배경

#include "common.h"

KEY get_key(void);
void printc(POSITION pos, char ch, int color);
void printo(POSITION pos, char ch);
void info_print(char ch[100], int line);
void info_unit_list(char ch[100], int line, int num);
void sys_msg_print(char ch[100]);
void command_print(char ch[100], int line);
void gotoxy(POSITION pos);
void set_color(int color);
#endif