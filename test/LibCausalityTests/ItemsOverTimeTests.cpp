#include "pch.h"

#include "ItemsOverTime.h"

using namespace libcausality;
using namespace libmonad;

TEST(ItemsOverTime, GetLatestItemByKey)
{
	ItemsOverTime<char> charactersOverTime;

	charactersOverTime.Add(1, 'a', ToOption<std::string>("a"));
	charactersOverTime.Add(2, 'b', ToOption<std::string>("b"));
	charactersOverTime.Add(3, 'c', ToOption<std::string>("c"));	
	charactersOverTime.Add(5, 'a', ToOption<std::string>("a"));

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
	charactersOverTime.Add(1, 'a', ToOption<std::string>("a"));
	charactersOverTime.Add(2, 'b', ToOption<std::string>("b"));
	charactersOverTime.Add(3, 'c', ToOption<std::string>("c"));	
	charactersOverTime.Add(5, 'a', ToOption<std::string>("a"));
	charactersOverTime.Add(5, 'b', ToOption<std::string>("b"));
	charactersOverTime.Add(5, 'c', ToOption<std::string>("c"));
	charactersOverTime.Add(6, 'c', ToOption<std::string>("c"));
	charactersOverTime.Add(1, '1', ToOption<std::string>("1"));

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