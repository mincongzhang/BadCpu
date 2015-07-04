#include <iostream>
#include <windows.h>
#include <string.h>
#include <tchar.h>  
#include <sstream> 

using namespace std;

std::string NumberToString ( unsigned int num )
{
	stringstream ss;
	ss << num;
	return ss.str();
}

int _tmain(int argc, _TCHAR* argv[])
{
	//VAR
	HWND p_hwnd,c_hwnd,g_hwnd;
	int RetVal;
	HANDLE hProcess;
	DWORD pid;
	HDC hdc;
	//VAR    

	//Get ParentWindow's hwnd
	//wchar_t title[15]=TEXT("Windows 任务管理器");//char* =>wchar*
	p_hwnd=FindWindow(NULL,"Windows 任务管理器");
	if(p_hwnd==0){
		cout<<"FindWindow() FAILED!"<<endl;
		system("PAUSE");
		return 0;
	} else {
		cout<<"p_hwnd="<<p_hwnd<<endl;            
	}
	//Get ParentWindow's hwnd


	/////////////////////////////////////////////////
	/////////////////Get image and draw//////////////
	//Get Pid
	pid=GetWindowThreadProcessId(p_hwnd, &pid);
	if(pid!=0){cout<<"pid="<<pid<<endl;}
	else{cout<<"GetWindowThreadProcessId() FAILED!"<<endl;}
	//Get Pid

	//Get ChildWindow's hwnd
	c_hwnd=GetTopWindow(p_hwnd);
	if(c_hwnd!=0){cout<<"c_hwnd="<<c_hwnd<<endl;}
	else{cout<<"GetTopWindow() FAILED!"<<endl;}
	//Get ChildWindow's hwnd

	//Get GrandWindow's hwnd
	g_hwnd=GetDlgItem(c_hwnd, 5000);//5001 second window, and so on
	if(g_hwnd!=0){cout<<"g_hwnd="<<g_hwnd<<endl;}
	else{cout<<"GetDlgItem() FAILED!"<<endl;}
	//Get GrandWindow's hwnd

	//Get hdc
	hdc=GetWindowDC(g_hwnd);
	if(hdc!=0){cout<<"hdc="<<hdc<<endl;}
	else{cout<<"GetWindowDC() FAILED!"<<endl;}
	//Get hdc

	std::cout<<"prepare to draw..."<<std::endl;
	system("PAUSE");

	//*************************Draw*************************
	HANDLE PichWnd;
	HDC mHdc=CreateCompatibleDC(hdc);
	HBITMAP holdbmp;										  

	//hard coded...
	//char *path="E://Github//HackingWindowsTaskManager//badapple_frames//1.bmp";
	//char *image_path="E://ba//2.bmp";
	char *background="E://frames//1.bmp";
	std::string image_path="E://frames//";
	std::string suffix = ".bmp";
	unsigned int frame_num = 1000;
	char *c_image_address = new char[100];

	for(unsigned int i=1;i<frame_num;i++){
		Sleep(30);
		std::string image_address =image_path+NumberToString(i)+ suffix;
		strcpy(c_image_address, image_address.c_str());

		PichWnd=LoadImage(0,(LPCTSTR)c_image_address,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//Load image from path
		cout<<"Loading image["<<c_image_address<<"] :"<<PichWnd<<endl;

		SelectObject(mHdc,PichWnd);
		StretchBlt (hdc, 0, 0, 280, 200, //target window and its size
			mHdc, 0, 0, 280, 200,//source image and its size
			SRCPAINT);			 //painting method:using the Boolean XOR 

		///clear window
		/*
		PichWnd=LoadImage(0,(LPCTSTR)background,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//Load image from path
		SelectObject(mHdc,PichWnd);
		StretchBlt (hdc, 0, 0, 280, 200, //target window and its size
			mHdc, 0, 0, 280, 200,//source image and its size
			SRCAND);			 //painting method:using the Boolean XOR
		  */
	}
	delete [] c_image_address;

	system("PAUSE");
	return 0;
}


