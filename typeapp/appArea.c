#if defined(APPAREA) //字母生成区 
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
#include "imgui.h"
#include "predefine.h"

void appArea(){
	double h = GetWindowHeight() * 4 / 5;  // 字母生成区判定高度 
	double fH = GetFontHeight();
	double h0 = fH*2;  // 控件高度 
	double x = 0;
	double y = h0 * 2;  
	double w = GetWindowWidth()/ 2; // 字母生成区宽度
	char * c[] = {"Black","Dark Gray","Gray","Light Gray" , "Cyan",
			"Brown", "Red", "Orange", "Yellow", "Green","Blue","Violet", "Magenta"};
	int N = sizeof(c)/sizeof(c[0]);
	 
	if(flag != -1) {
		SetPenColor("Black");
		char letter_now[2] = "";
		letter_now[0] = letter[n_letter];
		letter_now[1] = 0;
		drawLabel(x + fH * 3, y_letter, letter_now);
	}
	SetPenColor(c[((int)(times*100) / 10) % 12]);
	drawRectangle(x, y, w, h, 0);
	
}
#endif
