#ifndef COMPONENT_H
#define COMPONENT_H
#include "common.h"

// 건물 (지금은 건물과 유닛 선언이 같지만 이후 체력과 자원 등이 추가되면 구분할 필요가 있기에 지금부터 분리해둠)
typedef struct {
	POSITION pos;
	int team; // 소속 0: 중립, 1: 아군, 2: 적군
	int layer;
	int width;
	int height;
	char title;
} STRUCTURE;

// 유닛 (지금은 건물과 유닛 선언이 같지만 이후 체력과 자원 등이 추가되면 구분할 필요가 있기에 지금부터 분리해둠)
typedef struct {
	POSITION pos;
	int team; // 소속 0: 중립, 1: 아군, 2: 적군
	int layer;
	int width;
	int height;
	char title;
} UNIT;

#endif