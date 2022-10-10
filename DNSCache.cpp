#include "DNSCache.h"

#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <chrono>
#include <cassert>

class Storage
{
	class Item {
		std::string _ip, _name;
	public:
		Item() {
			_ip.reserve(size_t(3) * 4 + 3); // allocate for "xxx.xxx.xxx.xxx"
		}
		void setIp(const std::string& ip) {
			_ip = ip;
		}
		std::string getIp() { return _ip; }

		void setName(const std::string& name) {
			_name = name;
		}
		std::string getName() { return _name; }
	};

	std::map<time_t, size_t> mapByTime;
	std::unordered_map<std::string, size_t> mapByName;
	std::vector<Item> data;
	size_t max_size = 0;
	size_t lastIdx = 0;

	time_t getTime() {
		return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	}

public:

	void init(size_t maxSize) {
		max_size = maxSize;
		data.resize(maxSize);
	}

	std::string get(const std::string& name) {
		auto it = mapByName.find(name);
		if (it == end(mapByName)) {
			throw new std::exception{ "Not found ip by name" };
		}
		return data[it->second].getIp();
	}
	
	void add(const std::string& name, const std::string& ip) {
		time_t time = getTime();
		size_t newIdx;
		bool bFilled = mapByName.size() == max_size;

		auto it = mapByName.find(name);
		// name exists
		if (it != mapByName.end()) {
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
		mapByName[name] = newIdx;
		//mapByTime.insert()
		mapByTime.insert({ time, newIdx });
		data[newIdx].setIp(ip);	
		data[newIdx].setName(name);
	}
};

// We can using one storage because DNSCache is singleton
static Storage storage;

std::shared_ptr<DNSCache> DNSCache::pInstance = nullptr;
std::mutex DNSCache::mutex;

std::shared_ptr<DNSCache> DNSCache::GetInstance(size_t max_size) {
	std::lock_guard<std::mutex> lock(mutex);
	if (!pInstance)
	{
		// For access to protected constructor/destructor for make_shared
		struct make_shared_enabler : public DNSCache {
			make_shared_enabler(size_t max_size): DNSCache{max_size} {}
		};
		pInstance = std::make_shared<make_shared_enabler>(max_size);
	}
	return pInstance;
}

DNSCache::DNSCache(size_t max_size)
{
	storage.init(max_size);
}

void DNSCache::update(const std::string& name, const std::string& ip)
{
	storage.add(name, ip);
}

std::string DNSCache::resolve(const std::string& name)
{
	return storage.get(name);
}
