#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"

void init(void);
void intro(void);
void outro(void);
void get_info(POSITION pos);
void clear_info(void);
void clear_command(void);
void cursor_move(DIRECTION dir);
void cursor_move2(DIRECTION dir);
void create_spice(POSITION pos);
void remove_b_harvester(int index);
void remove_r_harvester(int index);
void sandworm_move(void);
void sandworm2_move(void);
POSITION sandworm_next_position(void);
POSITION sandworm2_next_position(void);


/* ================= control =================== */
int sys_clock = 0;		// system-wide clock(ms)
CURSOR cursor = { { 1, 1 }, { 1, 1 } };


/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };
char ob_info[OB_INFO_HEIGHT][OB_INFO_WIDTH] = { 0 };
char system_message[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH] = { 0 };
char command[COMMAND_HEIGHT][COMMAND_WIDTH] = { 0 };
int bb_pos[][2] = {
	{1, 16},
	{2, 16},
	{1, 15},
	{2, 15}
};
int rb_pos[][2] = {
	{57, 1},
	{58, 2},
	{57, 2},
	{58, 1}
};
int p_pos[][2] = {
	{55, 1},
	{56, 2},
	{55, 2},
	{56, 1},
	{3, 16},
	{4, 16},
	{3, 15},
	{4, 15}
};
int r_pos[][2] = {
	{50, 8},
	{10, 8},
	{20, 13},
	{40, 4},
	{30, 7},
	{30, 12},
	{30, 13},
	{31, 12},
	{31, 13},
	{20, 5},
	{20, 6},
	{21, 5},
	{21, 6}
};
int s_pos[][2] = {
	{1, 12},
	{58, 5}
};
int bh_pos[][2] = {
	{1, 14}
};
int rh_pos[][2] = {
	{58, 3}
};

RESOURCE resource = { 
	.spice = 0,
	.spice_max = 0,
	.population = 0,
	.population_max = 0
};

SANDWORM sandworm1 = {
	.pos = {4, 3},
	.dest = {MAP_HEIGHT - 2, MAP_WIDTH - 2},
	.repr = 'W',
	.speed = 100,
	.next_move_time = 100,
	.next_defecation_time = 0, // 초기화
	.last_defecation_time = 0   // 초기화
};

SANDWORM sandworm2 = {
	.pos = {12, 50},
	.dest = {MAP_HEIGHT - 2, MAP_WIDTH - 2},
	.repr = 'W',
	.speed = 100,
	.next_move_time = 100,
	.next_defecation_time = 0, // 초기화
	.last_defecation_time = 0   // 초기화
};

/* ================= main() =================== */
int main(void) {
	srand((unsigned int)time(NULL));

	init();
	intro();
	display(resource, map, cursor, ob_info, system_message, command);

	KEY press_key = 0;
	int press_count = 0;
	int last_sys_clock = 0;

	while (1) {
		// loop 돌 때마다(즉, TICK==10ms마다) 키 입력 확인
		KEY key = get_key();

		if (is_arrow_key(key)) {
			// 방향키 입력 처리
			if (press_key == key) {
				if (sys_clock - last_sys_clock <= 100) { press_count++; }
				else { press_count = 1; }
			}
			else {
				press_key = key;
				press_count = 1;
			}
			last_sys_clock = sys_clock;
			if (press_count == 2) {
				cursor_move2(ktod(key));
				press_key = 0;
				press_count = 0;
			}
			else { cursor_move(ktod(key)); }
		}
		else if (key == k_space) {
			get_info(cursor.current);
		}
		else if (key == k_esc) {
			clear_info();
			clear_command();
		}
		else {
			// 방향키 외의 입력
			switch (key) {
			case k_quit: outro();
			case k_none:
			case k_undef:
			default: break;
			}
		}

		// 샌드웜 동작
		sandworm_move();
		sandworm2_move();

		// 화면 출력
		display(resource, map, cursor, ob_info, system_message, command);
		Sleep(TICK);
		sys_clock += 10;
	}
}

