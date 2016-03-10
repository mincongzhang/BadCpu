#include <windows.h>
#include <iostream>
#include <string.h>
#include <tchar.h>  

#include <boost/thread.hpp>

#include "ReplaceColor.h"
#include "ScreenCapture.h"
#include "ToolBox.h"

namespace {
char * SCREENSHOT_ADDRESS = "C://Users//mizhang//Desktop//screen.bmp";
char * WINDOW_NAME = "Windows Task Manager";
const std::string IMAGE_PATH="C://frames//";
const std::string SUFFIX = ".bmp";
const unsigned int FRAME_NUM = 6566;
const int WINDOW_WIDTH = 280;
const int WINDOW_HEIGHT = 200;
const int CPU1_ID = 5000;
const int CPU2_ID = 5001;
const int CPU4_ID = 5005;
const int FRAME_WAIT_TIME_MS = 10;
}

void frameSleeper(){
	boost::this_thread::sleep(boost::posix_time::millisec(FRAME_WAIT_TIME_MS));
}

int main()
{ 
	//Get hdc
	HDC cpu1_hdc = GetCpuWindow(WINDOW_NAME,CPU1_ID);
	HDC cpu2_hdc = GetCpuWindow(WINDOW_NAME,CPU2_ID);
	if(cpu1_hdc!=0){std::cout<<"cpu1_hdc="<<cpu1_hdc<<std::endl;}
	else{std::cout<<"GetWindowDC() FAILED!: "<<WINDOW_NAME<<std::endl;}
	if(cpu2_hdc!=0){std::cout<<"cpu2_hdc="<<cpu2_hdc<<std::endl;}
	else{std::cout<<"GetWindowDC() FAILED!"<<std::endl;}

	//boost::this_thread::sleep(boost::posix_time::millisec(1000));

	std::cout<<"Ready to draw..."<<std::endl;
	system("PAUSE");

	//*************************Draw*************************
	RECT rc = GetCpuWindowRect(WINDOW_NAME,CPU4_ID);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	std::cout<<"rc.right:"<<rc.right<<"rc.left:"<<rc.left<<std::endl;
	std::cout<<"rc.bottom:"<<rc.bottom<<"rc.top:"<<rc.top<<std::endl;
	std::cout<<"width:"<<width<<"height:"<<height<<std::endl;

	char *c_image_address = new char[100];
	char *c_freq_address = new char[100];
	HDC frame_dc=CreateCompatibleDC(cpu1_hdc);//create a Memory Device Contexts(DC)
	//HANDLE frame_image;
	//HANDLE freq_image;
	//HANDLE frame_background;
	//HANDLE original_frame_image;
	//HBITMAP grid_background;
	std::string image_address;
	std::string freq_address;

	for(unsigned int i=1;i<FRAME_NUM;i++){
		boost::thread frame_sleep_pthread(frameSleeper);

		screenCapturePart(rc.left, rc.top,width,height,SCREENSHOT_ADDRESS);

		image_address =IMAGE_PATH + NumberToString(i) + SUFFIX;
		freq_address =IMAGE_PATH + "a" + NumberToString(i) + SUFFIX;

		strcpy(c_image_address, image_address.c_str());
		strcpy(c_freq_address, freq_address.c_str());

		HANDLE frame_image;
		HANDLE freq_image;
		HANDLE frame_background;
		HBITMAP grid_background;

		frame_image=LoadImage(0,(LPCTSTR)c_image_address,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//Load image from path
		freq_image=LoadImage(0,(LPCTSTR)c_freq_address,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//Load image from path

		frame_background = LoadImage(0,(LPCTSTR)SCREENSHOT_ADDRESS,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//Load image from path
		std::cout<<"Loading image["<<c_image_address<<"] :"<<frame_image<<std::endl;

		grid_background = ReplaceColor(HBITMAP(frame_background),0x00ff00,0x000000,frame_dc); // replace green by black

		//select into frame_dc
		SelectObject(frame_dc,grid_background);    //(A handle to the DC, A handle to the object to be selected) 
		StretchBlt (cpu1_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,	    //target window and its size
			frame_dc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,	        //source image and its size
			SRCCOPY);
		StretchBlt (cpu2_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,	    //target window and its size
			frame_dc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,	        //source image and its size
			SRCCOPY);

		//Draw Image
		SelectObject(frame_dc,frame_image);    //(A handle to the DC, A handle to the object to be selected) 
		StretchBlt (cpu1_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,	    //target window and its size
			frame_dc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,	        //source image and its size
			SRCPAINT);

		//Draw Freq
		SelectObject(frame_dc,freq_image);    //(A handle to the DC, A handle to the object to be selected) 
		StretchBlt (cpu2_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,	    //target window and its size
			frame_dc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,	        //source image and its size
			SRCPAINT);

		frame_sleep_pthread.join();	   

		//NOTE: have to release the memory (Bitmap=>DeleteObject)
		DeleteObject(frame_image);
		DeleteObject(freq_image);
		DeleteObject(frame_background);
		DeleteObject(grid_background);
	}

	delete [] c_image_address;
	delete [] c_freq_address;
	system("PAUSE");
	return 0;
}
