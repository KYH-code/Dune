/*
���ӿ� �ʿ��� �ǹ�, ���� ����ü ����, ���� ->  component.h, component.c �� ���� ����
����Ű ���� Ŭ��
map, status, system_message, command â ����(���� ��)
*/

#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"
#include "component.h"

void init(void);
void intro(void);
void outro(void);
void cursor_move(DIRECTION dir);
void sample_obj_move(void);
POSITION sample_obj_next_position(void);
// �߰�
void status_layout(void);
void system_layout(void);
void command_layout(void);

void setting_structure(STRUCTURE compo);
void setting_unit(UNIT unit);

/* ================= control =================== */
int sys_clock = 0;		// system-wide clock(ms)
CURSOR cursor = { { 1, 1 }, {1, 1} };


/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };
char status[MAP_HEIGHT][COMMAND_WIDTH] = { 0 };
char sys_message_map[N_LAYER][SYSTEM_HEIGHT][MAP_WIDTH] = { 0 };
char object_info_map[N_LAYER][SYSTEM_HEIGHT][COMMAND_WIDTH] = { 0 };

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

#define DOUBLE_CLICK_TIME 200  // ����Ŭ�� �� �ִ� �ð�(ms)
#define MOVE_DISTANCE_NORMAL 1
#define MOVE_DISTANCE_FAST 3

// Ű �Է� ��Ͽ� ����ü
typedef struct {
	DIRECTION last_direction;
	DWORD last_time;
} KEY_RECORD;

KEY_RECORD key_record = { d_stay, 0 };

/* ���߿� ������ ����ü���� component.c�� �ű� ���� */
// ��Ʈ���̵� ����
STRUCTURE atreides_base = {
	.pos = {MAP_HEIGHT - 3, 1},
	.layer = 0,
	.width = 2,
	.height = 2,
	.team = 1,
	.title = 'B',
};

// ���ڳ� ����
STRUCTURE harkonnen_base = {
	.pos = {1, MAP_WIDTH - 3},
	.layer = 0,
	.width = 2,
	.height = 2,
	.team = 2,
	.title = 'B',
};

// �����̽� ������1
STRUCTURE spice_area1 = {
	.pos = {6, MAP_WIDTH - 2},
	.layer = 0,
	.width = 1,
	.height = 1,
	.team = 0,
	.title = '5',
};

// �����̽� ������2
STRUCTURE spice_area2 = {
	.pos = {12, 1},
	.layer = 0,
	.width = 1,
	.height = 1,
	.team = 0,
	.title = '5',
};

// ���� 1
STRUCTURE plate1 = {
	.pos = {1, MAP_WIDTH - 5},
	.layer = 0,
	.width = 2,
	.height = 2,
	.team = 0,
	.title = 'P',
};

// ���� 2
STRUCTURE plate2 = {
	.pos = {MAP_HEIGHT - 3, 3},
	.layer = 0,
	.width = 2,
	.height = 2,
	.team = 0,
	.title = 'P',
};

// ���� 1
STRUCTURE rock1 = {
	.pos = {3, 25},
	.layer = 0,
	.width = 2,
	.height = 2,
	.team = 0,
	.title = 'R',
};

// ���� 2
STRUCTURE rock2 = {
	.pos = {6, 48},
	.layer = 0,
	.width = 1,
	.height = 1,
	.team = 0,
	.title = 'R',
};

// ���� 3
STRUCTURE rock3 = {
	.pos = {11, 5},
	.layer = 0,
	.width = 1,
	.height = 1,
	.team = 0,
	.title = 'R',
};

// ���� 4
STRUCTURE rock4 = {
	.pos = {12, 32},
	.layer = 0,
	.width = 2,
	.height = 2,
	.team = 0,
	.title = 'R',
};

// ���� 5
STRUCTURE rock5 = {
	.pos = {13, 50},
	.layer = 0,
	.width = 1,
	.height = 1,
	.team = 0,
	.title = 'R',
};

