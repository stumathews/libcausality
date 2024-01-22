#include "EventTap.h"
#include "GameDataManager.h"
#include "Player.h"
#include "algorithms/TemporalPaths.h"

namespace libcausality
{
	EventTap* EventTap::Get()
	{
		if (instance == nullptr)
		{
			instance = new EventTap();
		}
		return instance;
	}

	EventTap::~EventTap()
	{
		instance = nullptr;
	}

	EventTap::EventTap(): elapsedTimeMs(0)
	{
	}

	void EventTap::Tap(const std::shared_ptr<gamelib::Event>& evt, const std::string& to, const unsigned long elapsedTimeMs)
	{
		this->elapsedTimeMs = elapsedTimeMs;

		CausalityTracker.TrackEvent(evt, to, elapsedTimeMs);

		//tglib::minimum_duration_path();
		//tglib::earliest_arrival_path()
	}

	EventTap* EventTap::instance = nullptr;
}
