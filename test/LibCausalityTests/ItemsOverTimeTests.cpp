#include "pch.h"

#include "ItemsOverTime.h"

using namespace libcausality;
using namespace libmonad;

TEST(ItemsOverTime, GetLatestItemByKey)
{
	ItemsOverTime<char> charactersOverTime;

	charactersOverTime.Add( 'a', "a", 1);
	charactersOverTime.Add( 'b', "b", 2);
	charactersOverTime.Add('c', "c", 3);	
	charactersOverTime.Add('a', "a", 5);

	charactersOverTime.GetLatestItemByKey("a").Match([](None){ FAIL(); }, [](const Snapshot<char> some)
	{
			if(some.Item() == 'a' && some.DeltaMs() == 5)
				SUCCEED();
			else
				FAIL();
	});
	charactersOverTime.GetLatestItemByKey("d").Match([](None){ SUCCEED(); }, [](const Snapshot<char> some)
	{
		FAIL();
	});
}

TEST(ItemsOverTime, GetAllLatestItems)
{
	ItemsOverTime<char> charactersOverTime;
	charactersOverTime.Add( 'a', "a",1);
	charactersOverTime.Add( 'b', "b", 2);
	charactersOverTime.Add( 'c', "c", 3);	
	charactersOverTime.Add( 'a', "a", 5);
	charactersOverTime.Add( 'b', "b", 5);
	charactersOverTime.Add( 'c', "c", 5);
	charactersOverTime.Add( 'c', "c", 6);
	charactersOverTime.Add( '1', "1", 1);

	auto items = charactersOverTime.GetAllLatestItems();
	EXPECT_EQ(items.size(), 4);

	const auto a = std::ranges::find_if(items, [](const Snapshot<char> item){ return item.Item() == 'a';});
	const auto b = std::ranges::find_if(items, [](const Snapshot<char> item){ return item.Item() == 'b';});
	const auto c = std::ranges::find_if(items, [](const Snapshot<char> item){ return item.Item() == 'c';});
	const auto one = std::ranges::find_if(items, [](const Snapshot<char> item){ return item.Item() == '1';});

	EXPECT_TRUE(a != items.end());
	EXPECT_TRUE(b != items.end());
	EXPECT_TRUE(c != items.end());
	EXPECT_TRUE(one != items.end());

	EXPECT_EQ((*a).DeltaMs(), 5);
	EXPECT_EQ((*b).DeltaMs(), 5);
	EXPECT_EQ((*c).DeltaMs(), 6);
	EXPECT_EQ((*one).DeltaMs(), 1);		
}

TEST(ItemsOverTime, Purge)
{
	ItemsOverTime<char> charactersOverTime;
	charactersOverTime.Add( 'a', "a",1);
	charactersOverTime.Add( 'b', "b", 2);
	charactersOverTime.Add( 'c', "c", 3);	
	charactersOverTime.Add( 'a', "a", 5);
	charactersOverTime.Add( 'b', "b", 5);
	charactersOverTime.Add( 'c', "c", 5);
	charactersOverTime.Add( 'c', "c", 6);
	charactersOverTime.Add( '1', "1", 1);
	
	EXPECT_EQ(charactersOverTime.GetAllLatestItems().size(), 4);
	charactersOverTime.Purge();
	EXPECT_EQ(charactersOverTime.GetAllLatestItems().size(), 0);

}