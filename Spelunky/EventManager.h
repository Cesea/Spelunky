#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "singletonBase.h"
#include "Delegate.h"
#include <unordered_map>
#include <list>

typedef Delegate<void, const IEvent *> EventDelegate;

class EventManager : public singletonBase<EventManager>
{
	typedef std::list<EventDelegate> EventDelegates;
	typedef std::unordered_map<EventType, EventDelegates> EventDelegatesMap;
	typedef std::list<const IEvent *> EventQueue;

public:
	EventManager();
	virtual ~EventManager();

	HRESULT Init();
	void Release();

	bool RegisterDelegate(const EventType type, const EventDelegate &delegate);
	void UnRegisterDelegate(const EventType type, const EventDelegate &delegate);

	bool FireEvent(const IEvent *event);
	bool QueueEvent(const IEvent *event);

	void DiscardAllEvents();
	//void RemoveEvent(IEvent *event);

	bool Update(float deltaTime);

private:
	EventDelegatesMap _eventDelegatesMap;
	EventQueue _eventQueue[2];
	int _activeQueue{ 0 };

};


#endif