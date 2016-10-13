# - Try to find LibXml2
# Once done this will define
#  LIBXML2_FOUND - System has LibXml2
#  LIBXML2_INCLUDE_DIRS - The LibXml2 include directories
#  LIBXML2_LIBRARIES - The libraries needed to use LibXml2
#  LIBXML2_DEFINITIONS - Compiler switches required for using LibXml2

find_package(PkgConfig)
pkg_check_modules(PC_KYTEA QUIET libkytea)
set(KYTEA_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})

find_path(KYTEA_INCLUDE_DIR kytea/kytea.h
        HINTS ${PC_KYTEA_INCLUDEDIR} ${PC_KYTEA_INCLUDE_DIRS})

find_library(KYTEA_LIBRARY NAMES kytea
        HINTS ${PC_KYTEA_LIBDIR} ${PC_KYTEA_LIBRARY_DIRS} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Kytea DEFAULT_MSG
        KYTEA_LIBRARY KYTEA_INCLUDE_DIR)

mark_as_advanced(KYTEA_INCLUDE_DIR KYTEA_LIBRARY )

set(KYTEA_LIBRARIES ${KYTEA_LIBRARY} )
set(KYTEA_INCLUDE_DIRS ${KYTEA_INCLUDE_DIR} )