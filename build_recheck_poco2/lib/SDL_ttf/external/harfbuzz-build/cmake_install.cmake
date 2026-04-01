# Install script for directory: C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/bobsgame")
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

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/harfbuzz" TYPE FILE FILES
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-aat-layout.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-aat.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-blob.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-buffer.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-common.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-cplusplus.hh"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-deprecated.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-draw.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-face.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-font.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-map.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-ot-color.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-ot-deprecated.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-ot-font.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-ot-layout.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-ot-math.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-ot-meta.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-ot-metrics.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-ot-name.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-ot-shape.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-ot-var.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-ot.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-paint.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-set.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-shape-plan.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-shape.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-style.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-unicode.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-version.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-ft.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-gdi.h"
    "C:/Users/hyper/workspace/bg/okgame/lib/SDL_ttf/external/harfbuzz/src/hb-uniscribe.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/hyper/workspace/bg/okgame/build_recheck_poco2/lib/SDL_ttf/external/harfbuzz-build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
