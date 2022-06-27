#include "pch.h"
#include "AllEvents.h"
#include "EventTimer.h"

//
//Class that handles scheduling events during the game
//


EventTimer EventTimer::Instance;

// Adds all Events to the list of spawnable events
void EventTimer::GenerateAllEvents()
{
	for (Events pos : EventsVector) {
		AddEvent(pos);
	}
}

//Adds a event of type "ev" to the queue
bool EventTimer::SpawnEvent(Events ev)
{
	if (!FindEvent(ev))return false;
	switch (ev)
	{
	case Events::none:
		break;
	case Events::Erdbeben:
		queue.push_back(new Erdbeben);
		break;
	case Events::Race:
		queue.push_back(new Race);
		break;
	default:
		break;
	}
	return true;
}

//Returns the period in seconds for which events have already been scheduled
float EventTimer::CountScheduledTime()
{
	float time = 0;
	for (auto i = queue.begin(); i != queue.end(); i++)
	{
		time += (*i)->GetTimer()+ (*i)->GetDuration();
	}
	return time + minuteMarker;
}

// Initializes the event queue
bool EventTimer::Init(float eventsPerMin)
{
	float usedTime = 0;

	queue.clear();

	this->eventsPerMin = eventsPerMin;

	//Event** genEvents = new  Event * [eventsPerMin];

	// Fills the queue with the neede amount of events
	for (int i = 0; i < eventsPerMin; i++){
		if (SpawnRandomEvent()) {
			usedTime += queue.back()->GetDuration();
		}
		else
			break;
	}
	if (usedTime > 60)return false;

	float availableTime = 60 - usedTime - (eventsPerMin) * minAbst  ;

	// Sets the start times for the events and checks for time constraints
	for (auto i = queue.begin(); i != queue.end(); i++)
	{
		if (availableTime < 1)
			return false;
		float timerValue = rand() % static_cast<int>(availableTime);
		timerValue += minAbst;

		(*i)->SetEventTimer(timerValue);

		availableTime -= timerValue;
	}
	//No Events in the first 10 Seconds
	//queue.front()->SetEventTimer(queue.front()->GetTimer() + 10);

	//Adds the MinuteMarker to mark for which intervall the quota has been met
	minuteMarker = availableTime;


	return true;
}

Event* EventTimer::Tick(float fTimeDelta)
{
	if (markedToDelete) {
		delete markedToDelete;
		markedToDelete = nullptr;
	}


	//Decrease timer or delete event 
	if (!queue.front()->DecreaseTimer(fTimeDelta)) {
		if (queue.front()->GetActive()) {
			markedToDelete = queue.front();
			queue.pop_front();
			GenerateNewEvent();
			return nullptr;
		}
		return queue.front();
	}
	return nullptr;
}


bool EventTimer::GenerateNewEvent()
{
	float reservedTime = CountScheduledTime();

	Event* next = nullptr;
	if (SpawnRandomEvent())
	next = queue.back();
	else return false;
	//Checks for time Constraints
	int remainingTime = static_cast<int>(60 - reservedTime - next->GetDuration());
	next->SetEventTimer((rand() % remainingTime) + minAbst);


	minuteMarker = 60 - CountScheduledTime();



	return true;
}

// Adds thus event to the list of spawnable events
void EventTimer::AddEvent(Events ev)
{
	if (ev == Events::none) {
		GenerateAllEvents();
		return;
	}


	for (auto i = eventList.begin(); i < eventList.end(); i++)
	{
		if (*i == ev)
			return;
	}
	eventList.push_back(ev);
}
