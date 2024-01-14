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

	auto fromEvent = std::make_shared<Event>(fetchedPickupEventId);
	fromEvent->Origin = party1->GetId();

	tracker.TrackEvent(fromEvent, party2->GetId());

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

	auto fromEvent = std::make_shared<Event>(fetchedPickupEventId);
	fromEvent->Origin = party1->GetId();

	tracker.TrackEvent(fromEvent, party2->GetId());
	tracker.TrackEvent(fromEvent, party3->GetId());

	EXPECT_FALSE(tracker.GetParties().empty());
	EXPECT_EQ(tracker.GetParties().size(), 3);
	
}

TEST(CausalityTracker, DuplicatePartiesIgnoed)
{
	
	const EventId fetchedPickupEventId(FetchedPickup, "FetchedPickup");

	const auto party1 = std::make_shared<Party>("party1");
	const auto party2 = std::make_shared<Party>("party2");
	const auto party3 = std::make_shared<Party>("party3");

	libcausality::CausalityTracker tracker;

	auto fromEvent = std::make_shared<Event>(fetchedPickupEventId);
	fromEvent->Origin = party1->GetId();

	tracker.TrackEvent(fromEvent, party2->GetId());
	tracker.TrackEvent(fromEvent, party3->GetId());
	tracker.TrackEvent(fromEvent, party3->GetId());

	EXPECT_FALSE(tracker.GetParties().empty());
	EXPECT_EQ(tracker.GetParties().size(), 3);

	auto result1 = tracker.GetParty(party1->GetId());
	auto result2 = tracker.GetParty(party2->GetId());
	auto result3 = tracker.GetParty(party3->GetId());
	auto result4 = tracker.GetParty("unknownParty");

	EXPECT_TRUE(result1.IsSome());
	EXPECT_TRUE(result2.IsSome());
	EXPECT_TRUE(result3.IsSome());
	EXPECT_TRUE(result4.IsNone());

}

TEST(CausalityTracker, PartiesTrackedCorrectly)
{
	
	const EventId fetchedPickupEventId(FetchedPickup, "FetchedPickup");

	const auto party1 = std::make_shared<Party>("party1");
	const auto party2 = std::make_shared<Party>("party2");
	const auto party3 = std::make_shared<Party>("party3");

	// Causality tracker
	libcausality::CausalityTracker tracker;

	// Type of event to tracker
	auto fromEvent = std::make_shared<Event>(fetchedPickupEventId);

	// set who sent the event
	fromEvent->Origin = party1->GetId();

	// Track the event and who it was sent to
	tracker.TrackEvent(fromEvent, party2->GetId());
	tracker.TrackEvent(fromEvent, party3->GetId());

	EXPECT_FALSE(tracker.GetParties().empty());
	EXPECT_EQ(tracker.GetParties().size(), 3);

	auto result1 = tracker.GetParty(party1->GetId());
	auto result2 = tracker.GetParty(party2->GetId());
	auto result3 = tracker.GetParty(party3->GetId());
	auto result4 = tracker.GetParty("unknownParty");

	EXPECT_TRUE(result1.IsSome());
	EXPECT_TRUE(result2.IsSome());
	EXPECT_TRUE(result3.IsSome());
	EXPECT_TRUE(result4.IsNone());

}