//========================================================================
// TP Animation / Synthese image
//
//------------------------------------------------------------------------
// Copyright (c) 2010-2011 Equipe SAMSARA (T. LE NAOUR, N. COURTY, S. GIBET)
// VALORIA - UBS 
//========================================================================


//-----------------------------------------------------------------
// On inclu les fichiers .h qui contiennent les déclarations des fonctions
// Les implémentations de nos fonctions ont besoin de leurs déclarations
// ainsi que des fonctions open gl
//-----------------------------------------------------------------
#include "GUI.h"
#include <iostream>					// utiliser par la fonction "cout"

bool GUI::m_running(1);
UserEvents* GUI::m_UserEvents(0);
WindowSetup* GUI::m_windowSetup(0);

void GUI::ProcessKeyEvent()
{
	if(m_UserEvents->m_keyBoardEvents[Key_ESCAPE])
		m_running = 0;
}	


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// SDL
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Si gui SDL !!!
#ifdef GUI_SDL
#include "SDL_opengl.h"	//

/////////////////////////////////////////////////////////////
/// Initialise le GUI (Graphic user interface)
///
/// \return code d'erreur
////////////////////////////////////////////////////////////
bool GUISDL::Init()
{	
	// intilisation de SDL, vérification que le mode Video marche
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "Impossible d'initialiser SDL : " << SDL_GetError() << std::endl;
		return 0;
	}

	SDL_Surface* screen = SDL_SetVideoMode(m_windowSetup->m_width, m_windowSetup->m_height, 32, SDL_OPENGL);

	const char *caption = "TP Traitement de l information Multimedia";
	SDL_WM_SetCaption(caption, "Nom de mon icône");

	//SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8);
	//SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8);

	//SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,  8);
	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

	if (screen == NULL)
		return 0;

	return 1;
}

/////////////////////////////////////////////////////////////
/// gère la file d'évements appelés durant la dernière image
/// lien sympa sur la gestion des évenements avec SDL: http://www.siteduzero.com/tutoriel-3-145225-simplifier-les-events-avec-sdl.html
///
////////////////////////////////////////////////////////////
void GUISDL::StartRender() 
{
	m_keyEvents.clear();
	
	// remise à zéro du déplacament de la souris
	m_UserEvents->mousexrel	= 0;
	m_UserEvents->mouseyrel	= 0;

	// tant que SDL contient des événement pour l'image précédente
	while(SDL_PollEvent(&m_event))
	{
		switch (m_event.type)
		{
			/* code à exécuter en cas de pression sur une touche du clavier */
			case SDL_KEYDOWN: 
				{
					m_keyEvents.push_back( Key(((SDL_KeyboardEvent*)&m_event)->keysym.sym) );
					std::cout << m_event.type << std::endl;
					if( ((SDL_KeyboardEvent*)&m_event)->keysym.sym == SDLK_ESCAPE )
						m_running = 0;
				} break;
			

			/* code à exécuter en cas de relâchement d'une touche du clavier */
			case SDL_KEYUP: 
				{
					std::cout << "SDL_KEYUP" << std::endl; 
				}break;
			

			/* code à exécuter en cas de mouvement de la souris */
			case SDL_MOUSEMOTION:
				m_UserEvents->mousexrel	= m_event.motion.x - m_UserEvents->mousex	;
				m_UserEvents->mouseyrel	= m_event.motion.y - m_UserEvents->mousey	;
				m_UserEvents->mousex		= m_event.motion.x		;
				m_UserEvents->mousey		= m_event.motion.y		;
			break;
			

			/* code à exécuter en cas de pression sur une touche de la souris */
			/* rappel :
				SDL_BUTTON_LEFT			1
				SDL_BUTTON_MIDDLE		2
				SDL_BUTTON_RIGHT		3
				SDL_BUTTON_WHEELUP		4
				SDL_BUTTON_WHEELDOWN	5
			*/
			case SDL_MOUSEBUTTONDOWN:
				m_UserEvents->mousebuttons[m_event.button.button-1] = 1;
			break;


			/* code à exécuter en cas de relâchement d'une touche de la souris */
			case SDL_MOUSEBUTTONUP:
				m_UserEvents->mousebuttons[m_event.button.button-1] = 0;
			break;
					

			/* code à exécuter en cas de fermeture de la fenêtre */
			case SDL_QUIT: 
				m_running = 0; 
			break;
				

			/* etc ... */
		}
	}
}


