cmake_minimum_required (VERSION 3.8)

project("PT_DNSCache")

set(CMAKE_CPP_COMPILER g++)
set(TARGET_NAME PT_DNSCache)

set(SOURCES "PT_DNSCache.cpp" "DNSCache.cpp" "Storage.cpp")
set(HEADERS "DNSCache.h" "Storage.h")

add_executable (${TARGET_NAME} ${SOURCES} ${HEADERS})

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
