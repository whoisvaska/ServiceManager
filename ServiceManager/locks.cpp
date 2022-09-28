#include <Windows.h>

extern DWORD g_allLocks;

void Lock()
{
	++g_allLocks;
}

void UnLock()
{
	--g_allLocks;

	if (g_allLocks == 0)
		PostQuitMessage(0);
}
