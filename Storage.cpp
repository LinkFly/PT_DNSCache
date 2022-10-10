#include "Storage.h"

inline time_t Storage::getTime() {
	return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

/* Constant on average, worst case linear in the size of the container. */
std::string Storage::get(const std::string& name) {
	auto it = mapByName.find(name);
	if (it == end(mapByName)) {
		throw new ENotFoundIpByName();
	}
	return data[it->second].getIp();
}

void Storage::add(const std::string& name, const std::string& ip) {
	time_t time = getTime();
	size_t newIdx;
	bool bFilled = mapByName.size() == max_size;

	auto it = mapByName.find(name);
	bool bFound = it != mapByName.end();
	// name exists
	if (bFound) {
		newIdx = it->second;
	}
	else {
		// name don't exists
		if (bFilled) {
			auto itOld = begin(mapByTime);
			newIdx = itOld->second;
			mapByTime.erase(itOld);
			auto&& oldName = data[newIdx].getName();
			mapByName.erase(oldName);
		}
		else {
			assert(lastIdx < data.size());
			newIdx = lastIdx;
			++lastIdx;
		}
	}
	(bFound ? it->second : mapByName[name]) = newIdx;
	mapByTime.insert({ time, newIdx });
	data[newIdx].setIp(ip);
	data[newIdx].setName(name);
}
