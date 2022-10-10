#pragma once

#include <string>
#include <mutex>
#include <memory>

class DNSCache
{
    static std::shared_ptr<DNSCache> pInstance;
    static std::mutex mutex;
protected:

    DNSCache(size_t max_size);
    ~DNSCache() {}
public:

    //friend std::make_shared<DNSCache, size_t&>;
    static std::shared_ptr<DNSCache> GetInstance(size_t max_size);

    DNSCache(DNSCache& other) = delete;
    void operator=(const DNSCache&) = delete;

	void update(const std::string& name, const std::string& ip);
	std::string resolve(const std::string& name);
};