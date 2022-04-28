#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <math.h>
#include <ctype.h>
#include "imgui.h"
#include "predefine.h"
#define DISPLAY
#define MENU
#define BUTTONS
#define APPAREA
#define SCOREAREA
#define TYPEAREA
#define N2S
#define RANK
#define TIMER_TOTAL 1

void CharEventProcess(char ch){
	uiGetChar(ch); // GUI字符输入
	display(); //刷新显示
}
// 用户的键盘事件响应函数
void KeyboardEventProcess(int key, int event){
	uiGetKeyboard(key,event); // GUI获取键盘
	display(); // 刷新显示
}
// 用户的鼠标事件响应函数
void MouseEventProcess(int x, int y, int button, int event){
	uiGetMouse(x,y,button,event); //GUI获取鼠标
	display(); // 刷新显示
}

void TimerEventProcess(int timerID){
	times += 0.0001;
	switch (timerID){
		case TIMER_TOTAL:
			if(flag != -1 && flag != 0) y_letter -= 0.01 * speed;
			if(y_letter <= 0.7 && flag == 1) {
				ranking(rankTable);
				flag = -1;
				fail = 1; 
				time_set = times;
				y_letter = 6.1;
				n_letter = rand() % 26;
				x_letter = rand() % 490 / 100.0;
			}
			display();
			break;
	}
}

void initialize(){
	n_letter = rand() % 26;
	x_letter = rand() % 490 / 100.0;
	fail = 0;
}

void Main (){
    InitGraphics();
	registerCharEvent(CharEventProcess);        // 字符
	registerKeyboardEvent(KeyboardEventProcess);// 键盘
	registerMouseEvent(MouseEventProcess);      // 鼠标
	registerTimerEvent(TimerEventProcess);      // 定时器
	startTimer(TIMER_TOTAL, 10);
}

