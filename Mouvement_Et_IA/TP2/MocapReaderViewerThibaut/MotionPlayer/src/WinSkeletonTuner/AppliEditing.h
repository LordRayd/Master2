#ifndef __MotionEditor_H__	// routine qui sert à pas inclure 2 fois le même fichier
#define __MotionEditor_H__	//

#include "Application.h"
// This is a compiler directive that includes libraries (For Visual Studio).
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

using namespace std;


// Plateforme Windows
#if defined _WIN32 || defined WIN32 || defined __NT__ || defined __WIN32__
#include <windows.h>
#  define PLATFORM_WIN32
#  define PLATFORM_NAME "Windows"
#endif

////////////////////////////////////////////////////////////////////////
/// classe DemoObjManipulator: utilise le renderer spécifique à la manipulation de maillage
////////////////////////////////////////////////////////////////////////
class AppliEditing: public Application
{
public:

	struct MotionBar
	{
		MotionBar()
			:m_bar(0), m_step(1), m_openFile(false), m_play(false), m_fIn(1), m_fOut(100), m_extract(false), m_ground(true), m_axis(true), m_multipleAnimation(false),
				m_currentChainName(""), m_extractPosRot(false), m_rotation(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),m_scale(1.0f), m_up(0), m_removeDouble(false),m_segments(1),
				m_displayJointNames(0), m_radius(0.5f), m_shadow(1), m_speedType(ST_1) {}
		
		
		void Create(const std::string& currentChain, const std::vector<std::string>& chainNames, const std::vector<std::string>& models)
		{
			m_currentChainName = currentChain;
			m_chainNames = chainNames;

			if(m_bar) TwDeleteBar(m_bar);

			m_bar = TwNewBar("GraphEditor");
			TwDefine(" GraphEditor label='Graph Editor' position='10 10' size='200 520' help='Use this bar to edit graph in the scene.' ");
			
			TwAddVarRW(m_bar, "Open file", TW_TYPE_BOOLCPP, &m_openFile, " help='open all file you want.' ");

			TwAddVarRW(m_bar, "Axis", TW_TYPE_BOOLCPP, &m_axis, "label='Axis' group='Scene' help='draw axis.' ");
			TwAddVarRW(m_bar, "Ground", TW_TYPE_BOOLCPP, &m_ground, "label='Ground' group='Scene' help='draw ground.' ");
			TwAddVarRW(m_bar , "Rotation", TW_TYPE_QUAT4F, &m_rotation, "group='Scene' label='Rotation de l objet' help='Change l orientation de l objet.' ");
			TwAddVarRW(m_bar , "Scale", TW_TYPE_FLOAT, &m_scale, "group='Scene' label='Zoom de l objet' min=0 max=10000 step=0.1 help='Change l orientation de l objet.' ");
			TwAddVarRW(m_bar , "Up-Axis", TW_TYPE_UINT32, &m_up, "group='Scene' label='Up-Axis' min=0 max=6 step=1 help='Change l orientation de l objet.' ");
			

			TwAddVarRW(m_bar, "play time", TW_TYPE_BOOLCPP, &m_play, " key='space' group='Time' help='Select a graph.' ");
			TwAddVarRW(m_bar, "step time", TW_TYPE_UINT32, &m_step, " group='Time'  min=0 max=10000 step=1 help='Select a graph.' ");
			m_playBindType[0].Value = PT_Stop;		m_playBindType[0].Label = "PlayAndStop";
			m_playBindType[1].Value = PT_Repeat;	m_playBindType[1].Label = "Repeat";
			TwType motionTypeDef = TwDefineEnum( "Play Mode", m_playBindType, 2 );
			TwAddVarRW(m_bar, "Play Mode", motionTypeDef, &m_playType,  " group='Time' help='Play and Stop or repeat animation.' ");
			
			m_speedBindType[0].Value = ST_01;		m_speedBindType[0].Label = "0.1x";
			m_speedBindType[1].Value = ST_02;		m_speedBindType[1].Label = "0.2x";
			m_speedBindType[2].Value = ST_05;		m_speedBindType[2].Label = "0.5x";
			m_speedBindType[3].Value = ST_1;		m_speedBindType[3].Label = "1.0x";
			m_speedBindType[4].Value = ST_2;		m_speedBindType[4].Label = "2.0x";
			m_speedBindType[5].Value = ST_5;		m_speedBindType[5].Label = "5.0x";
			TwType speedTypeDef = TwDefineEnum( "Speed", m_speedBindType, 6 );
			TwAddVarRW(m_bar, "Speed", speedTypeDef, &m_speedType,  " group='Time' help='Control the speed of the motion.' ");


			TwAddVarRW(m_bar, "Draw segments", TW_TYPE_BOOLCPP, &m_segments, "label='Draw segments' group='Rendering' help='draw axis.' ");
			TwAddVarRW(m_bar, "Display joint names", TW_TYPE_BOOLCPP, &m_displayJointNames, "label='Display joint names' group='Rendering' help='draw axis.' ");
			TwAddVarRW(m_bar, "Segment radius", TW_TYPE_FLOAT, &m_radius, "label='Segment radius' min=0 max=10000 step=0.1 group='Rendering' help='draw axis.' ");
			TwAddVarRW(m_bar, "Shadow", TW_TYPE_BOOLCPP, &m_shadow, "label='Shadow' group='Rendering' help='draw axis.' ");
			
			// animation en cours
			unsigned int i;
			for(i=0; i<chainNames.size(); i++) {ChainBindType[i].Value = TransformModel(i);	ChainBindType[i].Label = chainNames[i].c_str();}
			TwType typeDef1 = TwDefineEnum( "Current Chain", ChainBindType, chainNames.size() );
			TwAddVarRW(m_bar, "Current motion", typeDef1, &m_chainType,  " group='Extract Edition' help='Stop or change the editing mode.' ");
			

			//TwAddSeparator(m_bar, "Sep0", "");
			TwAddVarRW(m_bar, "Frame In", TW_TYPE_UINT32, &m_fIn, " group='Extract Edition'  min=0 max=10000 step=1 help='Set frame in of submotion.' ");
			TwAddVarRW(m_bar, "Frame Out", TW_TYPE_UINT32, &m_fOut, " group='Extract Edition'  min=0 max=10000 step=1 help='Set frame out of submotion.' ");
			for(i=0; i<models.size(); i++) {ModelBindType[i].Value = TransformModel(i);	ModelBindType[i].Label = models[i].c_str();}
			TwType typeDef = TwDefineEnum( "Editing type", ModelBindType, models.size() );
			TwAddVarRW(m_bar, "Joint root", typeDef, &m_modelType,  " group='Extract Edition' help='Stop or change the editing mode.' ");
			TwAddVarRW(m_bar, "Remove double", TW_TYPE_BOOLCPP, &m_removeDouble, " group='Extract Edition' help='Select a graph.' ");
			TwAddVarRW(m_bar, "Multi character", TW_TYPE_BOOLCPP, &m_multipleAnimation, " group='Time' help='Select a graph.' ");
			

			TwAddSeparator(m_bar, "Sep2", "group='Extract Edition'");
			TwAddVarRW(m_bar, "Extract", TW_TYPE_BOOLCPP, &m_extract, " group='Extract Edition' help='Extract submotion' ");
			TwAddVarRW(m_bar, "Extract Positions et rotations", TW_TYPE_BOOLCPP, &m_extractPosRot, " group='Extract Edition' help='Extract submotion' ");
		}
		~MotionBar(){m_bar = 0;}

