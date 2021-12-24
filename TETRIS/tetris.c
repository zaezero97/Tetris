#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <string.h>

#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15
#define ESC 0x1b
#define ENTER 13
#define WIDTH  12
#define HEIGHT 27
#define UP  0x48 // Up key는 0xe0 + 0x48 두개의 값이 들어온다.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d
#define DROP1 0x20
#define HOLD1 'v'
#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.
#define UP2		'w'//player2 는 AWSD 로 방향키 대신
#define DOWN2	's'
#define LEFT2	'a'
#define RIGHT2	'd'
#define DROP2   'g'
#define HOLD2 'h'
int player1_map[HEIGHT][WIDTH];
int player2_map[HEIGHT][WIDTH];
int out1_map[2][WIDTH];
int out2_map[2][WIDTH];
int gameover[HEIGHT][WIDTH] = {
	{2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,0,0,0,0,0,0,0,0,2,2},
	{2,2,0,0,0,0,0,0,0,0,2,2},
	{2,2,0,0,2,2,2,2,2,2,2,2},
	{2,2,0,0,0,0,0,0,0,0,2,2},
	{2,2,0,0,0,0,0,0,0,0,2,2},
	{2,2,0,0,2,2,2,2,2,2,2,2},
	{2,2,0,0,0,0,0,0,0,0,2,2},
	{2,2,0,0,0,0,0,0,0,0,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,0,0,2,2,2,2,0,0,2,2},
	{2,2,0,0,0,2,2,2,0,0,2,2},
	{2,2,0,0,2,0,2,2,0,0,2,2},
	{2,2,0,0,2,2,0,2,0,0,2,2},
	{2,2,0,0,2,2,2,0,0,0,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,0,0,0,0,0,0,2,2,2,2},
	{2,2,0,0,2,2,2,0,0,2,2,2},
	{2,2,0,0,2,2,2,2,0,0,2,2},
	{2,2,0,0,2,2,2,2,0,0,2,2},
	{2,2,0,0,2,2,2,2,0,0,2,2},
	{2,2,0,0,2,2,2,2,0,0,2,2},
	{2,2,0,0,0,0,0,0,0,2,2,2},
	{2,2,0,0,0,0,0,0,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2},

};

int rorate_flag1,rorate_flag2;
int temp_pos[4][2];
int temp2_pos[4][2];

int block[7][4][4] = {
	{
		{0,0,0,0},
		{1,1,1,1},
		{0,0,0,0},
		{0,0,0,0}              
		},
	{
		{0,1,0,0},
		{0,1,0,0},
		{0,1,1,0},
		{0,0,0,0}
		},
	{
		{0,1,0,0},
		{1,1,1,0},
		{0,0,0,0},
		{0,0,0,0}
		},
	{
		{0,1,1,0},
		{0,1,1,0},
		{0,0,0,0},
		{0,0,0,0}
		},
	{
		{0,0,1,0},
		{0,0,1,0},
		{0,1,1,0},
		{0,0,0,0}
		},
	{
		{0,1,1,0},
		{1,1,0,0},
		{0,0,0,0},
		{0,0,0,0}
	},
	{
		{0,1,1,0},
		{0,0,1,1},
		{0,0,0,0},
		{0,0,0,0}
		}
};
int block1_n;
int block2_n;
int block1_next = 0;
int block2_next = 0;
int pos1[4][2];
int pos2[4][2];
int count1=0, count2=0;
int block1_hold=-1;
int block2_hold=-1;
void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos; // Windows.h 에 정의
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

