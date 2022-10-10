// PT_DNSCache.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "DNSCache.h"

#include <string>
#include <vector>

int main()
{
    size_t sizeMax = 2;
    size_t namesCount = 3;
    auto pDNSCache = DNSCache::GetInstance(sizeMax);
    std::string name{ "ya.ru" };
    std::string ip{ "123.45.67.0" };

    // Helper generator
    auto fnGenIncStrs = [namesCount](const std::string& base, size_t start, size_t count) {
        std::vector<std::string> resStrs;
        for (size_t i = start; i < namesCount; ++i) {
            auto&& curIp = base + std::to_string(i);
            resStrs.push_back(curIp);
        }
        return resStrs;
    };

    // Prepare test data
    std::vector<std::string> names = fnGenIncStrs(name, 0, namesCount);
    std::vector<std::string> ips = fnGenIncStrs(ip, 0, namesCount);
    std::vector<std::string> waitIps = fnGenIncStrs(ip, namesCount - sizeMax, namesCount);
    std::vector<std::string> resIps;

    // Main functional
    for (size_t i = 0; i < names.size(); ++i) {
        pDNSCache->update(names[i], ips[i]);
    }
    for (size_t i = namesCount - sizeMax; i < namesCount; ++i) {
        auto&& name = pDNSCache->resolve(names[i]);
        // Prepare for checking
        resIps.push_back(name);
    }
    
    // Checking result
    size_t successCount = 0;
    for (size_t i = 0; i < waitIps.size(); ++i) {
        if (resIps[i] == waitIps[i])
            ++successCount;
    }
    bool bTestRes = successCount = waitIps.size();
    std::cout << (bTestRes ? "TEST PASS" : "TEST FAIL") << std::endl;
}

