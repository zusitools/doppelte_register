cmake_minimum_required (VERSION 3.1)
project (doppelte_register)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(parser)

find_package(Boost COMPONENTS filesystem program_options REQUIRED)

add_executable(doppelte_register doppelte_register.cpp nowide/src/iostream.cpp)
# TODO: target_link_libraries(Boost::filesystem) funktioniert derzeit unter MXE nicht
target_include_directories(doppelte_register PRIVATE ${Boost_INCLUDE_DIRS} nowide/include)
target_link_libraries(doppelte_register PRIVATE zusi_parser ${Boost_LIBRARIES})

install(TARGETS doppelte_register RUNTIME DESTINATION bin)
