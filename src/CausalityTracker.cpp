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
		const auto& sender = UpdateParty(senderId, elapsedTimeMs);						
		const auto& receiver = UpdateParty(receiverId, elapsedTimeMs);

		// Trigger the circumstance (sender -[stimulus:contact]-> receiver : result)
		auto circumstance = ContactCircumstanceBuilder::Build(sender, receiver, event->Id.Name);
						
		// keep a copy of the encountered circumstances as the a history of circumstances
		circumstancesHistory.Add(elapsedTimeMs, circumstance, circumstance->GetId());

		return circumstance;
	}
	
	shared_ptr<IParty> CausalityTracker::UpdateParty(const string& senderId, const unsigned long elapsedTimeMs)
	{
		return FindParty(senderId).WhenNone([=, this]
		{
			// Make new party
			const auto& newParty = To<IParty>(make_shared<Party>(senderId));
			partyHistory.Add(elapsedTimeMs, newParty, senderId);
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

	Option<shared_ptr<IParty>> CausalityTracker::FindParty(const string& partyId)
	{
		return partyHistory
		.GetLatestItemByKey(partyId)
		.Map<shared_ptr<IParty>>([](const Snapshot<shared_ptr<IParty>>& item)
		{
			return item.Item();
		});
	}

	Option<shared_ptr<IParty>> CausalityTracker::GetParty(const string& partyId)
	{
		return FindParty(partyId);
	}
}