void get_info(POSITION pos) {
	char terrain0 = map[0][pos.row][pos.column];
	char terrain1 = map[1][pos.row][pos.column];
	clear_info();
	clear_command();
	switch (terrain0) {
	case ' ':
		switch (terrain1) {
		case 'W':
			info_print_line1("선택된 오브젝트 : 샌드웜 ( 중립 )\n");
			info_print_line2("생산 비용 : 없음 | 인구수 : 없음\n");
			info_print_line3("이동 주기 : 2500\n");
			info_print_line4("공격력 : 무한대\n");
			info_print_line5("공격 주기 : 10000\n");
			info_print_line6("체력 : 무한대\n");
			info_print_line7("시야 : 무한대\n");
			command_print_line1("명령어 : 없음\n");
			break;
		case 'H':
			info_print_line1("선택된 오브젝트 : 하베스터 ( 공통 )\n");
			info_print_line2("생산 비용 : 5 | 인구수 : 5\n");
			info_print_line3("이동 주기 : 2000\n");
			info_print_line4("공격력 : 없음\n");
			info_print_line5("공격 주기 : 없음\n");
			info_print_line6("체력 : 70\n");
			info_print_line7("시야 : 0\n");
			command_print_line1("명령어 : H ( Harvest ), M ( Move )\n");
			break;
		default:
			info_print_line1("선택된 오브젝트 : 사막 지형\n");
			break;
		}
		break;
	case 'B':
		info_print_line1("선택된 오브젝트 : 본진\n");
		info_print_line2("건설 비용 : 없음\n");
		info_print_line3("내구도 : 50\n");
		command_print_line1("명령어 : H ( 하베스터 생산 )\n");
		break;
	case 'P':
		info_print_line1("선택된 오브젝트 : 장판\n");
		info_print_line2("설명 : 건물 짓기 전에 깔기\n");
		info_print_line3("건설 비용 : 1\n");
		info_print_line4("내구도 : 없음\n");
		command_print_line1("명령어 : 없음\n");
		break;
	case 'R':
		info_print_line1("선택된 오브젝트 : 바위\n");
		break;
	case '5':
		info_print_line1("선택된 오브젝트 : 스파이시\n");
		break;
	default:
		break;
	}
}

void clear_info(void) {
	info_print_line1("                                    \n");
	info_print_line2("                                    \n");
	info_print_line3("                                    \n");
	info_print_line4("                                    \n");
	info_print_line5("                                    \n");
	info_print_line6("                                    \n");
	info_print_line7("                                    \n");
	info_print_line8("                                    \n");
}

void clear_command(void) {
	command_print_line1("                                    \n");
	command_print_line2("                                    \n");
	command_print_line3("                                    \n");
}

/* ================= subfunctions =================== */
void intro(void) {
	printf("20242397 류준서\n");
	printf("DUNE 1.5\n");	
	POSITION pos = { 2, 8 };
	gotoxy(pos);
	printf("Loading...\n");
	for (int i = 3; i < 6; i++) {
		for (int j = 0; j < 24; j++) {
			if ((i != 4)) {
				POSITION pos = { i, j };
				gotoxy(pos);
				printf("#");
			}
			else if (i == 4 && (j == 0 || j == 23)) {
				POSITION pos = { i, j };
				gotoxy(pos);
				printf("#");
			}
		}
	}
	for (int j = 1; j < 22; j += 2) {
		int loading_ms = rand() % 10 + 30; // 원래 700 + 300
		POSITION pos = { 4, j };
		gotoxy(pos);
		Sleep(loading_ms);
		printf("==");
	}
	Sleep(500);
	POSITION pos2 = { 6, 0 };
	gotoxy(pos2);
	printf("Loaded Complete!\n");
	printf("GAME START!\n");
	Sleep(1000);
	system("cls");
}

void outro(void) {
	exit(0);
}

