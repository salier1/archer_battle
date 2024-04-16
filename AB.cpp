#include<stdio.h>
#include<easyx.h>
#include<conio.h>
#include<time.h>
#include<math.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
IMAGE bk;
IMAGE hero;
IMAGE role_l[2];
IMAGE role_r[2];
IMAGE enemy[3];
IMAGE arrow_l[2];
IMAGE arrow_r[2];
IMAGE w1;
IMAGE l1;
IMAGE menv;
const int WIDTH = 800;
const int HEIGHT = 450;
const double G = 0.5;
const double PI = 3.14;
int bulletnum;

int menu()
{
	mciSendString("open ./sounds/menu.mp3 alias BGM1", 0, 0, 0);
	mciSendString("play BGM1 repeat", 0, 0, 0);
	putimage(0, 0, &menv);
	ExMessage msg;
	while (true)
	{
		if (peekmessage(&msg, EM_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
				if (msg.x >= 250 && msg.x <= 550 && msg.y >= 200 && msg.y <= 250)
				{
					mciSendString("close BGM1 ", 0, 0, 0);
					return 0;
				}
				else if(msg.x >= 250 && msg.x <= 550 && msg.y >= 300 && msg.y <= 350)
				{
					exit(888);
				}
			}
		}
	}

}
struct player  
{
	double w, h;
	double x, y;//��������
	double yy;//��¼�����ʱ��y����
	double speed; //��������
	bool isalive;//������״̬
	bool l_r;// ���﷽��
	double power;// ����
	bool canrecycle;//���Ƿ��ܹ�����
	int hp;//Ѫ��
	bool damaged;//�Ƿ���ɹ��˺�
	double ang;//�ӵ��Ƕ�

}hero1,enemy1,arrow,bullet[7];
//�ýṹ����������  


void imgLoad()
{
	loadimage(&bk, "./pics/bg1.jpg");//����
	loadimage(&hero, "./pics/role.png");
	loadimage(&role_l[0], "./pics/role_l0.png");
	loadimage(&role_l[1], "./pics/role_l1.png");
	loadimage(&role_r[0], "./pics/role_r0.png");
	loadimage(&role_r[1], "./pics/role_r1.png");
	loadimage(&arrow_r[0], "./pics/arrow_r0.png");
	loadimage(&arrow_r[1], "./pics/arrow_r1.png");
	loadimage(&arrow_l[0], "./pics/arrow_l0.png");
	loadimage(&arrow_l[1], "./pics/arrow_l1.png");
	loadimage(&enemy[0], "./pics/enemy0.png");
	loadimage(&enemy[1], "./pics/enemy1.png");
	loadimage(&enemy[2], "./pics/enemy1_1.png");
	loadimage(&w1, "./pics/win.png");
	loadimage(&l1, "./pics/lose.png");
	loadimage(&menv, "./pics/menu.jpg");
}
//����ͼƬ
void gameInit()
{

	mciSendString("close BGM3 ", 0, 0, 0);
	mciSendString("close BGM4 ", 0, 0, 0);
	mciSendString("open ./sounds/battle.mp3 alias BGM2", 0, 0, 0);
	mciSendString("play BGM2 repeat", 0, 0, 0);
	hero1.x = 50;
	hero1.y = HEIGHT - 100;
	hero1.isalive = true;
	hero1.l_r = false;
	hero1.speed = 0.1;
	hero1.power = 5;
	hero1.hp = 10;
	hero1.w = 70;
	hero1.h = 76;
	
	arrow.isalive = false;
	arrow.canrecycle = false;
	arrow.w = 40;
	arrow.h = 5;

	enemy1.x = WIDTH / 2-100;
	enemy1.y = HEIGHT - 400;
	enemy1.hp = 10;
	enemy1.speed = 0.05;
	enemy1.l_r = true;
	enemy1.w = 200;
	enemy1.h = 212;
	bulletnum = 3;

	for (int i = 0; i < 7; i++)
	{
		bullet[i].isalive = false;  
		bullet[i].damaged = false;
		bullet[i].speed = 0.1;
		

	}
}


