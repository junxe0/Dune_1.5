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
void clear_info();
void cursor_move(DIRECTION dir);
void cursor_move2(DIRECTION dir);
void sample_obj_move(void);
POSITION sample_obj_next_position(void);


/* ================= control =================== */
int sys_clock = 0;		// system-wide clock(ms)
CURSOR cursor = { { 1, 1 }, { 1, 1 } };


/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };
char ob_info[OB_INFO_HEIGHT][OB_INFO_WIDTH] = { 0 };
char system_message[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH] = { 0 };
char command[COMMAND_HEIGHT][COMMAND_WIDTH] = { 0 };

RESOURCE resource = { 
	.spice = 0,
	.spice_max = 0,
	.population = 0,
	.population_max = 0
};

OBJECT_SAMPLE obj = {
	.pos = {1, 1},
	.dest = {MAP_HEIGHT - 2, MAP_WIDTH - 2},
	.repr = 'o',
	.speed = 300,
	.next_move_time = 300
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

		// 샘플 오브젝트 동작
		sample_obj_move();

		// 화면 출력
		display(resource, map, cursor, ob_info, system_message, command);
		Sleep(TICK);
		sys_clock += 10;
	}
}

void get_info(POSITION pos) {
	char terrain0 = map[0][pos.row][pos.column];
	char terrain1 = map[1][pos.row][pos.column];
	switch (terrain0) {
	case ' ':
		clear_info();
		switch (terrain1) {
		case 'W':
			print_line1("선택된 오브젝트 : 샌드웜 ( 중립 )\n");
			print_line2("생산 비용 : 없음 | 인구수 : 없음\n");
			print_line3("이동 주기 : 2500\n");
			print_line4("공격력 : 무한대\n");
			print_line5("공격 주기 : 10000\n");
			print_line6("체력 : 무한대\n");
			print_line7("시야 : 무한대\n");
			print_line8("명령어 : 없음\n");
			break;
		case 'H':
			print_line1("선택된 오브젝트 : 하베스터 ( 공통 )\n");
			print_line2("생산 비용 : 5 | 인구수 : 5\n");
			print_line3("이동 주기 : 2000\n");
			print_line4("공격력 : 없음\n");
			print_line5("공격 주기 : 없음\n");
			print_line6("체력 : 70\n");
			print_line7("시야 : 0\n");
			print_line8("명령어 : H ( Harvest ), M ( Move )\n");
			break;
		default:
			print_line1("선택된 오브젝트 : 사막 지형\n");
			break;
		}
		break;
	case 'B':
		clear_info();
		print_line1("선택된 오브젝트 : 본진\n");
		print_line2("건설 비용 : 없음\n");
		print_line3("내구도 : 50\n");
		print_line4("명령어 : H ( 하베스터 생산 )\n");
		break;
	case 'P':
		clear_info();
		print_line1("선택된 오브젝트 : 장판\n");
		print_line2("설명 : 건물 짓기 전에 깔기\n");
		print_line3("건설 비용 : 1\n");
		print_line4("내구도 : 없음\n");
		print_line5("명령어 : 없음\n");
		break;
	case 'R':
		clear_info();
		print_line1("선택된 오브젝트 : 바위\n");
		break;
	case '5':
		clear_info();
		print_line1("선택된 오브젝트 : 스파이시\n");
		break;
	default:
		clear_info();
		break;
	}
}

void clear_info() {
	print_line1("                                    \n");
	print_line2("                                    \n");
	print_line3("                                    \n");
	print_line4("                                    \n");
	print_line5("                                    \n");
	print_line6("                                    \n");
	print_line7("                                    \n");
	print_line8("                                    \n");
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
		int loading_ms = rand() % 100 + 300; // 원래 700 + 300
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
	// 각 문자 위치
	int b_pos[][2] = { 
		{1, MAP_HEIGHT - 2}, 
		{2, MAP_HEIGHT - 2}, 
		{1, MAP_HEIGHT - 3}, 
		{2, MAP_HEIGHT - 3},
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
		{3, MAP_HEIGHT - 2}, 
		{4, MAP_HEIGHT - 2}, 
		{3, MAP_HEIGHT - 3}, 
		{4, MAP_HEIGHT - 3} 
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
		{1, MAP_HEIGHT - 6}, 
		{58, 5} 
	};
	int h_pos[][2] = {
		{1, MAP_HEIGHT - 4},
		{58, 3}
	};
	int w_pos[][2] = {
		{4, 3},
		{50, MAP_HEIGHT - 6}
	};

	// 기본 설정
	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		map[0][i][0] = '#';
		map[0][i][MAP_WIDTH - 1] = '#';

		for (int j = 1; j < MAP_WIDTH - 1; j++) {
			map[0][i][j] = ' '; // 기본값을 공백으로 설정
		}
	}
	// 본진
	for (int k = 0; k < sizeof(b_pos) / sizeof(b_pos[0]); k++) {
		int j = b_pos[k][0];
		int i = b_pos[k][1];
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
	// 하베스터
	for (int k = 0; k < sizeof(h_pos) / sizeof(h_pos[0]); k++) {
		int j = h_pos[k][0];
		int i = h_pos[k][1];
		map[1][i][j] = 'H';
	}
	// 샌드웜
	for (int k = 0; k < sizeof(w_pos) / sizeof(w_pos[0]); k++) {
		int j = w_pos[k][0];
		int i = w_pos[k][1];
		map[1][i][j] = 'W';
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

	// object sample
	map[1][obj.pos.row][obj.pos.column] = 'o';
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

/* ================= sample object movement =================== */
POSITION sample_obj_next_position(void) {
	// 현재 위치와 목적지를 비교해서 이동 방향 결정	
	POSITION diff = psub(obj.dest, obj.pos);
	DIRECTION dir;

	// 목적지 도착. 지금은 단순히 원래 자리로 왕복
	if (diff.row == 0 && diff.column == 0) {
		if (obj.dest.row == 1 && obj.dest.column == 1) {
			// topleft --> bottomright로 목적지 설정
			POSITION new_dest = { MAP_HEIGHT - 2, MAP_WIDTH - 2 };
			obj.dest = new_dest;
		}
		else {
			// bottomright --> topleft로 목적지 설정
			POSITION new_dest = { 1, 1 };
			obj.dest = new_dest;
		}
		return obj.pos;
	}
	
	// 가로축, 세로축 거리를 비교해서 더 먼 쪽 축으로 이동
	if (abs(diff.row) >= abs(diff.column)) {
		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}
	
	// validation check
	// next_pos가 맵을 벗어나지 않고, (지금은 없지만)장애물에 부딪히지 않으면 다음 위치로 이동
	// 지금은 충돌 시 아무것도 안 하는데, 나중에는 장애물을 피해가거나 적과 전투를 하거나... 등등
	POSITION next_pos = pmove(obj.pos, dir);
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0) {
		
		return next_pos;
	}
	else {
		return obj.pos;  // 제자리
	}
}

void sample_obj_move(void) {
	if (sys_clock <= obj.next_move_time) {
		// 아직 시간이 안 됐음
		return;
	}

	// 오브젝트(건물, 유닛 등)은 layer1(map[1])에 저장
	map[1][obj.pos.row][obj.pos.column] = -1;
	obj.pos = sample_obj_next_position();
	map[1][obj.pos.row][obj.pos.column] = obj.repr;

	obj.next_move_time = sys_clock + obj.speed;
}