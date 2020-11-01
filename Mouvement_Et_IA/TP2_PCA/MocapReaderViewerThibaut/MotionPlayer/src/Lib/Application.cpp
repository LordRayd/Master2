//========================================================================
// TP Animation / Synthese image
//
//------------------------------------------------------------------------
// Copyright (c) 2010-2011 Equipe SAMSARA (T. LE NAOUR, N. COURTY, S. GIBET)
// VALORIA - UBS 
//========================================================================


//-----------------------------------------------------------------
// On inclu les fichiers .h qui contiennent les d�clarations des fonctions
// Les impl�mentations de nos fonctions ont besoin de leurs d�clarations 
// ainsi que des d�clarations des fonctions OPENGL
//-----------------------------------------------------------------
#include "Application.h"

// utilisation dans ce fichier des fonctions de la stl soit nous disons au compilateur que nous travaillons avec le namespace 'std'
// nous �vite donc d'appeler std:: pour chaque m�thodes de la stl
using namespace std; 

/////////////////////////////////////////////////////////////
/// Initialise le GUI (Graphic user interface), le renderer et l'application enfant
///
/// \return code d'erreur
////////////////////////////////////////////////////////////
bool Application::Init()
{
	// infos li�es � la souris/cam�ra (pointeur car paratag� par deux objets)
	m_UserEvents = new UserEvents				;

	// intialisation de notre objet gui 
	// gui est un pointeur car GUI est une classe m�re abstraite (m�thodes non d�finies) 
	m_gui = new GuiGlfw(m_windowSetUp, m_UserEvents);

	// construction de la cam�ra
	m_camera = new CameraLookAt();

	// D�claration du renderer (le renderer va automatiquement appeler son contructeur)
	m_renderer.Create(m_windowSetUp, m_UserEvents, m_camera)	;

	// On intialise notre gui et notre renderer puis on v�rifie leurs �tats de retour
	if( (!m_gui->Init()) || (!m_renderer.Init()) ) 
	{
		delete m_camera		;
		delete m_windowSetUp;
		delete m_UserEvents;
		delete m_gui		;		// on supprime la m�moire allou�e pour l'objet
		return 0			;		// ternmine la fonction
	}

	// appel � la m�thode de d�but enfant
	OnInit();

	return true;
}


/////////////////////////////////////////////////////////////
/// lance la boucle d'affichage, g�re les �venements syst�me et le rendu de la sc�ne
///
////////////////////////////////////////////////////////////
void Application::Run()
{
	// on lance la boucle d'affichage et on la laisse tourner tant que l'utilisateur ne la termine pas 
	// dans quel cas GetRunning renvoit faux
	while( m_gui->GetRunning() )
	{
		// fonction lanc� par le gui � chaque image (�venements li�s au clavier, souris, etc ...)
		m_gui->StartRender();

		// on lance l'affichage de notre sc�ne via l'objet renderer
		m_renderer.StartRender();

		// appel � la m�thode de rendu enfant
		OnRender();

		// fonction de fin d'image g�r� par le gui (utilisation du double buffering)
		m_gui->EndRender();
	}

	// La boucle d'affichage est termin�, on lance le processus de fin d'application du GUI, nettoyage de la m�moire ...
	m_gui->Release();
}

/////////////////////////////////////////////////////////////
/// Fini l'application: d�truit les objets plus utilis�s
///
////////////////////////////////////////////////////////////
void Application::Release()
{
	// appel � la m�thode de fin enfant
	OnRelease();

	// on supprime la m�moire allou�e aux objets gui et mouse events
	delete m_camera		;
	delete m_windowSetUp;
	delete m_UserEvents;
	delete m_gui		;
}