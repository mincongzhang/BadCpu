#ifndef _WINDOW_HANDLER_H_
#define _WINDOW_HANDLER_H_

#include <windows.h>
#include <string>
#include <sstream>

#include "Logger.h"

namespace WINDOW_HANDLER {

	HDC GetSubWindow(char * window_name,int control_id){
		HWND p_hwnd,c_hwnd,g_hwnd;

		//Get ParentWindow's hwnd
		p_hwnd=FindWindow(NULL,window_name);

		if(p_hwnd==0){
			logInfo("FindWindow() FAILED!");
		} else {
			SetForegroundWindow(p_hwnd); //brings that window to foreground
			logInfo("p_hwnd="<<p_hwnd);            
		}

		//Get ChildWindow's hwnd
		c_hwnd=GetTopWindow(p_hwnd);
		if(c_hwnd!=0){logInfo("c_hwnd="<<c_hwnd);}
		else{logInfo("GetTopWindow() FAILED!");}

		//Get GrandWindow's hwnd
		//(The identifier of the control to be retrieved.)
		//control ID = 00001388 in hex, = 5000 in dec
		g_hwnd=GetDlgItem(c_hwnd, control_id);//5001 second window, and so on
		if(g_hwnd!=0){logInfo("g_hwnd="<<g_hwnd);}
		else{logInfo("GetDlgItem() FAILED!");}

		return GetWindowDC(g_hwnd);
	}

	RECT GetSubWindowRect(char * window_name,int control_id){
		HWND p_hwnd,c_hwnd,g_hwnd;

		//Get ParentWindow's hwnd
		p_hwnd=FindWindow(NULL,window_name);

		if(p_hwnd==0){
			logInfo("FindWindow() FAILED!");
		} else {
			SetForegroundWindow(p_hwnd); //brings that window to foreground
			logInfo("p_hwnd="<<p_hwnd);            
		}

		//Get ChildWindow's hwnd
		c_hwnd=GetTopWindow(p_hwnd);
		if(c_hwnd!=0){logInfo("c_hwnd="<<c_hwnd);}
		else{logInfo("GetTopWindow() FAILED!");}

		//Get GrandWindow's hwnd
		//(The identifier of the control to be retrieved.)
		//control ID = 00001388 in hex, = 5000 in dec
		g_hwnd=GetDlgItem(c_hwnd, control_id);//5001 second window, and so on
		if(g_hwnd!=0){logInfo("g_hwnd="<<g_hwnd);}
		else{logInfo("GetDlgItem() FAILED!");}

		RECT rc;
		GetWindowRect(g_hwnd, &rc);
		return rc;
	}

}

#endif // _WINDOW_HANDLER_H_