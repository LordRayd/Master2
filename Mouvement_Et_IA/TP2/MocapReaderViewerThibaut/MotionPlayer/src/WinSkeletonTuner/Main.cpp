//=====================================================================
// Programme principal 
//=====================================================================


#include "AppliEditing.h"
#include "SkeletonTunerApp.h"


////////////////////////////////////////////////////////////////////////
/// la fonction main est spéciale 
/// puisque la premiere lancée à l'execution automatiquement
///
/// return code de fin d'application
////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	// l'objet Application encapsule l'ensemble des fonctions nécéssaires à l'application
	Application* appli = new SkeletonTunerApp;

	//configurons notre appli
	appli->SetUpWindow()->SetSize(1200, 700);
	//appli->SetUpRenderer()->SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//appli->SetUpRenderer()->SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	appli->SetUpRenderer()->SetClearColor(112.0f/255.0f, 117.0f/255.0f, 130.0f/255.0f, 1.0f);
	//appli->SetUpRenderer()->SetClearColor(200.0f/255.0f, 200.0f/255.0f, 200.0f/255.0f, 1.0f);
	appli->SetUpRenderer()->m_drawGround = false;

	// On initialise l'appli: GUI et renderer
	appli->Init();

	// lancement de la boucle de rendu
	appli->Run();

	// destruction des divers objets utilisés durant l'appli
	appli->Release();

	// destruction de l'appli
	delete appli;

	// faut bien retourner qq chose
	return 0;
}