void init(void) {
	// layer 0(map[0])에 지형 생성
	for (int j = 0; j < MAP_WIDTH; j++) {
		map[0][0][j] = '#';
		map[0][MAP_HEIGHT - 1][j] = '#';
	}
	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		map[0][i][0] = '#';
		map[0][i][MAP_WIDTH - 1] = '#';

		for (int j = 1; j < MAP_WIDTH - 1; j++) {
			map[0][i][j] = ' '; // 기본값을 공백으로 설정
		}
	}
	// 아군 본진
	for (int k = 0; k < sizeof(bb_pos) / sizeof(bb_pos[0]); k++) {
		int j = bb_pos[k][0];
		int i = bb_pos[k][1];
		map[0][i][j] = 'A';
	}
	// 적군 본진
	for (int k = 0; k < sizeof(rb_pos) / sizeof(rb_pos[0]); k++) {
		int j = rb_pos[k][0];
		int i = rb_pos[k][1];
		map[0][i][j] = 'B';
	}
	// 장판
	for (int k = 0; k < sizeof(p_pos) / sizeof(p_pos[0]); k++) {
		int j = p_pos[k][0];
		int i = p_pos[k][1];
		map[0][i][j] = 'P';
	}
	// 바위
	for (int k = 0; k < sizeof(r_pos) / sizeof(r_pos[0]); k++) {
		int j = r_pos[k][0];
		int i = r_pos[k][1];
		map[0][i][j] = 'R';
	}
	// 스파이시
	for (int k = 0; k < sizeof(s_pos) / sizeof(s_pos[0]); k++) {
		int j = s_pos[k][0];
		int i = s_pos[k][1];
		map[0][i][j] = '5';
	}

	// layer 1(map[1])은 비워 두기(-1로 채움)
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[1][i][j] = -1;
		}
	}
	// 아군 하베스터
	for (int k = 0; k < sizeof(bh_pos) / sizeof(bh_pos[0]); k++) {
		int j = bh_pos[k][0];
		int i = bh_pos[k][1];
		map[1][i][j] = 'X';
	}
	// 적군 하베스터
	for (int k = 0; k < sizeof(rh_pos) / sizeof(rh_pos[0]); k++) {
		int j = rh_pos[k][0];
		int i = rh_pos[k][1];
		map[1][i][j] = 'Y';
	}

	// 상태창
	for (int j = 0; j < OB_INFO_WIDTH; j++) {
		ob_info[0][j] = '#';
		ob_info[OB_INFO_HEIGHT - 1][j] = '#';
	}
	for (int i = 1; i < OB_INFO_HEIGHT - 1; i++) {
		ob_info[i][0] = '#';
		ob_info[i][OB_INFO_WIDTH - 1] = '#';
		for (int j = 1; j < OB_INFO_WIDTH - 1; j++) {
			ob_info[i][j] = ' ';
		}
	}

	// 시스템 메시지
	for (int j = 0; j < SYS_MESSAGE_WIDTH; j++) {
		system_message[0][j] = '#';
		system_message[SYS_MESSAGE_HEIGHT - 1][j] = '#';
	}
	for (int i = 1; i < SYS_MESSAGE_HEIGHT - 1; i++) {
		system_message[i][0] = '#';
		system_message[i][SYS_MESSAGE_WIDTH - 1] = '#';
		for (int j = 1; j < SYS_MESSAGE_WIDTH - 1; j++) {
			system_message[i][j] = ' ';
		}
	}
	
	// 명령창
	for (int j = 0; j < COMMAND_WIDTH; j++) {
		command[0][j] = '#';
		command[COMMAND_HEIGHT - 1][j] = '#';
	}
	for (int i = 1; i < COMMAND_HEIGHT - 1; i++) {
		command[i][0] = '#';
		command[i][COMMAND_WIDTH - 1] = '#';
		for (int j = 1; j < COMMAND_WIDTH - 1; j++) {
			command[i][j] = ' ';
		}
	}

	// 샌드웜
	map[1][sandworm1.pos.row][sandworm1.pos.column] = sandworm1.repr;
	map[1][sandworm2.pos.row][sandworm2.pos.column] = sandworm2.repr;
}

