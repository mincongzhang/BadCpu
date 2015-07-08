//http://stackoverflow.com/questions/7292757/how-to-get-screenshot-of-a-window-as-bitmap-object-in-c
#include <windows.h>
#include <iostream>
using namespace std;
int main()
{ 
    RECT rc;
    HWND hwnd = FindWindow(NULL,"Windows 任务管理器");    //the window can't be min
    if (hwnd == NULL)
    {
        cout << "it can't find any 'note' window" << endl;

    }
    GetClientRect(hwnd, &rc);

    //create
    HDC hdcScreen = GetDC(NULL); //If this value is NULL, GetDC retrieves the DC for the entire screen.
    HDC hdc = CreateCompatibleDC(hdcScreen);
    HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen,  //can also use HANDLE
        rc.right - rc.left, rc.bottom - rc.top);
    SelectObject(hdc, hbmp);

    //Print to memory hdc
    PrintWindow(hwnd, hdc, PW_CLIENTONLY);

    //copy to clipboard
    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, hbmp);
    CloseClipboard();

    //release
    DeleteDC(hdc);
    DeleteObject(hbmp);
    ReleaseDC(NULL, hdcScreen);

    cout << "success copy to clipboard, please paste it to the 'mspaint'" << endl;
	 system("PAUSE");
    return 0;
}