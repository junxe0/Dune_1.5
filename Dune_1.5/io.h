/*
* raw(?) I/O
*/
#ifndef _IO_H_
#define _IO_H_

/* ================= �ƽ�Ű ���� =================== */
#define COLOR_RESET "\033[0m"          // ���� �ʱ�ȭ
#define TEXT_WHITE "\033[38;5;255m"    // ��� �ؽ�Ʈ
#define BACKGROUND_ORANGE "\033[48;5;208m" // ��Ȳ�� ���

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