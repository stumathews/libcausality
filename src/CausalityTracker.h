#pragma once
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <core/IncidentLists.h>
#include "ContactCircumstanceBuilder.h"
#include "ItemsOverTime.h"
#include "Option.h"
#include "events/Event.h"
#include "utils/Utils.h"
#include <core/BasicTypes.h>

namespace libcausality
{
	// Tracks causal events
	class CausalityTracker
	{
	public:

		std::shared_ptr<ExpectationLib::ICircumstance> TrackEvent(const std::shared_ptr<gamelib::Event>& event, const std::string& to, const unsigned long elapsedTimeMs);

		[[nodiscard]] std::unordered_map<std::string, std::shared_ptr<ExpectationLib::IParty>> GetLatestParties();
		[[nodiscard]] libmonad::Option<std::shared_ptr<ExpectationLib::IParty>> GetLatestParty(const std::string& partyId);
		tglib::IncidentLists<tglib::TGNode, tglib::TemporalEdge> GenerateTemporalNetwork();

		int Exchange(const std::string& itemKey);
		libmonad::Option<std::string> Exchange(int itemNumber);
	private:
		
		[[nodiscard]] libmonad::Option<std::shared_ptr<ExpectationLib::IParty>> FindLatestParty(const std::string& partyId);
		

		std::shared_ptr<ExpectationLib::IParty> GetLatestPartyOrAdd(const std::string& senderId, unsigned long elapsedTimeMs);
		
		ItemsOverTime<std::shared_ptr<ExpectationLib::ICircumstance>> circumstancesHistory;
		ItemsOverTime<std::shared_ptr<ExpectationLib::IParty>> partyHistory;
	};
}
