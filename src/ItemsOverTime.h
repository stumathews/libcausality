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
		void Add(const T& item, std::string itemKey, unsigned long elapsedTimeMs)
		{
			auto snapShot = Snapshot(item, elapsedTimeMs);
			Add(snapShot, itemKey);
		}

		void Add(Snapshot<T> itemSnapshot, std::string itemKey)
		{
			auto elapsedTimeMs = itemSnapshot.DeltaMs();

			itemsByTime[elapsedTimeMs][itemKey].push_back(itemSnapshot);
			timesByKey[itemKey][elapsedTimeMs].push_back(itemSnapshot);

			if(!itemNumbers.contains(itemKey)) itemNumbers[itemKey] = lastItemNumber++;
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

		int Exchange(const std::string itemKey)
		{
			if(!itemNumbers.contains(itemKey)) itemNumbers[itemKey] = lastItemNumber++;
			return itemNumbers[itemKey];
		}

		libmonad::Option<std::string> Exchange(const unsigned int itemNumber)
		{
			for(auto& [candidateItemKey, candidateItemNumber] : itemNumbers)
			{
				if(candidateItemNumber == itemNumber) return candidateItemKey;
			}

			return libmonad::None();
		}

	private:

		std::map<unsigned long, std::map<std::string, std::list<Snapshot<T>>>> itemsByTime;
		std::map<std::string, std::map<unsigned long, std::list<Snapshot<T>>>> timesByKey;
		std::map<std::string, unsigned int> itemNumbers;
		static unsigned int lastItemNumber;
	};

	template <typename T>
	unsigned int ItemsOverTime<T>::lastItemNumber = 0;
}
