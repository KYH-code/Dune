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
// 추가
const POSITION status_outline_pos = { 1, MAP_WIDTH + 1 };
const POSITION status_message_pos = { 2, MAP_WIDTH + 1 };
const POSITION system_outline_pos = { MAP_HEIGHT, 1 };
const POSITION system_message_pos = { MAP_HEIGHT, 1 };
const POSITION command_outline_pos = { MAP_HEIGHT, MAP_WIDTH + 1 };
const POSITION command_message_pos = { MAP_HEIGHT, MAP_WIDTH + 1 };

char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);
void display_system_message();
void display_object_info();
void display_commands();

void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor
	)
{
	display_resource(resource);
	display_map(map);
	display_cursor(cursor);
	display_system_message();
	display_object_info();
	display_commands();
	// ...
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
				POSITION pos = { i, j };
				char ch = backbuf[i][j];

                // background
                if (backbuf[i][j] == ' ') {
                    POSITION pos = { i, j };
                    printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT + 220);
                }
                // atreides_base
                else if (backbuf[i][j] == 'B' && i > MAP_HEIGHT - (MAP_HEIGHT / 2)) {
                    POSITION pos = { i, j };
                    printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT + 48);
                }
                // harkonnen_base
                else if (backbuf[i][j] == 'B') {
                    POSITION pos = { i, j };
                    printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT + 64);
                }
                // atreides_harvester
                else if (backbuf[i][j] == 'H' && i > MAP_HEIGHT - (MAP_HEIGHT / 2)) {
                    POSITION pos = { i, j };
                    printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT + 48);
                }
                // harkonnen_harvester
                else if (backbuf[i][j] == 'H' && i < MAP_HEIGHT - (MAP_HEIGHT / 2)) {
                    POSITION pos = { i, j };
                    printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT + 64);
                }
                // plate
                else if (backbuf[i][j] == 'P') {
                    POSITION pos = { i, j };
                    printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
                }
                // rock
                else if (backbuf[i][j] == 'R') {
                    POSITION pos = { i, j };
                    printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT + 128);
                }
                // spice
                else if (backbuf[i][j] == '5' || backbuf[i][j] == '3') {
                    POSITION pos = { i, j };
                    printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT + 32);
                }
                // sandworm
                else if (backbuf[i][j] == 'W') {
                    POSITION pos = { i, j };
                    printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT + 96);
                }
                else {
                    POSITION pos = { i, j };
                    printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
                }

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

    // background
    if (frontbuf[prev.row][prev.column] == ' ') {
        printc(padd(map_pos, prev), ch, COLOR_DEFAULT + 224);
    }
    // atreides_base
    else if (frontbuf[prev.row][prev.column] == 'B' && prev.row > MAP_HEIGHT - (MAP_HEIGHT / 2)) {
        printc(padd(map_pos, prev), ch, COLOR_DEFAULT + 15);
    }
    // harkonnen_base
    else if (frontbuf[prev.row][prev.column] == 'B') {
        printc(padd(map_pos, prev), ch, COLOR_DEFAULT + 64);
    }
    // atreides_harvester
    else if (frontbuf[prev.row][prev.column] == 'H' && prev.row > MAP_HEIGHT - (MAP_HEIGHT / 2)) {
        printc(padd(map_pos, prev), ch, COLOR_DEFAULT + 15);
    }
    // harkonnen_harvester
    else if (frontbuf[prev.row][prev.column] == 'H') {
        printc(padd(map_pos, prev), ch, COLOR_DEFAULT + 64);
    }
    // plate
    else if (frontbuf[prev.row][prev.column] == 'P') {
        printc(padd(map_pos, prev), ch, COLOR_DEFAULT);
    }
    // plate
    else if (frontbuf[prev.row][prev.column] == 'R') {
        printc(padd(map_pos, prev), ch, COLOR_DEFAULT + 128);
    }
    // spice
    else if (frontbuf[prev.row][prev.column] == '5' || frontbuf[prev.row][prev.column] == '3') {
        printc(padd(map_pos, prev), ch, COLOR_DEFAULT + 48);
    }
    // sandworm
    else if (frontbuf[prev.row][prev.column] == 'W') {
        printc(padd(map_pos, prev), ch, COLOR_DEFAULT + 96);
    }
    else {
        printc(padd(map_pos, prev), ch, COLOR_DEFAULT);
    }
    ch = frontbuf[curr.row][curr.column];
    printc(padd(map_pos, curr), ch, COLOR_CURSOR);

}

// 시스템 메세지
void display_system_message() {

}

// subfunction of draw_status()
void status_project(char src[MAP_HEIGHT][COMMAND_WIDTH], char dest[MAP_HEIGHT][COMMAND_WIDTH]) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < COMMAND_WIDTH; j++) {
            if (src[i][j] >= 0) {
                dest[i][j] = src[i][j];
            }
        }
    }
}

// 상태창
void display_object_info() {

}

// 명령창
void display_commands() {

}