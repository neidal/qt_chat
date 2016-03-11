
#include "stdafx.h"
#include "../global/types.h"
#include "animation.h"
namespace 
{
	AnimationManager *manager = 0;
};

int getms()
{
#ifdef Q_OS_WIN
	int nMs = GetTickCount();
	return nMs;
#elif defined Q_OS_MAC
	
#else
	
#endif
}

namespace anim 
{
	void start(Animated *obj)
	{
		if (!manager) return;
		manager->start(obj);
	}

	void step(Animated *obj) 
	{
		if (!manager) return;
		manager->step(obj);
	}

	void stop(Animated *obj) 
	{
		if (!manager) return;
		manager->stop(obj);
	}

	void startManager() 
	{
		delete manager;
		manager = new AnimationManager();
	}

	void stopManager() 
	{
		delete manager;
		manager = 0;
	}

}