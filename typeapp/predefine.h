#ifndef _PREDEFINE_H_
#define _PREDEFINE_H_
static double x_letter = 0;
static double y_letter = 0;
static int speed = 1;
int flag = -1;//初始情况，游戏未开始：-1。
			  //游戏开始后：1：运动，0：暂停
int fail = 0; //是否中途触底，触底后fail = 1 
static char scores[30] = "0";//显示当前分数，用户无法修改 
static int score = 0;
static double times = 0;
double time_set = 0; 
static int n_letter = 1;

static char letter[30] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
void display();
void Menu();
void ranking(FILE *op);
void drawButtons();
void appArea(); 
void numtos(int num, char *s);
FILE *rankTable;
//以下为字母区判定用参数 
double h_APP;  
double fH_APP;
double h0_APP; 
double x_APP;
double y_APP;  
double w_APP; 
//判定用参数定义结束 
//对所用值进行初始化 


#endif
