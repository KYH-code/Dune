#ifndef COMPONENT_H
#define COMPONENT_H
#include "common.h"

// �ǹ� (������ �ǹ��� ���� ������ ������ ���� ü�°� �ڿ� ���� �߰��Ǹ� ������ �ʿ䰡 �ֱ⿡ ���ݺ��� �и��ص�)
typedef struct {
	POSITION pos;
	int team; // �Ҽ� 0: �߸�, 1: �Ʊ�, 2: ����
	int layer;
	int width;
	int height;
	char title;
} STRUCTURE;

// ���� (������ �ǹ��� ���� ������ ������ ���� ü�°� �ڿ� ���� �߰��Ǹ� ������ �ʿ䰡 �ֱ⿡ ���ݺ��� �и��ص�)
typedef struct {
	POSITION pos;
	int team; // �Ҽ� 0: �߸�, 1: �Ʊ�, 2: ����
	int layer;
	int width;
	int height;
	char title;
} UNIT;

#endif