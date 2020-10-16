//========================================================================
// TP Animation / Synthese image
//
//------------------------------------------------------------------------
// Copyright (c) 2010-2011 Equipe SAMSARA (T. LE NAOUR, N. COURTY, S. GIBET)
// VALORIA - UBS 
//========================================================================


//=====================================================================
// classe APPLICATION, s'occupe d'encapsuler l'ensemble des fonction communes à une application
//=====================================================================
#ifndef __APPLICATION_H__	// routine qui sert à pas inclure 2 fois le même fichier
#define __APPLICATION_H__	//



//-----------------------------------------------------------------
// Notre programme va avoir besoin 
// des fonctions spécifiques à l'utilisateur (iterface graphique utilisateur GUI) et
// des fonctions nécéssaires au rendu d'un objet dans la scene
//
// On inclu les fichiers .h qui contiennent les déclarations des fonctions
//-----------------------------------------------------------------
#include "Common.h"				// données liées à la souris
#include "GUI.h"				// pointeur GUI
#include "Renderer.h"			// objet Renderer


////////////////////////////////////////////////////////////////////////
/// classe Application: elle encapsule l'ensemble de l'application à savoir l'utilisation du GUI et du renderer
/// L'objectif étant qu'elle sert de classe commune à ttes applications qui se serviront des fonctions suivantes
////////////////////////////////////////////////////////////////////////
class Application
{
public:
	//----------------------------------------------------------
	// constructeur de l'objet
	//----------------------------------------------------------
	Application()
		:m_windowSetUp(new WindowSetup(800, 640)), m_UserEvents(0), m_gui(0), m_camera(0) {}

	//----------------------------------------------------------
	// destructeur de l'objet
	//----------------------------------------------------------
	~Application(){}

	//----------------------------------------------------------
	// initilisation de l'appli
	//----------------------------------------------------------
	bool Init()		;

	//----------------------------------------------------------
	// boucle de rendu
	//----------------------------------------------------------
	void Run()		;

	//----------------------------------------------------------
	// fin de l'appli, destruction et nettoyage des objets utilisé*
	// au cour de l'appli
	//----------------------------------------------------------
	void Release()	;

	//----------------------------------------------------------
	// appel à la structure de config de la fenêtre
	//----------------------------------------------------------
	WindowSetup*	SetUpWindow();

	//----------------------------------------------------------
	// appel à la structure de config du renderer
	//----------------------------------------------------------
	RendererSetup*	SetUpRenderer();


	//==================================================================
	// Méthodes dédiées à l'héritage
	//==================================================================
	//----------------------------------------------------------
	// l'appel à la fonction d'initialisation de notre sous objet "appli spécifique"
	//----------------------------------------------------------
	virtual bool OnInit() = 0;

	//----------------------------------------------------------
	// l'appel à la fonction de  rendu de notre sous objet "appli spécifique"
	//----------------------------------------------------------
	virtual void OnRender() = 0;

	//----------------------------------------------------------
	// l'appel à la fonction de destruction de notre sous objet "appli spécifique"
	//----------------------------------------------------------
	virtual void OnRelease() = 0;

protected:


	//----------------------------------------------------------
	// Données membres
	//----------------------------------------------------------
	WindowSetup*	m_windowSetUp;	///< infos liées à la fenêtre (pointeur car paratagé par deux objets)
	UserEvents*		m_UserEvents;	///< infos liées à la souris/caméra (pointeur car paratagé par deux objets)
	GUI*			m_gui		;	///< gui est un pointeur car GUI est une classe mère abstraite (méthodes non définies) 
	Renderer		m_renderer	;	///< Déclaration du renderer 
	Camera*			m_camera	;	// la camera
};


// les fonctions inline permette une optimisation du code. Brievement, le corps des fonctions suivantes
// remplace les valeurs appelés dans le code par ces mêmes fonctions 
inline WindowSetup*		Application::SetUpWindow()		{ return m_windowSetUp;}
inline RendererSetup*	Application::SetUpRenderer()	{ return m_renderer.SetUpRenderer();}





#endif // __APPLICATION_H__ fin de la routine