/////////////////////////////////////////////////////////////
/// Fini le rendu du GUI, échange l'image en cour avec la prochaine image
///
////////////////////////////////////////////////////////////
void GUISDL::EndRender()
{
	// technique appelée double-buffering, qui consiste à afficher une image pendant que l'on calcule l'autre, 
	// et lorsque celle-ci est terminée, on échange les deux
	SDL_GL_SwapBuffers();
}


/////////////////////////////////////////////////////////////
/// Termine l'appel du GUI
///
////////////////////////////////////////////////////////////
void GUISDL::Release() 
{
	//SDL_FreeSurface(sTexture);
	//SDL_FreeSurface(sScreen);
	SDL_Quit();
	//exit(0);
}

/////////////////////////////////////////////////////////////
/// Donne l'état du GUI
///
/// \return état du GUI
////////////////////////////////////////////////////////////
bool GUISDL::GetRunning()
{
	return m_running;
}

#endif // fin gui SDL











// Si gui GLFW !!!
#ifdef GUI_GLFW

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GLFW
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int GLFWCALL GuiGlfw::windowCloseListener()
{
	GUI::m_running = false;
	return 0;
}

void GLFWCALL GuiGlfw::keyPressListener( int key, int action )
{
	if( !TwEventKeyGLFW(key, action) )  // Send event to AntTweakBar
    {
        if( key==GLFW_KEY_ESC && action==GLFW_PRESS ) // Want to quit?
            glfwCloseWindow();
        else
        {
            // Event has not been handled
            // Do something if needed.
        }
    }

	if( action == GLFW_PRESS )
	{	
		switch (key)
		{
		case GLFW_KEY_SPACE:	GUI::m_UserEvents->m_keyBoardEvents[Key_SPACE] = true; break;
		case GLFW_KEY_ESC:		GUI::m_UserEvents->m_keyBoardEvents[Key_ESCAPE] = true; break;
		case GLFW_KEY_F1:		GUI::m_UserEvents->m_keyBoardEvents[Key_F1] = true; break;
		case GLFW_KEY_F2:		GUI::m_UserEvents->m_keyBoardEvents[Key_F2] = true; break;
		case GLFW_KEY_F3:		GUI::m_UserEvents->m_keyBoardEvents[Key_F3] = true; break;
		case GLFW_KEY_F4:		GUI::m_UserEvents->m_keyBoardEvents[Key_F4] = true; break;
		case GLFW_KEY_F5:		GUI::m_UserEvents->m_keyBoardEvents[Key_F5] = true; break;
		case GLFW_KEY_F6:		GUI::m_UserEvents->m_keyBoardEvents[Key_F6] = true; break;
		case GLFW_KEY_F7:		GUI::m_UserEvents->m_keyBoardEvents[Key_F7] = true; break;
		case GLFW_KEY_F8:		GUI::m_UserEvents->m_keyBoardEvents[Key_F8] = true; break;
		case GLFW_KEY_F9:		GUI::m_UserEvents->m_keyBoardEvents[Key_F9] = true; break;
		case GLFW_KEY_F10:		GUI::m_UserEvents->m_keyBoardEvents[Key_F10] = true; break;
		case GLFW_KEY_F11:		GUI::m_UserEvents->m_keyBoardEvents[Key_F11] = true; break;
		case GLFW_KEY_F12:		GUI::m_UserEvents->m_keyBoardEvents[Key_F12] = true; break;

		case GLFW_KEY_RCTRL:	GUI::m_UserEvents->m_keyBoardEvents[Key_RCTRL] = true; break;
		case GLFW_KEY_LCTRL:	GUI::m_UserEvents->m_keyBoardEvents[Key_LCTRL] = true; break;
		case GLFW_KEY_RSHIFT:	GUI::m_UserEvents->m_keyBoardEvents[Key_RSHIFT] = true; break;
		case GLFW_KEY_LSHIFT:	GUI::m_UserEvents->m_keyBoardEvents[Key_LSHIFT] = true; break;
		case GLFW_KEY_RALT:		GUI::m_UserEvents->m_keyBoardEvents[Key_RALT] = true; break;
		case GLFW_KEY_LALT:		GUI::m_UserEvents->m_keyBoardEvents[Key_LALT] = true; break;

		// ... todo
		case GLFW_KEY_KP_0:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP0] = true; break;
		case GLFW_KEY_KP_1:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP1] = true; break;
		case GLFW_KEY_KP_2:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP2] = true; break;
		case GLFW_KEY_KP_3:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP3] = true; break;
		case GLFW_KEY_KP_4:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP4] = true; break;
		case GLFW_KEY_KP_5:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP5] = true; break;
		case GLFW_KEY_KP_6:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP6] = true; break;
		case GLFW_KEY_KP_7:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP7] = true; break;
		case GLFW_KEY_KP_8:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP8] = true; break;
		case GLFW_KEY_KP_9:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP9] = true; break;

		case 'A':				GUI::m_UserEvents->m_keyBoardEvents[Key_a] = true; break;
		case 'B':				GUI::m_UserEvents->m_keyBoardEvents[Key_b] = true; break;
		case 'C':				GUI::m_UserEvents->m_keyBoardEvents[Key_c] = true; break;
		case 'D':				GUI::m_UserEvents->m_keyBoardEvents[Key_d] = true; break;
		case 'E':				GUI::m_UserEvents->m_keyBoardEvents[Key_e] = true; break;
		case 'F':				GUI::m_UserEvents->m_keyBoardEvents[Key_f] = true; break;
		case 'G':				GUI::m_UserEvents->m_keyBoardEvents[Key_g] = true; break;
		case 'H':				GUI::m_UserEvents->m_keyBoardEvents[Key_h] = true; break;
		case 'I':				GUI::m_UserEvents->m_keyBoardEvents[Key_i] = true; break;
		case 'J':				GUI::m_UserEvents->m_keyBoardEvents[Key_j] = true; break;
		case 'K':				GUI::m_UserEvents->m_keyBoardEvents[Key_k] = true; break;
		case 'L':				GUI::m_UserEvents->m_keyBoardEvents[Key_l] = true; break;
		case 'M':				GUI::m_UserEvents->m_keyBoardEvents[Key_m] = true; break;
		case 'N':				GUI::m_UserEvents->m_keyBoardEvents[Key_n] = true; break;
		case 'O':				GUI::m_UserEvents->m_keyBoardEvents[Key_o] = true; break;
		case 'P':				GUI::m_UserEvents->m_keyBoardEvents[Key_p] = true; break;
		case 'Q':				GUI::m_UserEvents->m_keyBoardEvents[Key_q] = true; break;
		case 'R':				GUI::m_UserEvents->m_keyBoardEvents[Key_r] = true; break;
		case 'S':				GUI::m_UserEvents->m_keyBoardEvents[Key_s] = true; break;
		case 'T':				GUI::m_UserEvents->m_keyBoardEvents[Key_t] = true; break;
		case 'U':				GUI::m_UserEvents->m_keyBoardEvents[Key_u] = true; break;
		case 'V':				GUI::m_UserEvents->m_keyBoardEvents[Key_v] = true; break;
		case 'W':				GUI::m_UserEvents->m_keyBoardEvents[Key_w] = true; break;
		case 'X':				GUI::m_UserEvents->m_keyBoardEvents[Key_x] = true; break;
		case 'Y':				GUI::m_UserEvents->m_keyBoardEvents[Key_y] = true; break;
		case 'Z':				GUI::m_UserEvents->m_keyBoardEvents[Key_z] = true; break;

		case GLFW_KEY_RIGHT:	GUI::m_UserEvents->m_keyBoardEvents[Key_RIGHT]	= true; break;
		case GLFW_KEY_LEFT:		GUI::m_UserEvents->m_keyBoardEvents[Key_LEFT]	= true; break;
		case GLFW_KEY_DOWN:		GUI::m_UserEvents->m_keyBoardEvents[Key_DOWN]	= true; break;
		case GLFW_KEY_UP:		GUI::m_UserEvents->m_keyBoardEvents[Key_UP]		= true; break;
		
		default: break;
		}
	}

	if( action == GLFW_RELEASE )
	{	
		switch (key)
		{
		case GLFW_KEY_SPACE:	GUI::m_UserEvents->m_keyBoardEvents[Key_SPACE] = false; break;
		case GLFW_KEY_ESC:		GUI::m_UserEvents->m_keyBoardEvents[Key_ESCAPE] = false; break;
		case GLFW_KEY_F1:		GUI::m_UserEvents->m_keyBoardEvents[Key_F1] = false; break;
		case GLFW_KEY_F2:		GUI::m_UserEvents->m_keyBoardEvents[Key_F2] = false; break;
		case GLFW_KEY_F3:		GUI::m_UserEvents->m_keyBoardEvents[Key_F3] = false; break;
		case GLFW_KEY_F4:		GUI::m_UserEvents->m_keyBoardEvents[Key_F4] = false; break;
		case GLFW_KEY_F5:		GUI::m_UserEvents->m_keyBoardEvents[Key_F5] = false; break;
		case GLFW_KEY_F6:		GUI::m_UserEvents->m_keyBoardEvents[Key_F6] = false; break;
		case GLFW_KEY_F7:		GUI::m_UserEvents->m_keyBoardEvents[Key_F7] = false; break;
		case GLFW_KEY_F8:		GUI::m_UserEvents->m_keyBoardEvents[Key_F8] = false; break;
		case GLFW_KEY_F9:		GUI::m_UserEvents->m_keyBoardEvents[Key_F9] = false; break;
		case GLFW_KEY_F10:		GUI::m_UserEvents->m_keyBoardEvents[Key_F10] = false; break;
		case GLFW_KEY_F11:		GUI::m_UserEvents->m_keyBoardEvents[Key_F11] = false; break;
		case GLFW_KEY_F12:		GUI::m_UserEvents->m_keyBoardEvents[Key_F12] = false; break;

		case GLFW_KEY_RCTRL:	GUI::m_UserEvents->m_keyBoardEvents[Key_RCTRL] = false; break;
		case GLFW_KEY_LCTRL:	GUI::m_UserEvents->m_keyBoardEvents[Key_LCTRL] = false; break;
		case GLFW_KEY_RSHIFT:	GUI::m_UserEvents->m_keyBoardEvents[Key_RSHIFT] = false; break;
		case GLFW_KEY_LSHIFT:	GUI::m_UserEvents->m_keyBoardEvents[Key_LSHIFT] = false; break;
		case GLFW_KEY_RALT:		GUI::m_UserEvents->m_keyBoardEvents[Key_RALT] = false; break;
		case GLFW_KEY_LALT:		GUI::m_UserEvents->m_keyBoardEvents[Key_LALT] = false; break;

		// ... todo
		case GLFW_KEY_KP_0:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP0] = false; break;
		case GLFW_KEY_KP_1:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP1] = false; break;
		case GLFW_KEY_KP_2:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP2] = false; break;
		case GLFW_KEY_KP_3:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP3] = false; break;
		case GLFW_KEY_KP_4:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP4] = false; break;
		case GLFW_KEY_KP_5:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP5] = false; break;
		case GLFW_KEY_KP_6:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP6] = false; break;
		case GLFW_KEY_KP_7:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP7] = false; break;
		case GLFW_KEY_KP_8:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP8] = false; break;
		case GLFW_KEY_KP_9:		GUI::m_UserEvents->m_keyBoardEvents[Key_KP9] = false; break;

		case 'A':				GUI::m_UserEvents->m_keyBoardEvents[Key_a] = false; break;
		case 'B':				GUI::m_UserEvents->m_keyBoardEvents[Key_b] = false; break;
		case 'C':				GUI::m_UserEvents->m_keyBoardEvents[Key_c] = false; break;
		case 'D':				GUI::m_UserEvents->m_keyBoardEvents[Key_d] = false; break;
		case 'E':				GUI::m_UserEvents->m_keyBoardEvents[Key_e] = false; break;
		case 'F':				GUI::m_UserEvents->m_keyBoardEvents[Key_f] = false; break;
		case 'G':				GUI::m_UserEvents->m_keyBoardEvents[Key_g] = false; break;
		case 'H':				GUI::m_UserEvents->m_keyBoardEvents[Key_h] = false; break;
		case 'I':				GUI::m_UserEvents->m_keyBoardEvents[Key_i] = false; break;
		case 'J':				GUI::m_UserEvents->m_keyBoardEvents[Key_j] = false; break;
		case 'K':				GUI::m_UserEvents->m_keyBoardEvents[Key_k] = false; break;
		case 'L':				GUI::m_UserEvents->m_keyBoardEvents[Key_l] = false; break;
		case 'M':				GUI::m_UserEvents->m_keyBoardEvents[Key_m] = false; break;
		case 'N':				GUI::m_UserEvents->m_keyBoardEvents[Key_n] = false; break;
		case 'O':				GUI::m_UserEvents->m_keyBoardEvents[Key_o] = false; break;
		case 'P':				GUI::m_UserEvents->m_keyBoardEvents[Key_p] = false; break;
		case 'Q':				GUI::m_UserEvents->m_keyBoardEvents[Key_q] = false; break;
		case 'R':				GUI::m_UserEvents->m_keyBoardEvents[Key_r] = false; break;
		case 'S':				GUI::m_UserEvents->m_keyBoardEvents[Key_s] = false; break;
		case 'T':				GUI::m_UserEvents->m_keyBoardEvents[Key_t] = false; break;
		case 'U':				GUI::m_UserEvents->m_keyBoardEvents[Key_u] = false; break;
		case 'V':				GUI::m_UserEvents->m_keyBoardEvents[Key_v] = false; break;
		case 'W':				GUI::m_UserEvents->m_keyBoardEvents[Key_w] = false; break;
		case 'X':				GUI::m_UserEvents->m_keyBoardEvents[Key_x] = false; break;
		case 'Y':				GUI::m_UserEvents->m_keyBoardEvents[Key_y] = false; break;
		case 'Z':				GUI::m_UserEvents->m_keyBoardEvents[Key_z] = false; break;

		case GLFW_KEY_RIGHT:	GUI::m_UserEvents->m_keyBoardEvents[Key_RIGHT]	= false; break;
		case GLFW_KEY_LEFT:		GUI::m_UserEvents->m_keyBoardEvents[Key_LEFT]	= false; break;
		case GLFW_KEY_DOWN:		GUI::m_UserEvents->m_keyBoardEvents[Key_DOWN]	= false; break;
		case GLFW_KEY_UP:		GUI::m_UserEvents->m_keyBoardEvents[Key_UP]		= false; break;
		
		default: break;
		}
	}
}