void removeCursor(void) { // 커서를 안보이게 한다
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void cls(int text_color, int bg_color) // 화면 지우기
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);
}
void printtitle(int x, int y, char* ch)
{
	int i,j=0;
	
	gotoxy(x, y);
	textcolor(GREEN1, BLACK);
	printf("%s",ch);
	textcolor(WHITE, BLACK);

}
int block_down(int player_map[HEIGHT][WIDTH],int pos[4][2]) {

	int i;
	for (i = 0; i < 4; i++)
	{
		if (player_map[pos[i][1] + 1][pos[i][0]] == -1)
			return 0;
		else if (player_map[pos[i][1] + 1][pos[i][0]] == 2)
			return 0;
	}
	for (i = 0; i < 4; i++)
	{
		player_map[pos[i][1]][pos[i][0]] = 0;

		pos[i][1]++;

	}
	for (i = 0; i < 4; i++) {
		player_map[pos[i][1]][pos[i][0]] = 1;

	}
	return 1;
}
void block_LEFT(int player_map[HEIGHT][WIDTH],int pos[4][2])
{
	int i;
	for (i = 0; i < 4; i++)
	{
		if (player_map[pos[i][1]][pos[i][0]-1] == -1)
			return;
		else if (player_map[pos[i][1]][pos[i][0]-1] == 2)
			return;
	}
	for (i = 0; i < 4; i++)
	{
		player_map[pos[i][1]][pos[i][0]] = 0;

		pos[i][0]--;
	}
	for (i = 0; i < 4; i++) {
		player_map[pos[i][1]][pos[i][0]] = 1;

	}
}
void block_RIGHT(int player_map[HEIGHT][WIDTH], int pos[4][2])
{
	int i;
	for (i = 0; i < 4; i++)
	{
		if (player_map[pos[i][1]][pos[i][0] +1] == -1)
			return;
		else if (player_map[pos[i][1]][pos[i][0] +1] == 2)
			return;
	}
	for (i = 0; i < 4; i++)
	{
		player_map[pos[i][1]][pos[i][0]] = 0;

		pos[i][0]++;
	}
	for (i = 0; i < 4; i++) {
		player_map[pos[i][1]][pos[i][0]] = 1;

	}
}
void block_rorate(int pos[4][2],int block_n,int player_map[HEIGHT][WIDTH],int rorate_flag) {
	
	
	int i;
	switch (block_n)
	{
		case 0:
		{
			for (i = 0; i < 4; i++)
			{
				temp_pos[i][0] = pos[i][0] - pos[1][0];
				temp_pos[i][1] = pos[i][1] - pos[1][1];
			}
			for (i = 0; i < 4; i++)
			{
				
				temp2_pos[i][0] = -temp_pos[i][1];
				temp2_pos[i][1] = -temp_pos[i][0];
			}
			for (i = 0; i < 4; i++)
			{
				temp_pos[i][0] = (temp2_pos[i][0] - temp_pos[i][0]) + pos[i][0];
				temp_pos[i][1] = (temp2_pos[i][1] - temp_pos[i][1]) + pos[i][1];
			}
			if (block_crash(temp_pos,player_map) == 1)
				break;
			else if (block_crash(temp_pos,player_map) == 0)
			{
				for (i = 0; i < 4; i++)
				{
					player_map[pos[i][1]][pos[i][0]] = 0;
				}
				for (i = 0; i < 4; i++)
				{
					pos[i][0] = temp_pos[i][0];
					pos[i][1] = temp_pos[i][1];
					player_map[pos[i][1]][pos[i][0]] = 1;
				}
			}
			break;
		}
		case 1:
		{
			for (i = 0; i < 4; i++)
			{
				temp_pos[i][0] = pos[i][0] - pos[1][0];
				temp_pos[i][1] = pos[i][1] - pos[1][1];
			}
			for (i = 0; i < 4; i++)
			{
				if (rorate_flag % 2 == 1)
				{
					temp2_pos[i][0] = -temp_pos[i][1];
					temp2_pos[i][1] = -temp_pos[i][0];

					if (i == 3)
					{
						temp2_pos[i][0] = -temp_pos[i][0];
						temp2_pos[i][1] = temp_pos[i][1];
					}
				}
				else
				{
					if (i == 3)
						temp2_pos[i][1] = -temp_pos[i][1];
					else
					{
						temp2_pos[i][0] = temp_pos[i][1];
						temp2_pos[i][1] = temp_pos[i][0];
					}
				}
			}
			for (i = 0; i < 4; i++)
			{
				temp_pos[i][0] = (temp2_pos[i][0] - temp_pos[i][0]) + pos[i][0];
				temp_pos[i][1] = (temp2_pos[i][1] - temp_pos[i][1]) + pos[i][1];
			}
		
			if (block_crash(temp_pos,player_map) == 1)
				break;
			else if (block_crash(temp_pos,player_map) == 0)
			{
				for (i = 0; i < 4; i++)
				{
					player_map[pos[i][1]][pos[i][0]] = 0;
				}
				for (i = 0; i < 4; i++)
				{
					pos[i][0] = temp_pos[i][0];
					pos[i][1] = temp_pos[i][1];
					player_map[pos[i][1]][pos[i][0]] = 1;
				}
			}
			break;
		}
		case 2:
		{

			for (i = 0; i < 4; i++)
			{
				temp_pos[i][0] = pos[i][0] - pos[2][0];
				temp_pos[i][1] = pos[i][1] - pos[2][1];
			}
			for (i = 0; i < 4; i++)
			{
				if (rorate_flag % 2 == 0)
				{
					
					temp2_pos[i][0] = -temp_pos[i][1];
					temp2_pos[i][1] = temp_pos[i][0];
				}
				else {
					
					temp2_pos[i][0] = -temp_pos[i][1];
					temp2_pos[i][1] = -temp_pos[i][0];
				}
			}
			for (i = 0; i < 4; i++)
			{
				temp_pos[i][0] = (temp2_pos[i][0] - temp_pos[i][0]) + pos[i][0];
				temp_pos[i][1] = (temp2_pos[i][1] - temp_pos[i][1]) + pos[i][1];
			}
			if (block_crash(temp_pos,player_map) == 1)
				break;
			else if (block_crash(temp_pos,player_map) == 0)
			{
				for (i = 0; i < 4; i++)
				{
					player_map[pos[i][1]][pos[i][0]] = 0;
				}
				for (i = 0; i < 4; i++)
				{
					pos[i][0] = temp_pos[i][0];
					pos[i][1] = temp_pos[i][1];
					player_map[pos[i][1]][pos[i][0]] = 1;
				}
			}
			break;
		}
		case 3:
			break;
		case 4:
		{
			for (i = 0; i < 4; i++)
			{
				temp_pos[i][0] = pos[i][0] - pos[1][0];
				temp_pos[i][1] = pos[i][1] - pos[1][1];
			}
			for (i = 0; i < 4; i++)
			{
				if (rorate_flag % 2 == 1)
				{
					temp2_pos[i][0] = -temp_pos[i][1];
					temp2_pos[i][1] = -temp_pos[i][0];

					if (i == 2)
					{
						temp2_pos[i][0] = temp_pos[i][0];
						temp2_pos[i][1] = -temp_pos[i][1];
					}
				}
				else
				{
					if (i == 2)
					{
						temp2_pos[i][0] = -temp_pos[i][0];
						temp2_pos[i][1] = temp_pos[i][1];
					}
					else
					{
						temp2_pos[i][0] = temp_pos[i][1];
						temp2_pos[i][1] = temp_pos[i][0];
					}
				}
			}
			for (i = 0; i < 4; i++)
			{
				temp_pos[i][0] = (temp2_pos[i][0] - temp_pos[i][0]) + pos[i][0];
				temp_pos[i][1] = (temp2_pos[i][1] - temp_pos[i][1]) + pos[i][1];
			}

			if (block_crash(temp_pos,player_map) == 1)
				break;
			else if (block_crash(temp_pos,player_map) == 0)
			{
				for (i = 0; i < 4; i++)
				{
					player_map[pos[i][1]][pos[i][0]] = 0;
				}
				for (i = 0; i < 4; i++)
				{
					pos[i][0] = temp_pos[i][0];
					pos[i][1] = temp_pos[i][1];
					player_map[pos[i][1]][pos[i][0]] = 1;
				}
			}
			break;
		}
		case 5:
		{
			for (i = 0; i < 4; i++)
			{
				temp_pos[i][0] = pos[i][0] - pos[3][0];
				temp_pos[i][1] = pos[i][1] - pos[3][1];
			}
			for (i = 0; i < 4; i++)
			{
				
				temp2_pos[i][0] = -temp_pos[i][1];
				temp2_pos[i][1] = -temp_pos[i][0];

				if (i == 1)
				{
					temp2_pos[i][0] = temp_pos[i][0];
					temp2_pos[i][1] = -temp_pos[i][1];
				}
				else if (i == 2)
				{
					
					temp2_pos[i][0] = temp_pos[i][1];
					temp2_pos[i][1] = temp_pos[i][0];
				}

			}
			for (i = 0; i < 4; i++)
			{
				temp_pos[i][0] = (temp2_pos[i][0] - temp_pos[i][0]) + pos[i][0];
				temp_pos[i][1] = (temp2_pos[i][1] - temp_pos[i][1]) + pos[i][1];
			}

			if (block_crash(temp_pos,player_map) == 1)
				break;
			else if (block_crash(temp_pos,player_map) == 0)
			{
				for (i = 0; i < 4; i++)
				{
					player_map[pos[i][1]][pos[i][0]] = 0;
				}
				for (i = 0; i < 4; i++)
				{
					pos[i][0] = temp_pos[i][0];
					pos[i][1] = temp_pos[i][1];
					player_map[pos[i][1]][pos[i][0]] = 1;
				}
			}
			break;
		}
		case 6:
		{
			for (i = 0; i < 4; i++)
			{
				temp_pos[i][0] = pos[i][0] - pos[2][0];
				temp_pos[i][1] = pos[i][1] - pos[2][1];
			}
			for (i = 0; i < 4; i++)
			{
				
				temp2_pos[i][0] = -temp_pos[i][1];
				temp2_pos[i][1] = -temp_pos[i][0];

				if (i == 0)
				{
					temp2_pos[i][0] = -temp_pos[i][0];
					temp2_pos[i][1] = temp_pos[i][1];
				}
				else if (i == 3)
				{
					temp2_pos[i][0] = temp_pos[i][1];
					temp2_pos[i][1] = temp_pos[i][0];
				}

			}
			for (i = 0; i < 4; i++)
			{
				temp_pos[i][0] = (temp2_pos[i][0] - temp_pos[i][0]) + pos[i][0];
				temp_pos[i][1] = (temp2_pos[i][1] - temp_pos[i][1]) + pos[i][1];
			}

			if (block_crash(temp_pos,player_map) == 1)
				break;
			else if (block_crash(temp_pos,player_map) == 0)
			{
				for (i = 0; i < 4; i++)
				{
					player_map[pos[i][1]][pos[i][0]] = 0;
				}
				for (i = 0; i < 4; i++)
				{
					pos[i][0] = temp_pos[i][0];
					pos[i][1] = temp_pos[i][1];
					player_map[pos[i][1]][pos[i][0]] = 1;
				}
			}
			break;
		
		}

	}
}
int block_crash(int temp_pos[4][2],int player_map[HEIGHT][WIDTH]) {
	int i,j;
	
	re:
	for (i = 0; i < 4; i++) {
		if (player_map[temp_pos[i][1]][temp_pos[i][0]] == -1)
			return 1;
	}
	for (i = 0; i < 4; i++) {
		if (temp_pos[i][0] <= 0)
		{
			for (j = 0; j < 4; j++)
			{
				temp_pos[j][0]++;
			}
			goto re;
		}
		else if (temp_pos[i][1] >= HEIGHT - 1)
		{
			for (j = 0; j < 4; j++)
			{
				temp_pos[j][1]--;
			}
			goto re;
		}
		else if(temp_pos[i][0] >= WIDTH-1)
		{
			for (j = 0; j < 4; j++)
			{
				temp_pos[j][0]--;
			}
			goto re;
		}
	}
	return 0;
}
void block_clear(int player_map[HEIGHT][WIDTH]) {
	int i, j,z,flag;
	
	for (i = HEIGHT-2; i >0; i--)
	{
		flag = 0;
		for (j = 1; j < WIDTH-1; j++)
		{
			if (player_map[i][j] != -1)
			{
				flag = 1;
				break;
			}
		}
		if (flag == 1)
			continue;
		for (z = i; z > 4; z--)
		{
			for (j = 1; j < WIDTH - 1; j++)
			{
				player_map[z][j] = player_map[z - 1][j];
			}
		}
		i++;
		
		
	}
}
void draw_box2(int x1, int y1, int x2, int y2, char* ch)
{
	int x, y;

	for (x = x1; x <= x2; x += 2)
	{
		gotoxy(x, y1);
		printf("%s", ch);
		gotoxy(x, y2);
		printf("%s", ch);
	}
	for (y = y1; y <= y2; y++)
	{
		gotoxy(x1, y);
		printf("%s", ch);
		gotoxy(x2, y);
		printf("%s", ch);
	}
	
}
void printxy(int x, int y, char* ch) {
	gotoxy(x, y);
	printf("%s", ch);
}



