/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#include "display.h"
#include "io.h"

// 출력할 내용들의 좌상단(topleft) 좌표
const POSITION resource_pos = { 0, 0 };
const POSITION map_pos = { 1, 0 };

char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

char backbuf2[OB_INFO_HEIGHT][OB_INFO_WIDTH] = { 0 };
char frontbuf2[OB_INFO_HEIGHT][OB_INFO_WIDTH] = { 0 };

char backbuf3[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH] = { 0 };
char frontbuf3[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH] = { 0 };

char backbuf4[COMMAND_HEIGHT][COMMAND_WIDTH] = { 0 };
char frontbuf4[COMMAND_HEIGHT][COMMAND_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void project2(char src[OB_INFO_HEIGHT][OB_INFO_WIDTH], char dest[OB_INFO_HEIGHT][OB_INFO_WIDTH]);
void project3(char src[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH], char dest[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH]);
void project4(char src[COMMAND_HEIGHT][COMMAND_WIDTH], char dest[COMMAND_HEIGHT][COMMAND_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);
void display_object_info(char ob_info[OB_INFO_HEIGHT][OB_INFO_WIDTH]);
void display_system_message(char system_message[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH]);
void display_commands(char command[COMMAND_HEIGHT][COMMAND_WIDTH]);

void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], 
	CURSOR cursor,
	char ob_info[OB_INFO_HEIGHT][OB_INFO_WIDTH],
	char system_message[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH],
	char command[COMMAND_HEIGHT][COMMAND_WIDTH])
{
	display_resource(resource);
	display_map(map);
	display_cursor(cursor);
	display_object_info(ob_info);
	display_system_message(system_message);
	display_commands(command);
}

void display_resource(RESOURCE resource) {
	set_color(COLOR_RESOURCE);
	gotoxy(resource_pos);
	printf("spice = %d/%d, population=%d/%d\n",
		resource.spice, resource.spice_max,
		resource.population, resource.population_max
	);
}

// subfunction of draw_map()
void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			for (int k = 0; k < N_LAYER; k++) {
				if (src[k][i][j] >= 0) {
					dest[i][j] = src[k][i][j];
				}
			}
		}
	}
}

void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project(map, backbuf);

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (frontbuf[i][j] != backbuf[i][j]) {
				POSITION pos = {i, j};
				printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
			}
			frontbuf[i][j] = backbuf[i][j];
		}
	}
}

// frontbuf[][]에서 커서 위치의 문자를 색만 바꿔서 그대로 다시 출력
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	char ch = frontbuf[prev.row][prev.column];
	printc(padd(map_pos, prev), ch, COLOR_DEFAULT);

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR);
}


void project2(char src[OB_INFO_HEIGHT][OB_INFO_WIDTH], char dest[OB_INFO_HEIGHT][OB_INFO_WIDTH]) {
	for (int i = 0; i < OB_INFO_HEIGHT; i++) {
		for (int j = 0; j < OB_INFO_WIDTH; j++) {
			if (src[i][j] >= 0) {
				dest[i][j] = src[i][j];
			}
		}
	}
}

void display_object_info(char ob_info[OB_INFO_HEIGHT][OB_INFO_WIDTH]) {
	project2(ob_info, backbuf2);

	for (int i = 0; i < OB_INFO_HEIGHT; i++) {
		for (int j = 0; j < OB_INFO_WIDTH; j++) {
			if (frontbuf2[i][j] != backbuf2[i][j]) {
				POSITION pos = { i, j + MAP_WIDTH };
				printc(padd(map_pos, pos), backbuf2[i][j], COLOR_DEFAULT);
			}
			frontbuf2[i][j] = backbuf2[i][j];
		}
	}
}

void project3(char src[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH], char dest[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH]) {
	for (int i = 0; i < SYS_MESSAGE_HEIGHT; i++) {
		for (int j = 0; j < SYS_MESSAGE_WIDTH; j++) {
			if (src[i][j] >= 0) {
				dest[i][j] = src[i][j];
			}
		}
	}
}

void display_system_message(char system_message[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH]) {
	project3(system_message, backbuf3);

	for (int i = 0; i < SYS_MESSAGE_HEIGHT; i++) {
		for (int j = 0; j < SYS_MESSAGE_WIDTH; j++) {
			if (frontbuf3[i][j] != backbuf3[i][j]) {
				POSITION pos = { i + MAP_HEIGHT, j };
				printc(padd(map_pos, pos), backbuf3[i][j], COLOR_DEFAULT);
			}
			frontbuf3[i][j] = backbuf3[i][j];
		}
	}
}

void project4(char src[COMMAND_HEIGHT][COMMAND_WIDTH], char dest[COMMAND_HEIGHT][COMMAND_WIDTH]) {
	for (int i = 0; i < COMMAND_HEIGHT; i++) {
		for (int j = 0; j < COMMAND_WIDTH; j++) {
			if (src[i][j] >= 0) {
				dest[i][j] = src[i][j];
			}
		}
	}
}

void display_commands(char command[COMMAND_HEIGHT][COMMAND_WIDTH]) {
	project4(command, backbuf4);

	for (int i = 0; i < COMMAND_HEIGHT; i++) {
		for (int j = 0; j < COMMAND_WIDTH; j++) {
			if (frontbuf4[i][j] != backbuf4[i][j]) {
				POSITION pos = { i + MAP_HEIGHT, j + MAP_WIDTH };
				printc(padd(map_pos, pos), backbuf4[i][j], COLOR_DEFAULT);
			}
			frontbuf4[i][j] = backbuf4[i][j];
		}
	}
}