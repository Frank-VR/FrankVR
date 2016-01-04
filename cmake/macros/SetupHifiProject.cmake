#
#  SetupHifiProject.cmake
#
#  Copyright 2013 High Fidelity, Inc.
#
#  Distributed under the Apache License, Version 2.0.
#  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
#

macro(SETUP_HIFI_PROJECT)
  project(${TARGET_NAME})

  # grab the implemenation and header files
  file(GLOB TARGET_SRCS src/*)

  file(GLOB SRC_SUBDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

  # inlcude the generated application version header
  include_directories("${CMAKE_BINARY_DIR}/includes")

  foreach(DIR ${SRC_SUBDIRS})
    if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/${DIR}")
      file(GLOB DIR_CONTENTS "src/${DIR}/*")
      set(TARGET_SRCS ${TARGET_SRCS} "${DIR_CONTENTS}")
    endif ()
  endforeach()

  if (DEFINED BUILD_BUNDLE AND BUILD_BUNDLE AND APPLE)
    add_executable(${TARGET_NAME} MACOSX_BUNDLE ${TARGET_SRCS} ${AUTOMTC_SRC} ${AUTOSCRIBE_SHADER_LIB_SRC})
  else ()
    add_executable(${TARGET_NAME} ${TARGET_SRCS} ${AUTOMTC_SRC} ${AUTOSCRIBE_SHADER_LIB_SRC})
  endif()

  set(${TARGET_NAME}_DEPENDENCY_QT_MODULES ${ARGN})
  list(APPEND ${TARGET_NAME}_DEPENDENCY_QT_MODULES Core)

  # find these Qt modules and link them to our own target
  find_package(Qt5 COMPONENTS ${${TARGET_NAME}_DEPENDENCY_QT_MODULES} REQUIRED)

  # disable /OPT:REF and /OPT:ICF for the Debug builds
  # This will prevent the following linker warnings
  # LINK : warning LNK4075: ignoring '/INCREMENTAL' due to '/OPT:ICF' specification
  if (WIN32)
     set_property(TARGET ${TARGET_NAME} APPEND_STRING PROPERTY LINK_FLAGS_DEBUG "/OPT:NOREF /OPT:NOICF")
  endif()

  foreach(QT_MODULE ${${TARGET_NAME}_DEPENDENCY_QT_MODULES})
    target_link_libraries(${TARGET_NAME} Qt5::${QT_MODULE})
  endforeach()

  target_glm()

endmacro()