		TwBar*			m_bar;

		bool			m_openFile;
		std::string		m_currentChainName	;
		std::vector<std::string>
						m_chainNames		;
		glm::vec4		m_rotation			;
		unsigned int	m_step, m_up,
						m_fIn, m_fOut		;	// frame in and out of motion
		bool			m_axis,m_ground, m_segments, 
						m_play,				// some tools to visualize scene
						m_extract, m_extractPosRot,
						m_removeDouble,
						m_multipleAnimation,
						m_displayJointNames,
						m_shadow;				
		float			m_scale, m_radius			;
		enum			PlayType { PT_Stop, PT_Repeat};
		PlayType		m_playType;   // scene rotation mode (off, clockwise, counter-clockwise)
		TwEnumVal		m_playBindType[2];

		enum			SpeedType { ST_01, ST_02, ST_05, ST_1, ST_2, ST_5};
		SpeedType		m_speedType;   // scene rotation mode (off, clockwise, counter-clockwise)
		TwEnumVal		m_speedBindType[2];

		enum			TransformModel { TM_M1, TM_M2, TM_M3, TM_M4, TM_M5, TM_M6, TM_M7, TM_M8, TM_M9, TM_M10, TM_M11, TM_M12, TM_M13, TM_M14, TM_M15, TM_M16, TM_M17, TM_M18, TM_M19, TM_M20, TM_M21, TM_M22, TM_M23, TM_M24, TM_M25, TM_M26, TM_M27, TM_M28, TM_M29, TM_M30,
										 TM_M31, TM_M32, TM_M33, TM_M34, TM_M35, TM_M36, TM_M37, TM_M38, TM_M39, TM_M40, TM_M41, TM_M42, TM_M43, TM_M44, TM_M45, TM_M46, TM_M47, TM_M48, TM_M49, TM_M50, TM_M51, TM_M52, TM_M53, TM_M54, TM_M55, TM_M56, TM_M57, TM_M58, TM_M59, TM_M60,
										 TM_M61, TM_M62, TM_M63, TM_M64, TM_M65, TM_M66, TM_M67, TM_M68, TM_M69, TM_M70, TM_M71, TM_M72, TM_M73, TM_M74, TM_M75, TM_M76, TM_M77, TM_M78, TM_M79, TM_M80, TM_M81, TM_M82, TM_M83, TM_M84, TM_M85, TM_M86, TM_M87, TM_M88, TM_M89, TM_M90,
										 TM_M91, TM_M92, TM_M93, TM_M94, TM_M95, TM_M96, TM_M97, TM_M98, TM_M99, TM_M100, TM_M4101, TM_M102, TM_M103, TM_M104, TM_M105, TM_M106, TM_M107, TM_M108, TM_M109, TM_M110, TM_M111, TM_M112, TM_M113, TM_M114, TM_M115, TM_M116, TM_M117, TM_M118, TM_M119, TM_M120
										};
		TransformModel  m_modelType;   
		TwEnumVal		ModelBindType[120];

