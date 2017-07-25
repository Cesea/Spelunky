#include "stdafx.h"
#include "EventCollector.h"

EventCollector::EventCollector()
{
}

EventCollector::~EventCollector()
{
}

HRESULT EventCollector::Init()
{
	RegisterDelegates();
	return S_OK;
}

void EventCollector::Release()
{
	UnRegisterDelegates();
}

const IEvent * EventCollector::GetFirstEnemyDeadEvent()
{
	const IEvent *result = nullptr;
	auto &found = _eventMap.find(EVENT_ENEMY_DEAD);
	if (found != _eventMap.end())
	{
		if (found->second.size() > 0)
		{
			result = found->second.front();
			found->second.pop_front();
		}
	}
	return result;
}

const IEvent * EventCollector::GetFirstCollectMoneyEvent()
{
	const IEvent *result = nullptr;
	auto &found = _eventMap.find(EVENT_COLLECT_MONEY);
	if (found != _eventMap.end())
	{
		if (found->second.size() > 0)
		{
			result = found->second.front();
			found->second.pop_front();
		}
	}
	return result;
}

void EventCollector::RegisterDelegates()
{
	EVENTMANAGER->RegisterDelegate(EVENT_ENEMY_DEAD, 
		EventDelegate::FromFunction<EventCollector, &EventCollector::HandleEnemyDeadEvent>(this));
	EVENTMANAGER->RegisterDelegate(EVENT_COLLECT_MONEY, 
		EventDelegate::FromFunction<EventCollector, &EventCollector::HandleCollectMoneyEvent>(this));
}


void EventCollector::UnRegisterDelegates()
{
	EVENTMANAGER->UnRegisterDelegate(EVENT_ENEMY_DEAD, 
		EventDelegate::FromFunction<EventCollector, &EventCollector::HandleEnemyDeadEvent>(this));
	EVENTMANAGER->UnRegisterDelegate(EVENT_COLLECT_MONEY, 
		EventDelegate::FromFunction<EventCollector, &EventCollector::HandleCollectMoneyEvent>(this));
}

void EventCollector::HandleEnemyDeadEvent(const IEvent * event)
{
	IEvent *copyiedEvent = event->Copy();

	EventType type = copyiedEvent->GetType();
	auto &found = _eventMap.find(type);
	if (found == _eventMap.end())
	{
		_eventMap.insert(std::make_pair(type, EventList()));
		auto &toInsert = _eventMap.find(type);
		toInsert->second.push_back(copyiedEvent);
	}
	else
	{
		found->second.push_back(copyiedEvent);
	}
}

void EventCollector::HandleCollectMoneyEvent(const IEvent * event)
{
	IEvent *copyiedEvent = event->Copy();

	EventType type = copyiedEvent->GetType();
	auto &found = _eventMap.find(type);
	if (found == _eventMap.end())
	{
		_eventMap.insert(std::make_pair(type, EventList()));
		auto &toInsert = _eventMap.find(type);
		toInsert->second.push_back(copyiedEvent);
	}
	else
	{
		found->second.push_back(copyiedEvent);
	}
}
