#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ZLIB::external_zlib" for configuration "Release"
set_property(TARGET ZLIB::external_zlib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ZLIB::external_zlib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/zs.lib"
  )

list(APPEND _cmake_import_check_targets ZLIB::external_zlib )
list(APPEND _cmake_import_check_files_for_ZLIB::external_zlib "${_IMPORT_PREFIX}/lib/zs.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