		TwEnumVal		ChainBindType[120];
		TransformModel  m_chainType;   
		
	};


	//----------------------------------------------------------
	// constructeur de Demo
	//----------------------------------------------------------
	AppliEditing():m_stepCounter(0) {}

	//----------------------------------------------------------
	// destructeur de Demo
	//----------------------------------------------------------
	~AppliEditing(){}

	//----------------------------------------------------------
	// initilisation de notre appli (rien car dessiner un cube à l'écran ne nécéssite aucune initialisation)
	// !! Méthode appelée dans la méthode Init de la classe Application
	//----------------------------------------------------------
	bool OnInit();

	//----------------------------------------------------------
	// notre scène à dessiner
	// !! Méthode appelée dans la méthode Run de la classe Application
	//----------------------------------------------------------
	void OnRender();

	//----------------------------------------------------------
	// destructuion de notre appli (rien car dessiner un cube à l'écran ne nécéssite aucune destruction)
	// !! Méthode appelée dans la méthode Release de la classe Application
	//----------------------------------------------------------
	void OnRelease();

protected:

	std::vector<AnimatedChain*>		m_animatedChains		;
	static MotionBar				m_motionBar				;
	bool							m_value					;
	std::vector<std::string>		m_modelsStr				;
	std::string						rep, name, extension	;
	unsigned int					m_stepCounter			;

	void ExtractTransformations();
	void Extract();
	void OpenFile();
};



#endif // __MotionEditor_H__ fin de la routine