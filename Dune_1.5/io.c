/*
* raw(?) I/O
*/
#include "io.h"

void gotoxy(POSITION pos) {
	COORD coord = { pos.column, pos.row }; // 행, 열 반대로 전달
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void set_color(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printc(POSITION pos, char ch, int color) {
	if (color >= 0) {
		set_color(color);
	}
	gotoxy(pos);
	printf("%c", ch);
}

void printo(POSITION pos, char ch) {
	gotoxy(pos);
	printf("%s%s%c!%s\n", BACKGROUND_ORANGE, TEXT_WHITE, ch, COLOR_RESET);
}

void print_line1(char ch[100]) {
	POSITION line1_pos = { 2, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(line1_pos);
	printf("                                   ");
	gotoxy(line1_pos);
	printf("%s", ch);
}

void print_line2(char ch[100]) {
	POSITION line2_pos = { 4, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(line2_pos);
	printf("                                   ");
	gotoxy(line2_pos);
	printf("%s", ch);
}

void print_line3(char ch[100]) {
	POSITION line3_pos = { 6, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(line3_pos);
	printf("                                   ");
	gotoxy(line3_pos);
	printf("%s", ch);
}

void print_line4(char ch[100]) {
	POSITION line4_pos = { 8, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(line4_pos);
	printf("                                   ");
	gotoxy(line4_pos);
	printf("%s", ch);
}

void print_line5(char ch[100]) {
	POSITION line5_pos = { 10, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(line5_pos);
	printf("                                   ");
	gotoxy(line5_pos);
	printf("%s", ch);
}

void print_line6(char ch[100]) {
	POSITION line6_pos = { 12, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(line6_pos);
	printf("                                   ");
	gotoxy(line6_pos);
	printf("%s", ch);
}

void print_line7(char ch[100]) {
	POSITION line7_pos = { 14, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(line7_pos);
	printf("                                   ");
	gotoxy(line7_pos);
	printf("%s", ch);
}

void print_line8(char ch[100]) {
	POSITION line8_pos = { 16, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(line8_pos);
	printf("                                   ");
	gotoxy(line8_pos);
	printf("%s", ch);
}


KEY get_key(void) {
	if (!_kbhit()) {  // 입력된 키가 있는지 확인
		return k_none;
	}

	int byte = _getch();    // 입력된 키를 전달 받기
	switch (byte) {
	case 'q': return k_quit;  // 'q'를 누르면 종료
	case 224:
		byte = _getch();  // MSB 224가 입력 되면 1바이트 더 전달 받기
		switch (byte) {
		case 72: return k_up;
		case 75: return k_left;
		case 77: return k_right;
		case 80: return k_down;
		default: return k_undef;
		}
	case 32:
		return k_space;
	case 27:
		return k_esc;
	default: return k_undef;
	}
}
