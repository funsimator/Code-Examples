#pragma once
#include <list>
#include <unordered_set>

class EventTimer
{
private: 
	EventTimer() {};

	void GenerateAllEvents();

	Event* markedToDelete = nullptr;

	static EventTimer Instance;

	std::list<Event *> queue;

	// list of spawnable events
	std::vector<Events> eventList;

	bool SpawnRandomEvent() { return SpawnEvent(*(eventList.begin() + (rand() % eventList.size()))); };

	float minAbst = 2;

	bool SpawnEvent(Events ev);

	bool FindEvent(Events ev) {
		for (auto i = eventList.begin(); i != eventList.end(); i++)
		{
			if (*i == ev)return true;
		}
		return false;
	}

	float eventsPerMin = 4;

	float minuteMarker=0;
	/// <summary>
	/// Returns the Combined Time of all Scheduled Events + Durations + MinuteMarker
	/// </summary>
	float CountScheduledTime();


	bool GenerateNewEvent();

public:
	EventTimer(EventTimer&) = delete;
	EventTimer(EventTimer&&) = delete;


	bool Init(float eventsPerMin=3);

	Event* Tick(float fTimeDelta);

	void EventEnded() { queue.pop_front(); GenerateNewEvent();
	}

	static EventTimer& GetInstance() {
		return Instance;
	}

	/// <summary>
	/// Adds the Eventtype to the pool of Events that can be spawned.
	/// If ev = none (default value) than all events are added to the pool.
	/// </summary>
	/// <param name="ev">Event to be added.</param>
	void AddEvent(Events ev = Events::none);



};

