#include <windows.h>
#include <iostream>
#include <string.h>
#include <tchar.h>  
#include <sstream>

#include "ReplaceColor.h"
#include "ScreenCapture.h"

using namespace std;

char * SCREENSHOT_ADDRESS = "E://screen.bmp";

std::string NumberToString ( unsigned int num ){
	stringstream ss;ss << num;return ss.str();
}

int main()
{ 
	HWND p_hwnd,c_hwnd,g_hwnd1,g_hwnd4;

	//Get ParentWindow's hwnd
	//wchar_t title[15]=TEXT("Windows 任务管理器");//char* =>wchar*
	p_hwnd=FindWindow(NULL,"Windows 任务管理器");

	if(p_hwnd==0){
		std::cout<<"FindWindow() FAILED!"<<std::endl;
	} else {
		SetForegroundWindow(p_hwnd); //brings that window to foreground
		std::cout<<"p_hwnd="<<p_hwnd<<std::endl;            
	}

	//Get ChildWindow's hwnd
	c_hwnd=GetTopWindow(p_hwnd);
	if(c_hwnd!=0){std::cout<<"c_hwnd="<<c_hwnd<<std::endl;}
	else{std::cout<<"GetTopWindow() FAILED!"<<std::endl;}

	//Get GrandWindow's hwnd
	//(The identifier of the control to be retrieved.)
	//control ID = 00001388 in hex, = 5000 in dec
	g_hwnd1=GetDlgItem(c_hwnd, 5000);//5001 second window, and so on
	g_hwnd4=GetDlgItem(c_hwnd, 5003);//5001 second window, and so on
	if(g_hwnd1!=0){std::cout<<"g_hwnd1="<<g_hwnd1<<std::endl;}
	else{std::cout<<"GetDlgItem() FAILED!"<<std::endl;}

	//Get hdc
	HDC cpu1_hdc;
	cpu1_hdc=GetWindowDC(g_hwnd1);
	if(cpu1_hdc!=0){std::cout<<"cpu1_hdc="<<cpu1_hdc<<std::endl;}
	else{std::cout<<"GetWindowDC() FAILED!"<<std::endl;}

	std::cout<<"prepare to draw..."<<std::endl;
	system("PAUSE");

	RECT rc;
	int width,height;
	GetWindowRect(g_hwnd4, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
	std::cout<<"rc.right:"<<rc.right<<"rc.left:"<<rc.left<<std::endl;
	std::cout<<"rc.bottom:"<<rc.bottom<<"rc.top:"<<rc.top<<std::endl;
	std::cout<<"width:"<<width<<"height:"<<height<<std::endl;

	//*************************Draw*************************
	//hard coded path...
	std::string image_path="E://frames//";
	std::string suffix = ".bmp";
	unsigned int frame_num = 6566;
	char *c_image_address = new char[100];

	HDC frame_dc=CreateCompatibleDC(cpu1_hdc);//create a Memory Device Contexts(DC)
	HANDLE frame_image;
	HANDLE frame_background;
	HANDLE original_frame_image;

	for(unsigned int i=1;i<frame_num;i++){
		screenCapturePart(rc.left, rc.top,width,height,SCREENSHOT_ADDRESS);

		std::string image_address =image_path + NumberToString(i) + suffix;
		strcpy(c_image_address, image_address.c_str());

		frame_image=LoadImage(0,(LPCTSTR)c_image_address,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//Load image from path
		std::cout<<"Loading image["<<c_image_address<<"] :"<<frame_image<<std::endl;

		frame_background = LoadImage(0,(LPCTSTR)SCREENSHOT_ADDRESS,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//Load image from path
		std::cout<<"Loading background["<<SCREENSHOT_ADDRESS<<"] :"<<frame_background<<std::endl;
		HBITMAP grid_background = ReplaceColor(HBITMAP(frame_background),0x00ff00,0x000000,frame_dc); // replace green by black

		//select into frame_dc
		SelectObject(frame_dc,grid_background);    //(A handle to the DC, A handle to the object to be selected) 
		StretchBlt (cpu1_hdc, 0, 0, 256, 192,	    //target window and its size
			frame_dc, 0, 0, 256, 192,	        //source image and its size
			SRCCOPY);
		//select into frame_dc
		SelectObject(frame_dc,frame_image);    //(A handle to the DC, A handle to the object to be selected) 
		StretchBlt (cpu1_hdc, 0, 0, 256, 192,	    //target window and its size
			frame_dc, 0, 0, 256, 192,	        //source image and its size
			SRCPAINT);
	}

	delete [] c_image_address;

	system("PAUSE");
	return 0;
}
