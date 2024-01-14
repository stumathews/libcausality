#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "ContactCircumstanceBuilder.h"
#include "IParty.h"
#include "Party.h"
#include "events/Event.h"
#include "Option.h"

namespace libcausality
{
	// Tracks causal events
	class CausalityTracker
	{
		public:
		std::shared_ptr<ExpectationLib::ICircumstance> TrackEvent(const std::shared_ptr<gamelib::Event>& event,
		                                                          const std::string& to)
		{
			auto& senderId = event->Origin;
			auto receiverId = to;			

			if(!parties.contains(senderId))
			{
				parties.insert({{senderId, std::make_shared<ExpectationLib::Party>(senderId)}});
			}

			if(!parties.contains(receiverId))
			{
				parties.insert({{receiverId, std::make_shared<ExpectationLib::Party>(receiverId)}});
			}

			// establish the circumstance
			const auto sender = parties[senderId];
			const auto receiver = parties[receiverId];
			auto circumstance = ExpectationLib::ContactCircumstanceBuilder::Build(sender, receiver);

			// keep a copy of the encountered circumstances as the a history of circumstances
			history.insert(history.end(), circumstance);

			// Always keep the latest state of any parties involved in the experienced circumstance
			parties[senderId] = circumstance->GetResponse()->GetSender();
			parties[receiverId] = circumstance->GetResponse()->GetReceiver();			

			return circumstance;
		}

		std::unordered_map<std::string, std::shared_ptr<ExpectationLib::IParty>>& GetParties()
		{
			return parties;
		}

		libmonad::Option<std::shared_ptr<ExpectationLib::IParty>> GetParty(const std::string& partyId)
		{
			if(!parties.contains(partyId)) 
			{
				return libmonad::None();
			}

			return parties[partyId];
		}
		

	private:
		std::unordered_map<std::string, std::shared_ptr<ExpectationLib::IParty>> parties;
		std::list<std::shared_ptr<ExpectationLib::ICircumstance>> history;
	};
}