void GLFWCALL GuiGlfw::mouseMoveListener( int x, int y )
{
	if( !TwEventMousePosGLFW(x, y) )  // Send event to AntTweakBar
    {
        // Event has not been handled by AntTweakBar
        // Do something if needed.
    }


	/* rappel :
		BUTTON_LEFT			0 
		BUTTON_MIDDLE		1 
		BUTTON_RIGHT		2 
	*/
	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT))
	{
		m_UserEvents->mousebuttons[1] = 1;
		m_UserEvents->mousexrel	= x - m_UserEvents->mousex	;
		m_UserEvents->mouseyrel	= y - m_UserEvents->mousey	;
	}
	else
	{
		m_UserEvents->mousebuttons[1] = 0;
	}

	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
	{
		m_UserEvents->mousebuttons[0] = 1;
		m_UserEvents->mousexrel	= x - m_UserEvents->mousex	;
		m_UserEvents->mouseyrel	= y - m_UserEvents->mousey	;
	}
	else
	{
		m_UserEvents->mousebuttons[0] = 0;
	}

	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE))
	{
		m_UserEvents->mousebuttons[2] = 1;
		m_UserEvents->mousexrel	= x - m_UserEvents->mousex	;
		m_UserEvents->mouseyrel	= y - m_UserEvents->mousey	;
	}
	else
	{
		m_UserEvents->mousebuttons[2] = 0;
	}

	m_UserEvents->mousex		= x		;
	m_UserEvents->mousey		= y		;
}

