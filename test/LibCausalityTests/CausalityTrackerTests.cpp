#include "pch.h"
#include "CausalityTracker.h"
#include "events/EventNumbers.h"

using namespace gamelib;
using namespace ExpectationLib;

TEST(CausalityTracker, InstantiateWorks)
{
  libcausality::CausalityTracker tracker;
}

TEST(CausalityTracker, TrackSingleEvent)
{
	
	const EventId fetchedPickupEventId(FetchedPickup, "FetchedPickup");

	const auto party1 = std::make_shared<Party>("party1");
	const auto party2 = std::make_shared<Party>("party2");
	const auto party3 = std::make_shared<Party>("party3");

	libcausality::CausalityTracker tracker;

	const auto fromEvent = std::make_shared<Event>(fetchedPickupEventId);
	fromEvent->Origin = party1->GetId();

	tracker.TrackEvent(fromEvent, party2->GetId(), 0);

	EXPECT_FALSE(tracker.GetParties().empty());
	EXPECT_EQ(tracker.GetParties().size(), 2);
}
TEST(CausalityTracker, TrackMultipleEvent)
{
	
	const EventId fetchedPickupEventId(FetchedPickup, "FetchedPickup");

	const auto party1 = std::make_shared<Party>("party1");
	const auto party2 = std::make_shared<Party>("party2");
	const auto party3 = std::make_shared<Party>("party3");

	libcausality::CausalityTracker tracker;

	const auto fromEvent = std::make_shared<Event>(fetchedPickupEventId);
	fromEvent->Origin = party1->GetId();

	tracker.TrackEvent(fromEvent, party2->GetId(),0 );
	tracker.TrackEvent(fromEvent, party3->GetId(), 0);

	EXPECT_FALSE(tracker.GetParties().empty());
	EXPECT_EQ(tracker.GetParties().size(), 3);
	
}

TEST(CausalityTracker, DuplicatePartiesIgnored)
{
	
	const EventId fetchedPickupEventId(FetchedPickup, "FetchedPickup");

	const auto party1 = std::make_shared<Party>("party1");
	const auto party2 = std::make_shared<Party>("party2");
	const auto party3 = std::make_shared<Party>("party3");

	libcausality::CausalityTracker tracker;

	auto fromEvent = std::make_shared<Event>(fetchedPickupEventId);
	fromEvent->Origin = party1->GetId();

	tracker.TrackEvent(fromEvent, party2->GetId(), 0);
	tracker.TrackEvent(fromEvent, party3->GetId(), 0);
	tracker.TrackEvent(fromEvent, party3->GetId(), 0);

	EXPECT_FALSE(tracker.GetParties().empty());
	EXPECT_EQ(tracker.GetParties().size(), 3); // party 1, 2, 3

	auto result1 = tracker.GetParty(party1->GetId());
	auto result2 = tracker.GetParty(party2->GetId());
	auto result3 = tracker.GetParty(party3->GetId());
	auto result4 = tracker.GetParty("unknownParty"); // should not be able to find untracked party

	EXPECT_TRUE(result1.IsSome());
	EXPECT_TRUE(result2.IsSome());
	EXPECT_TRUE(result3.IsSome());
	EXPECT_TRUE(result4.IsNone()); // see, not found.

}