// (가능하다면) 지정한 방향으로 커서 이동
void cursor_move(DIRECTION dir) {
	POSITION curr = cursor.current;
	POSITION new_pos = pmove(curr, dir);

	// validation check
	if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 && \
		1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2) {

		cursor.previous = cursor.current;
		cursor.current = new_pos;
	}
}

void cursor_move2(DIRECTION dir) {
	POSITION curr = cursor.current;
	POSITION new_pos = pmove2(curr, dir);

	// validation check
	if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 && \
		1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2) {

		cursor.previous = cursor.current;
		cursor.current = new_pos;
	}
}

/* ================= sandworm movement =================== */
void create_spice(POSITION pos) {
	// 스파이스 매장지를 비어 있는 곳에 생성
	if (map[0][pos.row][pos.column] == ' ' || map[0][pos.row][pos.column] == -1) {
		int spice_amount = rand() % 5 + 1; // 1에서 5 사이의 랜덤한 매장량
		map[0][pos.row][pos.column] = '0' + spice_amount; // 스파이스 표시
	}
}

void remove_b_harvester(int index) {
	// 하베스터 삭제
	for (int i = index; i < sizeof(bh_pos) / sizeof(bh_pos[0]); i++) {
		bh_pos[i][0] = bh_pos[i + 1][0];
		bh_pos[i][1] = bh_pos[i + 1][1];
	}
}

void remove_r_harvester(int index) {
	// 하베스터 삭제
	for (int i = index; i < sizeof(rh_pos) / sizeof(rh_pos[0]); i++) {
		rh_pos[i][0] = rh_pos[i + 1][0];
		rh_pos[i][1] = rh_pos[i + 1][1];
	}
}

int sandworm1_r = 0;
int sandworm1_r_avoid = 0;
int sandworm2_r = 0;
int sandworm2_r_avoid = 0;
POSITION sandworm_next_position(void) {
	POSITION diff = psub(sandworm1.dest, sandworm1.pos);
	DIRECTION dir;

	// 목적지 도착 처리
	if (diff.row == 0 && diff.column == 0) {
		if (sandworm1.dest.row == 1 && sandworm1.dest.column == 1) {
			sandworm1.dest = (POSITION){ MAP_HEIGHT - 2, MAP_WIDTH - 2 };
		}
		else {
			sandworm1.dest = (POSITION){ 1, 1 };
		}
		return sandworm1.pos; // 현재 위치 반환
	}

	// 이동 방향 결정
	if (abs(diff.row) >= abs(diff.column)) {
		if (sandworm1_r == 1 && sandworm1_r_avoid == d_up && diff.row >= 0) {
			dir = d_right;
		}
		else if (sandworm1_r == 1 && sandworm1_r_avoid == d_down && diff.row >= 0) {
			dir = d_left;
		}
		else if (sandworm1_r == 1 && sandworm1_r_avoid == d_up && diff.row < 0) {
			dir = d_left;
		}
		else if (sandworm1_r == 1 && sandworm1_r_avoid == d_down && diff.row < 0) {
			dir = d_right;
		}
		else {
			dir = (diff.row >= 0) ? d_down : d_up;
		}
	}
	else {
		if (sandworm1_r == 1 && sandworm1_r_avoid == d_right && diff.column >= 0) {
			dir = d_up;
		}
		else if (sandworm1_r == 1 && sandworm1_r_avoid == d_left && diff.column >= 0) {
			dir = d_down;
		}
		else if (sandworm1_r == 1 && sandworm1_r_avoid == d_left && diff.column < 0) {
			dir = d_down;
		}
		else if (sandworm1_r == 1 && sandworm1_r_avoid == d_right && diff.column < 0) {
			dir = d_up;
		}
		else {
			dir = (diff.column >= 0) ? d_right : d_left;
		}
	}

	// 다음 위치 계산
	POSITION next_pos = pmove(sandworm1.pos, dir);
	// 바위가 있는지 확인
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 &&
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 &&
		map[0][next_pos.row][next_pos.column] != 'R') {
		sandworm1_r = 0;
		return next_pos; // 바위가 없으면 유효한 위치로 이동
	}

	// 바위가 있는 경우 다른 방향으로 이동
	DIRECTION direction_arr[] = { d_up, d_right, d_left, d_down };
	for (int i = 0; i < 4; i++) {
		POSITION direction_pos = pmove(sandworm1.pos, direction_arr[i]);
		if (1 <= direction_pos.row && direction_pos.row <= MAP_HEIGHT - 2 &&
			1 <= direction_pos.column && direction_pos.column <= MAP_WIDTH - 2 &&
			map[0][direction_pos.row][direction_pos.column] != 'R') {
			sandworm1_r = 1;
			sandworm1_r_avoid = direction_arr[i];
			// 대체 가능한 위치로 이동
			return direction_pos;
		}
	}

	// 만약 모든 방향에 바위가 있어도, 움직일 수 있는 방향을 찾지 못했을 경우
	return sandworm1.pos; // 이동할 수 있는 위치가 없을 경우 제자리
}

