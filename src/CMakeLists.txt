set(TARGET leap-motion-x11)

include_directories(${X11_INCLUDE_DIRS})
include_directories(${LEAP_INCLUDE_DIRS})
link_directories(${LEAP_LIBRARY_DIRS})

set(SRCS main.cpp mylistener.cpp)
add_executable(${TARGET} ${SRCS})
target_link_libraries(${TARGET} ${X11_LIBRARIES} ${LEAP_LIBRARIES})

# install stage
install(TARGETS ${TARGET} DESTINATION bin)