void enemy_Move()
{

	if (enemy1.y<0 || enemy1.y>HEIGHT - enemy1.h)
	{
		enemy1.l_r = !enemy1.l_r;
	}

	if (enemy1.l_r) {
		enemy1.y += enemy1.speed;
	}
	else
	{
		enemy1.y -= enemy1.speed;
	}

}



void enemy_Damage()
{
	if (!arrow.damaged) {
		if (arrow.x > enemy1.x - arrow.w && arrow.x<enemy1.x + enemy1.w 
			&& arrow.y>enemy1.y - arrow.h && arrow.y < enemy1.y + enemy1.h)
		{
			arrow.damaged = true;
			enemy1.hp -= 1;
		}
	}
}

void hero_Damage()
{
	for (int i = 0; i < bulletnum; i++)
	{
		if (bullet[i].isalive && !bullet[i].damaged)
		{
			if (bullet[i].x > hero1.x - 20 && bullet[i].x<hero1.x + hero1.w 
				&& bullet[i].y>hero1.y - 20 && bullet[i].y < hero1.y + hero1.h)
			{
				bullet[i].isalive = false;
				bullet[i].damaged = true;
				hero1.hp -= 1;
			}

		}


	}



}

//���˹�����bulllet hp-1������enemy1ֱ�ӱ���ɱ��
void enemy_Attack()
{
	for (int i = 0; i < bulletnum; i++)
	{
		if (!bullet[i].isalive)
		{

			bullet[i].isalive = true;
			bullet[i].damaged = false;
			bullet[i].x = enemy1.x + enemy1.w / 2;
			bullet[i].y = enemy1.y;
			bullet[i].ang = rand() * PI;
		}
	}
}
void bullet_Fly()
{
	for (int i = 0; i < bulletnum; i++)
	{
		if (bullet[i].isalive)
		{
			
				bullet[i].x += bullet[i].speed * sin(bullet[i].ang - PI / 2);
				
				if (cos(bullet[i].ang - PI / 2)>0)
				{
					bullet[i].y += bullet[i].speed * cos(bullet[i].ang - PI / 2);
				}
				else
				{
					bullet[i].y -= bullet[i].speed * cos(bullet[i].ang - PI / 2);
				}

		}
		if (bullet[i].x<0 || bullet[i].x > WIDTH-20 || bullet[i].y > HEIGHT-20 || bullet[i].y < 0)
		{
			bullet[i].isalive = false;
		}
	}

}
void power_Change()
{
	if (hero1.power < 5)
	{
		hero1.power += 0.002;
	}

}
//power��ʱ��䶯
void arrow_Shoot() {
	arrow.isalive = true;//��Ϊ���״̬
	arrow.damaged = false;
	arrow.x = hero1.x;
	arrow.y = hero1.y;
	arrow.l_r = hero1.l_r;
	arrow.power = hero1.power;
	arrow.yy = hero1.y+50;
	if (hero1.power >= 2)
	{
		hero1.power -= 2;
	}
	else
	{
		hero1.power = 0;
	}
}
//�������״̬�ı�
void arrow_Fly() //40*5

