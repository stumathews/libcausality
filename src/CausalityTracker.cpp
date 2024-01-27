#include "CausalityTracker.h"

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
		auto circumstance = ContactCircumstanceBuilder::Build(sender, receiver, event->Id.Name);

		// Add the result/change to the party history
		partyHistory.Add(circumstance->GetResponse()->GetSender(), senderId, elapsedTimeMs);
		partyHistory.Add(circumstance->GetResponse()->GetReceiver(), receiverId, elapsedTimeMs);
						
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

	unordered_map<string, shared_ptr<IParty>> CausalityTracker::GetParties()
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

	Option<shared_ptr<IParty>> CausalityTracker::GetParty(const string& partyId) { return FindLatestParty(partyId); }
}
