//========================================================================
// TP Animation / Synthese image
//
//------------------------------------------------------------------------
// Copyright (c) 2010-2011 Equipe SAMSARA (T. LE NAOUR, N. COURTY, S. GIBET)
// VALORIA - UBS 
//========================================================================


//=====================================================================
// classe APPLICATION, s'occupe d'encapsuler l'ensemble des fonction communes � une application
//=====================================================================
#ifndef __APPLICATION_H__	// routine qui sert � pas inclure 2 fois le m�me fichier
#define __APPLICATION_H__	//



//-----------------------------------------------------------------
// Notre programme va avoir besoin 
// des fonctions sp�cifiques � l'utilisateur (iterface graphique utilisateur GUI) et
// des fonctions n�c�ssaires au rendu d'un objet dans la scene
//
// On inclu les fichiers .h qui contiennent les d�clarations des fonctions
//-----------------------------------------------------------------
#include "Common.h"				// donn�es li�es � la souris
#include "GUI.h"				// pointeur GUI
#include "Renderer.h"			// objet Renderer


////////////////////////////////////////////////////////////////////////
/// classe Application: elle encapsule l'ensemble de l'application � savoir l'utilisation du GUI et du renderer
/// L'objectif �tant qu'elle sert de classe commune � ttes applications qui se serviront des fonctions suivantes
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
	// fin de l'appli, destruction et nettoyage des objets utilis�*
	// au cour de l'appli
	//----------------------------------------------------------
	void Release()	;

	//----------------------------------------------------------
	// appel � la structure de config de la fen�tre
	//----------------------------------------------------------
	WindowSetup*	SetUpWindow();

	//----------------------------------------------------------
	// appel � la structure de config du renderer
	//----------------------------------------------------------
	RendererSetup*	SetUpRenderer();


	//==================================================================
	// M�thodes d�di�es � l'h�ritage
	//==================================================================
	//----------------------------------------------------------
	// l'appel � la fonction d'initialisation de notre sous objet "appli sp�cifique"
	//----------------------------------------------------------
	virtual bool OnInit() = 0;

	//----------------------------------------------------------
	// l'appel � la fonction de  rendu de notre sous objet "appli sp�cifique"
	//----------------------------------------------------------
	virtual void OnRender() = 0;

	//----------------------------------------------------------
	// l'appel � la fonction de destruction de notre sous objet "appli sp�cifique"
	//----------------------------------------------------------
	virtual void OnRelease() = 0;

protected:


	//----------------------------------------------------------
	// Donn�es membres
	//----------------------------------------------------------
	WindowSetup*	m_windowSetUp;	///< infos li�es � la fen�tre (pointeur car paratag� par deux objets)
	UserEvents*		m_UserEvents;	///< infos li�es � la souris/cam�ra (pointeur car paratag� par deux objets)
	GUI*			m_gui		;	///< gui est un pointeur car GUI est une classe m�re abstraite (m�thodes non d�finies) 
	Renderer		m_renderer	;	///< D�claration du renderer 
	Camera*			m_camera	;	// la camera
};


// les fonctions inline permette une optimisation du code. Brievement, le corps des fonctions suivantes
// remplace les valeurs appel�s dans le code par ces m�mes fonctions 
inline WindowSetup*		Application::SetUpWindow()		{ return m_windowSetUp;}
inline RendererSetup*	Application::SetUpRenderer()	{ return m_renderer.SetUpRenderer();}





#endif // __APPLICATION_H__ fin de la routine