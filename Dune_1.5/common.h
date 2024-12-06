#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <conio.h>
#include <assert.h>
#include <math.h>

/* ================= system parameters =================== */
#define TICK 10		// time unit(ms)

// UI 범위
#define N_LAYER 2
#define MAP_WIDTH	60
#define MAP_HEIGHT	18
#define OB_INFO_WIDTH 50
#define OB_INFO_HEIGHT 18
#define SYS_MESSAGE_WIDTH 60
#define SYS_MESSAGE_HEIGHT 7
#define COMMAND_WIDTH 50
#define COMMAND_HEIGHT 7

// 최대 갯수 지정
#define MAX_HARVESTORS 100
#define MAX_SPICE 100

/* ================= 위치와 방향 =================== */
// 맵에서 위치를 나타내는 구조체
typedef struct {
	int row, column;
} POSITION;

// 커서 위치
typedef struct {
	POSITION previous[4];  // 직전 위치
	POSITION current[4];   // 현재 위치
	int cursor_size;
} CURSOR;

// 입력 가능한 키 종류.
// 수업에서 enum은 생략했는데, 크게 어렵지 않으니 예제 검색
typedef enum {
	// k_none: 입력된 키가 없음. d_stay(안 움직이는 경우)에 대응
	k_none = 0, k_up, k_right, k_left, k_down, 
	k_space, k_esc, k_h, k_m, k_b,
	k_p, k_d, k_g, k_s, k_u,
	k_quit,
	k_undef, // 정의되지 않은 키 입력	
} KEY;


// DIRECTION은 KEY의 부분집합이지만, 의미를 명확하게 하기 위해서 다른 타입으로 정의
typedef enum {
	d_stay = 0, d_up, d_right, d_left, d_down
} DIRECTION;


/* ================= 위치와 방향(2) =================== */
// 편의성을 위한 함수들. KEY, POSITION, DIRECTION 구조체들을 유기적으로 변환

// 편의성 함수
inline POSITION padd(POSITION p1, POSITION p2) {
	POSITION p = { p1.row + p2.row, p1.column + p2.column };
	return p;
}

// p1 - p2
inline POSITION psub(POSITION p1, POSITION p2) {
	POSITION p = { p1.row - p2.row, p1.column - p2.column };
	return p;
}

// 방향키인지 확인하는 함수
#define is_arrow_key(k)		(k_up <= (k) && (k) <= k_down)

// 화살표 '키'(KEY)를 '방향'(DIRECTION)으로 변환. 정수 값은 똑같으니 타입만 바꿔주면 됨
#define ktod(k)		(DIRECTION)(k)

// DIRECTION을 POSITION 벡터로 변환하는 함수
inline POSITION dtop(DIRECTION d) {
	static POSITION direction_vector[] = { {0, 0}, {-1, 0} /* 좌 */, {0, 1} /* 상 */, {0, -1}/* 하 */, {1, 0}/* 우 */ };
	return direction_vector[d];
}

inline POSITION dtop2(DIRECTION d) {
	static POSITION direction_vector[] = { {0, 0}, {-3, 0} /* 좌 */, {0, 3} /* 상 */, {0, -3}/* 하 */, {3, 0}/* 우 */ };
	return direction_vector[d];
}

// p를 d 방향으로 이동시킨 POSITION
#define pmove(p, d)		(padd((p), dtop(d)))
#define pmove2(p, d)	(padd((p), dtop2(d)))

/* ================= game data =================== */
typedef 
struct {
	int spice;		// 현재 보유한 스파이스
	int spice_max;  // 스파이스 최대 저장량
	int population; // 현재 인구 수
	int population_max;  // 수용 가능한 인구 수
} RESOURCE;

// 스파이스
typedef 
struct {
	int row;
	int column;
	int amount;
} SPICE;

// 샌드웜
typedef 
struct {
	// 현재 좌표
	POSITION pos;
	// 목적지 좌표
	POSITION dest;
	// 문자
	char repr;
	// 이동
	int speed;
	int next_move_time;
	// 공격
	int next_attack_time;
	int last_attack_time;
	// 배설
	int next_defecation_time;
	int last_defecation_time;
} SANDWORM;

// 하베스터
typedef 
struct {
	// 현재 좌표
	POSITION pos;
	POSITION dest;
	// 체력
	int hp;
	// 이동
	char active[100];
	int speed;
	int next_move_time;
	// 수확
	POSITION spice_pos;
	char harvest[100];
	int harvest_speed;
	int spice;
} HARVESTOR;

#endif