PROJECT(WinSkeletonTuner)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

# OpenGL
FIND_PACKAGE(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)


INCLUDE_DIRECTORIES(
					${Lib_SOURCE_DIR}
					
					${SDLIMAGE_INCLUDE_DIR}
					${SDLTTF_INCLUDE_DIR}
					${SDL_INCLUDE_DIR}
					${GLEW_INCLUDE_DIR}
					${GLUT_INCLUDE_DIR}
					${GLM_INCLUDE_DIR}
					${FREETYPE_INCLUDE_DIRS}
					${ANT_INCLUDE_DIR}
					${GLFW_LIB_SOURCE_DIR}
)					

ADD_EXECUTABLE(WinSkeletonTuner

			Main.cpp
			AppliEditing.h
			AppliEditing.cpp
			
			SkeletonTuner.h
			SkeletonTuner.cpp
		
)

SOURCE_GROUP( WinSkeletonTuner FILES
 
			AppliEditing.h
			AppliEditing.cpp
			Main.cpp
)


SOURCE_GROUP( Tools FILES
 
			SkeletonTuner.h
			SkeletonTuner.cpp
)


TARGET_LINK_LIBRARIES(	WinSkeletonTuner 
						Lib 
						${GLU_LIBRARIES} ${GLEW_LIBRARIES} 
						${SDL_LIBRARIES} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY} ${FREETYPE_LIBRARY} 
						${ANT_LIBRARIES} 
						${GLFW_LIB}
						glfw
					)
# Pour visual studio debug
set_property(TARGET WinSkeletonTuner APPEND PROPERTY LINK_FLAGS /DEBUG)

# Pour visual studio debug
#Format des informations de debogage: Base de données du programme (/Zi)
					
# au final:
# glu32.lib
# opengl32.lib
# User32.lib
# OpenGL32.lib
# +
# %(AdditionalDependencies)