// Callback function called by GLFW when a mouse button is clicked
void GLFWCALL GuiGlfw::OnMouseButton(int glfwButton, int glfwAction)
{
	//if(glfwButton==GLFW_MOUSE_BUTTON_RIGHT &&  glfwAction==GLFW_PRESS)
	//	m_UserEvents->mousebuttonsRelease[1] = 1;
	//if(glfwButton==GLFW_MOUSE_BUTTON_LEFT &&  glfwAction==GLFW_PRESS)
	//	m_UserEvents->mousebuttonsRelease[0] = 1;
	//if(glfwButton==GLFW_MOUSE_BUTTON_MIDDLE &&  glfwAction==GLFW_PRESS)
	//	m_UserEvents->mousebuttonsRelease[2] = 1;

	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT))
		m_UserEvents->mousebuttons[1] = 1;
	else
		m_UserEvents->mousebuttons[1] = 0;

	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
		m_UserEvents->mousebuttons[0] = 1;
	else
		m_UserEvents->mousebuttons[0] = 0;

	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE))
		m_UserEvents->mousebuttons[2] = 1;
	else
		m_UserEvents->mousebuttons[2] = 0;

    if( !TwEventMouseButtonGLFW(glfwButton, glfwAction) )   // Send event to AntTweakBar
    {
        // Event has not been handled by AntTweakBar
        // Do something if needed.
    }
}

