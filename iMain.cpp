/*
    Project Name: Type & Climb
    supervisor: Nishat Anjum Bristy

    Mady By:
        Sadatul Islam Sadi
        2005077

        Souvik Mondol
        2005069
*/

# include "iGraphics.h"
# include <math.h>
# include <stdlib.h>
# include <time.h>

int lower = 97, upper = 122;
int number, number1, num, ch, numbern1, numbern2;
char cht[50], cht1[50], chtn1[5], chtn2[5];

typedef struct rgb_color
{
    int r;
    int g;
    int b;
} Color;
//Function prototypes.
void playboxWork(void);
void printMenubar(int x, int y, int dx, int dy, char s[]);
void generate_pillar_gap(void); // Generates pillargaps and also sets pillar's X co-ordinate
void jumpAnimation(void); // This function animates the jump action;
void get_game_time(char s[]);
void end_the_game(void);
void game_reset(void);
void printer(void);

int random_generator();
int random_generator1();
void random_char(char ch[]);
void random_char1(char ch[]);
int random_generator_n2();
int random_generator_n1();

// All constants determining the size of different parts
#define PLAYBOX_W 500
#define PLAYBOX_H 680
#define TIMEBOX_W 400
#define TIMEBOX_H 100
#define JUMPBOX_W 200
#define JUMPBOX_H 180
#define LINEBOX_W 400
#define LINEBOX_H 400

// Variables: PLease try to write a short description of where that variable is being used.

// Variables for Pillars:
int num_of_pillars = 9; // The number of pillars in playboxWork
int pillar_width = 40; // dx of pillar
int pillar_height[9] = {240, 270, 300, 330, 360, 390, 420, 450, 480}; // Heights of pillars
int widht_of_gap = 25; // The widht of gaps;
int pillar_gaps[9]; // Gaps between pillar.
int pillar_X[9]; // The x co-ordinate of the pillars.

// Variables for Character
int characterX; // This will be initialized when the pillar_X array will be intialized. Check generate_pillar_gap func.
int characterY = pillar_height[2];
char character_images[][50] = {"bmps\\stand.bmp", "bmps\\jump.bmp", "bmps\\hanging2.bmp", "bmps\\hanging.bmp"};
int character_image_tracker = 0;

// Time Tracking Variables.
clock_t t1; // For tracking the penalty time due to falling.
clock_t game_start_time, current_time, game_cycle_start; // game_cycle_start is used to reset times in the game.
int total_cycle_time = 30; // The game starts with a 30s time. This will be reduced over time and increase when a cycle ends.
int remaining_game_time;
int gain_time = 20;
int gain_time_reduce = 2;

// Game Control Variables.
int inputKey = 0; // This is detecting the inputKey 0 state means no keypressed
// 1 means short jump key is pressed
// 2 means long jump key is pressed
int has_fallen = 0; // This is for detecting whether the character has fallen.
// 0 means not fallen
// 1 means has fallen and the KEY REQUIRED was 1 ...means small jump was required
// 2 means has fallen and the KEY REQUIRED was 2 ...means long jump was required
int print_frame_after_fall = 0;// This varibale is being used to animate the the frame after the fall.
// 1 means the character has fallen.
// 0 means the character is standing.
int pillars_generated = 9; // This keeps track of the number of pillars already been generated in a cycle.
int green_pillar_tracker = -1; // This variables tracks the red pillar. -1 value represents that it hasn't been generated yet.
int total_cycle_pillars = 15; //  total_cycle_pillars and total_cycle_time will be used to adjust the games hardness.(NOT GOOD AT ENGLISH)
int pillars_crossed = 4; // This is used to show the user how far he is from the green pillar.
char pillars_to_green[50]; // Used to hold text to print how far from greenpillar

// Variables for score management;
int scores = 0;
int highscore; // Holds the current high score. It gets intialized in the main function.
int new_high_score = 0; // 1 if the user scored new_high score. 0 if he didn't
char score_text[50]; // Used to hold text to print with itext

