# CMake generated Testfile for 
# Source directory: C:/Users/hyper/workspace/bg/okgame/build_recheck_poco4/lib/SDL_image/external/zlib-build/test/add_subdirectory_test
# Build directory: C:/Users/hyper/workspace/bg/okgame/build_recheck_poco4/lib/SDL_image/external/zlib-build/test/add_subdirectory_test/build_output
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[zlib_test_example_static]=] "C:/Users/hyper/workspace/bg/okgame/build_recheck_poco4/lib/SDL_image/external/zlib-build/test/add_subdirectory_test/build_output/Debug/test_example_static.exe")
  set_tests_properties([=[zlib_test_example_static]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/hyper/workspace/bg/okgame/build_recheck_poco4/lib/SDL_image/external/zlib-build/test/add_subdirectory_test/CMakeLists.txt;27;add_test;C:/Users/hyper/workspace/bg/okgame/build_recheck_poco4/lib/SDL_image/external/zlib-build/test/add_subdirectory_test/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[zlib_test_example_static]=] "C:/Users/hyper/workspace/bg/okgame/build_recheck_poco4/lib/SDL_image/external/zlib-build/test/add_subdirectory_test/build_output/Release/test_example_static.exe")
  set_tests_properties([=[zlib_test_example_static]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/hyper/workspace/bg/okgame/build_recheck_poco4/lib/SDL_image/external/zlib-build/test/add_subdirectory_test/CMakeLists.txt;27;add_test;C:/Users/hyper/workspace/bg/okgame/build_recheck_poco4/lib/SDL_image/external/zlib-build/test/add_subdirectory_test/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test([=[zlib_test_example_static]=] "C:/Users/hyper/workspace/bg/okgame/build_recheck_poco4/lib/SDL_image/external/zlib-build/test/add_subdirectory_test/build_output/MinSizeRel/test_example_static.exe")
  set_tests_properties([=[zlib_test_example_static]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/hyper/workspace/bg/okgame/build_recheck_poco4/lib/SDL_image/external/zlib-build/test/add_subdirectory_test/CMakeLists.txt;27;add_test;C:/Users/hyper/workspace/bg/okgame/build_recheck_poco4/lib/SDL_image/external/zlib-build/test/add_subdirectory_test/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[zlib_test_example_static]=] "C:/Users/hyper/workspace/bg/okgame/build_recheck_poco4/lib/SDL_image/external/zlib-build/test/add_subdirectory_test/build_output/RelWithDebInfo/test_example_static.exe")
  set_tests_properties([=[zlib_test_example_static]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/hyper/workspace/bg/okgame/build_recheck_poco4/lib/SDL_image/external/zlib-build/test/add_subdirectory_test/CMakeLists.txt;27;add_test;C:/Users/hyper/workspace/bg/okgame/build_recheck_poco4/lib/SDL_image/external/zlib-build/test/add_subdirectory_test/CMakeLists.txt;0;")
else()
  add_test([=[zlib_test_example_static]=] NOT_AVAILABLE)
endif()
subdirs("zlib")
