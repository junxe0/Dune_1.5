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
void cursor_move(DIRECTION dir, int c_click);
void add_b_harvester(void);
void remove_b_harvester(int index);
void remove_r_harvester(int index);
double calculate_distance(POSITION a, POSITION b);
POSITION find_closest_harvester(POSITION sandworm_pos);
void create_spice(POSITION pos);
void sandworm_move(void);
void sandworm2_move(void);
POSITION sandworm_next_position(void);
POSITION sandworm2_next_position(void);
void add_b_plate(void);
void remove_b_plate(void);
void add_barracks(void);
void add_dormitory(void);
void add_garage(void);
void add_shelter(void);

/* ================= control =================== */
int sys_clock = 0;		// system-wide clock(ms)
CURSOR cursor = {
	{ { 1, 1 } },
	{ { 1, 1 } },
	1
};

/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };
char ob_info[OB_INFO_HEIGHT][OB_INFO_WIDTH] = { 0 };
char system_message[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH] = { 0 };
char command[COMMAND_HEIGHT][COMMAND_WIDTH] = { 0 };
// 본진
int bbase_pos[][2] = {
	{1, 16},
	{2, 16},
	{1, 15},
	{2, 15}
};
int rbase_pos[][2] = {
	{57, 1},
	{58, 2},
	{57, 2},
	{58, 1}
};
// 장판
int bplate_pos[][2] = {
	{3, 16},
	{4, 16},
	{3, 15},
	{4, 15}
};
int rplate_pos[][2] = {
	{55, 1},
	{56, 2},
	{55, 2},
	{56, 1}
};
int bplate_count = 1;
int rplate_count = 1;
// 바위
int rock_pos[][2] = {
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
// 스파이스
int spice_pos[][2] = {
	{1, 12},
	{58, 5}
};
// 하베스터
int bharvestor_pos[MAX_HARVESTERS][2] = {
	{1, 14}
};
int rharvestor_pos[MAX_HARVESTERS][2] = {
	{58, 3}
};
int bharvestor_count = 1;
int rharvestor_count = 1;
// 샌드웜
int sandworm1_r = 0;
int sandworm1_r_avoid = 0;
int sandworm2_r = 0;
int sandworm2_r_avoid = 0;
// 숙소
int dormitory_pos[][2] = { 0 };
int dormitory_count = 0;
// 창고
int garage_pos[][2] = { 0 };
int garage_count = 0;
// 병영
int barracks_pos[][2] = { 0 };
int barracks_count = 0;
// 보병
int soldier_count = 0;
// 은신처
int shelter_pos[][2] = { 0 };
int shelter_count = 0;
// 프레멘
int fremen_count = 0;
// 명령어
int command_key = 0;
int build_key = 0;
int unit_key = 0;

/* ================= 구조체 =================== */
RESOURCE resource = { 
	// 스파이스
	.spice = 0,
	.spice_max = 0,
	// 인구
	.population = 0,
	.population_max = 0
};

SANDWORM sandworm1 = {
	// 위치 좌표
	.pos = {4, 3},
	// 목적지 좌표
	.dest = {MAP_HEIGHT - 2, MAP_WIDTH - 2},
	 // 문자
	.repr = 'W',
	// 이동
	.speed = 2500,
	.next_move_time = 2500,
	// 공격
	.next_attack_time = 10000,
	.last_attack_time = 0,
	// 배설
	.next_defecation_time = 0,
	.last_defecation_time = 0
};

SANDWORM sandworm2 = {
	// 위치 좌표
	.pos = {12, 50},
	// 목적지 좌표 
	.dest = {MAP_HEIGHT - 2, MAP_WIDTH - 2},
	// 문자
	.repr = 'W',
	// 이동
	.speed = 2500,
	.next_move_time = 2500,
	// 공격
	.next_attack_time = 10000,
	.last_attack_time = 0,
	// 배설
	.next_defecation_time = 0,
	.last_defecation_time = 0
};

/* ================= main() =================== */
int main(void) {
	srand((unsigned int)time(NULL));
	set_color(15);

	init();
	intro();
	display(resource, map, cursor, ob_info, system_message, command);

	KEY press_key = 0;
	int press_count = 0;
	int last_sys_clock = 0;

	command_print("명령어 : B ( Build ), U ( Unit List )", 1);

	while (1) {
		// loop 돌 때마다(즉, TICK==10ms마다) 키 입력 확인
		KEY key = get_key();

		if (is_arrow_key(key)) {
			if (build_key >= 2) { cursor.cursor_size = 2; }
			else { cursor.cursor_size = 1; }
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
				cursor_move(ktod(key), 2);
				press_key = 0;
				press_count = 0;
			}
			else { cursor_move(ktod(key), 1); }
		}
		else if (key == k_space) {
			if (2 <= build_key) {
				if (build_key == 2) { add_barracks(); }
				else if (build_key == 3) { add_b_plate(); }
				else if (build_key == 4) { add_dormitory(); }
				else if (build_key == 5) { add_garage(); }
				else if (build_key == 6) { add_shelter(); }
				command_print("명령어 : B ( Build ), U ( Unit List )", 1);
			}
			else if (build_key < 1 && unit_key < 1) { get_info(cursor.current[0]); }
			build_key = 0;
		}
		else if (key == k_h) {
			if (command_key == 2) {
				sys_msg_print("수확하기 8번에서 추가 ( 미구현 )");
			}
			else if (command_key == 3) {
				POSITION pos = { 14, 1 };
				if (5 <= resource.spice) {
					if (map[1][pos.row][pos.column] == 'X') { sys_msg_print("이미 해당 자리에 하베스터가 있습니다."); }
					else {
						sys_msg_print("새로운 하베스터가 준비되었습니다.");
						resource.spice -= 5;
						add_b_harvester();
					}
				}
				else { sys_msg_print("스파이스가 충분하지 않습니다."); }
			}
			command_key = 0;
		}
		else if (key == k_m) {
			if (command_key == 2) {
				sys_msg_print("이동하기 8번에서 추가 ( 미구현 )");
			}
			command_key = 0;
		}
		else if (key == k_b) {
			clear_info();
			clear_command();
			if (command_key == 0 && build_key == 0 && unit_key == 0) {
				build_key = 1;
				sys_msg_print("건설 가능한 건물 목록을 불러옵니다.");
				command_print("건설 가능한 건물 목록", 1);
				command_print("장판 : P, 숙소 : D, 창고 : G", 2);
				command_print("병영 : B, 은신처 : S", 3);
			}
			else if (command_key == 0 && build_key == 1) {
				build_key = 2;
				sys_msg_print("병영 건설을 선택하셨습니다.");
			}
		}
		else if (key == k_p) {
			if (command_key == 0 && build_key == 1) {
				build_key = 3;
				clear_command();
				sys_msg_print("장판 건설을 선택하셨습니다.");
			}
		}
		else if (key == k_d) {
			if (command_key == 0 && build_key == 1) {
				build_key = 4;
				clear_command();
				sys_msg_print("숙소 건설을 선택하셨습니다.");
			}
		}
		else if (key == k_g) {
			if (command_key == 0 && build_key == 1) {
				build_key = 5;
				clear_command();
				sys_msg_print("창고 건설을 선택하셨습니다.");
			}
		}
		else if (key == k_s) {
			if (command_key == 0 && build_key == 1) {
				build_key = 6;
				clear_command();
				sys_msg_print("은신처 건설을 선택하셨습니다.");
			}
		}
		else if (key == k_u) {
			if (command_key == 0 && build_key == 0) {
				unit_key = 1;
				clear_command();
				sys_msg_print("유닛 목록을 불러옵니다.");
				info_print("유닛 목록", 1);
				info_unit_list("하베스터 : ", 2, bharvestor_count);
				info_unit_list("보병 : ", 3, soldier_count);
				info_unit_list("프레멘 : ", 4, fremen_count);
			}
		}
		else if (key == k_esc) {
			clear_info();
			clear_command();
			command_print("명령어 : B ( Build ), U ( Unit List )", 1);
			if (command_key == 1) { sys_msg_print("오브젝트 선택이 취소되었습니다."); }
			else if (build_key == 2) { sys_msg_print("병영 건설을 취소하셨습니다."); }
			else if (build_key == 3) { sys_msg_print("장판 건설을 취소하셨습니다."); }
			else if (build_key == 4) { sys_msg_print("숙소 건설을 취소하셨습니다."); }
			else if (build_key == 5) { sys_msg_print("창고 건설을 취소하셨습니다."); }
			else if (build_key == 6) { sys_msg_print("은신처 건설을 취소하셨습니다."); }
			command_key = 0, build_key = 0, unit_key = 0;
		}
		else {
			switch (key) {
			case k_quit:
				outro();
				break;
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

/* ================= UI 메세지 =================== */
void get_info(POSITION pos) {
	char ch0 = map[0][pos.row][pos.column];
	char ch1 = map[1][pos.row][pos.column];
	clear_info();
	clear_command();
	command_key = 1;
	switch (ch0) {
	case ' ':
		switch (ch1) {
		case 'W':
			sys_msg_print("샌드웜을 선택하셨습니다.");
			info_print("선택된 오브젝트 : 샌드웜 ( 중립 )", 1);
			info_print("생산 비용 : 없음", 2);
			info_print("인구수 : 없음", 3);
			info_print("이동 주기 : 2500", 4);
			info_print("공격력 : 무한대", 5);
			info_print("공격 주기 : 10000", 6);
			info_print("체력 : 무한대", 7);
			info_print("시야 : 무한대", 8);
			command_print("명령어 : 없음", 1);
			break;
		case 'X':
		case 'Y':
			sys_msg_print("하베스터를 선택하셨습니다.");
			info_print("선택된 오브젝트 : 하베스터", 1);
			info_print("생산 비용 : 5", 2);
			info_print("인구수 : 5", 3);
			info_print("이동 주기 : 2000", 4);
			info_print("공격력 : 없음", 5);
			info_print("공격 주기 : 없음", 6);
			info_print("체력 : 70", 7);
			info_print("시야 : 0", 8);
			switch (ch1) {
			case 'X':
				command_print("명령어 : H ( Harvest ), M ( Move )", 1);
				command_key = 2;
			default:
				break;
			}
			break;
		default:
			sys_msg_print("사막지형을 선택하셨습니다.");
			info_print("선택된 오브젝트 : 사막 지형", 1);
			break;
		}
		break;
	case 'A':
	case 'B':
		sys_msg_print("본진을 선택하셨습니다.");
		info_print("선택된 오브젝트 : 본진", 1);
		info_print("건설 비용 : 없음", 2);
		info_print("내구도 : 50", 3);
		switch (ch0) {
		case 'A':
			command_print("명령어 : H ( 하베스터 생산 )", 1);
			command_key = 3;
		default:
			break;
		}
		break;
	case 'P':
		sys_msg_print("장판을 선택하셨습니다.");
		info_print("선택된 오브젝트 : 장판", 1);
		info_print("설명 : 건물 짓기 전에 깔기", 2);
		info_print("건설 비용 : 1", 3);
		info_print("내구도 : 없음", 4);
		break;
	case 'R':
		sys_msg_print("바위를 선택하셨습니다.");
		info_print("선택된 오브젝트 : 바위", 1);
		break;
	case '5':
	case '4':
	case '3':
	case '2':
	case '1':
		sys_msg_print("스파이스를 선택하셨습니다.");
		info_print("선택된 오브젝트 : 스파이스", 1);
		switch (ch0) {
		case '4':
			info_print("매장량 : 4", 2);
			break;
		case '3':
			info_print("매장량 : 3", 2);
			break;
		case '2':
			info_print("매장량 : 2", 2);
			break;
		case '1':
			info_print("매장량 : 1", 2);
			break;
		default:
			info_print("매장량 : 5", 2);
			break;
		}
		break;
	case 'D':
		sys_msg_print("숙소를 선택하셨습니다.");
		info_print("선택된 오브젝트 : 숙소", 1);
		info_print("설명 : 인구 최대치 증가 ( 10 )", 2);
		info_print("건설 비용 : 2", 3);
		info_print("내구도 : 10", 4);
		command_print("명령어 : 없음", 1);
		break;
	case 'G':
		sys_msg_print("창고를 선택하셨습니다.");
		info_print("선택된 오브젝트 : 창고", 1);
		info_print("설명 : 스파이스 보관 최대치 증가 ( 10 )", 2);
		info_print("건설 비용 : 4", 3);
		info_print("내구도 : 10", 4);
		command_print("명령어 : 없음", 1);
		break;
	case 'C':
		sys_msg_print("병영을 선택하셨습니다.");
		info_print("선택된 오브젝트 : 병영", 1);
		info_print("설명 : 보병 생산", 2);
		info_print("건설 비용 : 4", 3);
		info_print("내구도 : 20", 4);
		command_print("명령어 : S ( 보병 생산 )", 1);
		break;
	case 'S':
		sys_msg_print("은신처를 선택하셨습니다.");
		info_print("선택된 오브젝트 : 은신처", 1);
		info_print("설명 : 특수 유닛 생산", 2);
		info_print("건설 비용 : 5", 3);
		info_print("내구도 : 30", 4);
		command_print("명령어 : F ( 프레멘 생산 )", 1);
		break;
	default:
		break;
	}
}

/* ================= 청소 =================== */
void clear_info(void) {
	for (int i = 1; i <= 8; i++) {
		info_print("                                                ", i);
	}
}

void clear_command(void) {
	for (int i = 1; i <= 3; i++) {
		command_print("                                              ", i);
	}
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
		int loading_ms = rand() % 10 + 30; // 원래 701 + 300
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
	set_color(15);
	for (int i = 0; i < 10; i++) {
		system("cls");
		int color = rand() % 15 + 1;
		set_color(color);
		for (int j = 0; j < 8; j++) {
			POSITION End_pos = { 1 + j, 24 };
			gotoxy(End_pos);
			if (j == 0) {
				printf("   ********                                  ********               **\n");
			}
			else if (j == 1) {
				printf("  **//////**                                /**/////               /**\n");
			}
			else if (j == 2) {
				printf(" **      //   ******   **********   *****   /**       *******      /**\n");
			}
			else if (j == 3) {
				printf("/**          //////** //**//**//** **///**  /******* //**///**  ******\n");
			}
			else if (j == 4) {
				printf("/**    *****  *******  /** /** /**/*******  /**////   /**  /** **///**\n");
			}
			else if (j == 5) {
				printf("//**  ////** **////**  /** /** /**/**////   /**       /**  /**/**  /**\n");
			}
			else if (j == 6) {
				printf(" //******** //******** *** /** /**//******  /******** ***  /**//******\n");
			}
			else if (j == 7) {
				printf("  ////////   //////// ///  //  //  //////   //////// ///   //  ////// \n");
			}
		}
		for (int j = 0; j < 8; j++) {
			POSITION End_pos = { 11 + j, 33 };
			gotoxy(End_pos);
			if (j == 0) {
				printf("  ********                   **    **                 \n");
			}
			else if (j == 1) {
				printf(" **//////                   //**  **                  \n");
			}
			else if (j == 2) {
				printf("/**         *****   *****    //****    ******  **   **\n");
			}
			else if (j == 3) {
				printf("/********* **///** **///**    //**    **////**/**  /**\n");
			}
			else if (j == 4) {
				printf("////////**/*******/*******     /**   /**   /**/**  /**\n");
			}
			else if (j == 5) {
				printf("       /**/**//// /**////      /**   /**   /**/**  /**\n");
			}
			else if (j == 6) {
				printf(" ******** //******//******     /**   //****** //******\n");
			}
			else if (j == 7) {
				printf("////////   //////  //////      //     //////   ////// \n");
			}
		}
		for (int j = 0; j < 8; j++) {
			POSITION End_pos = { 21 + j, 24 };
			gotoxy(End_pos);
			if (j == 0) {
				printf(" ****     **                   **     ********** **                    \n");
			}
			else if (j == 1) {
				printf("/**/**   /**                  /**    /////**/// //                     \n");
			}
			else if (j == 2) {
				printf("/**//**  /**  *****  **   ** ******      /**     ** **********   ***** \n");
			}
			else if (j == 3) {
				printf("/** //** /** **///**//** ** ///**/       /**    /**//**//**//** **///**\n");
			}
			else if (j == 4) {
				printf("/**  //**/**/******* //***    /**        /**    /** /** /** /**/*******\n");
			}
			else if (j == 5) {
				printf("/**   //****/**////   **/**   /**        /**    /** /** /** /**/**//// \n");
			}
			else if (j == 6) {
				printf("/**    //***//****** ** //**  //**       /**    /** *** /** /**//******\n");
			}
			else if (j == 7) {
				printf("//      ///  ////// //   //    //        //     // ///  //  //  ////// \n");
			}
		}
		Sleep(500);
	}
	set_color(15);
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
	for (int k = 0; k < sizeof(bbase_pos) / sizeof(bbase_pos[0]); k++) {
		int j = bbase_pos[k][0];
		int i = bbase_pos[k][1];
		map[0][i][j] = 'A';
	}
	// 적군 본진
	for (int k = 0; k < sizeof(rbase_pos) / sizeof(rbase_pos[0]); k++) {
		int j = rbase_pos[k][0];
		int i = rbase_pos[k][1];
		map[0][i][j] = 'B';
	}
	// 아군 장판
	for (int k = 0; k < bplate_count * 4; k++) {
		int j = bplate_pos[k][0];
		int i = bplate_pos[k][1];
		map[0][i][j] = 'P';
	}
	// 적군 장판
	for (int k = 0; k < rplate_count * 4; k++) {
		int j = rplate_pos[k][0];
		int i = rplate_pos[k][1];
		map[0][i][j] = 'P';
	}
	// 아군 숙소
	for (int k = 0; k < dormitory_count; k++) {
		int j = dormitory_pos[k][0];
		int i = dormitory_pos[k][1];
		map[0][i][j] = 'D';
	}
	// 아군 창고
	for (int k = 0; k < garage_count; k++) {
		int j = garage_pos[k][0];
		int i = garage_pos[k][1];
		map[0][i][j] = 'G';
	}
	// 아군 병영
	for (int k = 0; k < barracks_count; k++) {
		int j = barracks_pos[k][0];
		int i = barracks_pos[k][1];
		map[0][i][j] = 'C';
	}
	// 아군 은신처
	for (int k = 0; k < shelter_count; k++) {
		int j = shelter_pos[k][0];
		int i = shelter_pos[k][1];
		map[0][i][j] = 'S';
	}
	// 바위
	for (int k = 0; k < sizeof(rock_pos) / sizeof(rock_pos[0]); k++) {
		int j = rock_pos[k][0];
		int i = rock_pos[k][1];
		map[0][i][j] = 'R';
	}
	// 스파이시
	for (int k = 0; k < sizeof(spice_pos) / sizeof(spice_pos[0]); k++) {
		int j = spice_pos[k][0];
		int i = spice_pos[k][1];
		map[0][i][j] = '5';
	}

	// layer 1(map[1])은 비워 두기(-1로 채움)
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[1][i][j] = -1;
		}
	}
	// 아군 하베스터
	for (int k = 0; k < bharvestor_count; k++) {
		int j = bharvestor_pos[k][0];
		int i = bharvestor_pos[k][1];
		map[1][i][j] = 'X';
	}
	// 적군 하베스터
	for (int k = 0; k < rharvestor_count; k++) {
		int j = rharvestor_pos[k][0];
		int i = rharvestor_pos[k][1];
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

/* ================= 커서 =================== */
void cursor_move(DIRECTION dir, int c_count) {
	POSITION curr = cursor.current[0]; // 커서의 왼쪽 위 좌표
	POSITION new_pos = c_count == 1 ? pmove(curr, dir) : pmove2(curr, dir);

	// 1x1 커서일 경우
	if (cursor.cursor_size == 1) {
		// 유효성 검사
		if (new_pos.row >= 1 && new_pos.row <= MAP_HEIGHT - 2 &&
			new_pos.column >= 1 && new_pos.column <= MAP_WIDTH - 2) {
			cursor.previous[0] = curr; // 이전 위치 저장
			cursor.current[0] = new_pos; // 현재 위치 업데이트
		}
	}
	// 2x2 커서일 경우
	else if (cursor.cursor_size == 2) {
		POSITION new_pos2 = { new_pos.row + 1, new_pos.column + 1 }; // 2x2 커서의 오른쪽 아래 좌표

		// 2x2 커서가 모두 유효한지 체크
		if (new_pos.row >= 1 && new_pos.row <= MAP_HEIGHT - 2 &&
			new_pos.column >= 1 && new_pos.column <= MAP_WIDTH - 2 &&
			new_pos2.row >= 1 && new_pos2.row <= MAP_HEIGHT - 2 &&
			new_pos2.column >= 1 && new_pos2.column <= MAP_WIDTH - 2) {

			// 이전 위치 저장
			for (int i = 0; i < 4; i++) {
				cursor.previous[i] = cursor.current[i];
			}

			// 현재 위치 업데이트 (2x2 커서)
			cursor.current[0] = new_pos; // 왼쪽 위
			cursor.current[1] = (POSITION){ new_pos.row, new_pos.column + 1 }; // 오른쪽 위
			cursor.current[2] = (POSITION){ new_pos.row + 1, new_pos.column }; // 왼쪽 아래
			cursor.current[3] = new_pos2; // 오른쪽 아래
		}
	}
}

/* ================= 하베스터 =================== */
void add_b_harvester(void) {
	bharvestor_count++; // 하베스터 수 증가

	bharvestor_pos[bharvestor_count - 1][0] = 1;
	bharvestor_pos[bharvestor_count - 1][1] = 14;

	POSITION pos = { 14, 1 };
	map[1][pos.row][pos.column] = 'X';
}

void remove_b_harvester(int index) {
	if (index < 0 || index >= bharvestor_count) { return; }

	// 하베스터 배열에서 해당 인덱스의 하베스터를 삭제
	for (int i = index; i < bharvestor_count; i++) {
		bharvestor_pos[i][0] = bharvestor_pos[i + 1][0];
		bharvestor_pos[i][1] = bharvestor_pos[i + 1][1];
	}

	bharvestor_count--; // 하베스터 수 감소

	// 마지막 요소를 초기화 (선택 사항)
	bharvestor_pos[bharvestor_count][0] = -1;
	bharvestor_pos[bharvestor_count][1] = -1;
}

void remove_r_harvester(int index) {
	if (index < 0 || index >= rharvestor_count) { return; }

	// 하베스터 배열에서 해당 인덱스의 하베스터를 삭제
	for (int i = index; i < rharvestor_count; i++) {
		rharvestor_pos[i][0] = rharvestor_pos[i + 1][0];
		rharvestor_pos[i][1] = rharvestor_pos[i + 1][1];
	}

	rharvestor_count--; // 하베스터 수 감소

	// 마지막 요소를 초기화 (선택 사항)
	rharvestor_pos[rharvestor_count][0] = -1;
	rharvestor_pos[rharvestor_count][1] = -1;
}

/* ================= 샌드웜 =================== */
double calculate_distance(POSITION a, POSITION b) {
	return sqrt(pow(a.row - b.row, 2) + pow(a.column - b.column, 2));
}

POSITION find_closest_harvester(POSITION sandworm_pos) {
	double min_distance = INFINITY;
	POSITION closest_harvester = { -1, -1 };

	// 아군 하베스터 찾기
	for (int i = 0; i < bharvestor_count; i++) {
		POSITION harvesterock_pos = { bharvestor_pos[i][1], bharvestor_pos[i][0] };
		double distance = calculate_distance(sandworm_pos, harvesterock_pos);

		if (distance < min_distance) {
			min_distance = distance;
			closest_harvester = harvesterock_pos;
		}
	}

	// 적군 하베스터 찾기
	for (int i = 0; i < rharvestor_count; i++) {
		POSITION harvesterock_pos = { rharvestor_pos[i][1], rharvestor_pos[i][0] };
		double distance = calculate_distance(sandworm_pos, harvesterock_pos);

		if (distance < min_distance) {
			min_distance = distance;
			closest_harvester = harvesterock_pos;
		}
	}

	return closest_harvester;
}

void create_spice(POSITION pos) {
	// 스파이스 매장지를 비어 있는 곳에 생성
	if (map[0][pos.row][pos.column] == ' ' || map[0][pos.row][pos.column] == -1) {
		int spice_amount = rand() % 5 + 1; // 1에서 5 사이의 랜덤한 매장량
		map[0][pos.row][pos.column] = '0' + spice_amount; // 스파이스 표시
	}
}

POSITION sandworm_next_position(void) {
	POSITION diff = psub(sandworm1.dest, sandworm1.pos);
	DIRECTION dir;

	// 목적지 도착 처리
	if (diff.row == 0 && diff.column == 0) {
        // 도착 시 가장 가까운 하베스터를 다시 찾음
        sandworm1.dest = find_closest_harvester(sandworm1.pos);
        // 새로운 목적지 찾기가 실패할 경우 현재 위치를 반환
        if (sandworm1.dest.row == -1 && sandworm1.dest.column == -1) {
            return sandworm1.pos; // 이동하지 않음
        }
        diff = psub(sandworm1.dest, sandworm1.pos); // 새로운 목적지와의 차이를 다시 계산
    }

	// 이동 방향 결정
	if (abs(diff.row) >= abs(diff.column)) {
		if (sandworm1_r == 1 && sandworm1_r_avoid == d_up && diff.row >= 0) { dir = d_right; }
		else if (sandworm1_r == 1 && sandworm1_r_avoid == d_down && diff.row >= 0) { dir = d_left; }
		else if (sandworm1_r == 1 && sandworm1_r_avoid == d_up && diff.row < 0) { dir = d_left; }
		else if (sandworm1_r == 1 && sandworm1_r_avoid == d_down && diff.row < 0) { dir = d_right; }
		else { dir = (diff.row >= 0) ? d_down : d_up; }
	}
	else {
		if (sandworm1_r == 1 && sandworm1_r_avoid == d_right && diff.column >= 0) { dir = d_up; }
		else if (sandworm1_r == 1 && sandworm1_r_avoid == d_left && diff.column >= 0) { dir = d_down; }
		else if (sandworm1_r == 1 && sandworm1_r_avoid == d_left && diff.column < 0) { dir = d_down; }
		else if (sandworm1_r == 1 && sandworm1_r_avoid == d_right && diff.column < 0) { dir = d_up; }
		else { dir = (diff.column >= 0) ? d_right : d_left; }
	}

	// 다음 위치 계산
	POSITION next_pos = pmove(sandworm1.pos, dir);
	// 바위가 있는지 확인
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 &&
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 &&
		map[0][next_pos.row][next_pos.column] != 'R') {
		sandworm1_r = 0;
		sandworm1_r_avoid = 0;
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
	int last_pos = -1;
	if (sys_clock <= sandworm1.next_move_time) {
		return; // 아직 이동할 시간이 아님
	}
	POSITION next_pos = sandworm_next_position();
	if (sys_clock < sandworm1.last_attack_time + sandworm1.next_attack_time &&\
		(map[1][next_pos.row][next_pos.column] == 'X' || map[1][next_pos.row][next_pos.column] == 'Y')) {
		return;
	}

	// 현재 위치를 맵에서 지우기
	map[1][sandworm1.pos.row][sandworm1.pos.column] = -1; // 현재 위치 지우기

	// 가장 가까운 하베스터를 찾고 목적지 설정
	POSITION closest_harvester = find_closest_harvester(sandworm1.pos);
	if (closest_harvester.row != -1 && closest_harvester.column != -1) {
		sandworm1.dest = closest_harvester; // 새로운 목적지 업데이트
		if (map[1][next_pos.row][next_pos.column] == 'X') {
			// 아군 하베스터를 잡아먹음
			map[1][next_pos.row][next_pos.column] = -1; // 맵에서도 삭제

			// 하베스터 좌표 배열에서 해당 하베스터 삭제
			for (int i = 0; i < bharvestor_count; i++) {
				// 좌표 비교
				if (bharvestor_pos[i][0] == next_pos.column && bharvestor_pos[i][1] == next_pos.row) {
					remove_b_harvester(i); // 배열에서 삭제
					break; // 삭제 후 루프 종료
				}
			}

			sys_msg_print("샌드웜이 아군 하베스터를 잡아먹었습니다.");

			sandworm1.last_attack_time = sys_clock;

			// 하베스터를 잡아먹은 후 새로운 하베스터를 다시 찾기
			closest_harvester = find_closest_harvester(sandworm1.pos);
			if (closest_harvester.row != -1 && closest_harvester.column != -1) {
				sandworm1.dest = closest_harvester; // 새로운 목적지로 업데이트
			}
		}
		else if (map[1][next_pos.row][next_pos.column] == 'Y') {
			// 적군 하베스터를 잡아먹음
			map[1][next_pos.row][next_pos.column] = -1; // 맵에서도 삭제

			// 하베스터 좌표 배열에서 해당 하베스터 삭제
			for (int i = 0; i < rharvestor_count; i++) {
				// 좌표 비교
				if (rharvestor_pos[i][0] == next_pos.column && rharvestor_pos[i][1] == next_pos.row) {
					remove_r_harvester(i); // 배열에서 삭제
					break; // 삭제 후 루프 종료
				}
			}

			sandworm1.last_attack_time = sys_clock;

			// 하베스터를 잡아먹은 후 새로운 하베스터를 다시 찾기
			closest_harvester = find_closest_harvester(sandworm1.pos);
			if (closest_harvester.row != -1 && closest_harvester.column != -1) {
				sandworm1.dest = closest_harvester; // 새로운 목적지로 업데이트
			}
		}
		sandworm1.pos = next_pos;
		map[1][sandworm1.pos.row][sandworm1.pos.column] = sandworm1.repr; // 새 위치에 'W' 출력
	}
	else {
		POSITION next_pos = { sandworm1.pos.row, sandworm1.pos.column };
		sandworm1.pos = next_pos;
		map[1][sandworm1.pos.row][sandworm1.pos.column] = sandworm1.repr; // 새 위치에 'W' 출력
	}

	// 스파이스 배설 처리
	if (sys_clock >= sandworm1.next_defecation_time) {
		POSITION build_pos = sandworm1.pos; // 현재 위치에 스파이스 생성
		if (map[0][build_pos.row][build_pos.column] == ' ' || map[0][build_pos.row][build_pos.column] == -1) {
			create_spice(build_pos); // 현재 위치에 스파이스 생성
		}
		int defecation_interval = rand() % 10001 + 5000; // 5초 ~ 15초
		sandworm1.next_defecation_time = sys_clock + defecation_interval; // 다음 배설 시간 설정
	}

	sandworm1.next_move_time = sys_clock + sandworm1.speed; // 다음 이동 시간 설정
}

POSITION sandworm2_next_position(void) {
	POSITION diff = psub(sandworm2.dest, sandworm2.pos);
	DIRECTION dir;

	// 목적지 도착 처리
	if (diff.row == 0 && diff.column == 0) {
		// 도착 시 가장 가까운 하베스터를 다시 찾음
		sandworm2.dest = find_closest_harvester(sandworm2.pos);
		// 새로운 목적지 찾기가 실패할 경우 현재 위치를 반환
		if (sandworm2.dest.row == -1 && sandworm2.dest.column == -1) {
			return sandworm2.pos; // 이동하지 않음
		}
		diff = psub(sandworm2.dest, sandworm2.pos); // 새로운 목적지와의 차이를 다시 계산
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
		sandworm2_r_avoid = 0;
		return next_pos; // 바위가 없으면 유효한 위치로 이동
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
			// 대체 가능한 위치로 이동
			return direction_pos;
		}
	}

	// 만약 모든 방향에 바위가 있어도, 움직일 수 있는 방향을 찾지 못했을 경우
	return sandworm2.pos; // 이동할 수 있는 위치가 없을 경우 제자리
}

void sandworm2_move(void) {
	if (sys_clock <= sandworm2.next_move_time) {
		return; // 아직 이동할 시간이 아님
	}

	POSITION next_pos = sandworm2_next_position();
	if (sys_clock < sandworm2.last_attack_time + sandworm2.next_attack_time && \
		(map[1][next_pos.row][next_pos.column] == 'X' || map[1][next_pos.row][next_pos.column] == 'Y')) {
		return;
	}

	// 현재 위치를 맵에서 지우기
	map[1][sandworm2.pos.row][sandworm2.pos.column] = -1; // 현재 위치 지우기

	// 가장 가까운 하베스터를 찾고 목적지 설정
	POSITION closest_harvester = find_closest_harvester(sandworm2.pos);
	if (closest_harvester.row != -1 && closest_harvester.column != -1) {
		sandworm2.dest = closest_harvester; // 새로운 목적지 업데이트
		if (map[1][next_pos.row][next_pos.column] == 'X' && sandworm2.last_attack_time + sandworm2.next_attack_time <= sys_clock) {
			// 아군 하베스터를 잡아먹음
			map[1][next_pos.row][next_pos.column] = -1; // 맵에서도 삭제

			// 하베스터 좌표 배열에서 해당 하베스터 삭제
			for (int i = 0; i < bharvestor_count; i++) {
				// 좌표 비교
				if (bharvestor_pos[i][0] == next_pos.column && bharvestor_pos[i][1] == next_pos.row) {
					remove_b_harvester(i); // 배열에서 삭제
					break; // 삭제 후 루프 종료
				}
			}

			sys_msg_print("샌드웜이 아군 하베스터를 잡아먹었습니다.");

			sandworm2.last_attack_time= sys_clock;

			// 하베스터를 잡아먹은 후 새로운 하베스터를 다시 찾기
			closest_harvester = find_closest_harvester(sandworm2.pos);
			if (closest_harvester.row != -1 && closest_harvester.column != -1) {
				sandworm2.dest = closest_harvester; // 새로운 목적지로 업데이트
			}
		}
		else if (map[1][next_pos.row][next_pos.column] == 'Y' && sandworm2.last_attack_time + sandworm2.next_attack_time <= sys_clock) {
			// 적군 하베스터를 잡아먹음
			map[1][next_pos.row][next_pos.column] = -1; // 맵에서도 삭제

			// 하베스터 좌표 배열에서 해당 하베스터 삭제
			for (int i = 0; i < rharvestor_count; i++) {
				// 좌표 비교
				if (rharvestor_pos[i][0] == next_pos.column && rharvestor_pos[i][1] == next_pos.row) {
					remove_r_harvester(i); // 배열에서 삭제
					break; // 삭제 후 루프 종료
				}
			}

			sandworm2.last_attack_time = sys_clock;

			// 하베스터를 잡아먹은 후 새로운 하베스터를 다시 찾기
			closest_harvester = find_closest_harvester(sandworm2.pos);
			if (closest_harvester.row != -1 && closest_harvester.column != -1) {
				sandworm2.dest = closest_harvester; // 새로운 목적지로 업데이트
			}
		}
		sandworm2.pos = next_pos;
		map[1][sandworm2.pos.row][sandworm2.pos.column] = sandworm2.repr; // 새 위치에 'W' 출력
	}
	else {
		POSITION next_pos = { sandworm2.pos.row, sandworm2.pos.column };
		sandworm2.pos = next_pos;
		map[1][sandworm2.pos.row][sandworm2.pos.column] = sandworm2.repr; // 새 위치에 'W' 출력
	}

	// 스파이스 배설 처리
	if (sys_clock >= sandworm2.next_defecation_time) {
		POSITION build_pos = sandworm2.pos; // 현재 위치에 스파이스 생성
		if (map[0][build_pos.row][build_pos.column] == ' ' || map[0][build_pos.row][build_pos.column] == -1) {
			create_spice(build_pos); // 현재 위치에 스파이스 생성
		}
		int defecation_interval = rand() % 10001 + 5000; // 5초 ~ 15초
		sandworm2.next_defecation_time = sys_clock + defecation_interval; // 다음 배설 시간 설정
	}

	sandworm2.next_move_time = sys_clock + sandworm2.speed; // 다음 이동 시간 설정
}

/* ================= 건설 =================== */
/* ================= 장판 =================== */
void add_b_plate(void) {
	POSITION curr = cursor.current[0];

	POSITION pos1 = { curr.row, curr.column };
	POSITION pos2 = { curr.row + 1, curr.column };
	POSITION pos3 = { curr.row, curr.column + 1 };
	POSITION pos4 = { curr.row + 1, curr.column + 1 };
	if (1 <= resource.spice) {
		if (map[0][pos1.row][pos1.column] == ' ' &&
			map[0][pos2.row][pos2.column] == ' ' &&
			map[0][pos3.row][pos3.column] == ' ' &&
			map[0][pos4.row][pos4.column] == ' ') {
			bplate_count++;
			resource.spice -= 1;

			// 오른쪽 아래
			bplate_pos[(bplate_count * 4) - 4][0] = pos4.row;
			bplate_pos[(bplate_count * 4) - 4][1] = pos4.column;
			map[0][pos4.row][pos4.column] = 'P';

			// 오른쪽 위
			bplate_pos[(bplate_count * 4) - 3][0] = pos3.row;
			bplate_pos[(bplate_count * 4) - 3][1] = pos3.column;
			map[0][pos3.row][pos3.column] = 'P';

			// 왼쪽 아래
			bplate_pos[(bplate_count * 4) - 2][0] = pos2.row;
			bplate_pos[(bplate_count * 4) - 2][1] = pos2.column;
			map[0][pos2.row][pos2.column] = 'P';

			// 왼쪽 위
			bplate_pos[(bplate_count * 4) - 1][0] = pos1.row;
			bplate_pos[(bplate_count * 4) - 1][1] = pos1.column;
			map[0][pos1.row][pos1.column] = 'P';

			sys_msg_print("장판 건설이 완료되었습니다.");
		}
		else {
			sys_msg_print("해당 위치에 오브젝트가 존재합니다.");
		}
	}
	else {
		sys_msg_print("보유 중인 스파이스가 1 보다 적습니다.");
	}
	build_key = 0;
}

void remove_b_plate(void) {
	POSITION curr = cursor.current[0];

	// 커서 위치에 기반한 장판 위치 정의
	POSITION positions[4] = {
		{ curr.row, curr.column },          // pos1: 현재 위치
		{ curr.row + 1, curr.column },      // pos2: 아래
		{ curr.row, curr.column + 1 },      // pos3: 오른쪽
		{ curr.row + 1, curr.column + 1 }   // pos4: 오른쪽 아래
	};

	for (int j = 0; j < 4; j++) {
		// 각 위치에 대해 bplate_pos에서 찾고 삭제
		for (int i = 0; i < bplate_count * 4; i++) {
			// 현재 위치와 비교
			if (bplate_pos[i][0] == positions[j].row && bplate_pos[i][1] == positions[j].column) {
				// 해당 위치를 삭제하기 위해 남은 요소를 앞으로 이동
				for (int k = i; k < (bplate_count * 4) - 1; k++) {
					bplate_pos[k][0] = bplate_pos[k + 1][0];
					bplate_pos[k][1] = bplate_pos[k + 1][1];
				}
				// 마지막 요소 초기화
				bplate_pos[bplate_count * 4][0] = -1;
				bplate_pos[bplate_count * 4][1] = -1;
				break;
			}
		}
	}
	bplate_count--;
}

/* ================= 병영 =================== */
void add_barracks(void) {
	POSITION curr = cursor.current[0];
	
	POSITION pos1 = { curr.row, curr.column };
	POSITION pos2 = { curr.row + 1, curr.column };
	POSITION pos3 = { curr.row, curr.column + 1 };
	POSITION pos4 = { curr.row + 1, curr.column + 1 };

	if (4 <= resource.spice) {
		if (map[0][pos1.row][pos1.column] == 'P' &&
			map[0][pos2.row][pos2.column] == 'P' &&
			map[0][pos3.row][pos3.column] == 'P' &&
			map[0][pos4.row][pos4.column] == 'P') {
			barracks_count++;
			resource.spice -= 4;

			remove_b_plate();

			// 오른쪽 아래
			barracks_pos[(barracks_count * 4) - 4][0] = pos4.row;
			barracks_pos[(barracks_count * 4) - 4][1] = pos4.column;
			map[0][pos4.row][pos4.column] = 'C';

			// 오른쪽 위
			barracks_pos[(barracks_count * 4) - 3][0] = pos3.row;
			barracks_pos[(barracks_count * 4) - 3][1] = pos3.column;
			map[0][pos3.row][pos3.column] = 'C';

			// 왼쪽 아래
			barracks_pos[(barracks_count * 4) - 2][0] = pos2.row;
			barracks_pos[(barracks_count * 4) - 2][1] = pos2.column;
			map[0][pos2.row][pos2.column] = 'C';

			// 왼쪽 위
			barracks_pos[(barracks_count * 4) - 1][0] = pos1.row;
			barracks_pos[(barracks_count * 4) - 1][1] = pos1.column;
			map[0][pos1.row][pos1.column] = 'C';

			sys_msg_print("병영 건설이 완료되었습니다.");
		}
		else {
			sys_msg_print("해당 위치에 장판이 없습니다.");
		}
	}
	else {
		sys_msg_print("보유 중인 스파이스가 4 보다 적습니다.");
	}
	build_key = 0;
}

/* ================= 숙소 =================== */
void add_dormitory(void) {
	POSITION curr = cursor.current[0];
	
	POSITION pos1 = { curr.row, curr.column };
	POSITION pos2 = { curr.row + 1, curr.column };
	POSITION pos3 = { curr.row, curr.column + 1 };
	POSITION pos4 = { curr.row + 1, curr.column + 1 };
	if (2 <= resource.spice) {
		if (map[0][pos1.row][pos1.column] == 'P' &&
			map[0][pos2.row][pos2.column] == 'P' &&
			map[0][pos3.row][pos3.column] == 'P' &&
			map[0][pos4.row][pos4.column] == 'P') {
			dormitory_count++;
			resource.spice -= 2;
			resource.population_max += 10;

			remove_b_plate();

			// 오른쪽 아래
			dormitory_pos[(dormitory_count * 4) - 4][0] = pos4.row;
			dormitory_pos[(dormitory_count * 4) - 4][1] = pos4.column;
			map[0][pos4.row][pos4.column] = 'D';

			// 오른쪽 위
			dormitory_pos[(dormitory_count * 4) - 3][0] = pos3.row;
			dormitory_pos[(dormitory_count * 4) - 3][1] = pos3.column;
			map[0][pos3.row][pos3.column] = 'D';

			// 왼쪽 아래
			dormitory_pos[(dormitory_count * 4) - 2][0] = pos2.row;
			dormitory_pos[(dormitory_count * 4) - 2][1] = pos2.column;
			map[0][pos2.row][pos2.column] = 'D';

			// 왼쪽 위
			dormitory_pos[(dormitory_count * 4) - 1][0] = pos1.row;
			dormitory_pos[(dormitory_count * 4) - 1][1] = pos1.column;
			map[0][pos1.row][pos1.column] = 'D';

			sys_msg_print("숙소 건설이 완료되었습니다.");
		}
		else {
			sys_msg_print("해당 위치에 장판이 없습니다.");
		}
	}
	else {
		sys_msg_print("보유 중인 스파이스가 2 보다 적습니다.");
	}
	build_key = 0;
}

/* ================= 창고 =================== */
void add_garage(void) {
	POSITION curr = cursor.current[0];

	POSITION pos1 = { curr.row, curr.column };
	POSITION pos2 = { curr.row + 1, curr.column };
	POSITION pos3 = { curr.row, curr.column + 1 };
	POSITION pos4 = { curr.row + 1, curr.column + 1 };

	if (4 <= resource.spice) {
		if (map[0][pos1.row][pos1.column] == 'P' &&
			map[0][pos2.row][pos2.column] == 'P' &&
			map[0][pos3.row][pos3.column] == 'P' &&
			map[0][pos4.row][pos4.column] == 'P') {
			garage_count++;
			resource.spice -= 4;
			resource.spice_max += 10;

			remove_b_plate();

			// 오른쪽 아래
			garage_pos[(garage_count * 4) - 4][0] = pos4.row;
			garage_pos[(garage_count * 4) - 4][1] = pos4.column;
			map[0][pos4.row][pos4.column] = 'G';

			// 오른쪽 위
			garage_pos[(garage_count * 4) - 3][0] = pos3.row;
			garage_pos[(garage_count * 4) - 3][1] = pos3.column;
			map[0][pos3.row][pos3.column] = 'G';

			// 왼쪽 아래
			garage_pos[(garage_count * 4) - 2][0] = pos2.row;
			garage_pos[(garage_count * 4) - 2][1] = pos2.column;
			map[0][pos2.row][pos2.column] = 'G';

			// 왼쪽 위
			garage_pos[(garage_count * 4) - 1][0] = pos1.row;
			garage_pos[(garage_count * 4) - 1][1] = pos1.column;
			map[0][pos1.row][pos1.column] = 'G';

			sys_msg_print("창고 건설이 완료되었습니다.");
		}
		else {
			sys_msg_print("해당 위치에 장판이 없습니다.");
		}
	}
	else {
		sys_msg_print("보유 중인 스파이스가 4 보다 적습니다.");
	}
	build_key = 0;
}

/* ================= 은신처 =================== */
void add_shelter(void) {
	POSITION curr = cursor.current[0];

	POSITION pos1 = { curr.row, curr.column };
	POSITION pos2 = { curr.row + 1, curr.column };
	POSITION pos3 = { curr.row, curr.column + 1 };
	POSITION pos4 = { curr.row + 1, curr.column + 1 };

	if (5 <= resource.spice) {
		if (map[0][pos1.row][pos1.column] == 'P' &&
			map[0][pos2.row][pos2.column] == 'P' &&
			map[0][pos3.row][pos3.column] == 'P' &&
			map[0][pos4.row][pos4.column] == 'P') {
			shelter_count++;
			resource.spice -= 5;

			remove_b_plate();

			// 오른쪽 아래
			shelter_pos[(shelter_count * 4) - 4][0] = pos4.row;
			shelter_pos[(shelter_count * 4) - 4][1] = pos4.column;
			map[0][pos4.row][pos4.column] = 'S';

			// 오른쪽 위
			shelter_pos[(shelter_count * 4) - 3][0] = pos3.row;
			shelter_pos[(shelter_count * 4) - 3][1] = pos3.column;
			map[0][pos3.row][pos3.column] = 'S';

			// 왼쪽 아래
			shelter_pos[(shelter_count * 4) - 2][0] = pos2.row;
			shelter_pos[(shelter_count * 4) - 2][1] = pos2.column;
			map[0][pos2.row][pos2.column] = 'S';

			// 왼쪽 위
			shelter_pos[(shelter_count * 4) - 1][0] = pos1.row;
			shelter_pos[(shelter_count * 4) - 1][1] = pos1.column;
			map[0][pos1.row][pos1.column] = 'S';

			sys_msg_print("은신처 건설이 완료되었습니다.");
		}
		else {
			sys_msg_print("해당 위치에 장판이 없습니다.");
		}
	}
	else {
		sys_msg_print("보유 중인 스파이스가 5 보다 적습니다.");
	}
	build_key = 0;
}