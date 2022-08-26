# include "iGraphics.h"
# include <math.h>
# include <stdlib.h>
# include <time.h>

void printer();

int num_of_rect = 15;
int inputKey = 0;
int moves = 1; //For testing purposes

int rectwidth = 20;
int rectHeight[16] = {30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360, 390, 420, 450, 480};
int rectgap[16] = {1, 2, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 2, 1, 2, 1}; // Gap after the rect...rectgap[0] 1 means gap after the first rect.
int rectX[16];
char s[5][20] = {"background1.bmp", "background2.bmp", "background3.bmp", "background4.bmp", "background5.bmp"};
int num_of_backgorund = 5;
int background_cycler = 0;

typedef struct _ball
{
    int x;
    int y;
    int r;
} Ball;

Ball ball;

//int ballR = 10;
//int ballX = 10 + rectwidth + (rectgap[0] * 20) + ballR;
//int ballY = rectHeight[1] + ballR;
/*
	function iDraw() is called again and again by the system.
*/
void iDraw()
{
    //place your drawing codes here
    iClear();
    iSetColor(220,220,220);
    iShowBMP(0, 0, s[background_cycler]);
    for (int i = 0; i < num_of_rect; i++)
    {
        iFilledRectangle(rectX[i], 10, rectwidth, rectHeight[i]);
    }
    iSetColor(250,69,0);
    iFilledCircle(ball.x, ball.y, ball.r);
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    //printf("x = %d, y= %d\n",mx,my);
    //place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{

}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    if(key == 'q')
    {
        exit(0);
    }
    if (key == 'a')
    {
        inputKey = 1;
        if (inputKey != rectgap[1])
        {
            exit(0);
        }
        iResumeTimer(0);
        background_cycler = (background_cycler < 4) ? background_cycler + 1 : 0;
    }

    else if (key == 's')
    {
        inputKey = 2;
        if (inputKey != rectgap[1])
        {
            exit(0);
        }
        iResumeTimer(0);
        background_cycler = (background_cycler < 4) ? background_cycler + 1 : 0;
    }
    //place your codes for other keys here
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{

    if(key == GLUT_KEY_END)
    {
        exit(0);
    }
    //place your codes for other keys here
}

void heightAnimator()
{
    num_of_rect = 16;
    for (int i = 0; i < 16; i++)
    {
        rectHeight[i] -= 6;
        if (inputKey == 1)
        {
            rectX[i] -= 8;
        }
        else if (inputKey == 2)
        {
            rectX[i] -= 12;
        }
    }
    if (rectHeight[0] == 0)
    {
        iPauseTimer(0);
        for (int i = 1; i < 16; i++)
        {
            rectHeight[i - 1] = rectHeight[i];
            rectX[i - 1] = rectX[i];
            rectgap[i - 1] = rectgap[i];
        }
        rectHeight[15] = 480;
        rectgap[15] = (rand() % 2) + 1;
        //printer();

        rectX[15] = rectX[14] + rectwidth + (20 * rectgap[14]);
        num_of_rect = 15;
    }
}

void printer()
{
    /*
    for (int i = 0; i < 16; i++)
    {
        printf("%d ", rectgap[i]);
    }
    printf("\n");
    */
    printf("%d\n", moves);
}

int main()
{
    //place your own initialization codes here.
    srand(time(0));
    ball.r = 15;
    ball.x = 10 + rectwidth + (rectgap[0] * 20) + (rectwidth / 2);
    ball.y = rectHeight[1] + ball.r + 10;
    int x = 10;
    for (int i = 0; i < 16; i++)
    {
        rectX[i] = x;
        x += rectwidth + (20 * rectgap[i]);
    }
    iSetTimer(20, heightAnimator);
    iPauseTimer(0);
    iInitialize(1300, 650, "Sadi's Demo!");
    return 0;
}