// Variables to Control the menu
int game_started = 0; // If the user has clicked start game the game will start .
// 1 means game started and 0 means still on game menu
int menu_start_x = 550;
int menu_start_y = 400;
Color menu_color_out = {64, 129, 46};
Color menu_color_in = {214, 232, 101};
int game_continue_menu = 0; // 1 means it will print a regame menu after the game ends
// 0 means regame menu won't be printed
int how_to_play = 0;

/*
	function iDraw() is called again and again by the system.
*/
void iDraw()
{
    //place your drawing codes here
    iClear();
    current_time = clock();
    if (game_started == 1)
    {
        playboxWork();
    }

    iSetColor(117, 83, 52); //186,200,99    161, 121, 85
    iFilledRectangle(500, 0, 400, 680);

    if (game_continue_menu == 1)
    {
        iShowBMP(0, 0, "bmps/GameOver.bmp");
        char score_text[50];
        sprintf(score_text, "Score: %d", scores);
        iSetColor(0, 255, 0);
        iText(670, 600, score_text, GLUT_BITMAP_HELVETICA_18);
        if (new_high_score == 1)
        {
            //iText(620, 560, "NEW HIGH SCORE", GLUT_BITMAP_HELVETICA_18);
            iShowBMP2(590, 500, "bmps/new_faculty.bmp", 0x00ffffff);
        }
        printMenubar(menu_start_x, menu_start_y, 300, 30, "Start New Game");
        printMenubar(menu_start_x, menu_start_y - 40, 300, 30, "Return to Menu");
        return;
    }

    if (game_started == 0)
    {
        iShowBMP(0, 0, "bmps/First_background.bmp");
        iSetColor(0, 255, 0);
        //iText(630, 550, "Type & Climb", GLUT_BITMAP_TIMES_ROMAN_24);
        //iText(635, 500, "Start Game", GLUT_BITMAP_TIMES_ROMAN_24);
        //iText(630, 470, "How to Play", GLUT_BITMAP_TIMES_ROMAN_24);
        printMenubar(menu_start_x, menu_start_y, 300, 30, "Start Game");
        printMenubar(menu_start_x, menu_start_y - 40, 300, 30, "How to Play");
        char highscore_text[100];
        sprintf(highscore_text, "Current HighScore: %d", highscore);
        printMenubar(menu_start_x, menu_start_y - 80, 300, 30, highscore_text);
        printMenubar(menu_start_x, menu_start_y - 200, 300, 30, "Quit Game");

        if (how_to_play == 1)
        {
            //iFilledRectangle(0, 0, PLAYBOX_W, PLAYBOX_H);
            iShowBMP(10, 14, "bmps/play.bmp");
            iFilledCircle(menu_start_x - 5, menu_start_y - 25, 3);
            iFilledCircle(menu_start_x + 305, menu_start_y - 25, 3);
        }
        return;
    }

    char time_text[50];
    get_game_time(time_text); // We are showing the text after the score
    iSetColor(0, 255, 0);
    if (remaining_game_time <= 10)
    {
        if (remaining_game_time % 2 == 0)
        {
            iSetColor(255, 0, 0);
        }
        else
        {
            iSetColor(200, 0, 0);
        }
    }

    if (print_frame_after_fall == 1)
    {
        iSetColor(220, 220, 220);
    }

    //char score_text[50];
    //sprintf(score_text, "Score: %d", scores);
    iText(670, 530, score_text, GLUT_BITMAP_HELVETICA_18);
    iText(600, 480, time_text, GLUT_BITMAP_TIMES_ROMAN_24);

    //char pillars_to_green[50];
    //sprintf(pillars_to_green, "From Green Pillar: %d",  total_cycle_pillars - pillars_crossed);
    iText(600, 400, pillars_to_green, GLUT_BITMAP_TIMES_ROMAN_24);
    iShowBMP2(145, 120, "bmps/score.bmp", 0x00ffffff);
    //iSetColor(214, 232, 101);
    iSetColor(255, 255, 255);
    iText(200, 190, cht, GLUT_BITMAP_TIMES_ROMAN_24);
    iText(352, 190, chtn1, GLUT_BITMAP_HELVETICA_12);
    iText(200, 155, cht1, GLUT_BITMAP_TIMES_ROMAN_24);
    iText(352, 155, chtn2, GLUT_BITMAP_HELVETICA_12);

    if (scores <= highscore)
    {
        if (scores >= 25 && scores < 50)
        {
            iShowBMP2(650, 200, "bmps/fire.bmp", 0x00ffffff);
        }
        else if (scores >= 50)
        {
            iShowBMP2(590, 220, "bmps/louho2.bmp", 0x00ffffff); //590 // faculty 560
        }
    }
    else
    {
        iShowBMP2(560, 220, "bmps/faculty.bmp", 0x00ffffff);
    }

}