void sandworm_move(void) {
	if (sys_clock <= sandworm1.next_move_time) {
		return; // 아직 이동할 시간이 아님
	}

	// 현재 위치를 맵에서 지우기
	map[1][sandworm1.pos.row][sandworm1.pos.column] = -1; // 현재 위치 지우기
	POSITION next_pos = sandworm_next_position(); // 다음 위치로 이동

	// 다음 위치에서 H가 있는지 확인
	if (map[1][next_pos.row][next_pos.column] == 'H') {
		map[1][next_pos.row][next_pos.column] = -1;

		// 하베스터 좌표 배열에서 해당 하베스터 삭제
		for (int i = 0; i < sizeof(bh_pos) / sizeof(bh_pos[0]); i++) {
			if (bh_pos[i][0] == next_pos.row && bh_pos[i][1] == next_pos.column) {
				remove_b_harvester(i);
				break;
			}
		}
		for (int i = 0; i < sizeof(rh_pos) / sizeof(rh_pos[0]); i++) {
			if (rh_pos[i][0] == next_pos.row && rh_pos[i][1] == next_pos.column) {
				remove_r_harvester(i);
				break;
			}
		}
	}

	// 샌드웜의 위치 업데이트
	sandworm1.pos = next_pos;
	map[1][sandworm1.pos.row][sandworm1.pos.column] = sandworm1.repr; // 새 위치에 'W' 출력

	// 스파이스 배설 처리
	if (sys_clock >= sandworm1.next_defecation_time) {
		POSITION build_pos = sandworm1.pos; // 현재 위치에 스파이스 생성
		if (map[0][build_pos.row][build_pos.column] == ' ' || map[0][build_pos.row][build_pos.column] == -1) {
			create_spice(build_pos); // 현재 위치에 스파이스 생성
		}
		int defecation_interval = rand() % 4000 + 1000; // 1초에서 5초 사이
		sandworm1.next_defecation_time = sys_clock + defecation_interval; // 다음 배설 시간 설정
	}

	sandworm1.next_move_time = sys_clock + sandworm1.speed; // 다음 이동 시간 설정
}

