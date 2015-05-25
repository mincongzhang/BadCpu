// badapple.cpp : Defines the entry point for the console application.
//


#include <iostream>
#include <windows.h>
#include <string.h>
#include <tchar.h>  

//#include "stdafx.h"

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
	if(p_hwnd!=0){
		cout<<"p_hwnd="<<p_hwnd<<endl;            
		//Get ParentWindow's hwnd

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
		g_hwnd=GetDlgItem(c_hwnd, 5000);
		if(g_hwnd!=0){cout<<"g_hwnd="<<g_hwnd<<endl;}
		else{cout<<"GetDlgItem() FAILED!"<<endl;}
		//Get GrandWindow's hwnd

		//Get hdc
		hdc=GetWindowDC(g_hwnd);
		if(hdc!=0){cout<<"hdc="<<hdc<<endl;}
		else{cout<<"GetWindowDC() FAILED!"<<endl;}
		//Get hdc

		system("PAUSE");

		//*************************Draw*************************
		//var
		//while(true){
		HANDLE PichWnd;
		HDC mHdc;
		HBITMAP holdbmp;

		char *PATH="E://Github//HackingWindowsTaskManager//badapple_frames//test.bmp";

		PichWnd=LoadImage(0,(LPCTSTR)PATH,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//Load image from path
		cout<<"Loading:"<<PichWnd<<endl;

		while(true){
		mHdc=CreateCompatibleDC(hdc);
		holdbmp=(HBITMAP)SelectObject(mHdc,PichWnd);
		GetObject(PichWnd,0,0);
		StretchBlt (hdc, 0, 0, 800, 400, mHdc, 50, 20, 1440, 1080, BLACKNESS);
		SelectObject(mHdc,holdbmp);

		DeleteObject (PichWnd);
		DeleteDC (mHdc);
		ReleaseDC (g_hwnd, hdc);
		}
		//}
		//*************************Draw*************************

	}
	else{
		cout<<"FindWindow() FAILED!"<<endl;
	}

	//hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE,pid);
	//if(hProcess!=0){
	//cout<<"hProcess="<<hProcess<<endl;}
	//else{
	//cout<<"OpenProcess() FAILED!"<<endl;

	system("PAUSE");
	return 0;
}