void playboxWork(void)
{
    // This Sadi's part of work. EveryChange he makes will be done here.
    // If he adds any function or animation they will only modify this part and keep other parts untouched.
    // If any necessary adjustments needs to be made on other parts Souvik will be notified.
    //iSetColor(0, 0, 0);
    //iFilledRectangle(0, 0, PLAYBOX_W, PLAYBOX_H); // Background
    iShowBMP(0, 0, "bmps/background3.bmp");
    iSetColor(161, 121, 85);
    for (int i = 0; i < num_of_pillars; i++)
    {
        if (pillar_X[i] >= PLAYBOX_W)
        {
            break;
        }
        if (i == green_pillar_tracker)
        {
            iSetColor(0, 255, 0);
            iFilledRectangle(pillar_X[i], 10, pillar_width, pillar_height[i]);
            iSetColor(220,220,220);
            continue;
        }
        iFilledRectangle(pillar_X[i], 10, pillar_width, pillar_height[i]);
    }

    // Placing the character on the third pillar
    iShowBMP2(characterX, characterY + 10, character_images[character_image_tracker], 0x00ffffff);
    //current_time = clock();
    if (print_frame_after_fall == 1)
    {
        //t2 = clock();
        if (has_fallen == 2)
        {
            character_image_tracker = 2;
        }
        else if(has_fallen == 1)
        {
            character_image_tracker = 3;
        }
        double time_elapsed = (double) current_time - t1; // current time gives us current time state for each frame
        if (time_elapsed >= 5000)
        {
            int tmp;
            character_image_tracker = 0;
            if (has_fallen == 2)
            {
                characterY += 55;
                characterX += 10;
                tmp = -25;
            }
            else if (has_fallen == 1)
            {
                characterY += 55;
                characterX -= 10;
                tmp = 25;
            }
            for (int i = 0; i < num_of_pillars; i++)
            {
                pillar_X[i] += tmp;
            }
            has_fallen = 0;
            print_frame_after_fall = 0;

        };
    }

}

void printMenubar(int x, int y, int dx, int dy, char s[])
{
    iSetColor(menu_color_out.r, menu_color_out.g, menu_color_out.b); // 161, 121, 85
    iFilledRectangle(x, y, dx, dy);
    iSetColor(menu_color_in.r, menu_color_in.g, menu_color_in.b);
    iText(x + 10, y + 6, s, GLUT_BITMAP_TIMES_ROMAN_24);
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
    if (mx >= menu_start_x && mx <= menu_start_x + 300 && my >= menu_start_y && my <= menu_start_y + 30)
    {
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            if (game_started == 0)
            {
                game_started = 1;
                game_continue_menu = 0;
                how_to_play = 0;
                game_reset();
            }
        }
    }

    if (mx >= menu_start_x && mx <= menu_start_x + 300 && my >= menu_start_y - 40 && my <= menu_start_y - 40 + 30)
    {
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            if (game_continue_menu == 1)
            {
                game_continue_menu = 0;
                //how_to_play = 0;
                game_started = 0;
            }
            else if (game_continue_menu == 0)
            {
                how_to_play = (how_to_play == 0) ? 1 : 0;
            }
        }
    }

    // printMenubar(menu_start_x, menu_start_y - 40, 300, 30, "How to Play");

    if (mx >= menu_start_x && mx <= menu_start_x + 300 && my >= menu_start_y - 200 && my <= menu_start_y - 200 + 30)
    {
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            exit(0);
        }
    }
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    //place your codes for other keys here
    if (game_started == 0)
    {
        return;
    }
    if (print_frame_after_fall == 1)
    {
        return;
    }
    if (number1 == number)
    {
        if (key != number1)
        {
            return;
        }
        inputKey = pillar_gaps[2];
        if (inputKey == 1)
        {
            number1 = numbern2;
            numbern2 = random_generator_n2();
            //random_generator1();
            random_char1(cht1);
            sprintf(chtn2, "%c", numbern2);
        }
        else
        {
            number = numbern1;
            numbern1 = random_generator_n1();
            //random_generator();
            random_char(cht);
            sprintf(chtn1, "%c", numbern1);
        }

        scores++;
        sprintf(score_text, "Score: %d", scores);
        iResumeTimer(0);
        return;
    }
    if (key == number1)
    {
        inputKey = 1;
        number1 = numbern2;
        numbern2 = random_generator_n2();
        //random_generator1();
        random_char1(cht1);
        sprintf(chtn2, "%c", numbern2);
        scores++;
        if (inputKey != pillar_gaps[2])
        {
            scores--;
            has_fallen = 2;
            t1 = clock();
        }
        sprintf(score_text, "Score: %d", scores);
        iResumeTimer(0);
        return;
    }
    if (key == number)
    {
        inputKey = 2;
        number = numbern1;
        numbern1 = random_generator_n1();
        //random_generator();
        random_char(cht);
        sprintf(chtn1, "%c", numbern1);
        scores++;
        if (inputKey != pillar_gaps[2])
        {
            scores--;
            has_fallen = 1;
            t1 = clock();
        }
        sprintf(score_text, "Score: %d", scores);
        iResumeTimer(0);
        return;
    }
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

    //place your codes for other keys here
}

