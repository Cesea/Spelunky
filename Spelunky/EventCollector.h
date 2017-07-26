#ifndef EVENT_COLLECTOR_H
#define EVENT_COLLECTOR_H

#include "singletonBase.h"


class EventCollector : public singletonBase<EventCollector>
{

	typedef std::list<const IEvent *> EventList;
	typedef std::unordered_map < EventType, EventList> EventListMap;

public :
	EventCollector();
	~EventCollector();

	HRESULT Init();
	void Release();

	const IEvent *GetFirstEnemyDeadEvent();
	const IEvent *GetFirstCollectMoneyEvent();

	int GetEnemyDeadCount() { return _enemyDeadCount; }

private :
	void RegisterDelegates();
	void UnRegisterDelegates();

	void HandleEnemyDeadEvent(const IEvent *event);
	void HandleCollectMoneyEvent(const IEvent *event);

	EventListMap _eventMap;

	int _enemyDeadCount{ 0 };
};


#endif