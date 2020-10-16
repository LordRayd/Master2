PROJECT(WinAppli_Editing)

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

ADD_EXECUTABLE(WinAppli_Editing

			Main.cpp
			AppliEditing.h
			AppliEditing.cpp
		
)

SOURCE_GROUP( WinAppli_Editing FILES
 
			AppliEditing.h
			AppliEditing.cpp
			Main.cpp
)



TARGET_LINK_LIBRARIES(	WinAppli_Editing 
						Lib 
						${GLU_LIBRARIES} ${GLEW_LIBRARIES} 
						${SDL_LIBRARIES} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY} ${FREETYPE_LIBRARY} 
						${ANT_LIBRARIES} 
						${GLFW_LIB}
						glfw
					)