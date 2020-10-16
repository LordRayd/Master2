#ifndef __SkeletonTunerApp_H__
#define __SkeletonTunerApp_H__

#include "Application.h"

// This is a compiler directive that includes libraries (For Visual Studio).
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

//#include "SceneController.h"
#include "SkeletonTuner.h"
//#include "RendererHelpers.h"

////////////////////////////////////////////////////////////////////////
/// classe DemoObjManipulator: utilise le renderer spécifique à la manipulation de maillage
////////////////////////////////////////////////////////////////////////
class SkeletonTunerApp: public Application
{
public:

	struct EditionBar
	{
		EditionBar() 
			:	m_openFile(false), m_bar(0),
				m_step(-0.1f), m_play(false), m_loadScene(0), m_saveScene(0), m_ground(true), m_axis(false), m_saveAni(false), m_scale(1.0f){}
		
		void Create();
		~EditionBar(){m_bar = 0;}
		void Remove(){if(m_bar) {TwDeleteBar(m_bar);}m_bar = 0;}

		TwBar*		m_bar;
		bool		m_openFile;
		bool		m_play, m_axis, m_ground, m_loadScene, m_saveScene, m_saveAni;
		float		m_step, m_scale;
	};

	struct SkeletonBar
	{
		SkeletonBar() 
			: m_bar(0),	m_jointIndex(0), m_axis(), m_currentAxis(), m_angle(0.0f), m_applyIn(0), m_applyOut(0), 
			m_save(0), m_dupliForChild(0), m_drawAllBasis(false), m_skeletonRadius(0.5f), m_basisScale(2.0f) {}

		void Create();
		~SkeletonBar(){m_bar = 0;}
		void Remove(){if(m_bar) {TwDeleteBar(m_bar);}m_bar = 0;}

		TwBar*			m_bar			;
		UInt			m_jointIndex	;

		enum			AxisType {AT_X, AT_Y, AT_Z};
		TwEnumVal		AxisBindType[3]	;
		AxisType		m_axis			;	///< axe selectionné
		AxisType		m_currentAxis	;	///< axe courant
		
		float			m_angle,			///< valeur de l'angle courant
						m_baseAngle		;	///< valeur de l'angle courant pour changer la base

		bool			m_applyIn,
						m_applyOut,			///< application de la valeur courante comme interval
						m_dupliForChild,	///< 
						m_save,
						m_load			;	///< save and laod skeleton tuner
		bool			m_drawAllBasis	;
		float			m_skeletonRadius,
						m_basisScale	;
	};

public:
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// constructeur de Demo
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SkeletonTunerApp():m_chain() {}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// destructeur de Demo
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~SkeletonTunerApp(){}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// initilisation de notre appli (rien car dessiner un cube à l'écran ne nécéssite aucune initialisation)
	// !! Méthode appelée dans la méthode Init de la classe Application
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool OnInit();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// notre scène à dessiner
	// !! Méthode appelée dans la méthode Run de la classe Application
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void OnRender();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// destructuion de notre appli (rien car dessiner un cube à l'écran ne nécéssite aucune destruction)
	// !! Méthode appelée dans la méthode Release de la classe Application
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void OnRelease(){/*Sc::ScTerminate();*/};


protected:

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Données membres
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static EditionBar		m_editionBar		;	///< menu
	static SkeletonBar		m_skeletonBar		;	///<

	std::string				m_rep				;	///< chemin d'acces
	std::string				m_name				;	///< nom du modèle
	std::string				m_extension			;	///< nom du modèle

	// data
	AnimatedChain			m_chain				;	///< chain animé à corriger
	SkeletonTuner			m_skeletonTuner		;	///< le systeme qui corrige l'animation

	std::vector<std::vector<float> >	m_dof				;	///< les degrés de liberté courants
	std::vector<std::vector<float> >	m_bases				;	///< les angles associés à chaque base


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// affiche une boite de dialogue pour trouver un fichier sur le disque dur
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void GetFile(std::string& rep, std::string& name, std::string& extension, bool open = true);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ouvre un fichier
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void OpenFile();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	AnimatedChain* CreateBAnimatedChain(const std::string& name, const std::string& motionName);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//void UpdateSceneController()
	//{
	//	unsigned int i;

	//	// update scene controller
	//	std::vector<unsigned int> mouseButtons(6);
	//	std::vector<Sc::Key> keyboardButtons(0);
	//	for(i=0; i<6; i++)
	//		mouseButtons[i] = m_userEvents->mousebuttons[i];
	//	for(i=0; i<KT_LAST; i++)
	//		if(m_userEvents->IsPressed(Key(i)))
	//			keyboardButtons.push_back(Sc::Key(i));

	//	Sc::ScUpdate(keyboardButtons, mouseButtons, m_userEvents->mousex, m_userEvents->mousey);
	//}
};


#endif //