// Callback function called by GLFW on char event
void GLFWCALL GuiGlfw::OnChar(int glfwChar, int glfwAction)
{
    if( !TwEventCharGLFW(glfwChar, glfwAction) )    // Send event to AntTweakBar
    {
        // Event has not been handled by AntTweakBar
        // Do something if needed.
    }
}

// Callback function called by GLFW when window size changes
void GLFWCALL GuiGlfw::OnWindowSize(int width, int height)
{
    // Send the new window size to AntTweakBar
    TwWindowSize(width, height);

	m_windowSetup->m_width = width;
	m_windowSetup->m_height= height;
}


////////////////////////////////////////////////////////////
/// Initialise le GUI (Graphic user interface)
///
/// \return code d'erreur
////////////////////////////////////////////////////////////
bool GuiGlfw::Init()
{	
	GLFWvidmode mode;   // GLFW video mode
	glfwGetDesktopMode(&mode);

	// Initialize GLFW
	glfwInit();

	// Send the new window size to AntTweakBar
    TwWindowSize(m_windowSetup->m_width, m_windowSetup->m_height);

	// Initialize AntTweakBar
    if( !TwInit(TW_OPENGL, NULL) )
    {
        // A fatal error occured    
		std::cout << "AntTweakBar initialization failed: " << TwGetLastError() << std::endl;
        glfwTerminate();
        return false;
    }

	// Create OpenGL window
	if( !glfwOpenWindow(m_windowSetup->m_width, m_windowSetup->m_height, mode.RedBits, mode.GreenBits, mode.BlueBits, 
                        0, 16, 0, GLFW_WINDOW /* or GLFW_FULLSCREEN */) )
	{
		glfwTerminate();
		return false;
	}

	const char *caption = "TP";
	glfwSetWindowTitle( caption );
	
	// Disable vertical synchronization
	glfwSwapInterval( 0 );

	// Set listeners
	glfwSetWindowSizeCallback(GuiGlfw::OnWindowSize);
	glfwSetWindowCloseCallback( GuiGlfw::windowCloseListener );
	glfwSetMouseButtonCallback(GuiGlfw::OnMouseButton);
	glfwSetCharCallback(GuiGlfw::OnChar);
	glfwSetKeyCallback( GuiGlfw::keyPressListener );
	glfwSetMousePosCallback( GuiGlfw::mouseMoveListener );
	
	return true;
}

