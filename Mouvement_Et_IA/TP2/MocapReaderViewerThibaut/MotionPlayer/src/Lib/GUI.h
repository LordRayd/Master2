//========================================================================
// TP Animation / Synthese image
//
//------------------------------------------------------------------------
// Copyright (c) 2010-2011 Equipe SAMSARA (T. LE NAOUR, N. COURTY, S. GIBET)
// VALORIA - UBS 
//========================================================================


//=====================================================================
// classe GUI, s'occupe du fenetrage et de l'interface avec l'utilisateur
//=====================================================================
//-----------------------------------------------------------------
// Ici fichier des déclarations des fonctions. Une fonction ne peut être déclarée qu'une seule fois
// d'ou l'ajout d'une routine qui dit que si votre fichier à déjà été inclu avant pas la peine de le re-inclure
//-----------------------------------------------------------------
#ifndef __GUI_H__	// routine qui sert à pas inclure 2 fois le même fichier
#define __GUI_H__	//
#pragma warning( disable : 4530)

//-----------------------------------------------------------------
// On inclu les fichiers .h qui contiennent les déclarations des fonctions
// On a besoin ici des objets SDL (SDLKey, SDL_Event) déclarés dans SDL.h
// ainsi que des déclarations des fonctions OPENGL
//-----------------------------------------------------------------
//#include "SDL.h"
#include <vector>					// utilisé par "std::vector<SDLKey> m_keyEvents;"
#include "Common.h"					// structure UserEvents utilisée pour récupérer les données liées à la souris
#include <AntTweakBar.h>			// Les contrôles

//Ici on définit quel GUI on veut
#define GUI_GLFW



////////////////////////////////////////////////////////////////////////
/// Class Interface qui définit l'utlisation d'un GUI, 
/// à savoir la création de la fenetre,  gestion de la caméra, clavier , joystik ...
////////////////////////////////////////////////////////////////////////
class GUI
{
public:

	//----------------------------------------------------------
	// Destructeur (virtuel car cette classe a un enfant: GUISDL)
	//----------------------------------------------------------
	virtual ~GUI(){m_UserEvents = 0;}

	//----------------------------------------------------------
	// intialisation du GUI
	//----------------------------------------------------------
	virtual bool Init() = 0;

	//----------------------------------------------------------
	// instruction a lancer avant le rendu de la scene
	//----------------------------------------------------------
	virtual void StartRender() = 0;

	//----------------------------------------------------------
	// instruction a lancer à la fin du rendu de la scene
	//----------------------------------------------------------
	virtual void EndRender() = 0;

	//----------------------------------------------------------
	// remise à zéro du GUI
	//----------------------------------------------------------
	virtual void Release() = 0;

	//----------------------------------------------------------
	// indication sur l'état du GUI
	//----------------------------------------------------------
	virtual bool GetRunning() = 0;


protected: // les fonctions suivantes peuvent être appelées seulement par cettte même classe et les classes filles de cette classe
	
	//----------------------------------------------------------
	// constructeur protégé car seules les classes enfants peuvent l'appeler
	//----------------------------------------------------------
	GUI(WindowSetup* windowSetup, UserEvents* UserEvents)
		{m_windowSetup=windowSetup; m_UserEvents = UserEvents;}

	//----------------------------------------------------------
	// constructeur protégé car seules les classes enfants peuvent l'appeler
	//----------------------------------------------------------
	GUI(){}

	//----------------------------------------------------------
	// traite certains evt clavier
	//----------------------------------------------------------
	void ProcessKeyEvent();

	//----------------------------------------------------------
	// Données membres
	//----------------------------------------------------------
	static WindowSetup*		m_windowSetup	;	///< config et evenement liés à la fenêtre 
	static UserEvents*		m_UserEvents	;	///< evenement liés à la souris 
	static bool				m_running		;	///< variable static indiquant l'état du GUI
	//static std::vector<Key>	m_keyEvents		;	///< liste des événements pour chaque frame
};






// Si gui SDL !!!
#ifdef GUI_SDL

