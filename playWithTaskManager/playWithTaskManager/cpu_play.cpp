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

	//*************************Draw*************************
	//hard coded path...
	std::string image_path="E://frames//";
	std::string suffix = ".bmp";
	unsigned int frame_num = 6566;
	char *c_image_address = new char[100];

	HDC frame_dc=CreateCompatibleDC(hdc);//create a Memory Device Contexts(DC)
	HANDLE frame_image;
	HANDLE original_frame_image;
	for(unsigned int i=1;i<frame_num;i++,Sleep(30)){
		std::string image_address =image_path + NumberToString(i) + suffix;
		strcpy(c_image_address, image_address.c_str());

		frame_image=LoadImage(0,(LPCTSTR)c_image_address,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//Load image from path
		std::cout<<"Loading image["<<c_image_address<<"] :"<<frame_image<<std::endl;

		//select into frame_dc
		original_frame_image = SelectObject(frame_dc,frame_image);    //(A handle to the DC, A handle to the object to be selected) 
		StretchBlt (hdc, 2, 2, 256, 192,	    //target window and its size
			frame_dc, 0, 0, 256, 192,	        //source image and its size
			SRCPAINT);
	}
	//Learning to Draw Basic Graphics in C++
	//http://www.informit.com/articles/article.aspx?p=328647&seqNum=3
	//Transparent Bitmaps
	//http://www.winprog.org/tutorial/transparency.html
	delete [] c_image_address;

	system("PAUSE");
	return 0;
}


