cmake_minimum_required(VERSION 3.1)

project(regex_verification)

set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
set(SOURCE pcreshow.c)
set(LIBS ${CURSES_LIBRARY} ${PCRE_LIBRARY})

find_library(CURSES_LIBRARY ncursesw)
find_library(PCRE_LIBRARY pcre2-8)

option(without_UTF "Disable UTF support" OFF)

if(without_UTF)
  add_definitions(-DPCRE2_UCP_FLAG=0)
else()
  add_definitions(-DPCRE2_UCP_FLAG=1)
endif()

add_executable(regex_verification ${SOURCE})

target_link_libraries(regex_verification ${LIBS})