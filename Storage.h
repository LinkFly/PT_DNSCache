#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <chrono>
#include <cassert>
#include <exception>

// Близкий к среднему размер доменного имени
constexpr size_t MID_SIZE_DNS_NAME = 10;

class Storage
{
	class Item {
		std::string _ip, _name;
	public:
		Item() {
			_ip.reserve(size_t(3) * 4 + 3); // allocate for "xxx.xxx.xxx.xxx"
			_name.reserve(MID_SIZE_DNS_NAME);
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
	size_t max_size;
	size_t lastIdx = 0;

	time_t getTime();
public:
	struct ENotFoundIpByName : public std::exception {
		virtual const char* what() const throw() override {
			return "Not found ip by name";
		}
	};

	Storage(size_t max_size) :
		data{ max_size }, max_size{ max_size }, mapByName{ max_size }
	{}

	std::string get(const std::string& name);

	void add(const std::string& name, const std::string& ip);
};