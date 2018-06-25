SET( pillowtalk_SRCS pillowtalk_impl.c pillowtalk_changes_feed.c)
SET (pillowtalk_HDRS pillowtalk.h pillowtalk_impl.h)

ADD_LIBRARY( pillowtalk SHARED ${pillowtalk_SRCS} ${pillowtalk_HDRS} )

#### setup shared library version number
SET_TARGET_PROPERTIES(pillowtalk PROPERTIES
                      SOVERSION ${PILLOWTALK_MAJOR}
                      VERSION ${PILLOWTALK_MAJOR}.${PILLOWTALK_MINOR}.${PILLOWTALK_MICRO})

IF(PTHREAD_FOUND)
  TARGET_LINK_LIBRARIES(pillowtalk ${YAJL_LIBRARY} ${CURL_LIBRARY} ${PTHREAD_LIBRARY})  
ELSE(PTHREAD_FOUND)
  TARGET_LINK_LIBRARIES(pillowtalk ${YAJL_LIBRARY} ${CURL_LIBRARY})  
ENDIF(PTHREAD_FOUND)

# Output Paths
SET (output_include ${CMAKE_CURRENT_BINARY_DIR}/../include)
FILE(MAKE_DIRECTORY ${output_include})

# Copy Public .h's into the output directory so the tests can get it
#SET (pillowtalk_PUB_HDRS pillowtalk.h)
#FOREACH (header ${pillowtalk_PUB_HDRS})
#  SET (header ${CMAKE_CURRENT_SOURCE_DIR}/${header})

#  EXEC_PROGRAM(${CMAKE_COMMAND} ARGS -E copy_if_different ${header} ${output_include})

#ENDFOREACH (header ${pillowtalk_PUB_HDRS})

# Installation Step
IF(WIN32)
  INSTALL( TARGETS pillowtalk LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
ELSE(WIN32)
  INSTALL( TARGETS pillowtalk LIBRARY DESTINATION lib)
ENDIF(WIN32)
INSTALL( FILES pillowtalk.h DESTINATION include)

