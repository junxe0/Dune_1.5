/*
* raw(?) I/O
*/
#include "io.h"
#include <string.h>

void gotoxy(POSITION pos) {
	COORD coord = { pos.column, pos.row }; // ��, �� �ݴ�� ����
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
	printf("%s%s%c%s\n", BACKGROUND_ORANGE, TEXT_WHITE, ch, COLOR_RESET);
}

void info_print(char ch[100], int line) {
	POSITION info_print_pos = { 2 * line, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(info_print_pos);
	printf("%s", ch);
}

char sys_msg[5][100] = { "" };
void sys_msg_print(char ch[100]) {
	for (int i = 4; i > 0; i--) {
		strcpy_s(sys_msg[i], sizeof(sys_msg[i]), sys_msg[i - 1]);
	}
	strcpy_s(sys_msg[0], sizeof(sys_msg[0]), ch);

	for (int i = 0; i < 5; i++) {
		POSITION command_print_pos = { MAP_HEIGHT + SYS_MESSAGE_HEIGHT - 1 - i, 1 };
		set_color(15);
		gotoxy(command_print_pos);
		printf("                                                   \n");
	}

	// �޽��� ���
	for (int i = 0; i < 5; i++) {
		POSITION command_print_pos = { MAP_HEIGHT + SYS_MESSAGE_HEIGHT - 1 - i, 1 }; // �Ʒ��������� ���
		set_color(15);
		gotoxy(command_print_pos);
		printf("%s\n", sys_msg[i]); // sys_msg �迭�� ������ ���
	}
}

void command_print(char ch[100], int line) {
	POSITION command_print_pos = { MAP_HEIGHT + (2 * line), MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(command_print_pos);
	printf("%s", ch);
}

KEY get_key(void) {
	if (!_kbhit()) {  // �Էµ� Ű�� �ִ��� Ȯ��
		return k_none;
	}

	int byte = _getch();    // �Էµ� Ű�� ���� �ޱ�
	switch (byte) {
	case 'q': return k_quit;  // 'q'�� ������ ����
	case 'H':
	case 'h':
		return k_h;
	case 'M':
	case 'm':
		return k_m;
	case 224:
		byte = _getch();  // MSB 224�� �Է� �Ǹ� 1����Ʈ �� ���� �ޱ�
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

void test_print_line1(char ch[100], int a, int b) {
	POSITION info_line1_pos = { 2, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(info_line1_pos);
	printf("%s (%03d, %03d)", ch, a, b);
}

void test_print_line2(char ch[100], int a, int b) {
	POSITION info_line1_pos = { 4, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(info_line1_pos);
	printf("%s (%03d, %03d)", ch, a, b);
}

void test_print_line3(int a, int b) {
	POSITION info_line1_pos = { 6, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(info_line1_pos);
	printf("sandworm1_r : %03d, sandworm1_r_avoid : %03d)", a, b);
}

void test_print_line4(int a) {
	POSITION info_line1_pos = { 8, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(info_line1_pos);
	printf("%d�� ���� �Ϸ�", a);
}

void test_print_line5(int a, int b) {
	POSITION info_line1_pos = { 10, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(info_line1_pos);
	printf("�Ϻ����� �迭 : %03d, %03d", a, b);
}

void test_print_line6(char ch[100]) {
	POSITION info_line1_pos = { 12, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(info_line1_pos);
	printf("%s", ch);
}

void test_print_line7(int a, int b) {
	POSITION info_line1_pos = { 14, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(info_line1_pos);
	printf("�Ϻ���Ʈ ��ġ : %03d, %03d", a, b);
}

void test_print_line8(int a, int b) {
	POSITION info_line1_pos = { 16, MAP_WIDTH + 1 };
	set_color(15);
	gotoxy(info_line1_pos);
	printf("W ���� ��ġ : %03d, %03d", a, b);
}