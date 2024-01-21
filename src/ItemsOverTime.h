#pragma once
#include <list>
#include <map>
#include <string>

#include "IParty.h"
#include "Snapshot.h"

template <typename T>
class ItemsOverTime 
{
public:
	void Add(unsigned long elapsedTimeMs, const T& item, libmonad::Option<std::string> itemKey)
	{
		auto snapshot = libcausality::Snapshot(item, elapsedTimeMs);		
		auto key = itemKey.WhenNone([]{ return "NoItemKey";});

		itemsByTime[elapsedTimeMs][key].push_back(snapshot);
		timesByKey[key][elapsedTimeMs].push_back(snapshot);
	}

	std::map<unsigned long, std::list<libcausality::Snapshot<T>>>& GetItemsOverTime() { return itemsByTime;}

private:

	std::map<unsigned long, std::map<std::string, std::list<libcausality::Snapshot<T>>>> itemsByTime;
	std::map<std::string, std::map<unsigned long, std::list<libcausality::Snapshot<T>>>> timesByKey;
};
