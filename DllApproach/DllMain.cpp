/*********************************************************************************

		把编译出的DLL注入到win10任务管理器就可以用CPU使用记录界面播放BadApple
					PS:里面可能有些需要修改的地方,详见注释
				改程序适用于win10 64位  其他系统未测试  可以自行更改
					64为编译环境设为x64  32位未测试
						项目类型为 Win32 DLL

**********************************************************************************/

#include <Windows.h>
#include <process.h>
#include <string>
#include <atlimage.h>
#include <sstream>
#include <vector>
#include <ctime>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib") 

using namespace std;


HWND EnumHWnd;   //用来保存CPU使用记录窗口的句柄
wstring ClassNameToEnum;  
BOOL CALLBACK EnumChildWindowsProc(HWND hWnd,LPARAM lParam) //寻找CPU使用记录界面的子窗口ID
{
	wchar_t WndClassName[255]={0};   
	GetClassName(hWnd,WndClassName,255);
	wstring str;
	str=WndClassName;
	if(str==ClassNameToEnum)
	{
		wstringstream wsstr;
		wsstr<<hWnd;
		
		RECT rect;
		GetWindowRect(hWnd,&rect);
		if(str==L"CtrlNotifySink")				
			if((rect.bottom-rect.top)<65)
				return true;
		if(str==L"CtrlNotifySink")
			if((rect.right-rect.left)<65)
				return true;
		EnumHWnd=hWnd;   //如果找到了CPU使用记录的窗口的话, 把他赋值给EnumHWnd
		return false;
	}
	
 	return true;   
}
void FindCPUWnd()    //要想适用其他版本的任务管理器 更改这个函数
{
	HWND h=FindWindow(L"TaskManagerWindow",L"任务管理器");
	ClassNameToEnum=L"NativeHWNDHost";     //一级子窗口
	EnumChildWindows(h,EnumChildWindowsProc,NULL);
	ClassNameToEnum=L"DirectUIHWND";		//二级子窗口
	EnumChildWindows(EnumHWnd,EnumChildWindowsProc,NULL);	//子窗口的名字适用Spy++获得
	ClassNameToEnum=L"CtrlNotifySink";		//三级子窗口
	EnumChildWindows(EnumHWnd,EnumChildWindowsProc,NULL);
	ClassNameToEnum=L"CvChartWindow";		//四级子窗口
	EnumChildWindows(EnumHWnd,EnumChildWindowsProc,NULL);

}

class FrameTimer //计时器
{
private:
	unsigned int flag;
public:
	
	void start()
	{
		flag=clock();
	}
	operator int()
	{
		return ((clock()-flag));
	}

};