// ��Ʈ���̵� �Ϻ���Ʈ
UNIT atreides_harvester = {
	.pos = {14, 1},
	.layer = 1,
	.width = 1,
	.height = 1,
	.team = 1,
	.title = 'H',
};

// ���ڳ� �Ϻ���Ʈ
UNIT harkonnen_harvester = {
	.pos = {3, MAP_WIDTH - 2},
	.layer = 1,
	.width = 1,
	.height = 1,
	.team = 2,
	.title = 'H',
};

// ����� 1
UNIT sandworm1 = {
	.pos = {4, 17},
	.layer = 1,
	.width = 1,
	.height = 1,
	.team = 0,
	.title = 'W',
};

// ����� 2
UNIT sandworm2 = {
	.pos = {12, 45},
	.layer = 1,
	.width = 1,
	.height = 1,
	.team = 0,
	.title = 'W',
};

/* ================= main() =================== */
int main(void) {
	srand((unsigned int)time(NULL));

	init();
	status_layout();
	system_layout();
	command_layout();
	intro();
	display(resource, map, cursor);


	while (1) {
		// loop �� ������(��, TICK==10ms����) Ű �Է� Ȯ��
		KEY key = get_key();

		// Ű �Է��� ������ ó��
		if (is_arrow_key(key)) {
			cursor_move(ktod(key));
		}
		else {
			// ����Ű ���� �Է�
			switch (key) {
			case k_quit: outro();
			case k_none:
			case k_undef:
			default: break;
			}
		}

		// ���� ������Ʈ ����
		sample_obj_move();

		// ȭ�� ���
		display(resource, map, cursor);
		Sleep(TICK);
		sys_clock += 10;
	}
}

/* ================= subfunctions =================== */
void intro(void) {
	printf("DUNE 1.5\n");
	Sleep(2000);
	system("cls");
}

void outro(void) {
	printf("exiting...\n");
	exit(0);
}

void init(void) {
	// layer 0(map[0])�� ���� ����
	for (int j = 0; j < MAP_WIDTH; j++) {
		map[0][0][j] = '#';
		map[0][MAP_HEIGHT - 1][j] = '#';
	}

	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		map[0][i][0] = '#';
		map[0][i][MAP_WIDTH - 1] = '#';
		for (int j = 1; j < MAP_WIDTH - 1; j++) {
			map[0][i][j] = ' ';
		}
	}

	// layer 1(map[1])�� ��� �α�(-1�� ä��)
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[1][i][j] = -1;
		}
	}

	setting_structure(atreides_base);
	setting_structure(harkonnen_base);
	setting_structure(spice_area1);
	setting_structure(spice_area2);
	setting_structure(plate1);
	setting_structure(plate2);
	setting_structure(rock1);
	setting_structure(rock2);
	setting_structure(rock3);
	setting_structure(rock4);
	setting_structure(rock5);
	setting_unit(atreides_harvester);
	setting_unit(harkonnen_harvester);
	setting_unit(sandworm1);
	setting_unit(sandworm2);

	// object sample
	// map[1][obj.pos.row][obj.pos.column] = 'o';
}

void status_layout() {
	
}

void system_layout() {

}

void command_layout() {

}

/*
// (�����ϴٸ�) ������ �������� Ŀ�� �̵�
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
*/


