#include <windows.h>
#include <iostream>
#include <string.h>
#include <tchar.h>  
#include <sstream>
#include <ole2.h>
#include <olectl.h>
using namespace std;

char * SCREENSHOT_ADDRESS = "E://screen.bmp";
bool screenCapturePart(int x, int y, int w, int h, LPCSTR fname);
bool saveBitmap(LPCSTR filename, HBITMAP bmp, HPALETTE pal);
std::string NumberToString ( unsigned int num ){
	stringstream ss;ss << num;return ss.str();
}

int main()
{ 
	HWND p_hwnd,c_hwnd,g_hwnd;

	//Get ParentWindow's hwnd
	//wchar_t title[15]=TEXT("Windows 任务管理器");//char* =>wchar*
	p_hwnd=FindWindow(NULL,"Windows 任务管理器");

	if(p_hwnd==0){
		cout<<"FindWindow() FAILED!"<<endl;
	} else {
		SetForegroundWindow(p_hwnd); //brings that window to foreground
		cout<<"p_hwnd="<<p_hwnd<<endl;            
	}

	//Get ChildWindow's hwnd
	c_hwnd=GetTopWindow(p_hwnd);
	if(c_hwnd!=0){cout<<"c_hwnd="<<c_hwnd<<endl;}
	else{cout<<"GetTopWindow() FAILED!"<<endl;}

	//Get GrandWindow's hwnd
	//(The identifier of the control to be retrieved.)
	//control ID = 00001388 in hex, = 5000 in dec
	g_hwnd=GetDlgItem(c_hwnd, 5000);//5001 second window, and so on	
	if(g_hwnd!=0){cout<<"g_hwnd="<<g_hwnd<<endl;}
	else{cout<<"GetDlgItem() FAILED!"<<endl;}

	//Get hdc
	HDC hdc;
	hdc=GetWindowDC(g_hwnd);
	if(hdc!=0){cout<<"hdc="<<hdc<<endl;}
	else{cout<<"GetWindowDC() FAILED!"<<endl;}

	std::cout<<"prepare to draw..."<<std::endl;
	system("PAUSE");

	RECT rc;
	int width,height;
	g_hwnd=GetDlgItem(c_hwnd, 5003);
	GetWindowRect(g_hwnd, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
	std::cout<<"rc.right:"<<rc.right<<"rc.left:"<<rc.left<<endl;
	std::cout<<"rc.bottom:"<<rc.bottom<<"rc.top:"<<rc.top<<endl;
	std::cout<<"width:"<<width<<"height:"<<height<<endl;


	//*************************Draw*************************
	//hard coded path...
	std::string image_path="E://frames//";
	std::string suffix = ".bmp";
	unsigned int frame_num = 6566;
	char *c_image_address = new char[100];

	HDC frame_dc=CreateCompatibleDC(hdc);//create a Memory Device Contexts(DC)
	HANDLE frame_image;
	HANDLE original_frame_image;
	for(unsigned int i=1;i<frame_num;i++){


		std::string image_address =image_path + NumberToString(i) + suffix;
		strcpy(c_image_address, image_address.c_str());

		frame_image=LoadImage(0,(LPCTSTR)c_image_address,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//Load image from path
		std::cout<<"Loading image["<<c_image_address<<"] :"<<frame_image<<std::endl;

		//select into frame_dc
		SelectObject(frame_dc,frame_image);    //(A handle to the DC, A handle to the object to be selected) 
		StretchBlt (hdc, 2, 2, 256, 192,	    //target window and its size
			frame_dc, 0, 0, 256, 192,	        //source image and its size
			SRCPAINT);

		frame_image=LoadImage(0,(LPCTSTR)SCREENSHOT_ADDRESS,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//Load image from path
		std::cout<<"Loading background["<<SCREENSHOT_ADDRESS<<"] :"<<frame_image<<std::endl;

		Sleep(60);
		screenCapturePart(rc.left, rc.top,width,height,SCREENSHOT_ADDRESS);
		SelectObject(frame_dc,frame_image);    //(A handle to the DC, A handle to the object to be selected) 
		StretchBlt (hdc, 0, 0, 256, 192,	    //target window and its size
			frame_dc, 0, 0, 256, 192,	        //source image and its size
			SRCCOPY);

	}

	delete [] c_image_address;

	system("PAUSE");
	return 0;
}


bool saveBitmap(LPCSTR filename, HBITMAP bmp, HPALETTE pal)
{
	bool result = false;
	PICTDESC pd;

	pd.cbSizeofstruct   = sizeof(PICTDESC);
	pd.picType      = PICTYPE_BITMAP;
	pd.bmp.hbitmap  = bmp;
	pd.bmp.hpal     = pal;

	LPPICTURE picture;
	HRESULT res = OleCreatePictureIndirect(&pd, IID_IPicture, false,
		reinterpret_cast<void**>(&picture));

	if (!SUCCEEDED(res))
		return false;

	LPSTREAM stream;
	res = CreateStreamOnHGlobal(0, true, &stream);

	if (!SUCCEEDED(res))
	{
		picture->Release();
		return false;
	}

	LONG bytes_streamed;
	res = picture->SaveAsFile(stream, true, &bytes_streamed);

	HANDLE file = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ, 0,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (!SUCCEEDED(res) || !file)
	{
		stream->Release();
		picture->Release();
		return false;
	}

	HGLOBAL mem = 0;
	GetHGlobalFromStream(stream, &mem);
	LPVOID data = GlobalLock(mem);

	DWORD bytes_written;

	result   = !!WriteFile(file, data, bytes_streamed, &bytes_written, 0);
	result  &= (bytes_written == static_cast<DWORD>(bytes_streamed));

	GlobalUnlock(mem);
	CloseHandle(file);

	stream->Release();
	picture->Release();

	return result;
}


bool screenCapturePart(int x, int y, int w, int h, LPCSTR fname){
	HDC hdcSource = GetDC(NULL);
	HDC hdcMemory = CreateCompatibleDC(hdcSource);

	int capX = GetDeviceCaps(hdcSource, HORZRES);
	int capY = GetDeviceCaps(hdcSource, VERTRES);

	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSource, w, h);
	HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMemory, hBitmap);

	BitBlt(hdcMemory, 0, 0, w, h, hdcSource, x, y, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(hdcMemory, hBitmapOld);

	DeleteDC(hdcSource);
	DeleteDC(hdcMemory);

	HPALETTE hpal = NULL;
	if(saveBitmap(fname, hBitmap, hpal)) return true;
	return false;
}
