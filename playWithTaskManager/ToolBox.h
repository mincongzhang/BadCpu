#include <windows.h>
#include <string>
#include <sstream>

std::string NumberToString ( unsigned int num ){
	std::stringstream ss;ss << num;return ss.str();
}

HDC GetCpuWindow(char * window_name,int control_id){
	HWND p_hwnd,c_hwnd,g_hwnd;

	//Get ParentWindow's hwnd
	p_hwnd=FindWindow(NULL,window_name);

	if(p_hwnd==0){
		std::cout<<"FindWindow() FAILED!"<<std::endl;
	} else {
		SetForegroundWindow(p_hwnd); //brings that window to foreground
		std::cout<<"p_hwnd="<<p_hwnd<<std::endl;            
	}

	//Get ChildWindow's hwnd
	c_hwnd=GetTopWindow(p_hwnd);
	if(c_hwnd!=0){std::cout<<"c_hwnd="<<c_hwnd<<std::endl;}
	else{std::cout<<"GetTopWindow() FAILED!"<<std::endl;}

	//Get GrandWindow's hwnd
	//(The identifier of the control to be retrieved.)
	//control ID = 00001388 in hex, = 5000 in dec
	g_hwnd=GetDlgItem(c_hwnd, control_id);//5001 second window, and so on
	if(g_hwnd!=0){std::cout<<"g_hwnd="<<g_hwnd<<std::endl;}
	else{std::cout<<"GetDlgItem() FAILED!"<<std::endl;}

	return GetWindowDC(g_hwnd);
}

RECT GetCpuWindowRect(char * window_name,int control_id){
	HWND p_hwnd,c_hwnd,g_hwnd;

	//Get ParentWindow's hwnd
	p_hwnd=FindWindow(NULL,window_name);

	if(p_hwnd==0){
		std::cout<<"FindWindow() FAILED!"<<std::endl;
	} else {
		SetForegroundWindow(p_hwnd); //brings that window to foreground
		std::cout<<"p_hwnd="<<p_hwnd<<std::endl;            
	}

	//Get ChildWindow's hwnd
	c_hwnd=GetTopWindow(p_hwnd);
	if(c_hwnd!=0){std::cout<<"c_hwnd="<<c_hwnd<<std::endl;}
	else{std::cout<<"GetTopWindow() FAILED!"<<std::endl;}

	//Get GrandWindow's hwnd
	//(The identifier of the control to be retrieved.)
	//control ID = 00001388 in hex, = 5000 in dec
	g_hwnd=GetDlgItem(c_hwnd, control_id);//5001 second window, and so on
	if(g_hwnd!=0){std::cout<<"g_hwnd="<<g_hwnd<<std::endl;}
	else{std::cout<<"GetDlgItem() FAILED!"<<std::endl;}

	RECT rc;
	GetWindowRect(g_hwnd, &rc);
	return rc;
}