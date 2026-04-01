#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ZLIB::external_zlib" for configuration "Debug"
set_property(TARGET ZLIB::external_zlib APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(ZLIB::external_zlib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/zsd.lib"
  )

list(APPEND _cmake_import_check_targets ZLIB::external_zlib )
list(APPEND _cmake_import_check_files_for_ZLIB::external_zlib "${_IMPORT_PREFIX}/lib/zsd.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
