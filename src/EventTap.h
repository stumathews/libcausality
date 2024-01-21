#pragma once
#include "events/EventSubscriber.h"
#include "CausalityTracker.h"

namespace libcausality
{
	class EventTap
	{
	public:
		static EventTap* Get();
		~EventTap();
		EventTap(const EventTap& other) = delete;
		EventTap(const EventTap&& other) = delete;
		EventTap& operator=(const EventTap& other) = delete;
		EventTap& operator=(const EventTap&& other) = delete;

		CausalityTracker CausalityTracker;
	protected:
		static EventTap* instance;
	private:
		EventTap();

		unsigned long elapsedTimeMs;

	public:
		void Tap(const std::shared_ptr<gamelib::Event>& evt, const std::string& subscriber, unsigned long deltaMs);
	};
}