POSITION sandworm2_next_position(void) {
	POSITION diff = psub(sandworm2.dest, sandworm2.pos);
	DIRECTION dir;

	// 목적지 도착 처리
	if (diff.row == 0 && diff.column == 0) {
		if (sandworm2.dest.row == 1 && sandworm2.dest.column == 1) {
			sandworm2.dest = (POSITION){ MAP_HEIGHT - 2, MAP_WIDTH - 2 };
		}
		else {
			sandworm2.dest = (POSITION){ 1, 1 };
		}
		return sandworm2.pos; // 현재 위치 반환
	}

	// 이동 방향 결정
	if (abs(diff.row) >= abs(diff.column)) {
		if (sandworm2_r == 1 && sandworm2_r_avoid == d_up && diff.row >= 0) { dir = d_right; }
		else if (sandworm2_r == 1 && sandworm2_r_avoid == d_down && diff.row >= 0) { dir = d_left; }
		else if (sandworm2_r == 1 && sandworm2_r_avoid == d_up && diff.row < 0) { dir = d_left; }
		else if (sandworm2_r == 1 && sandworm2_r_avoid == d_down && diff.row < 0) { dir = d_right; }
		else { dir = (diff.row >= 0) ? d_down : d_up; }
	}
	else {
		if (sandworm2_r == 1 && sandworm2_r_avoid == d_right && diff.column >= 0) { dir = d_up; }
		else if (sandworm2_r == 1 && sandworm2_r_avoid == d_left && diff.column >= 0) { dir = d_down; }
		else if (sandworm2_r == 1 && sandworm2_r_avoid == d_left && diff.column < 0) { dir = d_down; }
		else if (sandworm2_r == 1 && sandworm2_r_avoid == d_right && diff.column < 0) { dir = d_up; }
		else { dir = (diff.column >= 0) ? d_right : d_left; }
	}

	// 다음 위치 계산
	POSITION next_pos = pmove(sandworm2.pos, dir);
	// 바위가 있는지 확인
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 &&
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 &&
		map[0][next_pos.row][next_pos.column] != 'R') {
		sandworm2_r = 0;
		return next_pos; // 바위가 없으면 위치로 이동
	}

	// 바위가 있는 경우 다른 방향으로 이동
	DIRECTION direction_arr[] = { d_up, d_right, d_left, d_down };
	for (int i = 0; i < 4; i++) {
		POSITION direction_pos = pmove(sandworm2.pos, direction_arr[i]);
		if (1 <= direction_pos.row && direction_pos.row <= MAP_HEIGHT - 2 &&
			1 <= direction_pos.column && direction_pos.column <= MAP_WIDTH - 2 &&
			map[0][direction_pos.row][direction_pos.column] != 'R') {
			sandworm2_r = 1;
			sandworm2_r_avoid = direction_arr[i];
			return direction_pos;
		}
	}

	return sandworm2.pos;
}

void sandworm2_move(void) {
	if (sys_clock <= sandworm2.next_move_time) {
		return; // 아직 이동할 시간이 아님
	}

	// 현재 위치를 맵에서 지우기
	map[1][sandworm2.pos.row][sandworm2.pos.column] = -1; // 현재 위치 지우기
	POSITION next_pos = sandworm2_next_position(); // 다음 위치로 이동

	// 다음 위치에서 H가 있는지 확인
	if (map[1][next_pos.row][next_pos.column] == 'H') {
		map[1][next_pos.row][next_pos.column] = -1;

		// 하베스터 좌표 배열에서 해당 하베스터 삭제
		for (int i = 0; i < sizeof(bh_pos) / sizeof(bh_pos[0]); i++) {
			if (bh_pos[i][0] == next_pos.row && bh_pos[i][1] == next_pos.column) {
				remove_b_harvester(i);
				break;
			}
		}
		for (int i = 0; i < sizeof(rh_pos) / sizeof(rh_pos[0]); i++) {
			if (rh_pos[i][0] == next_pos.row && rh_pos[i][1] == next_pos.column) {
				remove_r_harvester(i);
				break;
			}
		}
	}

	// 샌드웜의 위치 업데이트
	sandworm2.pos = next_pos;
	map[1][sandworm2.pos.row][sandworm2.pos.column] = sandworm2.repr; // 새 위치에 'W' 출력

	// 스파이스 배설 처리
	if (sys_clock >= sandworm2.next_defecation_time) {
		POSITION build_pos = sandworm2.pos; // 현재 위치에 스파이스 생성
		if (map[0][build_pos.row][build_pos.column] == ' ' || map[0][build_pos.row][build_pos.column] == -1) {
			create_spice(build_pos); // 현재 위치에 스파이스 생성
		}
		int defecation_interval = rand() % 4000 + 1000; // 1초에서 5초 사이
		sandworm2.next_defecation_time = sys_clock + defecation_interval; // 다음 배설 시간 설정
	}

	sandworm2.next_move_time = sys_clock + sandworm2.speed; // 다음 이동 시간 설정
}