#if defined(RANK) //写入排名
void ranking(FILE *op){
	struct info{
		int rank;
		int score;
		int time;
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;  
	}; //存储之前的信息
	struct info info_pre[1001], tmp;
	char s;
	char *s_tmp;
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	op = fopen("rankingTable.txt", "r+");
	int i = 0, k = 0, j = 0;
	while (!feof(op)){
		fscanf(op, "%10d%10d%10d s     %4d年%2d月%2d日%2d:%2d:%2d\n", &info_pre[i].rank, &info_pre[i].score, &info_pre[i].time, &info_pre[i].year, 
													&info_pre[i].month, &info_pre[i].day, &info_pre[i].hour, &info_pre[i].minute, &info_pre[i].second);
		i++;
	}
	info_pre[i].rank = 0;
	info_pre[i].score = score;
	info_pre[i].time = (int)((times - time_set) * 1000 / 6);
	info_pre[i].year = (int)sys.wYear;
	info_pre[i].month = (int)sys.wMonth; 
	info_pre[i].day = (int)sys.wDay;
	info_pre[i].hour = (int)sys.wHour;
	info_pre[i].minute = (int)sys.wMinute;
	info_pre[i].second = (int)sys.wSecond;
	fclose(op);
	for(k = 0; k < i - 1; k++){
		for(j = i; j > k; j--){
			if(info_pre[j - 1].score < info_pre[j].score || info_pre[j - 1].score == info_pre[j].score && info_pre[j - 1].time > info_pre[j].time){
				tmp = info_pre[j];
				info_pre[j] = info_pre[j - 1];
				info_pre[j - 1] = tmp;
			}
		}
	}
	
	op = fopen("rankingTable.txt", "w+");
	for(k = 0; k <= i; k++){
		fprintf(op, "%10d%10d%10d s     %4d年%2d月%2d日%2d:%2d:%2d\n", k + 1, info_pre[k].score, info_pre[k].time, info_pre[k].year, 
													info_pre[k].month, info_pre[k].day, info_pre[k].hour, info_pre[k].minute, info_pre[k].second);
		//fprintf(op, "%10d%10d%10d s     %4d年%2d月%2d日%2d:%2d:%2d\n", rank, score, (int)((times - time_set) * 1000 / 6), sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	}
	fclose(op);
}
#endif 

#if defined(N2S) //整型转字符串 
void numtos(int num, char *s){
	int tmp = num, cnt = 0, cnt_s = 0;
	while (tmp){
		cnt++;
		tmp /= 10;
	}
	while (cnt){
		s[cnt_s] = num / (pow(10, cnt-1)) + '0';
		num = num - (num / (int)(pow(10, cnt-1))) * (int)(pow(10, cnt-1));
		cnt_s++;
		cnt--;
	}
}
#endif 

#if defined(MENU) //菜单 
void Menu(){
	
	static char *menuListSettings[] = {"设置", "开始", "暂停", "加速", "减速", "退出"};
	static char *menuListAbout[] = {"关于", "帮助", "排行榜"}; 
	double w = TextStringWidth(menuListSettings[0]) * 2, h = 0.4, x = 0, wsub = TextStringWidth(menuListAbout[2]) * 1.2, y = GetWindowHeight() - h;
	static char *selectedLabel = NULL;
	//以下与按钮关联 
	double fH1 = GetFontHeight();
	double h1 = fH1*2;  // 控件高度
	double x1 = GetWindowWidth() / 2.5;  
	double y1 = 0; 
	double w1 = GetWindowWidth() / 5; // 控件宽度
	char s[] = "当前速度：";
	int selection;
	//设置 
	selection = menuList(GenUIID(0), x, y, w, wsub, h, menuListSettings, sizeof(menuListSettings)/sizeof(menuListSettings[0]));
	if(selection > 0) selectedLabel = menuListSettings[selection];
	switch(selection){
		case 1: 
			if(flag == -1) time_set = times;
			fail = 0;
			y_letter = 6.1;
			flag = 1;//开始 
			score = 0;
			int tt = 0;
			while (scores[tt]){
				scores[tt] = 0;
				tt++;
			}
			scores[0]='0';
			break;
		case 2://暂停 
			if(flag != -1) flag = 0;
			break;
		case 3://加速 
			if(speed < 99) speed++;
			break;
		case 4://减速 
			if(speed > 1) speed--;
			break;
		case 5:
			ranking(rankTable); 
			fclose(rankTable);
			exit(-1); //退出 
			break;
	} 
	SetPenColor("Black");
	if(speed < 10) s[10] = speed + '0';
	else if(speed < 100){
		s[10] = speed / 10 + '0';
		s[11] = speed % 10 + '0';
	}
	drawLabel(x1, y1 + fH1 * 2.5, s);
	drawLabel(x1 + w1, y1 + fH1 * 2.5, s);
	//关联结束 
	//关于
	selection = menuList(GenUIID(0), x + w, y, w, wsub, h, menuListAbout, sizeof(menuListAbout)/sizeof(menuListAbout[0]));
	if(selection > 0) selectedLabel = menuListAbout[selection];
	switch(selection){
		case 1: //帮助 
			system("start howtoplay.txt");
			break;
		case 2: //排行榜 
			system("start rankingTable.txt");
			break;
	}
}
#endif

#if defined(BUTTONS) //按钮 
void drawButtons(){
	double fH = GetFontHeight();
	double h = fH*2;  // 控件高度
	double x = GetWindowWidth() / 2.5;  
	double y = 0; 
	double w = GetWindowWidth() / 5; // 控件宽度
	char s[] = "当前速度：";

	if( button(GenUIID(0), x - 2 * w, y, w, h, "开始") ){
		if(flag == -1) time_set = times;
		fail = 0;
		y_letter = 6.1;
		flag = 1;
		score = 0;
		int tt = 0;
		while (scores[tt]){
			scores[tt] = 0;
			tt++;
		}
		scores[0]='0';
	}

	if( button(GenUIID(0), x - w, y, w, h, "暂停") )
		if(flag != -1) flag = 0;

	if( button(GenUIID(0), x, y, w, h, "加速") ){
		if(score < 98) speed++;	
	}

	
	if( button(GenUIID(0), x + w, y, w, h, "减速") ){
		if(speed > 1) speed--;
	}
	
	if( button(GenUIID(0), x + 2 * w, y, w, h, "退出") ) {
		if(flag == 0){
			ranking(rankTable);
			fclose(rankTable);
		}
		exit(-1);
	}
	
	SetPenColor("Black");
	if(speed < 10) s[10] = speed + '0';
	else if(speed < 100){
		s[10] = speed / 10 + '0';
		s[11] = speed % 10 + '0';
	}
	drawLabel(x, y + fH * 2.5, s);
	drawLabel(x + w, y + fH * 2.5, s);
}
#endif

#if defined(APPAREA) //字母生成区 
void appArea(){
	double h = GetWindowHeight() * 4 / 5;  // 字母生成区判定高度 
	double fH = GetFontHeight();
	double h0 = fH * 2;  // 控件高度 
	double x = 0;
	double y = h0 * 2;  
	double w = GetWindowWidth()/ 2; // 字母生成区宽度
	char * c[] = {"Black", "Orange", "Gray", "Violet" , "Cyan", "Brown",
				"Red", "Dark Gray", "Yellow", "Green","Blue", "Light Gray", "Magenta"};
	int N = sizeof(c)/sizeof(c[0]);
	 
	if(flag != -1) {
		SetPenColor("Black");
		char letter_now[2] = "";
		letter_now[0] = letter[n_letter];
		letter_now[1] = 0;
		drawLabel(x_letter, y_letter, letter_now);
	}
	SetPenColor(c[((int)(times * 100) / 10) % 12]);
	drawRectangle(x, y, w, h, 0);
	
}
#endif

#if defined(SCOREAREA) //计分区
void scoreArea(){ 
	double h = GetWindowHeight() / 20;  // 计分区高度 
	double fH = GetFontHeight();
	double h0 = fH*2;  // 控件高度 
	double w = GetWindowWidth() / 6; // 计分区宽度
	double x = GetWindowWidth() / 2 - w / 2;
	double y = GetWindowHeight() - h; 
	SetPenColor("Black");
	if(fail) drawLabel(x + h0 , y, "当前得分：GameOver");
	else{
		drawLabel(x + h0 , y, "当前得分：");
		drawLabel(x + h0 * 3.5, y, scores);
	}
}
#endif

#if defined(TYPEAREA) //操作区
void typeArea(){ 
	double h = GetWindowHeight() / 20;  // 操作区高度 
	double fH = GetFontHeight();
	double h0 = fH*2;  // 控件高度 
	double w = GetWindowWidth() / 6; // 操作区宽度
	double x = GetWindowWidth() / 2 + w ;
	double y = GetWindowHeight() / 2 - h; 
	static char results[300]=""; 
	SetPenColor("Black"); 
	drawLabel(x-fH/2-TextStringWidth("输入"), y + fH * 0.7, "输入");
	textbox(GenUIID(0), x, y, w, h, results, sizeof(results));
	static int t = 0;
	if(flag != -1 && flag != 0) {
		if(results[t] == letter[n_letter]){
			score++;
			numtos(score, scores);
			int n = t;
			while(results[n]){
				results[n] = 0; 
				n--;
			}
			x_letter = rand() % 490 / 100.0;
			t = 0;
			y_letter = 6.1;
			n_letter = rand() % 26;
		}
		//else if(results[t] == 8) t -= 2; 
		else if(results[t]) t++;
		
	}
}
#endif

#if defined(DISPLAY) //刷新 
void display(){
	DisplayClear();
	Menu();
	drawButtons();
	appArea();
	scoreArea();
	typeArea();
} 
#endif 