void show_block()
{
	int x, y;
	
	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			gotoxy(x*2+15 , y+5);
			if (player1_map[y][x] == 2) {
				printf("▩"); // ㅁ 한자.
			}
			else if (player1_map[y][x] == 1)
			{
				textcolor(block1_n+1, BLACK);
				printf("■");
				textcolor(WHITE, BLACK);
			}
			else if (player1_map[y][x] == -1)
				printf("□");
			else if (player1_map[y][x] == 3)
				printf("ㅡ");
			else
				printf("  ");
		}
	}
	
	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			gotoxy(x * 2 + 75, y + 5);
			if (player2_map[y][x] == 2) {
				printf("▩"); // ㅁ 한자.
			}
			else if (player2_map[y][x] == 1)
			{
				textcolor(block2_n + 3, BLACK);
				printf("■");
				textcolor(WHITE, BLACK);
			}
			else if (player2_map[y][x] == -1)
				printf("□");
			else if (player2_map[y][x] == 3)
				printf("ㅡ");
			else
				printf("  ");
		}
	}
}

void initmap()
{
	int x, y;
	for (y = 0; y < HEIGHT; y++)
	{
		player1_map[y][0] = 2;
		player2_map[y][0] = 2;
		player1_map[y][WIDTH - 1] = 2;
		player2_map[y][WIDTH - 1] = 2;
	}
	for (x = 0; x < WIDTH; x++)
	{
		player1_map[HEIGHT-1][x] = 2;
		player2_map[HEIGHT - 1][x] = 2;
	}
	for (x = 1; x < WIDTH -1; x++)
	{
		player1_map[3][x] = 3;
		player2_map[3][x] = 3;
	}
	
}
void player1(unsigned char ch)
{
	
	switch (ch)
	{
		case UP:
			rorate_flag1++;
			block_rorate(pos1,block1_n,player1_map,rorate_flag1);
			show_block();
			break;
		case LEFT:
			block_LEFT(player1_map,pos1);
			show_block();
			break;
		case RIGHT:
			block_RIGHT(player1_map,pos1);
			show_block();
			break;
		case DOWN:
			if (block_down(player1_map,pos1) == 0)
			{
				count1++;
			}
			show_block();
			break;
		case DROP1:
			while (count1 != 4)
			{
				if (block_down(player1_map,pos1) == 0)
					count1++;

			}
			show_block();
		
		default:
			break;
	}
}
void player2(unsigned char ch)
{

	switch (ch)
	{
	case UP2:
		rorate_flag2++;
		block_rorate(pos2, block2_n, player2_map,rorate_flag2);
		show_block();
		break;
	case LEFT2:
		block_LEFT(player2_map,pos2);
		show_block();
		break;
	case RIGHT2:
		block_RIGHT(player2_map,pos2);
		show_block();
		break;
	case DOWN2:
		if (block_down(player2_map, pos2) == 0)
		{
			count2++;
		}
		show_block();
		break;
	case DROP2:
		while (count2 != 4)
		{
			if (block_down(player2_map, pos2) == 0)
				count2++;

		}
		show_block();
		break;
	default:
		break;
	}
}
void new_block(int block_n, int player)
{
	int x, y,start_x=5;
	int out_flag=0;
	int i = 0;
	if (player == 1) {
		
		for (y = 0; y < 4; y++)
		{
			for (x = 0; x < 4; x++)
			{
				if(block[block1_n][y][x]!=0||player1_map[y][start_x]!=3)				
					player1_map[y][start_x] = block[block1_n][y][x];
				
				start_x++;
				if (block[block1_n][y][x] == 1)
				{
					pos1[i][0] = start_x - 1;
					pos1[i++][1] = y;
				}
			}
			start_x = 5;
		}
		rorate_flag1 = 0;
	}
	else if(player ==2)
	{
		for (y = 0; y < 4; y++)
		{
			for (x = 0; x < 4; x++)
			{
				if (player2_map[y][start_x] != -1)
				{
					player2_map[y][start_x] = block[block2_n][y][x];
				}
				start_x++;
				if (block[block2_n][y][x] == 1)
				{
					pos2[i][0] = start_x - 1;
					pos2[i++][1] = y;
				}
			}
			start_x = 5;
		}
		rorate_flag2 = 0;
	}

}
void show_nextblock() {
	int x, y;
	printxy(55, 4, "NEXT");
	printxy(45, 5, "▩▩▩▩▩▩▩▩▩▩▩▩▩");
	printxy(45, 6, "▩          ▩          ▩");
	printxy(45, 7, "▩          ▩          ▩");
	printxy(45, 8, "▩          ▩          ▩");
	printxy(45, 9, "▩          ▩          ▩");
	printxy(45, 10, "▩          ▩          ▩");
	printxy(45, 11, "▩▩▩▩▩▩▩▩▩▩▩▩▩");
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			gotoxy((x*2)+48, y+7);
			if (block[block1_next][y][x] == 1)
				printf("■");
			else if(block[block1_next][y][x]==0)
				printf("  ");
		}
	}
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			gotoxy((x * 2) + 61, y + 7);
			if (block[block2_next][y][x] == 1)
				printf("■");
			else if (block[block2_next][y][x] == 0)
				printf("  ");
		}
	}
}
void show_holdblock() {
	int x, y;
	printxy(55, 14, "HOLD");
	printxy(45, 15, "▩▩▩▩▩▩▩▩▩▩▩▩▩");
	printxy(45,16, "▩          ▩          ▩");
	printxy(45, 17, "▩          ▩          ▩");
	printxy(45, 18, "▩          ▩          ▩");
	printxy(45, 19, "▩          ▩          ▩");
	printxy(45, 20, "▩          ▩          ▩");
	printxy(45, 21, "▩▩▩▩▩▩▩▩▩▩▩▩▩");
	if (block1_hold != -1) {
		for (y = 0; y < 4; y++) {
			for (x = 0; x < 4; x++) {
				gotoxy((x * 2) + 48, y + 17);
				if (block[block1_hold][y][x] == 1)
					printf("■");
				else if (block[block1_hold][y][x] == 0)
					printf("  ");
			}
		}

	}
	if (block2_hold != -1) {
		for (y = 0; y < 4; y++) {
			for (x = 0; x < 4; x++) {
				gotoxy((x * 2) + 61, y + 17);
				if (block[block2_hold][y][x] == 1)
					printf("■");
				else if (block[block2_hold][y][x] == 0)
					printf("  ");
			}
		}
	}
}
void check_finishline() {
	int x, y;

	for (x = 1; x < WIDTH - 1; x++)
	{
		if (player1_map[3][x] == 0)
			player1_map[3][x] = 3;
		else if (player2_map[3][x] == 0)
			player2_map[3][x] = 3;
	}
	
}