////////////////////////////////////////////////////////////////////////
// Class qui définit un GUI à partir de la bibliotheque SDL, à savoir la création de la fenetre,  gestion de la caméra, clavier , joystik ...
// (il existe d'autres librairies d'interfacage. Il en a une propre à chaque os ou encore GLFW, GLUT)
////////////////////////////////////////////////////////////////////////
class GUISDL: public GUI
{
public:	// les fonctions suivantes peuvent être appelées par d'autres classes / programmes

	//----------------------------------------------------------
	// constructeur
	//----------------------------------------------------------
	GUISDL()
		:GUI(){}

	//----------------------------------------------------------
	// constructeur 
	//----------------------------------------------------------
	GUISDL(WindowSetup* windowSetup, UserEvents* UserEvents)
		:GUI(windowSetup, UserEvents) {}

	//----------------------------------------------------------
	// Destructeur
	//----------------------------------------------------------
	virtual ~GUISDL(){}

	//----------------------------------------------------------
	// intialisation du GUI
	//----------------------------------------------------------
	bool Init();

	//----------------------------------------------------------
	// instruction a lancer avant le rendu de la scene
	//----------------------------------------------------------
	void StartRender(); 

	//----------------------------------------------------------
	// instruction a lancer à la fin du rendu de la scene
	//----------------------------------------------------------
	void EndRender();

	//----------------------------------------------------------
	// remise à zéro du GUI
	//----------------------------------------------------------
	void Release() ;

	//----------------------------------------------------------
	// indication sur l'état du GUI
	//----------------------------------------------------------
	bool GetRunning();
	

protected:	// les fonctions suivantes peuvent être appelées seulement pas cettte même classe et les classes filles de cette classe

	//----------------------------------------------------------
	// Données membres
	//----------------------------------------------------------
	SDL_Event				m_event;	// événement traité
};

#endif // fin gui SDL







// Si gui GLFW !!!
#ifdef GUI_GLFW

#include "glfw.h"
#pragma warning( disable : 4005 )


////////////////////////////////////////////////////////////////////////
// Class qui définit un GUI à partir de la bibliotheque GLFW, à savoir la création de la fenetre,  gestion de la caméra, clavier , joystik ...
////////////////////////////////////////////////////////////////////////
class GuiGlfw: public GUI
{
public:	// les fonctions suivantes peuvent être appelées par d'autres classes / programmes

	//----------------------------------------------------------
	// constructeur
	//----------------------------------------------------------
	GuiGlfw()
		:GUI(){}

	//----------------------------------------------------------
	// constructeur 
	//----------------------------------------------------------
	GuiGlfw(WindowSetup* windowSetup, UserEvents* UserEvents)
		:GUI(windowSetup, UserEvents) {}

	//----------------------------------------------------------
	// Destructeur
	//----------------------------------------------------------
	virtual ~GuiGlfw(){}

	//----------------------------------------------------------
	// intialisation du GUI
	//----------------------------------------------------------
	bool Init();

	//----------------------------------------------------------
	// instruction a lancer avant le rendu de la scene
	//----------------------------------------------------------
	void StartRender(); 

	//----------------------------------------------------------
	// instruction a lancer à la fin du rendu de la scene
	//----------------------------------------------------------
	void EndRender();

	//----------------------------------------------------------
	// remise à zéro du GUI
	//----------------------------------------------------------
	void Release() ;

	//----------------------------------------------------------
	// indication sur l'état du GUI
	//----------------------------------------------------------
	bool GetRunning();
	
	
protected:	// les fonctions suivantes peuvent être appelées seulement pas cettte même classe et les classes filles de cette classe
	
	static int GLFWCALL windowCloseListener();
	static void GLFWCALL keyPressListener( int key, int action );
	static void GLFWCALL mouseMoveListener( int x, int y );
	static void GLFWCALL OnMouseButton(int glfwButton, int glfwAction);
	static void GLFWCALL OnChar(int glfwChar, int glfwAction);
	static void GLFWCALL OnWindowSize(int width, int height);
};

/////////////////////////////////////////////////////////////
/// Donne l'état du GUI
///
/// \return état du GUI
////////////////////////////////////////////////////////////
inline bool GuiGlfw::GetRunning()
{
	return m_running;
}

#endif // fin gui GLFW


#endif // __GUI_H__ fin de la routine