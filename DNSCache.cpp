#include "DNSCache.h"
#include "Storage.h"
#include <memory>

// We can using one storage because DNSCache is singleton
static std::unique_ptr<Storage> pStorage;

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
	pStorage = std::make_unique<Storage>(max_size);
}

void DNSCache::update(const std::string& name, const std::string& ip)
{
	std::lock_guard<std::mutex> lock(mutex);
	pStorage->add(name, ip);
}

std::string DNSCache::resolve(const std::string& name)
{
	std::lock_guard<std::mutex> lock(mutex);
	return pStorage->get(name);
}
