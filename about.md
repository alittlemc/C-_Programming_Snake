## 说明
* > 基于`佚名`开源项目`超级贪吃蛇`该进与优化,并且写了说明与解析
* > 程序推荐 编译环境 :Visual Studio 2017 (VS)

## 游戏的实质:`解析dos框内的位置可以用自定义实现一些功能`
* > 设置光标位置的坐标,可以实现自定义打印:
``` C++
void gotoxy(int x, int y) //位置函数
{
	COORD pos;//调用Windows内置API-光标位置
	pos.X=2*x;//半屏游戏
	pos.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);//导入位置
}
```
* > 设置打印颜色,可以实现区分不同的
``` C++
inline void color(int a)
{//设置颜色,这里使用inline内联,可以优化内存
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),a);
}
```

## 游戏的设计
1. 游戏地图 为大小为N*N(正方形),最外围是围墙N+1
2. 字符★ ● □ ■ 分别是 蛇 果 空气 墙
3. 使用 w a s d 来移动
4. 吃到苹果加分数,分数的添加是随机的,分数越高,速度越快
5. 蛇的尾巴延迟 和 加长
6. 撞墙游戏结束

## 代码
> 地图初始化
``` C++
void init(int apple[2])//初始化函数（初始化围墙、显示信息、苹果）
{
	int i,j;//初始化围墙
	int wall[N+2][N+2]={{0}};//0为空气1为墙
	for(i=1;i<=N;i++) 
		for(j=1;j<=N;j++)
			wall[i][j]=1;//设置全部坐标

	color(11);//
	
	for(i=0;i<N+2;i++)
	{
		for(j=0;j<N+2;j++)
			if(wall[i][j])
				cout<<"■";
			else
				cout<<"□" ;
        
		cout<<endl;
	}
  
	//右边提示内容
	gotoxy(N+3,1);//显示"提示",在游戏区域N+2外
	color(20);
	cout<<"按 W S A D 移动方向"<<endl;

	gotoxy(N+3,2);//显示在下一行
	color(20);
	cout<<"按任意键暂停"<<endl;

	gotoxy(N+3,3);
	color(20);
	cout<<"得分："<<endl;

	apple[0]=rand()%N+1;//随机生成苹果位置
	apple[1]=rand()%N+1;
	gotoxy(apple[0],apple[1]);

	color(12);
	cout<<"●"<<endl;
}
```
> 准备参数
``` C++
  int i,j;//i用于循环 ,j用于存放技能的判定(暂时未实现)
	int** snake=NULL;//存放位置单元snake[x][y]
	int apple[2];//设置苹果位置,apple[1]=x;apple[2]=y;
	int score=0;//记录时间
	int tail[2];//尾巴位置,延缓更新
	int len=3;//初始长度值
	char ch='p';//存放_getch()
	srand((unsigned)time(NULL));//随机数发生器的初始化函数,获取时间戳并且设置为种子,实现伪随机
	init(apple);//调用初始化函数,传递苹果指针位置,用于初始化
```
> 开辟堆空间以及尾巴
``` C++
	snake=(int**)realloc(snake,sizeof(int*)*len);//通过realloc扩大p的空间，并把新的地址赋值
	for(i=0;i<len;i++)
		snake[i]=(int*)malloc(sizeof(int)*2);
    //动态开辟堆空间,用于存放尾巴的位置,因为尾巴可以延长所以需要动态开辟
		for(i=0;i<len;i++)//尾巴
		{
			snake[i][0]=N/2;
			snake[i][1]=N/2+i;
			gotoxy(snake[i][0],snake[i][1]);
			color(14);
			cout<<"★"<<endl;
		}
 ```
 ## 游戏的主体
 ```C++
 while(1)//循环
		{	
			//在外围设置墙
			tail[0]=snake[len-1][0];
			tail[1]=snake[len-1][1];
			gotoxy(tail[0],tail[1]);
			color(11);
			cout<<"■"<<endl;

			for(i=len-1;i>0;i--)
			{
				snake[i][0]=snake[i-1][0];
				snake[i][1]=snake[i-1][1];
				gotoxy(snake[i][0],snake[i][1]);
				color(14);
				cout<<"★"<<endl;
			}

			if(kbhit())//kbhit()检查当前是否有键盘输入,若有则返回一个非0值,否则返回0,用于检测按键更新 conio
			{
				gotoxy(0,N+2);//在下方显示
				ch=getche();//获取当前键入,设置为ch用于控制移动
				cout<<ch<<endl;
			}

			switch(ch)//设置移动,按下按键后不修改,可以持续移动
			{
				case 'w':snake[0][1]--;break;
				case 's':snake[0][1]++;break;
				case 'a':snake[0][0]--;break;
				case 'd':snake[0][0]++;break;
				default: break;
			}

			gotoxy(snake[0][0],snake[0][1]);
			color(14);
			cout<<"★"<<endl;
			Sleep((j ? 100 : (score < 400 ? abs((int)(400 - 0.3*score)) : 1)));//设置刷新时间 分数//设置刷新时间 分数

			if(snake[0][0]==apple[0]&&snake[0][1]==apple[1])//吃掉苹果后蛇分数加1，蛇长加1 位置重合
			{
				score = score + rand() % N;//分数加
				len++;//蛇加长
				snake=(int**)realloc(snake,sizeof(int*)*len);//重新设置空间
				snake[len-1]=(int*)malloc(sizeof(int)*2);

				apple[0]=rand()%N+1;//生成新的苹果
				apple[1]=rand()%N+1;
				gotoxy(apple[0],apple[1]);
				color(12);
				cout<<"●"<<endl;
				gotoxy(N+5,3);
				color(20);
				cout<<score<<endl;
			}
			gotoxy(N+3,5);//显示在下一行
			color(20);
			cout <<"当前位置 X="<<tail[0]<<"Y="<<tail[1]<<endl;

			if (snake[0][1] == 0 || snake[0][1] == N || snake[0][0] == 0 || snake[0][0] == N) //撞到围墙后失败
			{
				gotoxy(N / 2, N / 2);//在游戏界面中间显示游戏结束,和分数
				color(30);
				cout << "游戏结束,分数:"<<score<< endl;

				for (i = 0; i < len; i++)
					free(snake[i]); //释放全部的内存

				Sleep(INFINITE);//0xffffffff最大值,游戏结束无需更新
				exit(0);
			}
		}
 ```

## 游戏的难题与解决代码
1. 可以实现按键控制移动 
``` C++
  if(kbhit())//kbhit()检查当前是否有键盘输入,若有则返回一个非0值,否则返回0,用于检测按键更新 conio
	{
		gotoxy(0,N+2);//在下方显示
		ch=getche();//获取当前键入,设置为ch用于控制移动
		cout<<ch<<endl;
	}
	switch(ch)//设置移动,按下按键后不修改,可以持续移动
	{
		case 'w':snake[0][1]--;break;
		case 's':snake[0][1]++;break;
		case 'a':snake[0][0]--;break;
		case 'd':snake[0][0]++;break;
		default: break;
	}
```
2. 撞墙游戏结束
``` C++
//头的位置到达:x或y 到达 0或N
snake[0][1] == 0 || snake[0][1] == N || snake[0][0] == 0 || snake[0][0] == N
```
3. 随机数
``` C++
srand((unsigned)time(NULL));//初始化

r = rand() % N//调用
```
