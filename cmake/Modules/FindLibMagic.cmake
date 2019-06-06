# Rudimentary module to find the libmagic library.
# 
# Defines: 
# 
# LIBMAGIC_FOUND - Libmagic library is installed
# LIBMAGIC_INCLUDE_DIRS - Paths for the header include files
# LIBMAGIC_LIBRARIES - Paths to link against the library
#

find_package(PkgConfig QUIET)

if (PKG_CONFIG_FOUND)
   pkg_check_modules(PC_LIBMAGIC QUIET libmagic)
endif()


find_path(LIBMAGIC_INCLUDE 
          NAMES magic.h 
          HINTS ${PC_LIBMAGIC_INCLUDE_DIRS} ${CMAKE_INSTALL_PREFIX}
	  PATH_SUFFIXES include
)

find_library(LIBMAGIC_LIBRARY 
             NAMES magic 
             HINTS ${PC_LIBZIP_LIBRARY_DIRS} ${CMAKE_INSTALL_PREFIX}
	     PATH_SUFFIXES lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibMagic DEFAULT_MSG
                                  LIBMAGIC_INCLUDE
				  LIBMAGIC_LIBRARY
)

mark_as_advanced(LIBMAGIC_INCLUDE LIBMAGIC_LIBRARY)
set(LIBMAGIC_INCLUDE_DIRS ${LIBMAGIC_INCLUDE})
set(LIBMAGIC_LIBRARIES ${LIBMAGIC_LIBRARY})

