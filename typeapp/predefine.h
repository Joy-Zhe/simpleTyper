#ifndef _PREDEFINE_H_
#define _PREDEFINE_H_
static double x_letter = 0;
static double y_letter = 0;
static int speed = 1;
int flag = -1;//��ʼ�������Ϸδ��ʼ��-1��
			  //��Ϸ��ʼ��1���˶���0����ͣ
int fail = 0; //�Ƿ���;���ף����׺�fail = 1 
static char scores[30] = "0";//��ʾ��ǰ�������û��޷��޸� 
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
//����Ϊ��ĸ���ж��ò��� 
double h_APP;  
double fH_APP;
double h0_APP; 
double x_APP;
double y_APP;  
double w_APP; 
//�ж��ò���������� 
//������ֵ���г�ʼ�� 


#endif