void cursor_move(DIRECTION dir) {
	POSITION curr = cursor.current;
	POSITION new_pos = curr;

	// ���� �ð� ��������
	DWORD current_time = GetTickCount64();

	// �̵� �Ÿ� ���� (�⺻��: 1ĭ)
	int move_distance = MOVE_DISTANCE_NORMAL;

	// ����Ŭ�� ����
	if (key_record.last_direction == dir && (current_time - key_record.last_time <= DOUBLE_CLICK_TIME)) {
		move_distance = MOVE_DISTANCE_FAST;
	}

	// �̵� �Ÿ���ŭ Ŀ�� �̵�
	for (int i = 0; i < move_distance; i++) {
		new_pos = pmove(new_pos, dir);

		// ��ȿ ���� Ȯ��
		if (new_pos.row < 1 || new_pos.row > MAP_HEIGHT - 2 || \
			new_pos.column < 1 || new_pos.column > MAP_WIDTH - 2) {
			break;  // ������ ����� ����
		}
		cursor.previous = cursor.current;
		cursor.current = new_pos;
	}

	// Ű ��� ������Ʈ
	key_record.last_direction = dir;
	key_record.last_time = current_time;
}


/* ================= sample object movement =================== */
POSITION sample_obj_next_position(void) {
	// ���� ��ġ�� �������� ���ؼ� �̵� ���� ����	
	POSITION diff = psub(obj.dest, obj.pos);
	DIRECTION dir;

	// ������ ����. ������ �ܼ��� ���� �ڸ��� �պ�
	if (diff.row == 0 && diff.column == 0) {
		if (obj.dest.row == 1 && obj.dest.column == 1) {
			// topleft --> bottomright�� ������ ����
			POSITION new_dest = { MAP_HEIGHT - 2, MAP_WIDTH - 2 };
			obj.dest = new_dest;
		}
		else {
			// bottomright --> topleft�� ������ ����
			POSITION new_dest = { 1, 1 };
			obj.dest = new_dest;
		}
		return obj.pos;
	}

	// ������, ������ �Ÿ��� ���ؼ� �� �� �� ������ �̵�
	if (abs(diff.row) >= abs(diff.column)) {
		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}

	// validation check
	// next_pos�� ���� ����� �ʰ�, (������ ������)��ֹ��� �ε����� ������ ���� ��ġ�� �̵�
	// ������ �浹 �� �ƹ��͵� �� �ϴµ�, ���߿��� ��ֹ��� ���ذ��ų� ���� ������ �ϰų�... ���
	POSITION next_pos = pmove(obj.pos, dir);
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0) {

		return next_pos;
	}
	else {
		return obj.pos;  // ���ڸ�
	}
}

void sample_obj_move(void) {
	if (sys_clock <= obj.next_move_time) {
		// ���� �ð��� �� ����
		return;
	}

	// ������Ʈ(�ǹ�, ���� ��)�� layer1(map[1])�� ����
	map[1][obj.pos.row][obj.pos.column] = -1;
	obj.pos = sample_obj_next_position();
	map[1][obj.pos.row][obj.pos.column] = obj.repr;

	obj.next_move_time = sys_clock + obj.speed;
}

// ���� ��ġ �ʱ� ����
void setting_structure(STRUCTURE structure) {
	if (structure.height == 2 && structure.width == 2) {
		map[structure.layer][structure.pos.row][structure.pos.column] = structure.title;
		map[structure.layer][structure.pos.row + 1][structure.pos.column] = structure.title;
		map[structure.layer][structure.pos.row][structure.pos.column + 1] = structure.title;
		map[structure.layer][structure.pos.row + 1][structure.pos.column + 1] = structure.title;
	}
	else {
		map[structure.layer][structure.pos.row][structure.pos.column] = structure.title;
	}

}

// ���� ��ġ �ʱ� ����
void setting_unit(UNIT unit) {
	if (unit.height == 2 && unit.width == 2) {
		map[unit.layer][unit.pos.row][unit.pos.column] = unit.title;
		map[unit.layer][unit.pos.row + 1][unit.pos.column] = unit.title;
		map[unit.layer][unit.pos.row][unit.pos.column + 1] = unit.title;
		map[unit.layer][unit.pos.row + 1][unit.pos.column + 1] = unit.title;
	}
	else {
		map[unit.layer][unit.pos.row][unit.pos.column] = unit.title;
	}

}