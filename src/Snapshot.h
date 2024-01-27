#pragma once

namespace libcausality
{
	template<typename T>
	class Snapshot
	{
	public:
		Snapshot(const T& item, const unsigned long deltaMs): item(item), deltaMs(deltaMs)
		{
			// Note I think we are copying the item, which makes sense if we are making a snapshot
		}

		Snapshot(){}

		[[nodiscard]] T Item() const { return item; }
		[[nodiscard]] unsigned long DeltaMs() const { return deltaMs; }
		Snapshot& operator=(const Snapshot &other) { return *this;}
	private:
		T item;
		unsigned long deltaMs;
	};
}