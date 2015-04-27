cmake_minimum_required(VERSION 2.8)
MACRO(LISTLIBNAMES result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(listnames "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
	ELSE()
        LIST(APPEND listnames ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${listnames})
ENDMACRO()

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 NEW)
endif()
if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif()
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 NEW)
endif()
# ======================================================================
# ======================================================================
PROJECT(ImageValidationISLES)
# ======================================================================
# ======================================================================
if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11)
endif()

GET_FILENAME_COMPONENT(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)

IF(APPLE)

ELSEIF(WIN32)
	# this optional you have to provide to the program where are your dll lib of ITK
  	LINK_DIRECTORIES(C:/Developer/ITKs/InsightToolkit-4.7.1/build/bin/Release/)
	LINK_DIRECTORIES(C:/Developer/ITKs/InsightToolkit-4.7.1/build/lib/Release/)
ELSEIF(UNIX AND NOT APPLE)
	SET(CMAKE_INSTALL_PREFIX ${PARENT_DIR}/install/linux)
ENDIF(APPLE)


FIND_PACKAGE(OpenGL)

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE validateLabelImages-ISLES.cxx)
target_link_libraries(${PROJECT_NAME} ${ITK_LIBRARIES})

# > --------------------------------------------------------------------------''
SET(APPS \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME})

SET(APP_DESTINY bin)

IF(APPLE)
	SET(APPS "${APPS}.app")
	SET(APP_DESTINY bin/${PROJECT_NAME}.app/Contents/MacOS)

	SET(ITK_LIBRARY_DIRS ${ITK_INSTALL_PREFIX}/lib)
    SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH})
    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
ELSEIF(WIN32)
  	SET(APPS "${APPS}.exe")
	SET(ITK_LIBRARY_DIRS C:/Developer/ITKs/InsightToolkit-4.7.1/build/bin/Release)
ELSEIF(UNIX AND NOT APPLE)
	SET(ITK_LIBRARY_DIRS ${ITK_DIR})
ENDIF(APPLE)
#--------------------------------------------------------------------------------

INSTALL(TARGETS ${PROJECT_NAME}
	BUNDLE DESTINATION bin COMPONENT bin
	RUNTIME DESTINATION bin COMPONENT bin)

#--------------------------------------------------------------------------------
# Use BundleUtilities to get all other dependencies for the application to work.

IF(NOT CMAKE_CFG_INTDIR)
	SET(CMAKE_CFG_INTDIR "Release")
ENDIF(NOT CMAKE_CFG_INTDIR)

MESSAGE("\nProject: ${PROJECT_NAME}, Source path: ${CMAKE_CURRENT_SOURCE_DIR}")
MESSAGE("Project: ${PROJECT_NAME}, Install path: ${CMAKE_CURRENT_BINARY_DIR}")
MESSAGE("Project: ${PROJECT_NAME}, INTDIR path: ${CMAKE_CFG_INTDIR}")
MESSAGE("Project: ${PROJECT_NAME}, ITK Directories path: ${ITK_LIBRARY_DIRS}")


SET(DIRS ${ITK_LIBRARY_DIRS})

IF(APPLE)
    SET(DIRS ${DIRS} /opt/local/lib)
    INSTALL(FILES
                "/opt/local/lib/libxslt.1.dylib"
            DESTINATION ${APP_DESTINY}
            COMPONENT bin)
 # ELSEIF(WIN32)
 # 
ELSEIF(UNIX AND NOT APPLE)
	SET(DIRS ${DIRS} /opt/local/lib)
ENDIF(APPLE)

# Now the work of copying dependencies into the bundle/package
MESSAGE("${APPS}")

INSTALL(CODE "
    include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
    " COMPONENT bin)


# To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
# where CPackConfig.cmake is created by including CPack
# And then there's ways to customize this as well
INCLUDE(InstallRequiredSystemLibraries)

IF(APPLE)
	# SET(CPACK_BUNDLE_NAME ${PROJECT_NAME})
ELSEIF(WIN32)
	set(CPACK_GENERATOR NSIS)
	set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
    CreateShortCut \\\"$DESKTOP\\\\ImageValidationISLES.lnk\\\" \\\"$INSTDIR\\\\bin\\\\ImageValidationISLES.exe\\\"")
	SET(CPACK_NSIS_MODIFY_PATH ON)
	install(FILES ${LIBS_EXTERNALS}
			DESTINATION bin
			COMPONENT ${PROJECT_NAME})
	MESSAGE("Project: ${LIBNAMES}")
	# By default, do not warn when built on machines using only VS Express:
	if(NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
		SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)
	endif()
ELSEIF(UNIX AND NOT APPLE)
    SET(CPACK_SOURCE_INSTALLED_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR};/" )
    SET(CPACK_STRIP_FILES "bin/${PROJECT_NAME}")
    SET(CPACK_SOURCE_STRIP_FILES "")
    SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

    SET(CPACK_PACKAGING_INSTALL_PREFIX "/tmp")

    SET(CPACK_GENERATOR "TGZ;DEB")
    # Use dkpg-shlibdeps to generate dependency list
	SET( CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON )
    # Enable debug message
	SET( CPACK_DEBIAN_PACKAGE_DEBUG 1 )

    SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
    SET(CPACK_DEB_COMPONENT_INSTALL 1)
    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Waldo Valenzuela") #required
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libgcc1 (>= 1:3.4.2-12)")

    #Control files
	SET( CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
		"${CMAKE_CURRENT_SOURCE_DIR}/build/packaging/debian/postinst"
		"${CMAKE_CURRENT_SOURCE_DIR}/build/packaging/debian/postrm"
		"${CMAKE_CURRENT_SOURCE_DIR}/build/packaging/debian/preinst"
		"${CMAKE_CURRENT_SOURCE_DIR}/build/packaging/debian/prerm"
		"${CMAKE_CURRENT_SOURCE_DIR}/build/packaging/debian/conffiles"
	)
ENDIF(APPLE)

include(CPack)

