#include "stdafx.h"
#include "EventManager.h"

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

HRESULT EventManager::Init()
{
	return S_OK;
}

void EventManager::Release()
{
}

bool EventManager::RegisterDelegate(const EventType type, const EventDelegate & delegate)
{
	bool result = true;
	EventDelegates &delegateList = _eventDelegatesMap[type];
	for (auto &iter = delegateList.begin(); iter != delegateList.end(); ++iter)
	{
		if ((*iter) == delegate)
		{
			Console::Log("You tried to register same delegate function twice\n");
			result = false;
			return result;
		}
	}
	delegateList.push_back(delegate);
	return result;
}

void EventManager::UnRegisterDelegate(const EventType type, const EventDelegate & delegate)
{
	EventDelegates &delegateList = _eventDelegatesMap[type];
	for (auto &iter = delegateList.begin(); iter != delegateList.end();)
	{
		if ((*iter) == delegate)
		{
			iter = delegateList.erase(iter);
			break;
		}
		else
		{
			++iter;
		}
	}
}

bool EventManager::FireEvent(const IEvent * event)
{
	bool result = false;
	EventDelegates &delegateList = _eventDelegatesMap[event->GetType()];
	for (auto &iter = delegateList.begin(); iter != delegateList.end(); ++iter)
	{
		iter->operator()(event);
		result = true;
	}
	return result;
}

bool EventManager::QueueEvent(const IEvent * event)
{
	bool result = false;
	auto &find = _eventDelegatesMap.find(event->GetType());
	if (find != _eventDelegatesMap.end())
	{
		_eventQueue[_activeQueue].push_back(event);
		result = true;
	}
	return result;
}

void EventManager::DiscardAllEvents()
{
	int processingQueue = _activeQueue;
	_activeQueue = (++_activeQueue) % 2;
	_eventQueue[_activeQueue].clear();

	while (!_eventQueue[processingQueue].empty())
	{
		const IEvent *event = _eventQueue[processingQueue].front();
		_eventQueue[processingQueue].pop_front();	
		delete event;
	}
}

bool EventManager::Update(float deltaTime)
{
	bool flushed = false;

	int processingQueue = _activeQueue;
	_activeQueue = (++_activeQueue) % 2;
	_eventQueue[_activeQueue].clear();

	int count = 0;

	while (!_eventQueue[processingQueue].empty())
	{
		const IEvent *event = _eventQueue[processingQueue].front();
		_eventQueue[processingQueue].pop_front();

		auto &eventDelegates = _eventDelegatesMap.find(event->GetType());

		if (eventDelegates != _eventDelegatesMap.end())
		{
			const EventDelegates &delegates = eventDelegates->second;
			for (auto &iter = delegates.begin(); iter != delegates.end(); ++iter)
			{
				iter->operator()(event);
			}
		}

		delete event;

		if (TIMEMANAGER->HasToAdvance())
		{
			flushed = true;
			break;
		}
	}

	bool transfered = false;
	while (!_eventQueue[processingQueue].empty())
	{
		const IEvent *event = _eventQueue[processingQueue].back();
		_eventQueue[processingQueue].pop_back();
		_eventQueue[_activeQueue].push_front(event);

		transfered = true;
	}

	if (!flushed)
	{
		TIMEMANAGER->Tick();
	}

	return flushed;
}
