cmake_minimum_required (VERSION 3.8)

project("PT_DNSCache")

set(CMAKE_CPP_COMPILER g++)
set(TARGET_NAME PT_DNSCache)

add_executable (${TARGET_NAME} "PT_DNSCache.cpp" "DNSCache.cpp" "DNSCache.h")

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
