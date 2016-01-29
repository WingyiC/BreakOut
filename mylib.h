#define REG_DISPCTL *(u16 *)0x4000000
#define videoBuffer ((u16 *)0x6000000)
#define OFFSET(r,c,SCREENWIDTH) ((r)*(SCREENWIDTH)+(c))
#define SCANLINECOUNTER *(volatile u16 *)0x4000006

#define MODE3 3
#define BG2_ENABLE (1<<10)

#define COLOR(R,G,B) (((R) & 0x1F) | (((G) & 0x1F) << 5) | (((B) & 0x1F) << 10))
#define BLACK   COLOR(0 , 0 , 0 )
#define WHITE   COLOR(31, 31, 31)
#define RED     COLOR(31, 0 , 0 )
#define GREEN   COLOR(0 , 31, 0 )
#define BLUE    COLOR(0 , 0 , 31)
#define YELLOW  COLOR(31, 31, 0 )
#define MAGENTA COLOR(31, 0 , 31)
#define CYAN    COLOR(0 , 31, 31)
#define ORANGE  COLOR(31, 15, 0 )
#define BROWN   COLOR(18, 9 , 0 )
#define PURPLE  COLOR(15, 0 , 15)
#define TEAL    COLOR(0 , 15, 15)
#define MAROON  COLOR(15, 0 , 0 )
#define GREY    COLOR(15, 15, 15)
#define PINK    COLOR(31, 18, 19)

#define SCREENWIDTH 240
#define SCREENHEIGHT 160

#define BUTTONS (*(volatile unsigned int *)0x04000130)
#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

typedef unsigned short u16;
typedef int bool;
#define true 1
#define false 0

extern unsigned int oldButtons;
extern unsigned int buttons;

#define KEY_PRESSED(key) (!(~(oldButtons)&(key)) && (~buttons & (key)))
#define KEY_DOWN_NOW(key) (~(BUTTONS) & key)

// Prototypes
void setPixel(int , int , u16 );
void initialize();
void update();
void waitForVblank();
void draw();
void startScreen();
void winScreen();
void clearStartScreen();
void drawRect(int, int, int, int, u16);
void drawHollowRect(int , int , int , int, u16);
void drawImage3(int , int , int , int , const u16 *image);
void fillScreen(u16);