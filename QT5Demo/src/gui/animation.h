
#pragma once
#include "../global/def.h"
#include <QtCore/QTimer>
#include <QSet>
class Animated;

int getms();

namespace anim 
{
	void start(Animated *obj);
	void step(Animated *obj);
	void stop(Animated *obj);

	void startManager();
	void stopManager();

};

class Animated 
{
public:

	Animated() : animStarted(0), animInProcess(false) 
	{
	}

	virtual bool animStep(int ms) = 0;

	void animReset() 
	{
		animStarted = getms();
	}

	virtual ~Animated() 
	{
		if (animating()) 
		{
			anim::stop(this);
		}
	}

	bool animating() const 
	{
		return animInProcess;
	}

private:

	int animStarted;
	bool animInProcess;
	friend class AnimationManager;

};

class AnimationFunc 
{
public:
	virtual bool animStep(int ms) = 0;
	virtual ~AnimationFunc() 
	{
	}
};

template <typename Type>
class AnimationFuncOwned : public AnimationFunc 
{
public:
	typedef bool (Type::*Method)(int);
	AnimationFuncOwned(Type *obj, Method method) : _obj(obj), _method(method) 
	{
	}

	bool animStep(int ms) 
	{
		return (_obj->*_method)(ms);
	}
	
private:
	Type *_obj;
	Method _method;
	
};

template <typename Type>
AnimationFunc *animFunc(Type *obj, typename AnimationFuncOwned<Type>::Method method) 
{
	return new AnimationFuncOwned<Type>(obj, method);
}

class Animation : public Animated 
{
public:

	Animation(AnimationFunc *func) : _func(func) 
	{
	}

	void start() 
	{
		anim::start(this);
	}
	void stop() 
	{
		anim::stop(this);
	}

	//Animation
	bool animStep(int ms) 
	{
		return _func->animStep(ms);
	}

	~Animation()
	{
		delete _func;
	}

private:
	AnimationFunc *_func;

};

class AnimationManager : public QObject 
{
Q_OBJECT

public:

	AnimationManager() : timer(this), iterating(false) 
	{
		timer.setSingleShot(false);
		connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
	}

	void start(Animated *obj) 
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     	{
		obj->animReset();
		if (iterating)
		{
			toStart.insert(obj);
			if (!toStop.isEmpty()) 
			{
				toStop.remove(obj);
			}
		} 
		else 
		{
			if (!objs.size()) 
			{
				timer.start(TIMER_ANIMI_DELTA);
			}
			objs.insert(obj);
		}
		obj->animInProcess = true;
	}

	void step(Animated *obj) 
	{
		if (iterating) return;
		int ms = int(getms());
		AnimObjs::iterator i = objs.find(obj);
		if (i != objs.cend()) 
		{
			Animated *obj = *i;
			if (!obj->animStep(ms - obj->animStarted)) 
			{
				objs.erase(i);
				if (!objs.size()) {
					timer.stop();
				}
				obj->animInProcess = false;
			}
		}
	}

	void stop(Animated *obj) {
		if (iterating) {
			toStop.insert(obj);
			if (!toStart.isEmpty()) {
				toStart.insert(obj);
			}
		} else {
			AnimObjs::iterator i = objs.find(obj);
			if (i != objs.cend()) {
				objs.erase(i);
				if (!objs.size()) {
					timer.stop();
				}
			}
		}
		obj->animInProcess = false;
	}

public slots:
	void timeout() {
		iterating = true;
		int ms = int(getms());
		for (AnimObjs::iterator i = objs.begin(), e = objs.end(); i != e; ) {
			Animated *obj = *i;
			if (!obj->animStep(ms - obj->animStarted)) {
				i = objs.erase(i);
				obj->animInProcess = false;
			} else {
				++i;
			}
		}
		iterating = false;
		if (!toStart.isEmpty()) {
			for (AnimObjs::iterator i = toStart.begin(), e = toStart.end(); i != e; ++i) {
				objs.insert(*i);
			}
			toStart.clear();
		}
		if (!toStop.isEmpty()) {
			for (AnimObjs::iterator i = toStop.begin(), e = toStop.end(); i != e; ++i) {
				objs.remove(*i);
			}
			toStop.clear();
		}
		if (!objs.size()) {
			timer.stop();
		}
	}

private:

	typedef QSet<Animated*> AnimObjs;
	AnimObjs objs;
	AnimObjs toStart;
	AnimObjs toStop;
	QTimer timer;
	bool iterating;

};