#include "CausalityTracker.h"
#include <ranges>
#include "Party.h"

using namespace libmonad;
using namespace gamelib;
using namespace std;
using namespace ExpectationLib;

namespace libcausality
{
	shared_ptr<ICircumstance> CausalityTracker::TrackEvent(const shared_ptr<Event>& event, const string& to, const unsigned long elapsedTimeMs)
	{
		const auto senderId = event->Origin.empty() ? string("Unknown") : event->Origin;
		auto& receiverId = to;			
		
		// Get the sender and the receiver and update their states
		const auto& sender = GetLatestPartyOrAdd(senderId, elapsedTimeMs);						
		const auto& receiver = GetLatestPartyOrAdd(receiverId, elapsedTimeMs);

		// Trigger the circumstance (sender -[stimulus:contact]-> receiver : result)
		auto circumstance = ContactCircumstanceBuilder::Build(sender, receiver, event->Id.Name, elapsedTimeMs);

		// Add the result/change to the party history
		partyHistory.Add(circumstance->GetResponse()->GetResult().ThrowIfNone()->GetSender(), senderId, elapsedTimeMs);
		partyHistory.Add(circumstance->GetResponse()->GetResult().ThrowIfNone()->GetReceiver(), receiverId, elapsedTimeMs);
						
		// keep a copy of the encountered circumstances as the a history of circumstances
		circumstancesHistory.Add(circumstance, circumstance->GetId(), elapsedTimeMs);

		return circumstance;
	}
	
	shared_ptr<IParty> CausalityTracker::GetLatestPartyOrAdd(const string& senderId, const unsigned long elapsedTimeMs)
	{
		return FindLatestParty(senderId).WhenNone([=, this]
		{
			// Make new party
			const auto& newParty = To<IParty>(make_shared<Party>(senderId));
			partyHistory.Add(newParty, senderId, elapsedTimeMs);
			return newParty;
		});
	}

	unordered_map<string, shared_ptr<IParty>> CausalityTracker::GetLatestParties()
	{
		unordered_map<string, shared_ptr<IParty>> allParties;

		for (const auto& itemSnapshot : partyHistory.GetAllLatestItems())
		{
			auto party = itemSnapshot.Item();
			allParties.insert(allParties.end(), {party->GetId(), party});
		}
		return allParties;
	}

	Option<shared_ptr<IParty>> CausalityTracker::FindLatestParty(const string& partyId)
	{
		return partyHistory
		.GetLatestItemByKey(partyId)
		.Map<shared_ptr<IParty>>([](const Snapshot<shared_ptr<IParty>>& item)
		{
			return item.Item();
		});
	}

	int CausalityTracker::Exchange(const std::string& itemKey){ return partyHistory.Exchange(itemKey); }
	Option<std::string> CausalityTracker::Exchange(const int itemNumber){ return partyHistory.Exchange(itemNumber);}

	Option<shared_ptr<IParty>> CausalityTracker::GetLatestParty(const string& partyId) { return FindLatestParty(partyId); }

	void AddEdge(std::vector<tglib::TemporalEdge> &edges, const tglib::NodeId u, const tglib::NodeId v, const tglib::Time t, const tglib::Time tt)
	{
	    edges.push_back(tglib::TemporalEdge{u, v, t, tt});
	}

	tglib::IncidentLists<tglib::TGNode, tglib::TemporalEdge> CausalityTracker::GenerateTemporalNetwork()
	{
		std::map<int, int> nodeFrequencies;
		auto currentParties = GetLatestParties();

		std::vector<tglib::TemporalEdge> edges;
		for(const auto& party : currentParties | std::views::values)
		{
			for(auto& relation : party->GetRelations())
			{
				const auto startTime = relation.StartTime.WhenNone([]{ return 1;});
				const auto endTime = relation.EndTime.WhenNone([]{ return 1;});
				const auto u =  Exchange(party->GetId());
				const auto v = Exchange(relation.To->GetId());
				nodeFrequencies[u]++;
				nodeFrequencies[v]++;
				AddEdge(edges,u, v, startTime, endTime);
			}
		}
		const auto countNodes = static_cast<int>(nodeFrequencies.size());
		tglib::IncidentLists<tglib::TGNode, tglib::TemporalEdge> edgeList(countNodes, edges);
		return edgeList;
	}
}
