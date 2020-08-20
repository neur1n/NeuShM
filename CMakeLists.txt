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
  ADD_EXECUTABLE(producer.elf example/producer.cpp)
  TARGET_LINK_LIBRARIES(producer.elf rt)
  SET_TARGET_PROPERTIES(producer.elf PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/demo/linux
  )

  ADD_EXECUTABLE(consumer.elf example/consumer.cpp)
  TARGET_LINK_LIBRARIES(consumer.elf rt)
  SET_TARGET_PROPERTIES(consumer.elf PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/demo/linux
  )
ENDIF(UNIX)
