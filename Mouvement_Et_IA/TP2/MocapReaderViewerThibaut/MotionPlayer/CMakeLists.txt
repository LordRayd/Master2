
##---------------------------------------------------------------------------
## $RCSfile: 		CMakeLists.txt $
## $Source: 		CMakeLists.txt $
## $Revision: 		1.00 $
## $Date: 			Jan 0, 2009 0:00:00 PM $
##---------------------------------------------------------------------------
## Author:      	Tibo (tlenaour@gmail.com)
## Subject:			code example to introduce OPENGL with SDL
## Copyright:   	no copyright
## License:     	no license
## Date of birth : 	13 - 11 - 2009
##---------------------------------------------------------------------------


# define minimum cmake version
cmake_minimum_required(VERSION 2.6.2)

# define SAMSARA version
set(MotionEditing_VERSION_MAJOR 0)
set(MotionEditing_VERSION_MINOR 1)
set(MotionEditing_VERSION_SUBMINOR 0)
set(MotionEditing_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_SUBMINOR}")

# Our project is called 'MotionEditing' play with 3d
# visual studio, and in our makefiles. 
PROJECT(MotionEditing)

set(CMAKE_SUPPRESS_REGENERATION TRUE)

# Specifies the build type for make based generators.
SET(CMAKE_BUILD_TYPE Release)

# Create verbose makefiles if on.
SET(CMAKE_VERBOSE_MAKEFILE false)

# Output directory in which to build LIBRARY target files, 
# LIBRARY_OUTPUT_DIRECTORY replace LIBRARY_OUTPUT_PATH ( SET(LIBRARY_OUTPUT_PATH ${MotionEditing_SOURCE_DIR}/lib) )
SET(LIBRARY_OUTPUT_PATH 	${MotionEditing_SOURCE_DIR}/lib)	

# Output directory in which to build RUNTIME target files,
# RUNTIME_OUTPUT_DIRECTORY replace SET(EXECUTABLE_OUTPUT_PATH ${SOURCE_DIR}/bin)
SET(EXECUTABLE_OUTPUT_PATH 	${MotionEditing_SOURCE_DIR}/bin)

# Location where cmake first looks for cmake modules.
set(CMAKE_MODULE_PATH ${MotionEditing_SOURCE_DIR})



##---------------------------------------------------------------------------
# Special settings for UNIX systems
##---------------------------------------------------------------------------
IF(UNIX)

	# SDL Stuff (Apple: sudo port install libsdl, sudo port install libsdl_image)
	#-------------------------------------
	Find_Package(SDL REQUIRED)
	Find_Package(SDL_image REQUIRED)
	
	# Workaround for the non-working REQUIRED flag
	if ( NOT SDL_FOUND )
		message ( FATAL_ERROR "SDL not found!" )
	endif ( NOT SDL_FOUND )
	
	if ( NOT SDLIMAGE_FOUND )
		message ( FATAL_ERROR "SDLIMAGE not found!" )
	endif ( NOT SDLIMAGE_FOUND )
	
	
	# GLM Stuff
	#-------------------------------------
	SET(GLM_INCLUDE_DIR 		${MotionEditing_SOURCE_DIR}/thirdParty/glm-0.8.4.3)
	
	
	IF(APPLE) 						#sudo port install glew
		INCLUDE(FindOpenGL)
		SET(GLEW_INCLUDE_DIR /opt/local/include)
		SET(GLEW_LIBRARIES 	/opt/local/lib/libGLEW.a)
	ELSE(APPLE)
		INCLUDE(FindOpenGL)
		#SET (GLEW_LIBRARIES GLEW)
	ENDIF(APPLE)
ENDIF(UNIX)


##---------------------------------------------------------------------------
# Special settings for Windows systems
##---------------------------------------------------------------------------
IF(WIN32)
    SET(GUI "Win32")
	
    SET(GLEW_INCLUDE_DIR 		${MotionEditing_SOURCE_DIR}/thirdParty/win32/include)
	SET(SDL_INCLUDE_DIR 		${MotionEditing_SOURCE_DIR}/thirdParty/win32/SDL
								${MotionEditing_SOURCE_DIR}/thirdParty/win32/SDL/SDL/include
								${MotionEditing_SOURCE_DIR}/thirdParty/win32/SDL/SDL_image/include
								${MotionEditing_SOURCE_DIR}/thirdParty/win32/SDL/SDL_ttf-2.0.9/include)
	SET(GLM_INCLUDE_DIR 		${MotionEditing_SOURCE_DIR}/thirdParty/glm-0.8.4.3)
    
	SET(GLU_LIBRARIES 			${MotionEditing_SOURCE_DIR}/thirdParty/win32/lib/glu32.lib)
    SET(GLEW_LIBRARIES 			${MotionEditing_SOURCE_DIR}/thirdParty/win32/lib/glew32.lib)
	SET(SDL_LIBRARY 			${MotionEditing_SOURCE_DIR}/thirdParty/win32/SDL/SDL/lib/SDL.lib
								${MotionEditing_SOURCE_DIR}/thirdParty/win32/SDL/SDL/lib/SDLmain.lib
								${MotionEditing_SOURCE_DIR}/thirdParty/win32/SDL/SDL_image/lib/SDL_image.lib)
	SET(FREETYPE_INCLUDE_DIRS 	${MotionEditing_SOURCE_DIR}/thirdParty/win32/freetype-2.3.12/include)
	SET(FREETYPE_LIBRARY		${MotionEditing_SOURCE_DIR}/thirdParty/win32/freetype-2.3.12/lib/freetype2312.lib
								${MotionEditing_SOURCE_DIR}/thirdParty/win32/freetype-2.3.12/lib/freetype2312_D.lib)		
	SET(ANT_INCLUDE_DIR 		${MotionEditing_SOURCE_DIR}/thirdParty/AntTweakBar/include)
	SET(ANT_LIBRARIES 			${MotionEditing_SOURCE_DIR}/thirdParty/AntTweakBar/lib/AntTweakBar.lib)					
ENDIF(WIN32)


# Add a list of subdirectories to the build,
# subdirs is deprecated. Use the add_subdirectory() command instead. SUBDIRS(thirdParty SRC) -> 
add_subdirectory(src)
add_subdirectory(thirdParty/glfw)