int gamestart() {

	block1_n = block1_next;
	block2_n = block2_next;
	block1_next = rand() % 7;
	block2_next= rand()% 7;
	int start_time = time(NULL);
	int run_time,remain_time=start_time;
	int x, y;
	int start_x = 5;
	int temp;
	int j, z;
	int i = 0;
	unsigned char ch;
	
	new_block(block1_n, 1);
	new_block(block2_n, 2);
	show_holdblock();
	show_block();
	while (1)
	{
		show_nextblock();
		check_finishline();
		run_time = time(NULL)-start_time ;
		if (run_time !=remain_time)
		{
			if (block_down(player1_map,pos1) == 0)
			{
				count1++;
			}
			if (block_down(player2_map, pos2) == 0)
			{
				count2++;
			}
			show_block();
			remain_time = run_time;
		}
		if (kbhit() == 1) {
			ch = getch();
			if (ch == SPECIAL1)
			{
				ch = getch();
				switch (ch)
				{
				case UP:
				case LEFT:
				case RIGHT:
				case DOWN:
					player1(ch);
					break;
				}
			}
			else if (ch == DROP1)
			{
				player1(ch);
			}
			else if (ch == HOLD1)
			{
				if (block1_hold == -1)
				{
					for (i = 0; i < 4; i++)
					{
						player1_map[pos1[i][1]][pos1[i][0]] = 0;

					}
					
					block1_hold = block1_n;
					block1_n = block1_next;
					new_block(block1_n, 1);
					show_block();
					show_holdblock();
					continue;
				}
				else
				{
					for (i = 0; i < 4; i++)
					{
						player1_map[pos1[i][1]][pos1[i][0]] = 0;

					}
					check_finishline();
					temp = block1_n;
					block1_n = block1_hold;
					block1_hold = temp;
					show_holdblock();
					new_block(block1_n, 1);
					
					continue;
				}
				
			}
			else
			{
				switch (ch)
				{
				case UP2:
				case LEFT2:
				case RIGHT2:
				case DOWN2:
				case DROP2:
					player2(ch);
					break;
				case HOLD2:
				{	
					if (block2_hold == -1)
					{
					for (i = 0; i < 4; i++)
					{
						player2_map[pos2[i][1]][pos2[i][0]] = 0;

					}
					block2_hold = block2_n;
					block2_n = block2_next;
					new_block(block2_n, 2);
					show_block();
					show_holdblock();
					continue;
				}
				else
				{
					for (i = 0; i < 4; i++)
					{
						player2_map[pos2[i][1]][pos2[i][0]] = 0;

					}
					temp = block2_n;
					block2_n = block2_hold;
					block2_hold = temp;
					show_holdblock();
					new_block(block2_n, 2);
					continue;
				}
				
				}
				}
			}
		}
		check_finishline();
		Sleep(50);
		if (count1 == 4)
		{
			for (i = 0; i < 4; i++)
			{
				player1_map[pos1[i][1]][pos1[i][0]] = -1;
				
			}
			block_clear(player1_map);
			for (i = 0; i < 4; i++)
			{
				if (player1_map[pos1[i][1]][pos1[i][0]+1]==3|| player1_map[pos1[i][1]][pos1[i][0] -1] == 3)
				{
					for (j = 0; j < HEIGHT; j++)
					{
						for (z = 0; z < WIDTH; z++)
							player1_map[j][z] = gameover[j][z];
						show_block();
						Sleep(100);
					}
					while (1)
					{
						if (kbhit() == 1)
						{
							ch = getch();
							if (ch ==ESC)
								return 2;
						}
					}
				}

			}
			show_block();
			count1 = 0;
			block1_n = block1_next;
			block1_next = rand() % 7;
			new_block(block1_n, 1);
		}
		else if (count2 == 4)
		{
			{
				for (i = 0; i < 4; i++)
				{
					player2_map[pos2[i][1]][pos2[i][0]] = -1;

				}
				block_clear(player2_map);
				for (i = 0; i < 4; i++)
				{
					if (player2_map[pos2[i][1]][pos2[i][0] + 1] == 3 || player2_map[pos2[i][1]][pos2[i][0] - 1] == 3)
					{
						for (j = 0; j < HEIGHT; j++)
						{
							for (z = 0; z < WIDTH; z++)
								player2_map[j][z] = gameover[j][z];
							show_block();
							Sleep(100);
						}
						while (1)
						{
							if (kbhit() == 1)
							{
								ch = getch();
								if (ch ==ESC)
									return 2;
							}
						}
					}

				}
				show_block();
				count2 = 0;
				block2_n = block2_next;
				
				block2_next = rand() % 7;
				new_block(block2_n, 2);
			}
		}
		
	}

}
void draw_box3(int x1, int y1, int x2, int y2)
{
	int x, y;
	//int len = strlen(ch);
	for (x = x1; x <= x2; x += 2) { // 한글은 2칸씩 차지한다.
		gotoxy(x, y1);
		printf("%s", "─"); // ㅂ 누르고 한자키 누르고 선택
		gotoxy(x, y2);
		printf("%s", "─");
	}
	for (y = y1; y <= y2; y++) {
		gotoxy(x1, y);
		printf("%s", "│");
		gotoxy(x2, y);
		printf("%s", "│");
	}
	gotoxy(x1, y1); printf("┌");
	gotoxy(x1, y2); printf("└");
	gotoxy(x2, y1); printf("┐");
	gotoxy(x2, y2); printf("┘");
}
void gameexplain() {
	unsigned char ch;
	cls(GRAY1, BLACK);
	printxy(50, 3, "Game Explain");
	draw_box3(30, 5, 80, 25);
	printxy(40, 7, "Player 1             Player 2");
	printxy(35,9,"Move : ◁  ▷             a , d");
	printxy(35, 11, "Rorate : △                 w");
	printxy(35, 13, "Down: ▽                    s");
	printxy(35, 15, "Hard Drop: Space Bar        g");
	printxy(35, 17, "Hold: v                     h");
	printxy(50, 22, "돌아가기 : esc");
	while (1)
	{
		if (kbhit() == 1)
		{
			ch = getch();
			if (ch == ESC)
				break;

		}
	}

}
void main() {
	srand(time(NULL));
	removeCursor();
	unsigned char ch;
	int winner;
	while (1)//초기화면
	{
		//printtitle(20,5,"□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□");
		printtitle(25, 6, "■■■■■  ■■■■■  ■■■■■  ■■■■  ■■■  ■■■■■");
		printtitle(25, 7, "    ■      ■              ■      ■    ■    ■    ■        ");
		printtitle(25, 8, "    ■      ■■■■■      ■      ■■■■    ■    ■■■■■");
		printtitle(25, 9, "    ■      ■              ■      ■  ■      ■            ■");
		printtitle(25, 10, "    ■      ■■■■■      ■      ■    ■  ■■■  ■■■■■");
		//printtitle(20,11,"□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□");

		printxy(30, 20, "게임 시작 : enter");
		printxy(30, 22, "게임 설명: e");
		printxy(40, 30, "made by 컴퓨터공학부 신재영");
		if (kbhit() == 1)
		{
			ch = getch();
			if (ch == ENTER)
			{
				cls(GRAY1, BLACK);

				break;
				//gamestart
			}
			else if (ch == 'e' || ch == 'E')
			{
				gameexplain(); //게임설명창 esc누르면 다시돌아오게 만들기
				cls(GRAY1, BLACK);
			}
		}
	}
	
	//---------------------------
		initmap();
		show_block();
		winner=gamestart();
		cls(GRAY1, BLACK);
		
		draw_box3(20, 5, 110, 25);
		textcolor(YELLOW1, BLACK);
		printxy(27, 8,"■	  ■	■   ■    ■       ■    ■       ■    ■■■■   ■■■■■");
		printxy(27, 9,"   ■  ■  ■     ■    ■ ■    ■    ■ ■    ■    ■         ■      ■");
		printxy(27, 10,"	■■■	     ■    ■   ■  ■    ■    ■ ■    ■■■■   ■■■■■");
		printxy(27, 11,"       ■ 	     ■    ■     ■■    ■     ■■    ■         ■ ■");
		printxy(27, 12,"       ■         ■    ■     ■■    ■     ■■    ■■■■   ■   ■■");
		gotoxy(60, 20);
		printf("Player %d\n", winner);
		while (1)
		{

		}
		
}