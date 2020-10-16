#========================================================================
# TP Animation / Synthese image
#
#------------------------------------------------------------------------
# Copyright (c) 2010-2011 Equipe SAMSARA (T. LE NAOUR, N. COURTY, S. GIBET)
# VALORIA - UBS 
#========================================================================


PROJECT(Lib)

INCLUDE_DIRECTORIES(
					${SDL_INCLUDE_DIR}
					${GLEW_INCLUDE_DIR}
					${GLM_INCLUDE_DIR}

					${SDLIMAGE_INCLUDE_DIR}
					${SDLTTF_INCLUDE_DIR}
					${SDL_INCLUDE_DIR}
					${FREETYPE_INCLUDE_DIRS}
					${GLFW_LIB_SOURCE_DIR}
					${ANT_INCLUDE_DIR}
)					

LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

ADD_LIBRARY(Lib STATIC 

			GUI.cpp			
			GUI.h
			Common.h
			Application.h
			Application.cpp
			
			Renderer.h
			Renderer.cpp
			FontRenderer.h
			FontRenderer.cpp

			#Animation 
			AnimatedChain.h
			AnimatedChain.cpp
			
)

					