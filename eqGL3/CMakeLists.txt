
# Copyright (c) 2015 Carlos Duelo <cduelo@cesvima.upm.es>


set(HEADERS
   )

set(SOURCES
   EqGL3.cpp)

list(APPEND EQGL3_LINK_LIBRARIES ${EQUALIZER_LIBRARIES} ${GLEW_LIBRARY} GL)

add_executable(EqGL3 ${HEADERS} ${SOURCES})
target_link_libraries(EqGL3 ${EQGL3_LINK_LIBRARIES})