{

	if (arrow.isalive) 
	{
		if (arrow.y < arrow.yy)  //��ʸС���������y��
		{
			if (arrow.x<0 || arrow.x>WIDTH - arrow.w)//��ʸ�����߽練��
			{
				arrow.l_r = !arrow.l_r;
			}
			if (arrow.l_r)
			{
				
				arrow.x -= arrow.power/10;
				arrow.y += G / 10;

			}
			else
			{
				arrow.x += arrow.power/10;
				arrow.y += G / 10;
			}

		}
		else
		{
			arrow.canrecycle = true;//�������ϣ�����Ϊ�ɻ���״̬��
		}


	}
}
//�����ƶ�
void arrow_Recycle()
{
	if (arrow.canrecycle) {
		if (arrow.x > hero1.x - arrow.w && arrow.x<hero1.x + hero1.w 
			&& arrow.y>hero1.y - arrow.h && arrow.y < hero1.y + hero1.h)
		{
			arrow.canrecycle = false;
			arrow.isalive = false;//���գ��������Զ���Ϊfalse��
		}
	}
}
//���Ļ���
void gameDraw() 
{

	putimage(0, 0, &bk);
	if (hero1.l_r) //���������ƶ�
	{
		putimage(hero1.x, hero1.y, &role_l[0], SRCAND);
		putimage(hero1.x, hero1.y, &role_l[1], SRCPAINT);
	}
	else
	{
		putimage(hero1.x, hero1.y, &role_r[0], SRCAND);
		putimage(hero1.x, hero1.y, &role_r[1], SRCPAINT);
	}
	
	if (arrow.isalive)
	{
		if (arrow.l_r)
		{
			putimage(arrow.x, arrow.y, &arrow_l[0], SRCAND);
			putimage(arrow.x, arrow.y, &arrow_l[1], SRCPAINT);
		}
		else
		{
			putimage(arrow.x, arrow.y, &arrow_r[0], SRCAND);
			putimage(arrow.x, arrow.y, &arrow_r[1], SRCPAINT);
		}


	}


	putimage(enemy1.x, enemy1.y, &enemy[0], SRCAND);
	if (enemy1.hp > 5)
	{
		putimage(enemy1.x, enemy1.y, &enemy[1], SRCPAINT);
		settextstyle(15, 0, "����");
		settextcolor(WHITE);
		outtextxy(600, 0, "��ҹ������");
		outtextxy(600, 20, "�Ǻǣ���С�ĵ͵�����");

	}
	else
	{
		putimage(enemy1.x, enemy1.y, &enemy[2], SRCPAINT);
		settextstyle(15, 0, "����");
		settextcolor(RED);
		outtextxy(600, 10, "��ҹ������");
		settextstyle(30, 0, "����");
		outtextxy(600, 30, "����ŭ���ˣ�");
	}
		

	setfillcolor(YELLOW);
	for (int i = 0; i < bulletnum; i++)
	{
		if (bullet[i].isalive)
		{
			solidcircle(bullet[i].x, bullet[i].y, 10);
		}
	}

	
	
	
	setfillcolor(YELLOW);
	solidrectangle(0, 0, 30, 100);
	setfillcolor(RED);
	solidrectangle(0, 100-100*hero1.hp/10, 30, 100);
	settextcolor(YELLOW);
	settextstyle(15, 0, "����");
	outtextxy(0, 100, "Ѫ��");
	setfillcolor(YELLOW);
	solidrectangle(35, 0, 65, 100);
	setfillcolor(RED);
	solidrectangle(35, 100 - 100 * hero1.power / 5.0, 65, 100);
	settextcolor(YELLOW);
	settextstyle(15, 0, "����");
	outtextxy(35, 100, "����");
	setfillcolor(YELLOW);
	solidrectangle(WIDTH/2-150, 0, WIDTH/2 + 150, 20);
	setfillcolor(RED);
	solidrectangle(WIDTH/2 - 150, 0, WIDTH/2 -150+300*enemy1.hp/10, 20);
	settextcolor(YELLOW);
	settextstyle(15, 0, "����");
	outtextxy(35, 100, "����");
	

	


}

void win_Draw()
{
	putimage(0, 0, &w1);
}

