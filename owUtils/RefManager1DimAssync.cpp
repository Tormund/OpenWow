#include "stdafx.h"

// Inlcudes
#include "RefItem.h"

// General
#include "RefManager1DimAssync.h"

#ifndef DISABLE_ASSYNC
DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	RefManager1DimAssync<RefItem>* manager = (RefManager1DimAssync<RefItem>*) lpParam;

	while (true)
	{
		WaitForSingleObject(manager->m_Event_Add, INFINITE);

		string name;
		RefItem* refItem;
		if(!manager->m_ObjectsToLoad.pop_front(&name, &refItem))
		{
			ResetEvent(manager->m_Event_Add);
			continue;
		}

		manager->LoadAction(name, refItem);

		//Sleep(10); // DELETE ME
	}

	ExitThread(0);
}
#endif