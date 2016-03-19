#include <windows.h>
#include <iostream>
#include <string.h>
#include <tchar.h>  

#include <boost/thread.hpp>

#include "Logger.h"
#include "Config.h"
#include "ReplaceColor.h"
#include "ScreenCapture.h"
#include "WindowHandler.h"


namespace {
	const int FRAME_WAIT_TIME_MS = 30;

	std::string NumberToString ( unsigned int & num ){
		std::stringstream ss;ss << num;return ss.str();
	}

	void frameSleeper(){
		boost::this_thread::sleep(boost::posix_time::millisec(FRAME_WAIT_TIME_MS));
	}
}

int main()
{ 
	Config cfg("cfg.ini");

	//Get hdc
	HDC cpu_image_hdc = WINDOW_HANDLER::GetSubWindow(cfg.m_window_name,cfg.m_cpu_image_id);
	HDC cpu_wave_hdc = WINDOW_HANDLER::GetSubWindow(cfg.m_window_name,cfg.m_cpu_wave_id);
	if(cpu_image_hdc!=0){logInfo("cpu_image_hdc="<<cpu_image_hdc);}
	else{logInfo("GetWindowDC() FAILED!: "<<cfg.m_window_name);}
	if(cpu_wave_hdc!=0){logInfo("cpu_wave_hdc="<<cpu_wave_hdc);}
	else{logInfo("GetWindowDC() FAILED!");}

	//boost::this_thread::sleep(boost::posix_time::millisec(1000));

	logInfo("Ready to draw...");
	system("PAUSE");

	RECT rc = WINDOW_HANDLER::GetSubWindowRect(cfg.m_window_name,cfg.m_cpu_background_id);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	logInfo("rc.right:"<<rc.right<<"rc.left:"<<rc.left);
	logInfo("rc.bottom:"<<rc.bottom<<"rc.top:"<<rc.top);
	logInfo("width:"<<width<<"height:"<<height);

	char *c_image_address = new char[100];
	char *c_wave_address = new char[100];
	std::string image_address;
	std::string wave_address;
	HDC image_dc=CreateCompatibleDC(cpu_image_hdc);//create a Memory Device Contexts(DC)

		//*************************Draw*************************
	for(unsigned int i=1;i<cfg.m_frame_num;i++){
		boost::thread frame_sleep_pthread(frameSleeper);

		SCREEN_CAPTURE::screenCapture(rc.left, rc.top,width,height,cfg.m_screenshot_address);

		image_address =cfg.m_image_path + NumberToString(i) + cfg.m_image_file_suffix;
		wave_address =cfg.m_image_path + "a" + NumberToString(i) + cfg.m_image_file_suffix;
		strcpy(c_image_address, image_address.c_str());
		strcpy(c_wave_address, wave_address.c_str());

		HANDLE image;
		HANDLE wave_image;
		HANDLE image_background;
		HBITMAP grid_background;

		image=LoadImage(0,(LPCTSTR)c_image_address,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//Load image from path
		wave_image=LoadImage(0,(LPCTSTR)c_wave_address,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//Load image from path

		image_background = LoadImage(0,(LPCTSTR)cfg.m_screenshot_address,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//Load image from path
		logInfo("Loading image["<<c_image_address<<"] :"<<image);

		grid_background = REPLACE_COLOR::ReplaceColor(HBITMAP(image_background),0x00ff00,0x000000,image_dc); // replace green by black

		//select into image_dc
		SelectObject(image_dc,grid_background);    //(A handle to the DC, A handle to the object to be selected) 
		StretchBlt (cpu_image_hdc, 0, 0, cfg.m_window_width, cfg.m_window_height,	    //target window and its size
			image_dc, 0, 0, cfg.m_window_width, cfg.m_window_height,	                //source image and its size
			SRCCOPY);
		StretchBlt (cpu_wave_hdc, 0, 0, cfg.m_window_width, cfg.m_window_height,	    //target window and its size
			image_dc, 0, 0, cfg.m_window_width, cfg.m_window_height,	                //source image and its size
			SRCCOPY);

		//Draw Image
		SelectObject(image_dc,image);    //(A handle to the DC, A handle to the object to be selected) 
		StretchBlt (cpu_image_hdc, 0, 0, cfg.m_window_width, cfg.m_window_height,	    //target window and its size
			image_dc, 0, 0, cfg.m_window_width, cfg.m_window_height,	                //source image and its size
			SRCPAINT);

		//Draw Wave
		SelectObject(image_dc,wave_image);    //(A handle to the DC, A handle to the object to be selected) 
		StretchBlt (cpu_wave_hdc, 0, 0, cfg.m_window_width, cfg.m_window_height,	    //target window and its size
			image_dc, 0, 0, cfg.m_window_width, cfg.m_window_height,	                //source image and its size
			SRCPAINT);

		frame_sleep_pthread.join();	   

		//NOTE: have to release the memory (Bitmap=>DeleteObject)
		DeleteObject(image);
		DeleteObject(wave_image);
		DeleteObject(image_background);
		DeleteObject(grid_background);
	}

	delete [] c_image_address;
	delete [] c_wave_address;
	system("PAUSE");
	return 0;
}
