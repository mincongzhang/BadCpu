/*
#include <Windows.h>  
#include<stdlib.h>  
#include<tchar.h>  
int _tmain(int argc,_TCHAR* argv[])  
{  
    int busyTime = 3;  
    int idleTime = busyTime*5;  
    __int64 startTime = 0;  
    ::SetProcessAffinityMask(::GetCurrentProcess(),0x00000002);  
    while(true)  
    {  
        startTime = GetTickCount();  

		while((GetTickCount() - startTime) <= busyTime){}; //busy loop
		Sleep(idleTime); //idle  
    }  
    return 0;  
}
*/