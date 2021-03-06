project( depwalker LANGUAGES CXX )
cmake_minimum_required( VERSION 3.0 )

include( cmake/git_version.cmake )
git_version()
message( STATUS "\tVersion: ${GIT_VERSION_STRING}" )

set( Boost_USE_STATIC_LIBS TRUE )
find_package( Boost COMPONENTS filesystem program_options REQUIRED )
include_directories( ${Boost_INCLUDE_DIRS} )

file( GLOB wolker_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.[ch]pp )

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/depwalker.conf ${CMAKE_CURRENT_BINARY_DIR}/bin/depwalker.conf COPYONLY )

add_executable( ${PROJECT_NAME} ${wolker_sources} )
target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} )
if( WIN32 AND NOT MSVC )
    target_link_options( ${PROJECT_NAME} PUBLIC -static -static-libstdc++ -static-libgcc )
endif()
set_target_properties( ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin" )
target_compile_definitions( ${PROJECT_NAME} PUBLIC APP_VERSION="${GIT_VERSION_STRING}" )
