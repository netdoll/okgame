# Install script for directory: C:/Users/hyper/workspace/bg/okgame/lib/SDL_mixer/external/flac

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/bobsgame")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/hyper/workspace/bg/okgame/build_recheck/lib/SDL_mixer/external/flac-build/src/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/hyper/workspace/bg/okgame/build_recheck/lib/SDL_mixer/external/flac-build/doc/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/FLAC" TYPE FILE FILES
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_mixer/external/flac/include/FLAC/all.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_mixer/external/flac/include/FLAC/assert.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_mixer/external/flac/include/FLAC/callback.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_mixer/external/flac/include/FLAC/export.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_mixer/external/flac/include/FLAC/format.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_mixer/external/flac/include/FLAC/metadata.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_mixer/external/flac/include/FLAC/ordinals.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_mixer/external/flac/include/FLAC/stream_decoder.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_mixer/external/flac/include/FLAC/stream_encoder.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/FLAC++" TYPE FILE FILES
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_mixer/external/flac/include/FLAC++/all.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_mixer/external/flac/include/FLAC++/decoder.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_mixer/external/flac/include/FLAC++/encoder.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_mixer/external/flac/include/FLAC++/export.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_mixer/external/flac/include/FLAC++/metadata.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/hyper/workspace/bg/okgame/build_recheck/lib/SDL_mixer/external/flac-build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
