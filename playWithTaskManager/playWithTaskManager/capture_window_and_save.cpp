#include <windows.h>
#include <iostream>
#include <ole2.h>
#include <olectl.h>
using namespace std;

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

	RECT rc;
	int width,height;
    //GetClientRect(hwnd, &rc); //Retrieves the coordinates of a window's client area.
	GetWindowRect(g_hwnd, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
	std::cout<<"rc.right:"<<rc.right<<"rc.left:"<<rc.left<<endl;
	std::cout<<"rc.bottom:"<<rc.bottom<<"rc.top:"<<rc.top<<endl;
	std::cout<<"width:"<<width<<"height:"<<height<<endl;


	char *c_image_address = "E://screen.bmp";
	screenCapturePart(rc.left, rc.top,width,height,c_image_address);

	system("PAUSE");
    return 0;
}