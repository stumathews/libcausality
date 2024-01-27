#include "EventTap.h"


#include <algorithms/TemporalPaths.h>


#include "GameDataManager.h"
#include "Player.h"


using namespace tglib;
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

	
	
	

	void EventTap::Tap(const std::shared_ptr<gamelib::Event>& evt, const std::string& to, const unsigned long elapsedTimeMsIn)
	{
		elapsedTimeMs = elapsedTimeMsIn;

		CausalityTracker.TrackEvent(evt, to, elapsedTimeMsIn);

		const auto latestCausalRelations = CausalityTracker.GenerateTemporalNetwork();
		auto earliestArrivalPath = earliest_arrival_path(latestCausalRelations, 0, 3,latestCausalRelations.getTimeInterval());
	}

	EventTap* EventTap::instance = nullptr;
}
