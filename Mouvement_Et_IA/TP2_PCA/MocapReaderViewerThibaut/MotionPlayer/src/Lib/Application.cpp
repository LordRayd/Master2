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
// ainsi que des déclarations des fonctions OPENGL
//-----------------------------------------------------------------
#include "Application.h"

// utilisation dans ce fichier des fonctions de la stl soit nous disons au compilateur que nous travaillons avec le namespace 'std'
// nous évite donc d'appeler std:: pour chaque méthodes de la stl
using namespace std; 

/////////////////////////////////////////////////////////////
/// Initialise le GUI (Graphic user interface), le renderer et l'application enfant
///
/// \return code d'erreur
////////////////////////////////////////////////////////////
bool Application::Init()
{
	// infos liées à la souris/caméra (pointeur car paratagé par deux objets)
	m_UserEvents = new UserEvents				;

	// intialisation de notre objet gui 
	// gui est un pointeur car GUI est une classe mère abstraite (méthodes non définies) 
	m_gui = new GuiGlfw(m_windowSetUp, m_UserEvents);

	// construction de la caméra
	m_camera = new CameraLookAt();

	// Déclaration du renderer (le renderer va automatiquement appeler son contructeur)
	m_renderer.Create(m_windowSetUp, m_UserEvents, m_camera)	;

	// On intialise notre gui et notre renderer puis on vérifie leurs états de retour
	if( (!m_gui->Init()) || (!m_renderer.Init()) ) 
	{
		delete m_camera		;
		delete m_windowSetUp;
		delete m_UserEvents;
		delete m_gui		;		// on supprime la mémoire allouée pour l'objet
		return 0			;		// ternmine la fonction
	}

	// appel à la méthode de début enfant
	OnInit();

	return true;
}


/////////////////////////////////////////////////////////////
/// lance la boucle d'affichage, gère les évenements système et le rendu de la scène
///
////////////////////////////////////////////////////////////
void Application::Run()
{
	// on lance la boucle d'affichage et on la laisse tourner tant que l'utilisateur ne la termine pas 
	// dans quel cas GetRunning renvoit faux
	while( m_gui->GetRunning() )
	{
		// fonction lancé par le gui à chaque image (évenements liés au clavier, souris, etc ...)
		m_gui->StartRender();

		// on lance l'affichage de notre scène via l'objet renderer
		m_renderer.StartRender();

		// appel à la méthode de rendu enfant
		OnRender();

		// fonction de fin d'image géré par le gui (utilisation du double buffering)
		m_gui->EndRender();
	}

	// La boucle d'affichage est terminé, on lance le processus de fin d'application du GUI, nettoyage de la mémoire ...
	m_gui->Release();
}

/////////////////////////////////////////////////////////////
/// Fini l'application: détruit les objets plus utilisés
///
////////////////////////////////////////////////////////////
void Application::Release()
{
	// appel à la méthode de fin enfant
	OnRelease();

	// on supprime la mémoire allouée aux objets gui et mouse events
	delete m_camera		;
	delete m_windowSetUp;
	delete m_UserEvents;
	delete m_gui		;
}