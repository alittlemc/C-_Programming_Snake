#include<iostream>
#include<windows.h>
#include<ctime>//获取时间戳
#include<cstdlib>
#include<conio.h>//键盘事件
using namespace std;
#define N 30//游戏布局范围(正方形)
constexpr auto DIE = 3;//复活等待时间;


//★ ● □ ■//蛇 果 空气 墙

void gotoxy(int x, int y) //位置函数
{
	COORD pos;//调用Windows内置API-光标位置
	pos.X = 2 * x;//半屏游戏
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);//导入位置
}

inline void color(int a) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a); }//颜色

inline bool cout_ch(int x, int y, int id, const char* ch = "")//方便输出信息
{
	color(id);
	gotoxy(x,y);
	cout << ch << endl;
	return 0;
}

void init(int apple[2])//初始化函数（初始化围墙、显示信息、苹果）
{
	int i, j;//初始化围墙
	int wall[N + 2][N + 2] = { {0} };//0为空气1为墙
	for (i = 1; i <= N; i++)
		for (j = 1; j <= N; j++)
			wall[i][j] = 1;//设置全部坐标

	color(11);
	//设置地图
	for (i = 0; i < N + 2; i++)
	{
		for (j = 0; j < N + 2; j++)
			if (wall[i][j])
				cout << "■";
			else
				cout << "□";
		cout << endl;
	}
	
	//提示 在游戏区域(N+2)外(N+3)
	cout_ch(N + 3, 1, 10, "按 W S A D 移动方向,按任意键暂停");
	cout_ch(N + 3, 2, 10, "分数是随机加的,分数越高速度越快,得分也越高");
	cout_ch(N + 3, 3,14, "得分:");

	apple[0] = rand() % N + 1;//随机生成苹果位置
	apple[1] = rand() % N + 1;

	cout_ch(apple[0], apple[1], 12, "●");
	cout_ch(N + 3, 4, 12, "苹果:");
	cout_ch(N + 3, 5, 15, "速度:");
	cout_ch(N + 3, 6,13, "死亡:");
	cout_ch(N + 5, 3, 14, "0");
	cout_ch(N + 5, 4, 12, "0");
	cout_ch(N + 5, 5, 15, "10");
	cout_ch(N + 5, 6,13, "0");
}

int main(int argc,char *argv[])
{
	int i, die = 0,j = 0;//i用于循环 j用于复活 die用于记录复活
	int** snake = NULL;//存放位置单元snake[x][y]
	int apple[2];//设置苹果位置,apple[1]=x;apple[2]=y;
	int score = 0;
	int tail[2];//尾巴位置,延缓更新
	int len = 3;//初始长度值
	char ch = 'p';
	srand((unsigned)time(NULL));//随机数发生器的初始化函数,获取时间戳并且设置为种子
	init(apple);//初始化
	snake = (int**)realloc(snake, sizeof(int*)*len);//通过realloc扩大p的空间，并把新的地址赋值给p

	for (i = 0; i < len; i++)
		snake[i] = (int*)malloc(sizeof(int) * 2);//开辟堆空间
	
	gotoxy(0, N + 3);
	for (i = 0; i < len; i++)
	{
		snake[i][0] = N / 2;
		snake[i][1] = N / 2 + i;
		cout_ch(snake[i][0], snake[i][1], 14, "★");
	}
redo:
	while(1)//循环
	{
		tail[0] = snake[len - 1][0];
		tail[1] = snake[len - 1][1];
		cout_ch(tail[0], tail[1], 11, "■");
		for (i = len - 1; i > 0; i--)
		{
			snake[i][0] = snake[i - 1][0];
			snake[i][1] = snake[i - 1][1];
			cout_ch(snake[i][0], snake[i][1], 14, "★");
		}

		if (_kbhit())//kbhit()检查当前是否有键盘输入,若有则返回一个非0值,否则返回0,用于检测按键更新 conio
		{
			gotoxy(0, N + 2);//在下方显示
			ch = _getche();//获取当前键入
			//cout << ch << endl;
		}

		switch (ch)
		{
			case 'w':snake[0][1]--; break;
			case 's':snake[0][1]++; break;
			case 'a':snake[0][0]--; break;
			case 'd':snake[0][0]++; break;
			case 'r':j = 1; goto end; break;
			default: break;
		}

		cout_ch(snake[0][0], snake[0][1], 10, "★");//绿帽子代表力量
		Sleep((j ? 100 : (score < 400 ? abs((int)(400 - 0.8*score)) : 1)));//设置刷新时间 分数
		if (snake[0][0] == apple[0] && snake[0][1] == apple[1])//吃掉苹果后蛇分数加1，蛇长加1 位置重合
		{
			score = score + (int)((rand()% N)*(len-2)*0.5);//分数加
			len++;//蛇加长
			snake = (int**)realloc(snake, sizeof(int*)*len);//重新设置空间
			snake[len - 1] = (int*)malloc(sizeof(int) * 2);

			apple[0] = rand() % N + 1;//生成新的苹果
			apple[1] = rand() % N + 1;
			cout_ch(apple[0], apple[1], 1 + rand() % 14, "●");//苹果颜色随机

			gotoxy(N + 5, 3);
			color(14);
			cout << score << endl;

			gotoxy(N + 5, 4);
			color(12);
			cout <<len - 3<< endl;

			gotoxy(N + 5, 5);
			color(15);
			cout << 410 - (score < 400 ? abs((int)(400 - 0.8*score)) : 1) << endl;
		}

		gotoxy(N + 3, 7);//显示在下一行
		color(10);
		cout << "当前位置 X=" << snake[0][0] << " Y=" << snake[0][1] << endl;

		if(snake[0][1] < 0 || snake[0][1] > N || snake[0][0] < 0 || snake[0][0] > N)//出地图拉回来
		{
			snake[0][1] = rand() % N;
			snake[0][0] = rand() % N;
		}
		if (snake[0][1] == 0 || snake[0][1] == N || snake[0][0] == 0 || snake[0][0] == N)//撞到围墙后失败
		{
end:
			gotoxy(N / 2-5, N / 2);//在游戏界面中间显示游戏结束,和分数
			color(30);
			cout << "游戏结束,分数:" << score <<" 苹果:"<<len-3<<endl;
			
			gotoxy(N + 5, 6);
			color(13);
			cout << ++die << endl;
			if (j != 1)
			{
				for (i = 0; i < DIE; i++)
				{
					gotoxy(N + 3, 8);
					cout <<DIE-i<< "秒后复活" << endl;
					Sleep(1000);
				}
				score = (int)(score*0.5);//复活扣分
				snake[0][1] = rand() % N;
				snake[0][0] = rand() % N;
				goto redo;
			}
			for (i = 0; i < len; i++)
				free(snake[i]); //释放全部的内存
			Sleep(INFINITE);//0xffffffff最大值,游戏结束无需更新
			exit(0);
		}
	}
	return 0;
}
