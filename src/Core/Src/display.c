#include "display.h"
#include <stdio.h>
#include <stdbool.h>

// https://stackoverflow.com/questions/26423537/how-to-position-the-input-text-cursor-in-c
#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

#define BUFSIZE 64
char disp_buffer[BUFSIZE];

void display_packet(Packet_t *p, uint32_t max_loop_time)
{
    static bool first_time = true;

    if (first_time) {
        // fully buffered stream
        setvbuf(stdout, disp_buffer, _IOFBF, BUFSIZE);
        first_time = false;
    }

	clear();
	gotoxy(0, 0);
	printf("Temp:          %d (C)\r\n", p->acc_temp);
	printf("Acc:           [%d, %d, %d] (mg)\r\n", p->acc[0], p->acc[1], p->acc[2]);
	printf("Mag:           [%d, %d, %d] (mgauss)\r\n", p->mag[0], p->mag[1], p->mag[2]);
	printf("Gyro:          [%d, %d, %d]\r\n", p->gyro[0], p->gyro[1], p->gyro[2]);
	printf("Loop time:     %lu us\r\n", p->loop_time);
	printf("Max loop time: %lu us\r\n", max_loop_time);

	fflush(stdout);
}
