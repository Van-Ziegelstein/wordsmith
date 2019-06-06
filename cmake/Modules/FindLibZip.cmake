# Rudimentary module to find the libzip library.
# 
# Defines: 
# 
# LIBZIP_FOUND - Libzip library is installed
# LIBZIP_INCLUDE_DIRS - Paths for the header include files
# LIBZIP_LIBRARIES - Paths to link against the library
#

foreach(pkg ZLIB BZip2 OpenSSL GnuTLS)
  find_package(${pkg} REQUIRED)
endforeach()

find_package(PkgConfig QUIET)

if (PKG_CONFIG_FOUND)
   pkg_check_modules(PC_LIBZIP QUIET libzip)
endif()

find_path(LIBZIP_INCLUDE_ZIP 
          NAMES zip.h 
          HINTS ${PC_LIBZIP_INCLUDE_DIRS} ${CMAKE_INSTALL_PREFIX}
          PATH_SUFFIXES include
)

find_path(LIBZIP_INCLUDE_ZIPCONF 
          NAMES zipconf.h 
          HINTS ${PC_LIBZIP_INCLUDE_DIRS} ${CMAKE_INSTALL_PREFIX}
          PATH_SUFFIXES include
)

find_library(LIBZIP_LIBRARY 
             NAMES zip 
             HINTS ${PC_LIBZIP_LIBRARY_DIRS} ${CMAKE_INSTALL_PREFIX}
             PATH_SUFFIXES lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibZip DEFAULT_MSG
                                  LIBZIP_INCLUDE_ZIP
                                  LIBZIP_INCLUDE_ZIPCONF
                                  LIBZIP_LIBRARY
)

mark_as_advanced(LIBZIP_INCLUDE_ZIP LIBZIP_INCLUDE_ZIPCONF LIBZIP_LIBRARY)
set(LIBZIP_INCLUDE_DIRS ${LIBZIP_INCLUDE_ZIP} ${LIBZIP_INCLUDE_ZIPCONF})
set(LIBZIP_LIBRARIES ${LIBZIP_LIBRARY})

