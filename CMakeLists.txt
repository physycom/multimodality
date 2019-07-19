cmake_minimum_required (VERSION 3.1)
project(multimodality)

if (EXISTS "$ENV{WORKSPACE}/sysconfig/cmake/physycom_config.cmake")
  message(STATUS "Enabling physycom settings")
  include("$ENV{WORKSPACE}/sysconfig/cmake/physycom_config.cmake")
else()
  message(STATUS "Unable to find physycom settings file")
endif()
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX AND NOT APPLE)
  find_package(LibRt REQUIRED)
endif()
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(FLTK 1.3.3 REQUIRED)
find_package(OpenGL REQUIRED)
#find_package(Boost 1.65.0 COMPONENTS system REQUIRED)

if(MSVC)
  add_definitions(-DNOMINMAX)
endif()

include_directories(
  "${CMAKE_SOURCE_DIR}/src/"
  "${CMAKE_SOURCE_DIR}/src/utils"
  "${CMAKE_SOURCE_DIR}/src/libtracpp"
  "${CMAKE_SOURCE_DIR}/src/jsoncons/include"
  ${Boost_INCLUDE_DIRS}
  ${FLTK_INCLUDE_DIR}
)

add_executable(multimodality               "${CMAKE_SOURCE_DIR}/src/main_multimodal.cpp"
                                            "${CMAKE_SOURCE_DIR}/src/config.cpp"
                                            "${CMAKE_SOURCE_DIR}/src/poly.cpp"
                                            "${CMAKE_SOURCE_DIR}/src/node.cpp"
                                            "${CMAKE_SOURCE_DIR}/src/cartography.cpp"
                                            "${CMAKE_SOURCE_DIR}/src/public_transp.cpp"
                                            )
target_link_libraries(multimodality        ${FLTK_LIBRARIES} ${OPENGL_LIBRARIES} Threads::Threads)
install(TARGETS multimodality DESTINATION ${CMAKE_SOURCE_DIR})

if(UNIX AND NOT APPLE)
  target_link_libraries(multimodality       LIBRT::LIBRT )
endif()

if(UNIX)
  target_link_libraries(multimodality       ${CMAKE_DL_LIBS} )
endif()


