#pragma once
#include <list>
#include <map>
#include <string>
#include "IParty.h"
#include "Snapshot.h"
#include <algorithm>

namespace libcausality
{
	template <typename T>
	class ItemsOverTime 
	{
	public:
		void Add(const T& item, libmonad::Option<std::string> itemKey, unsigned long elapsedTimeMs)
		{
			auto snapShot = Snapshot(item, elapsedTimeMs);
			Add(snapShot, itemKey);
		}

		void Add(Snapshot<T> itemSnapshot, libmonad::Option<std::string> itemKey)
		{
			auto elapsedTimeMs = itemSnapshot.DeltaMs();
			auto key = itemKey.WhenNone([]{ return "NoItemKey";});

			itemsByTime[elapsedTimeMs][key].push_back(itemSnapshot);
			timesByKey[key][elapsedTimeMs].push_back(itemSnapshot);
		}

		std::map<unsigned long, std::list<Snapshot<T>>>& GetItemsOverTime() { return itemsByTime;}

		// // map is in ascending order, so largest time is the last item
		Snapshot<T>& GetLatestItem()
		{
			return itemsByTime.rbegin();
		}

		std::list<Snapshot<T>> GetAllLatestItems()
		{
			std::vector<std::string> keys;

			// Gather all Keys
			for(auto const& item: timesByKey) 
			{
				keys.push_back(item.first);
			}

			std::list<Snapshot<T>> latestItems;

			// Key latest item for key
			for( const auto& key : keys)
			{
				GetLatestItemByKey(key).Match(
					[](libmonad::None none){}, 
					[&](Snapshot<T> found){ latestItems.push_back(found);});
			}
			return latestItems;
		}

		libmonad::Option<Snapshot<T>> GetLatestItemByKey(const std::string& itemKey)
		{
			auto itemExists = timesByKey.contains(itemKey);
			if(!itemExists) return libmonad::None();
			
			auto& latestTime = timesByKey[itemKey].rbegin()->first;
			return *itemsByTime[latestTime][itemKey].rbegin();
		}

		void Purge()
		{
			itemsByTime.clear();
			timesByKey.clear();
		}

	private:

		std::map<unsigned long, std::map<std::string, std::list<Snapshot<T>>>> itemsByTime;
		std::map<std::string, std::map<unsigned long, std::list<Snapshot<T>>>> timesByKey;
	};
}
