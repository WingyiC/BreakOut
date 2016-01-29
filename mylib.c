#include "mylib.h"
#include <stdio.h>

unsigned int buttons;
unsigned int oldButtons;
unsigned int presses;

void setPixel(int row, int col, u16 color) {
	videoBuffer[OFFSET(row, col, SCREENWIDTH)] = color;
}

void delay(int n) {
  int i;
  volatile int b;
  for(i=0;i<10000*n;i++) {
     b++;
  }
}

void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void drawRect(int row, int col, int height, int width, u16 color) {
	int i; 
	int j;
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			setPixel(i + row, j + col, color);
		}
	}
}

void drawHollowRect(int r, int c, int width, int height, u16 color) {
	for (int i = c; i < (width + c); i++) {
		setPixel(i, r, color);
		setPixel(i, r + height - 1, color);
	}

	for (int j = (r + 1); j < (height + r - 1); j++) {
		setPixel(c, j, color);
		setPixel(c + width - 1, j, color);
	}
}

void drawImage3(int r, int c, int width, int height, const u16 *image) {
    int i;
    int j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
             videoBuffer[(r+i) * SCREENWIDTH + (c+j)] = image[(i*width) + j];
        }
    }
}

void fillScreen(u16 color) {
	int x;
	int	y;
	for(x = 0; x < SCREENWIDTH; x++) {
		for(y = 0; y < SCREENHEIGHT; y++) {
			videoBuffer[x + y * SCREENWIDTH] = color;
		}
	}

}
