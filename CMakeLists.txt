cmake_minimum_required(VERSION 2.8.9)
project (test)

#find_package(CURL REQUIRED)
add_executable(keyval src/kv/FastKeyValue.cpp)
add_executable(experiment src/experiment/Experiment4.cpp)

#include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(keyval ${CURL_LIBRARIES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(keyval Threads::Threads)

#add_executable(serialize src/data/DatastructureSerialize.cpp)
#target_link_libraries(serialize Threads::Threads)
