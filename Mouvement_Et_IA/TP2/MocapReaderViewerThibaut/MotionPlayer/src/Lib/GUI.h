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
// Ici fichier des d�clarations des fonctions. Une fonction ne peut �tre d�clar�e qu'une seule fois
// d'ou l'ajout d'une routine qui dit que si votre fichier � d�j� �t� inclu avant pas la peine de le re-inclure
//-----------------------------------------------------------------
#ifndef __GUI_H__	// routine qui sert � pas inclure 2 fois le m�me fichier
#define __GUI_H__	//
#pragma warning( disable : 4530)

//-----------------------------------------------------------------
// On inclu les fichiers .h qui contiennent les d�clarations des fonctions
// On a besoin ici des objets SDL (SDLKey, SDL_Event) d�clar�s dans SDL.h
// ainsi que des d�clarations des fonctions OPENGL
//-----------------------------------------------------------------
//#include "SDL.h"
#include <vector>					// utilis� par "std::vector<SDLKey> m_keyEvents;"
#include "Common.h"					// structure UserEvents utilis�e pour r�cup�rer les donn�es li�es � la souris
#include <AntTweakBar.h>			// Les contr�les

//Ici on d�finit quel GUI on veut
#define GUI_GLFW



////////////////////////////////////////////////////////////////////////
/// Class Interface qui d�finit l'utlisation d'un GUI, 
/// � savoir la cr�ation de la fenetre,  gestion de la cam�ra, clavier , joystik ...
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
	// instruction a lancer � la fin du rendu de la scene
	//----------------------------------------------------------
	virtual void EndRender() = 0;

	//----------------------------------------------------------
	// remise � z�ro du GUI
	//----------------------------------------------------------
	virtual void Release() = 0;

	//----------------------------------------------------------
	// indication sur l'�tat du GUI
	//----------------------------------------------------------
	virtual bool GetRunning() = 0;


protected: // les fonctions suivantes peuvent �tre appel�es seulement par cettte m�me classe et les classes filles de cette classe
	
	//----------------------------------------------------------
	// constructeur prot�g� car seules les classes enfants peuvent l'appeler
	//----------------------------------------------------------
	GUI(WindowSetup* windowSetup, UserEvents* UserEvents)
		{m_windowSetup=windowSetup; m_UserEvents = UserEvents;}

	//----------------------------------------------------------
	// constructeur prot�g� car seules les classes enfants peuvent l'appeler
	//----------------------------------------------------------
	GUI(){}

	//----------------------------------------------------------
	// traite certains evt clavier
	//----------------------------------------------------------
	void ProcessKeyEvent();

	//----------------------------------------------------------
	// Donn�es membres
	//----------------------------------------------------------
	static WindowSetup*		m_windowSetup	;	///< config et evenement li�s � la fen�tre 
	static UserEvents*		m_UserEvents	;	///< evenement li�s � la souris 
	static bool				m_running		;	///< variable static indiquant l'�tat du GUI
	//static std::vector<Key>	m_keyEvents		;	///< liste des �v�nements pour chaque frame
};






// Si gui SDL !!!
#ifdef GUI_SDL

////////////////////////////////////////////////////////////////////////
// Class qui d�finit un GUI � partir de la bibliotheque SDL, � savoir la cr�ation de la fenetre,  gestion de la cam�ra, clavier , joystik ...
// (il existe d'autres librairies d'interfacage. Il en a une propre � chaque os ou encore GLFW, GLUT)
////////////////////////////////////////////////////////////////////////
class GUISDL: public GUI
{
public:	// les fonctions suivantes peuvent �tre appel�es par d'autres classes / programmes

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
	// instruction a lancer � la fin du rendu de la scene
	//----------------------------------------------------------
	void EndRender();

	//----------------------------------------------------------
	// remise � z�ro du GUI
	//----------------------------------------------------------
	void Release() ;

	//----------------------------------------------------------
	// indication sur l'�tat du GUI
	//----------------------------------------------------------
	bool GetRunning();
	

protected:	// les fonctions suivantes peuvent �tre appel�es seulement pas cettte m�me classe et les classes filles de cette classe

	//----------------------------------------------------------
	// Donn�es membres
	//----------------------------------------------------------
	SDL_Event				m_event;	// �v�nement trait�
};

#endif // fin gui SDL







// Si gui GLFW !!!
#ifdef GUI_GLFW

#include "glfw.h"
#pragma warning( disable : 4005 )


////////////////////////////////////////////////////////////////////////
// Class qui d�finit un GUI � partir de la bibliotheque GLFW, � savoir la cr�ation de la fenetre,  gestion de la cam�ra, clavier , joystik ...
////////////////////////////////////////////////////////////////////////
class GuiGlfw: public GUI
{
public:	// les fonctions suivantes peuvent �tre appel�es par d'autres classes / programmes

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
	// instruction a lancer � la fin du rendu de la scene
	//----------------------------------------------------------
	void EndRender();

	//----------------------------------------------------------
	// remise � z�ro du GUI
	//----------------------------------------------------------
	void Release() ;

	//----------------------------------------------------------
	// indication sur l'�tat du GUI
	//----------------------------------------------------------
	bool GetRunning();
	
	
protected:	// les fonctions suivantes peuvent �tre appel�es seulement pas cettte m�me classe et les classes filles de cette classe
	
	static int GLFWCALL windowCloseListener();
	static void GLFWCALL keyPressListener( int key, int action );
	static void GLFWCALL mouseMoveListener( int x, int y );
	static void GLFWCALL OnMouseButton(int glfwButton, int glfwAction);
	static void GLFWCALL OnChar(int glfwChar, int glfwAction);
	static void GLFWCALL OnWindowSize(int width, int height);
};

/////////////////////////////////////////////////////////////
/// Donne l'�tat du GUI
///
/// \return �tat du GUI
////////////////////////////////////////////////////////////
inline bool GuiGlfw::GetRunning()
{
	return m_running;
}

#endif // fin gui GLFW


#endif // __GUI_H__ fin de la routine