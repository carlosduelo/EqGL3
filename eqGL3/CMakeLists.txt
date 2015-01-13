
# Copyright (c) 2015 Carlos Duelo <cduelo@cesvima.upm.es>


set(HEADERS
   )

set(SOURCES
   EqGL3.cpp)

list(APPEND EQGL3_LINK_LIBRARIES ${EQUALIZER_LIBRARIES})

add_executable(EqGL3 SHARED ${HEADERS} ${SOURCES})
target_link_libraries(EqGL3 ${EQGL3_LINK_LIBRARIES})