void MainThread(PVOID pv)
{
	MessageBoxW(NULL,(LPCWSTR)L"注入成功!",(LPCWSTR)L"注入成功!",MB_OK);
	FindCPUWnd(); //寻找CPU使用记录的窗口,  函数会把窗口句柄保存在全局变量EnumHWnd
	HDC hdc=GetDC(EnumHWnd);
	vector<CImage> Frames;  
	CImage img;
	wstringstream wsstr;
	WIN32_FIND_DATA wfd;
	HANDLE hFile = FindFirstFile(L"D:/TaskmgrPlayer/BadApple/*.*",&wfd); //D:/TaskmgrPlayer/BadAppl 为存放每帧图片的路径  
	FrameTimer ft;
	unsigned int FrameTick=0;
	//ft.SetFPSAndVideoLong(30,9999);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		int UpPixel=RGB(0,0,0);
		FindNextFile(hFile,&wfd);
		PlaySound(L"D:/TaskmgrPlayer/BadApple.wav",NULL,SND_FILENAME|SND_ASYNC);  //播放音乐
		ft.start();
		for(;FindNextFile(hFile,&wfd);)
		{
			wsstr.str(L"");
			wsstr<<L"D:/TaskmgrPlayer/BadApple/"<<wfd.cFileName;    //D:/TaskmgrPlayer/BadAppl 为存放每帧图片的路径
			img.Load(wsstr.str().c_str());
			wsstr.str(L"");
			byte* pRealData;  
			pRealData=(byte*)img.GetBits();  
			int pit=img.GetPitch();  
			int bitCount=img.GetBPP()/8;  
			RECT rect;
			GetWindowRect(EnumHWnd,&rect);
			int w=rect.right-rect.left;
		   	int h=rect.bottom-rect.top;

			for (int y=0; y<img.GetHeight(); y++)   //把图像二值化
			{  
				for (int x=0; x<img.GetWidth(); x++) 
				{  

					int grayVal=(int)(int)(*(pRealData + pit*y + x*bitCount))>192?255:0; 
					if(grayVal<128)
					{
						*(pRealData + pit*y + x*bitCount)=grayVal;       //B
						*(pRealData + pit*y + x*bitCount +1)=grayVal;    //G
						*(pRealData + pit*y + x*bitCount +2)=grayVal;     //R
					}
				}
			}

			for (int x=0; x<img.GetWidth(); x++)   //边缘检测
			{  
				for (int y=0; y<img.GetHeight(); y++) 
				{  

					int grayVal=(int)(int)(*(pRealData + pit*y + x*bitCount))>192?255:0; 
					if(abs(grayVal-UpPixel)>250)
					{
						*(pRealData + pit*y + x*bitCount)=187;  
						*(pRealData + pit*y + x*bitCount +1)=152;  
						*(pRealData + pit*y + x*bitCount +2)=17; 
					}
					UpPixel=grayVal;
				}
			}

			for (int y=0; y<img.GetHeight(); y++)   //边缘检测
			{  
				for (int x=0; x<img.GetWidth(); x++) 
				{  

					int grayVal=(int)(int)(*(pRealData + pit*y + x*bitCount))>192?255:0; 
					if(abs(grayVal-UpPixel)>250)
					{
						*(pRealData + pit*y + x*bitCount)=187;       //B
						*(pRealData + pit*y + x*bitCount +1)=152;    //G
						*(pRealData + pit*y + x*bitCount +2)=17;     //R
					}
					if((x==0||x==img.GetWidth()-1)||(y==0||y==img.GetHeight()-1))  //深蓝色边框
					{
						*(pRealData + pit*y + x*bitCount)=187;       //B
						*(pRealData + pit*y + x*bitCount +1)=152;    //G
						*(pRealData + pit*y + x*bitCount +2)=17;     //R
					}
					UpPixel=grayVal;
				}
			}

			for (int y=0; y<img.GetHeight(); y++)  //背景处理
			{  
				for (int x=0; x<img.GetWidth(); x++) 
				{  

					int grayVal=(int)(int)(*(pRealData + pit*y + x*bitCount)); 
					if(grayVal<128)       //背景设为浅蓝色
					{
						*(pRealData + pit*y + x*bitCount)=250;       //B
						*(pRealData + pit*y + x*bitCount +1)=246;    //G
						*(pRealData + pit*y + x*bitCount +2)=241;     //R
					}
					if((y%(h/10)==0)&&y!=0)   //画方格的横线
					{
						*(pRealData + pit*y + x*bitCount)=240;       //B    
						*(pRealData + pit*y + x*bitCount +1)=226;    //G
						*(pRealData + pit*y + x*bitCount +2)=206;    //R
					}
					if((x%(w/20)==(ft/1000)*((w/40))%(w/20))&&x!=0)   //画方格的竖线
					{
						int rx=x;
						*(pRealData + pit*y + rx*bitCount)=240;       //B
						*(pRealData + pit*y + rx*bitCount +1)=226;    //G
						*(pRealData + pit*y + rx*bitCount +2)=206;    //R
					}
				}
			}

			for(int i=(int)(ft/33.33);i==(int)(ft/33.33);)  //33.33为每帧所占的时间
				img.Draw(hdc,0,0,(int)w,(int)h);
			++FrameTick;
			img.Destroy();
		}
		FindClose(hFile);
	}
	_endthread();
}




int WINAPI DllMain(HINSTANCE  hInstance, DWORD fdwReason ,PVOID pReserved)
{
	switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		_beginthread(MainThread,NULL,NULL);
		break;
	case DLL_THREAD_ATTACH:

		break;
	case DLL_THREAD_DETACH:
		
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
