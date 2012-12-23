#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>

double mul[2048];

inline unsigned __int64 GetCycleCount(void)
{
	_asm _emit 0x0F
	_asm _emit 0x31
}  

class KTimer
{
	unsigned __int64 m_startcycle;
	public:
	unsigned __int64 m_overhead;
	KTimer(void)
	{
		m_overhead=0;
		Start();
		m_overhead=Stop();
	}
	void Start(void)
	{
		m_startcycle=GetCycleCount();
	}
	unsigned __int64 Stop(void)
	{
		return GetCycleCount()-m_startcycle-m_overhead;
	} 
};

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE,LPSTR lpCmd,int nShow)
{
	KTimer timer;
	TCHAR mess[128];
	timer.Start();
	Sleep(1000);
	unsigned cpuspeedIO=(unsigned)(timer.Stop()/100000);
	double j=1.0;
	for(int i=0;i<2048;i++){mul[i]=10000.0/(j+((double)i));}
	timer.Start();
	for(int i=0;i<2048;i++){mul[i]*=mul[i+1];}
	unsigned time1=(unsigned)timer.Stop();
	timer.Start();
	for(int i=0;i<2048;i++){mul[i]+=mul[i+1];}
	unsigned time2=(unsigned)timer.Stop();
	swprintf_s(mess,_T("CPU speed   %d.%d mhz \n")
	_T("Mul Gflops: %.4f,Time: %d ns\n")
	_T("Sum Gflops: %.4f,Time: %d ns\n"),
	cpuspeedIO/10,cpuspeedIO%10,
	2048.0*((double)cpuspeedIO)/(1000.0*((double)time1)),time1*10000/cpuspeedIO,
	2048.0*((double)cpuspeedIO)/(1000.0*((double)time2)),time2*10000/cpuspeedIO);
	MessageBox(NULL,mess,_T("How fast CPU?"),MB_OK);
	/*timer.Start();
	CreateSolidBrush(RGB(0xAA,0xAA,0xAA));
	unsigned time=(unsigned)timer.Stop();
	swprintf_s(mess,_T("CPU speed   %d.%d mhz \n")
	_T("KTimer overhead %d clock cycles\n")
	_T("CreateSolidBrush %d clock cycles %d ns\n"),
	cpuspeedIO/10,cpuspeedIO%10,(unsigned)timer.m_overhead,time,time*10000/cpuspeedIO);
	MessageBox(NULL,mess,_T("How fast WINGDI?"),MB_OK);*/
	return 0;
}