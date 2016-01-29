#include "mylib.h"
#include "text.h"
#include "brpic.h"
#include "sadbrk.h"
#include "hppbrk.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int oldCol;
	int row;
	int col;
	int thickness;
	int width;
	unsigned short color;
} paddle;

paddle Paddle(int col){
	paddle p = {col, 155-2 , col, 2, 30, YELLOW}; //initialize with default values
	return p;
}

typedef struct{
	bool active;
	int row;
	int col;
	int thickness;
	int width;
	unsigned short color;
} brick;

brick Brick(int row, int col, unsigned short color){
	brick br = {true, row, col, 4, 20, color}; //initialize with default values
	return br;
}

typedef struct{
	int rd;
	int cd;
	int oldRow;
	int oldCol;
	int row;
	int col;
	int diameter;
	unsigned short color;
} ball;

ball Ball(int row, int col){
	ball ba = {1, 1, row, col, row, col, 2, RED}; //initialize with default values
	return ba;
}

enum GBAState {
	START,
	START_NODRAW,
	drawGameSetup,
	gameLOOP,
	RESTART,
	BACK
};

int bricks_left;
brick deactivated_brick;

void drawPaddle(paddle);
void drawBrick(brick);
void drawBall(ball);
void drawAllBricks();

paddle paddle1;
brick brickList[40]; 
ball ball1;

int main() {
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	buttons = BUTTONS;
	enum GBAState state = START;
	int previous = KEY_DOWN_NOW(BUTTON_START);
	int goBack = KEY_DOWN_NOW(BUTTON_SELECT);
	
	while(1) {
		waitForVblank();
		switch(state) {
			case START:
				startScreen();
				state = START_NODRAW;
				break;

			case START_NODRAW:
				if (KEY_DOWN_NOW(BUTTON_START) && (!previous)) {
					state = drawGameSetup;
				}
				else if (KEY_DOWN_NOW(BUTTON_SELECT) && (!goBack)) {
					state = START;
				}
				break;

			case drawGameSetup:
				clearStartScreen();
				bricks_left = 40; 
				paddle1 = Paddle(120 - 30 / 2); //paddle width = 30
				ball1 = Ball(paddle1.row - 2, 120 - 2/2); //ball diameter = 2

				for (int r = 0; r < 4; r++){ //4 rows and 10 cols of bricks
					for (int c = 0; c < 10; c++) {
						brickList[r * 10 + c] = Brick(r * 5 + 1, c * 21 + 15, GREEN);
					}
				}
					draw();
					drawAllBricks();
					state = BACK;
					break;

			case BACK:
				if (KEY_DOWN_NOW(BUTTON_SELECT) && (!goBack)) {
					state = START;
				} else {
					state = gameLOOP;
				}
				break;

			case gameLOOP:
				while(1) {	
					oldButtons = buttons;
					buttons = BUTTONS;
					draw();
					waitForVblank();
					update();			
				}
				state = RESTART;
				break;
			
			case RESTART:
				if (KEY_DOWN_NOW(BUTTON_START) && (!previous)) {
						state = START;
				}
			}
			
		previous = KEY_DOWN_NOW(BUTTON_START);
	}	
}

void startScreen() { 
	drawImage3(0, 0, BRPIC_WIDTH, BRPIC_HEIGHT, brpic);
	drawString(50, 120-10*3, "BREAKOUT", MAGENTA);
	drawString(90, 120-10*6, "Press Enter to Start", CYAN);
}

void winScreen() {	
	drawImage3(0, 0, HPPBRK_WIDTH, HPPBRK_HEIGHT, hppbrk);
	drawString(80-4, 120-6*8/2, "Congratulations!You Won!", MAGENTA);
	drawString(90, 120-10*6, "Press Enter to Restart", CYAN);
}

void clearStartScreen() {
	fillScreen(BLACK);
}

void update(){
	//paddle
	if (KEY_DOWN_NOW(BUTTON_LEFT)){
		paddle1.oldCol = paddle1.col;
		paddle1.col -= 2;
		if (paddle1.col < 1) {	//check paddle collision with wall
			paddle1.col = 0;
		}
	} else if (KEY_DOWN_NOW(BUTTON_RIGHT)){
		paddle1.oldCol = paddle1.col;
		paddle1.col += 2;
		if (paddle1.col > 209) {
			paddle1.col = 210;
		}
	}

	//ball
	ball1.oldRow = ball1.row;
	ball1.oldCol = ball1.col;
	ball1.row += ball1.rd;
	ball1.col += ball1.cd;
	if(ball1.row<1){ // check ball collision with wall and paddle
		ball1.row = 0;
		ball1.rd *= -1;
	} else if (ball1.row>157 && bricks_left){
		drawImage3(0, 0, SADBRK_WIDTH, SADBRK_HEIGHT, sadbrk);
		drawString(80-9, 120-6*9/2, "Game Over", MAGENTA);
		drawString(90, 120-10*6, "Press Enter to Restart", CYAN);
		char buffer[41];
		sprintf(buffer, "Bricks Left: %d", bricks_left);
		drawString(80, 120-6*15/2, buffer, YELLOW);
	} else if ((ball1.row>paddle1.row-3) && //ball collises with paddle
			(ball1.col >= paddle1.col) && 
			(ball1.col <= paddle1.col + 28)){
		ball1.row = paddle1.row-2;
		ball1.rd *= -1;
	}
	if(ball1.col<1){
		ball1.col = 0;
		ball1.cd *= -1;
	} else if(ball1.col>237){
		ball1.col = 238;
		ball1.cd *= -1;
	}

	//bricks - collision with ball
	for (unsigned int i = 0; i < sizeof(brickList)/ sizeof(brickList[0]); i++){ //4 rows and 10 cols of bricks
		if(brickList[i].active){
			if((ball1.col+2 > brickList[i].col-1) 
					&& (ball1.col < brickList[i].col+20+1)
					&& (ball1.row+2 > brickList[i].row-1) 
					&& (ball1.row < brickList[i].row+4+1)){

				if((ball1.col+2 > brickList[i].col-1) 
						&& (ball1.col < brickList[i].col+20+1)){
					ball1.rd *= -1;
				} else if((ball1.row+2 > brickList[i].row-1) 
						&& (ball1.row < brickList[i].row+4+1)){
					ball1.cd *= -1;
				}

				brickList[i].active = false;
				deactivated_brick = brickList[i];
				bricks_left--;
			}
		}
	}
}

void draw(){
	drawPaddle(paddle1);
	drawBall(ball1);
	if (!bricks_left) {
		winScreen();
	}
	drawAllBricks();
	drawRect(deactivated_brick.row, deactivated_brick.col, deactivated_brick.thickness, deactivated_brick.width, BLACK);
}

void drawAllBricks(){
	for (unsigned int i = 0; i < sizeof(brickList)/ sizeof(brickList[0]); i++) {
		drawBrick(brickList[i]);
	}
}

void drawPaddle(paddle p){
	drawRect(p.row, p.oldCol, p.thickness, p.width, BLACK); //erase at the old position
	drawRect(p.row, p.col, p.thickness, p.width, p.color);
}

void drawBrick(brick br){
	if(br.active){
		drawRect(br.row, br.col, br.thickness, br.width, br.color);
	}
}

void drawBall(ball ba){
	drawRect(ba.oldRow, ba.oldCol, ba.diameter, ba.diameter, BLACK); //erase at the old position
	drawRect(ba.row, ba.col, ba.diameter, ba.diameter, ba.color);
}
