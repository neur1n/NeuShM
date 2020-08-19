CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(NeuShM)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE Release)

INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/include 
)

IF(MSVC)
  ADD_EXECUTABLE(producer example/producer.cpp)
  SET_TARGET_PROPERTIES(producer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/demo/win
    LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""
  )

  ADD_EXECUTABLE(consumer example/consumer.cpp)
  SET_TARGET_PROPERTIES(consumer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/demo/win
    LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""
  )
ENDIF(MSVC)

IF(UNIX)
  ADD_EXECUTABLE(producer src/producer.cpp)
  TARGET_LINK_LIBRARIES(producer rt)

  ADD_EXECUTABLE(consumer src/consumer.cpp)
  TARGET_LINK_LIBRARIES(consumer rt)
ENDIF(UNIX)
