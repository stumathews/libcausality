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
		
		// Always keep the latest state of any parties after they experience the circumstance
		AddOrUpdatePartySnapshot(elapsedTimeMs, senderId, circumstance->GetResponse()->GetSender());
		AddOrUpdatePartySnapshot(elapsedTimeMs, receiverId, circumstance->GetResponse()->GetReceiver());
				
		// keep a copy of the encountered circumstances as the a history of circumstances
		circumstancesHistory.Add(elapsedTimeMs, circumstance, circumstance->GetId());

		return circumstance;
	}
		
	void CausalityTracker::InsertPartySnapshot(const string& partyId, const Snapshot<shared_ptr<IParty>>& snapshot)
	{
		parties.insert(unordered_map<string, Snapshot<shared_ptr<IParty>>>::value_type(partyId, snapshot));
	}

	void CausalityTracker:: ReplacePartySnapshot(const shared_ptr<IParty>& found, const string& senderId, const Snapshot<shared_ptr<IParty>>& snapshot)
	{
		parties.erase(found->GetId());
	    InsertPartySnapshot(senderId, snapshot);
	}

	shared_ptr<IParty> CausalityTracker::AddOrUpdatePartySnapshot(const unsigned long elapsedTime, const string& partyId, const shared_ptr<IParty>& item)
	{		
		FindParty(partyId).Match(
			[&](None){ InsertPartySnapshot(partyId, Snapshot(item, elapsedTime));}, 
			[&](const shared_ptr<IParty>& found) { ReplacePartySnapshot(found, partyId, Snapshot(item, elapsedTime)); });

		partyHistory.Add(elapsedTime, item, item->GetId());

		return item;
	}

	shared_ptr<IParty> CausalityTracker::UpdateParty(const string& senderId, const unsigned long elapsedTimeMs)
	{
		return FindParty(senderId).WhenNone([=, this]
		{
			// Make new party
			const auto& newParty = To<IParty>(make_shared<Party>(senderId));
			return AddOrUpdatePartySnapshot(elapsedTimeMs, senderId, newParty);
		});
	}

	unordered_map<string, shared_ptr<IParty>> CausalityTracker::GetParties() const
	{
		unordered_map<string, shared_ptr<IParty>> allParties;

		for (const auto& [partyId, partySnapshot] : parties)
		{
			allParties.insert(allParties.end(), {partyId, partySnapshot.Item()});
		}
		return allParties;
	}

	Option<shared_ptr<IParty>> CausalityTracker::FindParty(const string& partyId) const
	{
		const auto findResult = parties.find(partyId);
		return findResult == parties.end()? libmonad::Option<shared_ptr<IParty>>(None()): findResult->second.Item();
	}

	Option<shared_ptr<IParty>> CausalityTracker::GetParty(const string& partyId) const
	{
		return parties.contains(partyId) ? FindParty(partyId) : None();
	}
}