void lose_Draw()
{
	putimage(0, 0, &l1);
}
//��Ϸ�������
void playerMove()
{
	if (GetAsyncKeyState(VK_UP))
	{
		if (hero1.y > 130)
		{
			hero1.y -= hero1.speed;
			if (GetAsyncKeyState(VK_SHIFT))
			{
				if (hero1.power > 0)
				{
					hero1.y -= hero1.speed;
					hero1.power -= 0.003;
				}
			}
		}

	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		if (hero1.y < HEIGHT-hero1.h)
		{
			hero1.y += hero1.speed;
			if (GetAsyncKeyState(VK_SHIFT))
			{
				if (hero1.power > 0)
				{
					hero1.y += hero1.speed;
					hero1.power -= 0.003;
				}
			}
		}
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		if (hero1.x>0)
		{
			hero1.x -= hero1.speed;
			hero1.l_r = true;
			if (GetAsyncKeyState(VK_SHIFT))
			{
				if (hero1.power > 0)
				{
					hero1.x -= hero1.speed;
					hero1.power -=0.003;
				}
			}
		}

	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		if (hero1.x < WIDTH-hero1.w)
		{
			hero1.x += hero1.speed;
			hero1.l_r = false;
			if (GetAsyncKeyState(VK_SHIFT))
			{
				if (hero1.power > 0)
				{
					hero1.x += hero1.speed;
					hero1.power -=0.003;
				}

			}

		}

	}
	if (!arrow.isalive)
	{
		if (GetAsyncKeyState(VK_SPACE))
		{
			arrow_Shoot();
		}
	}



	
}
bool enemy_Stage2()
{
	if (enemy1.hp <= 5)
	{
		bulletnum = 7;
		enemy1.speed = 0.1;
		return true;
	}
	else
	{
		return false;
	}

}
//����ʱ�ı�ȫ�ֱ�����ͬʱҲ�з���ֵ�����Ƿ���p2
//p2ʱ�ٶȼӿ��ͬʱ�ӵ��������
bool enemy_Dead()
{
	if (enemy1.hp <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool hero_Dead()
{
	if (hero1.hp <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//������������ҿ����ƶ�����סshift��̣�
bool Timer(int ms)
{
	static DWORD t1, t2;
	if (t1 - t2 > ms)
	{
		t1 = t2;
		return true;
	}
	t2 = clock();
	return false;
}

void judge()
{
	if (hero_Dead())
	{
		mciSendString("close BGM2 ", 0, 0, 0);
		lose_Draw();
		FlushBatchDraw();
		mciSendString("open ./sounds/lose.mp3 alias BGM3", 0, 0, 0);
		mciSendString("play BGM3 repeat", 0, 0, 0);
		int isok = MessageBox(GetHWnd(), "�����ˣ��ٴ���ս��", "��ʾ", MB_OKCANCEL);
		if (IDOK == isok)
		{
			gameInit();
			mciSendString("close BGM3 ", 0, 0, 0);
		}
		else
		{
			exit(88);
		}
	}
	if (enemy_Dead())
	{
		mciSendString("close BGM2 ", 0, 0, 0);
		win_Draw();
		FlushBatchDraw();
		mciSendString("open ./sounds/win.mp3 alias BGM4", 0, 0, 0);
		mciSendString("play BGM4 repeat", 0, 0, 0);
		int isok = MessageBox(GetHWnd(), "��Ӯ�ˣ��ٴ�������", "��ʾ", MB_OKCANCEL);
		if (IDOK == isok)
		{
			gameInit();
			mciSendString("close BGM4 ", 0, 0, 0);
		}
		else
		{
			exit(66);
		}
	}
}

void run()
{
	power_Change();
	enemy_Stage2();
	enemy_Move();
	if (Timer(500))
	{
		enemy_Attack();
	}
	bullet_Fly();
	playerMove();
	arrow_Fly();
	enemy_Damage();
	arrow_Recycle();
	hero_Damage();
	gameDraw();
	judge();
}

int main()
{


	imgLoad();
	srand(time(NULL));
	initgraph(WIDTH, HEIGHT);
	menu();
	gameInit();
	BeginBatchDraw();
	while (true)
	{
		run();
		FlushBatchDraw();

	}

	system("pause");
	return 0;
}