/////////////////////////////////////////////////////////////
/// gère la file d'évements appelés durant la dernière image
/// lien sympa sur la gestion des évenements avec SDL: http://www.siteduzero.com/tutoriel-3-145225-simplifier-les-events-avec-sdl.html
///
////////////////////////////////////////////////////////////
void GuiGlfw::StartRender() 
{
	// traite les evt clavier
	ProcessKeyEvent();

	if( !GUI::m_running ) return;

	// mise à jour du titre de la fenêtre avec le fps
	glfwSetWindowTitle("BVHEditor");
	
	
}


/////////////////////////////////////////////////////////////
/// Fini le rendu du GUI, échange l'image en cour avec la prochaine image
///
////////////////////////////////////////////////////////////
void GuiGlfw::EndRender()
{
	// Draw tweak bars
    TwDraw();

	// clear event list
	m_UserEvents->ClearKeyBardEvents();

	// remise à zéro du déplacament de la souris
	m_UserEvents->mousexrel	= 0;
	m_UserEvents->mouseyrel	= 0;

	glfwSwapBuffers();
}


/////////////////////////////////////////////////////////////
/// Termine l'appel du GUI
///
////////////////////////////////////////////////////////////
void GuiGlfw::Release() 
{
	TwTerminate();
	glfwTerminate();
}

#endif // fin gui GLFW