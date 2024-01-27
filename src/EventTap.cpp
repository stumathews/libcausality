#include "EventTap.h"
#include "GameDataManager.h"
#include "Player.h"
#include "algorithms/TemporalPaths.h"

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

	void addEdge(std::vector<TemporalEdge> &edges, const NodeId u, const NodeId v, const Time t, const Time tt)
	{
	    edges.push_back(TemporalEdge{u, v, t, tt});
	}
	


	void EventTap::Tap(const std::shared_ptr<gamelib::Event>& evt, const std::string& to, const unsigned long elapsedTimeMs)
	{
		this->elapsedTimeMs = elapsedTimeMs;

		const auto circumstance = CausalityTracker.TrackEvent(evt, to, elapsedTimeMs);

		/*const auto sender =  circumstance->GetResponse()->GetSender();
		const auto receiver = circumstance->GetResponse()->GetReceiver();
		auto senderRelations = sender->GetRelations();

		std::vector<TemporalEdge> edges;

		for(int i = 0; i <= senderRelations.size()-1;i++)
		{
			const auto startTime = senderRelations[i].StartTime.WhenNone([](){ return 1; });
			const auto endTime = senderRelations[i].EndTime.WhenNone([](){ return 1; });
			 addEdge(edges, senderRelations[i]. , 1, startTime, endTime );
		}

		OrderedEdgeList<tglib::TemporalEdge> tgs(10, edges, {0,15});*/
		//tglib::minimum_duration_path(tgs,)
		//tglib::earliest_arrival_path()
	}

	EventTap* EventTap::instance = nullptr;
}
