#include <iostream>
#include <windows.h>
#include <string.h>
#include <tchar.h>  

using namespace std;

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

	while(true){
		char *path="E://Github//HackingWindowsTaskManager//badapple_frames//1.bmp";
		PichWnd=LoadImage(0,(LPCTSTR)path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//Load image from path
		cout<<"Loading:"<<PichWnd<<endl;

		SelectObject(mHdc,PichWnd);
		StretchBlt (hdc, 0, 0, 240, 200, mHdc, 0, 0, 240, 200, SRCPAINT);
	}

	system("PAUSE");
	return 0;
}