int main()
{
    //place your own initialization codes here.
    // before initialization we need to generate some random values for gaps between pillars.

    srand(time(0));
    PlaySound("G_BGM.wav", NULL, SND_ASYNC | SND_LOOP);
    generate_pillar_gap();
    FILE *fp;
    fp = fopen("highscore.txt", "r");
    fscanf(fp, "%d", &highscore);
    fclose(fp);
    iSetTimer(20, jumpAnimation);
    iPauseTimer(0);

    sprintf(score_text, "Score: %d", scores);
    sprintf(pillars_to_green, "From Green Pillar: %d",  total_cycle_pillars - pillars_crossed);

    random_generator();
    random_generator1();
    numbern1 = random_generator_n1();
    numbern2 = random_generator_n2();

    random_char(cht);
    random_char1(cht1);
    sprintf(chtn1, "%c", numbern1);
    sprintf(chtn2, "%c", numbern2);

    iInitialize(900, 680, "Type and Climb");
    return 0;
}

void generate_pillar_gap(void)
{
    for (int i = 0; i < num_of_pillars; i++)
    {
        pillar_gaps[i] = (rand() % 2) + 1;
    }
    int x = 20; // Intial Gap

    for (int i = 0; i < num_of_pillars; i++)
    {
        pillar_X[i] = x;
        x += pillar_width + (widht_of_gap * pillar_gaps[i]);
    }
    characterX = pillar_X[2];
}


