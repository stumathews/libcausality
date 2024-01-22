#pragma once
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "ContactCircumstanceBuilder.h"
#include "ItemsOverTime.h"
#include "Option.h"
#include "Party.h"
#include "events/Event.h"
#include "utils/Utils.h"
#include "Snapshot.h"

namespace libcausality
{
	// Tracks causal events
	class CausalityTracker
	{
		public:

		std::shared_ptr<ExpectationLib::ICircumstance> TrackEvent(const std::shared_ptr<gamelib::Event>& event, const std::string& to, const unsigned long elapsedTimeMs);

		[[nodiscard]] std::unordered_map<std::string, std::shared_ptr<ExpectationLib::IParty>> GetParties() const;
		[[nodiscard]] libmonad::Option<std::shared_ptr<ExpectationLib::IParty>> GetParty(const std::string& partyId) const;

	private:

		
		[[nodiscard]] libmonad::Option<std::shared_ptr<ExpectationLib::IParty>> FindParty(
			const std::string& partyId) const;

		void InsertPartySnapshot(
			const std::string& partyId, const Snapshot<std::shared_ptr<ExpectationLib::IParty>>& snapshot);

		void ReplacePartySnapshot(
			const std::shared_ptr<ExpectationLib::IParty>& found, const std::string& senderId,
			const Snapshot<std::shared_ptr<ExpectationLib::IParty>>& snapshot);

		std::shared_ptr<ExpectationLib::IParty> AddOrUpdatePartySnapshot(
			unsigned long elapsedTime, const std::string& partyId,
			const std::shared_ptr<ExpectationLib::IParty>& item);
		std::shared_ptr<ExpectationLib::IParty> UpdateParty(const std::string& senderId, unsigned long elapsedTimeMs);

		std::unordered_map<std::string, Snapshot<std::shared_ptr<ExpectationLib::IParty>>> parties;
		ItemsOverTime<std::shared_ptr<ExpectationLib::ICircumstance>> circumstancesHistory;
		ItemsOverTime<std::shared_ptr<ExpectationLib::IParty>> partyHistory;
	};
}
