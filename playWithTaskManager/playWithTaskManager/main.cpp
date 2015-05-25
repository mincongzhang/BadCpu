#include <Windows.h>  
#include<stdlib.h>  
#include<tchar.h>  
int _tmain(int argc,_TCHAR* argv[])  
{  
    int busyTime = 2;  
    int idleTime = busyTime;  
    __int64 startTime = 0;  
    ::SetProcessAffinityMask(::GetCurrentProcess(),0x00000001);  
    while(true)  
    {  
        startTime = GetTickCount();  
                //busy loop  
        while((GetTickCount() - startTime) <= busyTime);  
                //idle loop  
            Sleep(idleTime);  
    }  
    return 0;  
}