int jump_tracker = 10;//Any change here...must change three more values..(1)
void jumpAnimation(void)
{
    character_image_tracker = 1;
    if (characterY >= 330)//Any change here...must change three more values..(2)
    {
        jump_tracker = -15;//Any change here...must change three more values..(3)
    }
    characterY += jump_tracker;
    for (int i = 0; i < num_of_pillars; i++)
    {
        pillar_height[i] -= 6;
        if (inputKey == 1)
        {
            pillar_X[i] -= 13;
        }
        else if (inputKey == 2)
        {
            pillar_X[i] -= 18;
        }
    }

    if (pillar_height[1] == 240)
    {
        character_image_tracker = 0;
        jump_tracker = 10; //Any change here...must change three more values..(4)
        iPauseTimer(0);
        if (has_fallen != 0)
        {
            if (has_fallen == 2)
            {
                characterY -= 55;
                characterX -= 10;
            }
            else if (has_fallen == 1)
            {
                characterY -= 55;
                characterX += 10;
            }

            print_frame_after_fall = 1;
        }

        for (int i = 1; i < 9; i++)
        {
            pillar_height[i - 1] = pillar_height[i];
            pillar_X[i - 1] = pillar_X[i];
            pillar_gaps[i - 1] = pillar_gaps[i];
        }
        pillar_height[num_of_pillars - 1] = 480;
        pillar_gaps[num_of_pillars - 1] = (rand() % 2) + 1;
        pillar_X[num_of_pillars - 1] = pillar_X[num_of_pillars - 2] + pillar_width + (widht_of_gap * pillar_gaps[num_of_pillars - 2]);
        pillars_generated++;
        pillars_crossed++;
        if (pillars_generated >= total_cycle_pillars)
            // After 13 jumps the pillar gets created. Then in the next 7 jumps character reaches the pillar.
        {
            pillars_generated = 0;
            green_pillar_tracker = num_of_pillars - 1;
        }
        if (green_pillar_tracker != -1)
        {
            green_pillar_tracker--;
            if(green_pillar_tracker == 2)
            {
                game_cycle_start = current_time;
                total_cycle_time = gain_time + remaining_game_time;
                gain_time = gain_time - gain_time_reduce;
                pillars_crossed = 0;
            }
        }
        sprintf(pillars_to_green, "From Green Pillar: %d",  total_cycle_pillars - pillars_crossed);
    }

}

void get_game_time(char s[])
{
    double tmp = (double) (current_time - game_cycle_start);
    tmp = tmp / 1000;
    remaining_game_time = total_cycle_time - round(tmp);
    if (remaining_game_time <= 0)
    {
        end_the_game();
    }
    sprintf(s, "Time Remaining: %d", remaining_game_time);
}

void end_the_game(void)
{
    if (scores > highscore)
    {
        highscore = scores;
        new_high_score = 1;
        FILE *fp = fopen("highscore.txt", "w");
        fprintf(fp, "%d", highscore);
        fclose(fp);
    }
    game_started = 0;
    game_continue_menu = 1;
}

void game_reset(void)
{
    srand(time(0));
    game_start_time = clock();
    game_cycle_start = game_start_time;
    total_cycle_time = 30;
    gain_time = 20;
    gain_time_reduce = 2;

    scores = 0;

    green_pillar_tracker = -1;
    pillars_generated = 9;
    pillars_crossed = 4;

    sprintf(score_text, "Score: %d", scores);
    sprintf(pillars_to_green, "From Green Pillar: %d",  total_cycle_pillars - pillars_crossed);

    characterY = pillar_height[2];
    character_image_tracker = 0;
    inputKey = 0;
    generate_pillar_gap();
    has_fallen = 0;
    print_frame_after_fall = 0;
    new_high_score = 0;

    random_generator();
    random_generator1();

    numbern1 = random_generator_n1();
    numbern2 = random_generator_n2();

    random_char(cht);
    random_char1(cht1);
    sprintf(chtn1, "%c", numbern1);
    sprintf(chtn2, "%c", numbern2);

}

void printer(void)
{
    // For debugging puposes
    for (int i = 0; i < 9; i++)
    {
        printf("%d ", pillar_height[i]);
    }
    printf("\n");

}

//SMT portion

int random_generator()
{
    num = (rand() % (upper - lower + 1)) + lower;

    if (num == number1)
    {
        number = (num == 122) ? (num - 1) : (num + 1);
        return number;
    }

    number = num;
    return number;
}


int random_generator1()
{
    num = (rand() % (upper - lower + 1)) + lower;

    if (num == number)
    {
        number1 = (num == 122) ? (num - 1) : (num + 1);
        return number1;
    }

    number1 = num;

    return number1;
}

int random_generator_n1()
{
    int num = (rand() % (upper - lower + 1)) + lower;
    if (num == number1)
    {
        num = (num == 122) ? (num - 1) : (num + 1);
        return num;
    }

    return num;
}

int random_generator_n2()
{
    int num = (rand() % (upper - lower + 1)) + lower;
    if (num == number)
    {
        num = (num == 122) ? (num - 1) : (num + 1);
        return num;
    }

    return num;
}

void random_char(char ch[])
{
    sprintf(ch, "Long jump : %c", number);
}

void random_char1(char ch[])
{
    sprintf(ch, "Short jump : %c", number1);
}

