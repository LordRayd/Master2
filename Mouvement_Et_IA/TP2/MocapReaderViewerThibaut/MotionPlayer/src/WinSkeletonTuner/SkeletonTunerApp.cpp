#include "SkeletonTunerApp.h"
// Plateforme Windows
#if defined _WIN32 || defined WIN32 || defined __NT__ || defined __WIN32__
#include <windows.h>
#  define PLATFORM_WIN32
#  define PLATFORM_NAME "Windows"
#endif


using namespace std;


SkeletonTunerApp::EditionBar	SkeletonTunerApp::m_editionBar	;
SkeletonTunerApp::SkeletonBar	SkeletonTunerApp::m_skeletonBar	;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SkeletonTunerApp::EditionBar::Create()
{
	m_bar = TwNewBar("EditionBar");
	std::string define = " EditionBar label='Edition Menu' position='10 10' size='200 260' help='EditionBar'";;
	TwDefine(define.c_str());
	
	TwAddVarRW(m_bar, "Open file", TW_TYPE_BOOLCPP, &m_openFile, " help='open all file you want.' ");
	
	TwAddVarRW(m_bar, "Axis", TW_TYPE_BOOLCPP, &m_axis, "label='Axis' group='Scene' help='draw axis.' ");
	TwAddVarRW(m_bar, "Ground", TW_TYPE_BOOLCPP, &m_ground, "label='Ground' group='Scene' help='draw ground.' ");
	
	TwAddVarRW(m_bar, "play time", TW_TYPE_BOOLCPP, &m_play, " key='space' group='Time' help='Select a graph.' ");
	TwAddVarRW(m_bar, "step time", TW_TYPE_FLOAT, &m_step, " group='Time'  min=-0.01 max=10000.01 step=0.05 help='Select a graph.' ");

	TwAddVarRW(m_bar, "load camera", TW_TYPE_BOOLCPP, &m_loadScene, " group='Camera' help='save screen.' ");
	TwAddVarRW(m_bar, "save camera", TW_TYPE_BOOLCPP, &m_saveScene, " group='Camera'  help='save video.' ");
	//TwAddVarRW(m_bar, "save screen", TW_TYPE_BOOLCPP, &Application::SaveImg::m_takeScreen, " group='Save' help='save screen.' ");
	//TwAddVarRW(m_bar, "save video", TW_TYPE_BOOLCPP, &Application::SaveImg::m_takeVideo, " group='Save'  help='save video.' ");
	TwAddVarRW(m_bar, "save Ani", TW_TYPE_BOOLCPP, &m_saveAni, " group='Save'  help='save video.' ");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SkeletonTunerApp::SkeletonBar::Create()
{
	m_bar = TwNewBar("SkeletonBar");
	std::string define = " SkeletonBar label='Skeleton Menu' position='10 280' size='200 290' help='EditionBar'";;
	TwDefine(define.c_str());
	

	// select a joint
	TwAddVarRW(m_bar, "Joint index", TW_TYPE_UINT32, &m_jointIndex, "group='Joint Selection' help='open all file you want.' ");
	AxisBindType[0].Value = AT_X;	AxisBindType[0].Label = "X";
	AxisBindType[1].Value = AT_Y;	AxisBindType[1].Label = "Y";
	AxisBindType[2].Value = AT_Z;	AxisBindType[2].Label = "Z";
	TwType typeDef = TwDefineEnum( "Axis type", AxisBindType, 3 );
	TwAddVarRW(m_bar, "Axis", typeDef, &m_axis,"group='Joint Selection' help='Stop or change the editing mode.' ");

	// set up base axis
	TwAddVarRW(m_bar, "Duplicate for childs", TW_TYPE_BOOLCPP, &m_dupliForChild, "group='Joint base' help='Select a graph.' ");
	TwAddVarRW(m_bar, "Base angle", TW_TYPE_FLOAT, &m_baseAngle, "min=-3.145 max=+3.145 step=0.05 group='Joint base' help='Select a graph.' ");

	// set up interval of dof
	TwAddVarRW(m_bar, "Current angle", TW_TYPE_FLOAT, &m_angle, "min=-3.145 max=+3.145 step=0.05 group='Joint DOF' help='Select a graph.' ");
	TwAddVarRW(m_bar, "Apply In interval", TW_TYPE_BOOLCPP, &m_applyIn, " group='Joint DOF'  help='Apply In interval' ");
	TwAddVarRW(m_bar, "Apply Out interval", TW_TYPE_BOOLCPP, &m_applyOut, " group='Joint DOF'  help='Apply Out interval' ");
	
	TwAddVarRW(m_bar, "Save config", TW_TYPE_BOOLCPP, &m_save, " group='Save Config'  help='Save dof' ");
	TwAddVarRW(m_bar, "Load config", TW_TYPE_BOOLCPP, &m_load, " group='Save Config'  help='Save dof' ");

	TwAddVarRW(m_bar, "Skeleton radius", TW_TYPE_FLOAT, &m_skeletonRadius, "min=0.1 max=10.0 step=0.1 group='Draw options' help='Select a graph.' ");
	TwAddVarRW(m_bar, "Axis scale", TW_TYPE_FLOAT, &m_basisScale, "min=0.1 max=10.0 step=0.1 group='Draw options' help='Select a graph.' ");
	TwAddVarRW(m_bar, "Draw all axis", TW_TYPE_BOOLCPP, &m_drawAllBasis, " group='Draw options'  help='Apply In interval' ");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SkeletonTunerApp::GetFile(std::string& rep, std::string& name, std::string& extension, bool open)
{
#if defined _WIN32 || defined WIN32 || defined __NT__ || defined __WIN32__
	OPENFILENAME ofn;
    char szFileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
    ofn.hwndOwner = 0;
    //ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFilter = "All 3d files\0*.obj;*.ply;*.off;*.bModel;*.bvh;*.bsk;*.graph;*.sk\0";         
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    if(open)
	{
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrTitle = TEXT("Open File");
	}
	else
	{
		 ofn.Flags = OFN_HIDEREADONLY;
		 ofn.lpstrTitle = TEXT("Save File As");
	}
    ofn.lpstrDefExt = "txt";

    if(GetOpenFileName(&ofn))
    {
		std::string fileName(szFileName);
		rep = File::GetFilePath(fileName);
		name = File::GetNameWithoutExtension(fileName);
		extension = File::GetExtension(fileName);
    }
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//AnimatedChain* SkeletonTunerApp::CreateBAnimatedChain(const std::string& name, const std::string& motionName)
//{
//	if(!File::Exists(name)) return 0;
//	
//	AnimatedChain* chain = new AnimatedChain(name);
//
//	// 1. skeleton
//	BSkeletonLoader::Load(name, chain->m_skeleton);
//	//m_chain->m_skeleton.m_transformationMode = Skeleton::TM_ABSOLUTE;
//
//	//2. animation
//	if(File::Exists(motionName))
//	{
//		chain->SetKeyFramer(Application::LoadMotion(motionName));
//		chain->Compute(0.0f);
//	}
//	
//	return chain;
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SkeletonTunerApp::OpenFile()
{
	//Sc::ScReset();

	// get file with interface
	GetFile(m_rep, m_name, m_extension);

	if( (m_name == string("")) || (m_name == string("")) || (m_extension == string(""))) return;
	
	// skeleton
	if(m_extension == "sk" && File::Exists(m_rep + m_name + ".sk"))
	{
		m_chain.m_name = File::GetNameWithoutExtension(m_name);
		SkeletonIO::Read(m_rep + m_name + "." + m_extension, m_chain);

		cout << m_chain.m_animatedJoints.size() << endl;

		// petit test
		m_chain.ToRelativeMode();
		m_chain.SetCurrentToBPValues();
		m_chain.ToAbsoluteMode();

		m_skeletonTuner = SkeletonTuner(m_chain);

		m_dof = vector<vector<float> >(m_chain.m_animatedJoints.size(), vector<float>(3, 0.0f)); 
		m_bases = vector<vector<float> >(m_chain.m_animatedJoints.size(), vector<float>(3, 0.0f)); 
	}

	// binary skeleton (+ binary animation: raw)
	//if(m_extension == "bsk" && File::Exists(m_rep + "\\" + m_name + ".raw"))
	//	m_chain = CreateBAnimatedChain(m_rep + "\\" + m_name + "." + m_extension, m_rep + "\\" + m_name + ".raw"); 

	// bvh
	//if(m_extension == "bvh")
	//	m_chain = Application::LoadBVH(m_rep + "\\" + m_name + "." + m_extension); 

	//
	//Application::SaveImg::m_takeName = m_rep+"\\"+m_name;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SkeletonTunerApp::OnInit()
{
	// light 
	//Vec4 pLight  = Renderer::GetInstance().GetLight(0)->GetParameter(Light::LP_POSITION);
	//ToolsManager::GetInstance().AddTool("lightTool", Vec3(pLight.x, pLight.y, pLight.z), 10.0f);

	//----------------------------------------------------------
	// scene controller initialisation
	//----------------------------------------------------------
	//Sc::ScInit();
	//Sc::ScSetMeshPointRadius(5.0f);
	//Sc::ScSetOptimiseMode(true); 
		
	//----------------------------------------------------------
	// m_motionBar initialisation
	//----------------------------------------------------------
	m_editionBar.Create();
	m_skeletonBar.Create();


	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SkeletonTunerApp::OnRender()
{
	//----------------------------------------------------------
	// Load light
	//----------------------------------------------------------
	//ToolsManager::GetInstance().Update();
	//Vec3 pLignt(ToolsManager::GetInstance().GetTool("lightTool")->m_positon);
	//Renderer::GetInstance().GetLight(0)->SetParameter(Light::LP_POSITION, Vec4(pLignt.x, pLignt.y, pLignt.z, 1.00f));
	
	//----------------------------------------------------------
	// Load geometry and load it to scene controller cylinder3d2//cylinder13d4_1
	//----------------------------------------------------------
	if(m_editionBar.m_openFile) {OpenFile(); m_editionBar.m_openFile=false;}
	

	//----------------------------------------------------------
	// time & motion
	//----------------------------------------------------------
	//if( m_editionBar.m_play) m_editionBar.m_step+=1.0f;//0.5f;
	//if( m_chain && m_chain->GetKeyFramer() && !(m_editionBar.m_step < m_chain->GetKeyFramer()->GetNumKeys()) ) m_editionBar.m_step = 0;
	//if( m_chain && m_chain->GetKeyFramer() && m_editionBar.m_step<0.0f )	{m_chain->Compute();}
	//if( m_chain && m_chain->GetKeyFramer() && m_editionBar.m_step>=0.0f )	{m_chain->m_skeleton.m_transformationMode = Skeleton::TM_RELATIVE; m_chain->ComputeFromStep(m_editionBar.m_step);}
	
	//if(m_chain)
	//{
	//	RendererHelpers::DrawSkeleton(m_renderer, m_chain->m_skeleton, RendererHelpers::SDS_Classic, Vec3(0.2f, 1.0f, 0.2f), m_skeletonBar.m_skeletonRadius);
	//	UInt i;
	//	for(i=0; i<m_chain->m_skeleton.m_joints.size(); i++)
	//	{
	//		GlTranslatef(m_chain->m_skeleton.m_joints[i]->m_transformation.m_position);
	//		//Renderer::GetInstance().DrawCoordinateSystem(glm::toMat4(orientations[m_skeletonBar.m_jointIndex].second), 2.0f);
	//		Renderer::GetInstance().DrawAxis(m_skeletonBar.m_basisScale, glm::toMat4(m_chain->m_skeleton.m_joints[i]->m_transformation.m_rotation), m_skeletonBar.m_basisScale / 60.0f);
	//		GlTranslatef(-1.0f*m_chain->m_skeleton.m_joints[i]->m_transformation.m_position);
	//	}
	//}

	//----------------------------------------------------------
	// Draw mesh and it tools
	//----------------------------------------------------------
	//
	//if(m_chain) 
	{
		m_chain.ToRelativeMode();
		m_chain.ToAbsoluteMode();
		//RendererHelpers::DrawSkeleton(m_renderer, m_chain, RendererHelpers::SDS_Classic, Vec3(0.2f, 1.0f, 0.2f), m_skeletonBar.m_skeletonRadius);
		m_renderer.DisplayAnimatedChain(&m_chain, 1, Vector3(0.4f, 0.9f, 0.9f), 0.3f, 1 );
	}
	//RendererHelpers::DrawSkeleton(m_renderer, m_skeletonTuner.GetSkeleton(), RendererHelpers::SDS_Classic, Vec3(0.2f, 1.0f, 0.2f), 0.4f);

	//----------------------------------------------------------
	// Draw joint orientations
	//----------------------------------------------------------
	std::vector<std::pair<Vec3, Quaternion> > orientations;
	m_skeletonTuner.GetAbsoluteOrientations(orientations);
	//UInt i; for(i=0; i<orientations.size(); i++) 
	//{
	//	GlTranslatef(orientations[i].first);
	//	Renderer::GetInstance().DrawCoordinateSystem(glm::toMat4(orientations[i].second), 2.0f);
	//	GlTranslatef(-1.0f*orientations[i].first);
	//}
	
	if(m_skeletonBar.m_jointIndex<orientations.size())
	{
		GlTranslatef(orientations[m_skeletonBar.m_jointIndex].first);
		//Renderer::GetInstance().DrawCoordinateSystem(glm::toMat4(orientations[m_skeletonBar.m_jointIndex].second), 2.0f);
		//Renderer::GetInstance().DrawAxis(m_skeletonBar.m_basisScale, glm::toMat4(orientations[m_skeletonBar.m_jointIndex].second), m_skeletonBar.m_basisScale/60.0f);
		Renderer::DrawAxis(m_skeletonBar.m_basisScale, glm::toMat4(orientations[m_skeletonBar.m_jointIndex].second), m_skeletonBar.m_basisScale/60.0f);
		GlTranslatef(-1.0f*orientations[m_skeletonBar.m_jointIndex].first);
	}

	if(m_skeletonBar.m_drawAllBasis)
	{
		UInt i;
		for(i=0; i<orientations.size(); i++)
		{
			GlTranslatef(orientations[i].first);
			//Renderer::GetInstance().DrawCoordinateSystem(glm::toMat4(orientations[m_skeletonBar.m_jointIndex].second), 2.0f);
			Renderer::DrawAxis(m_skeletonBar.m_basisScale, glm::toMat4(orientations[i].second), m_skeletonBar.m_basisScale / 60.0f);
			GlTranslatef(-1.0f*orientations[i].first);
		}
	}

	//----------------------------------------------------------
	// Update skeleton tuner
	//----------------------------------------------------------
	SkeletonTuner::TAxis axis;
	if(m_skeletonBar.m_jointIndex<m_dof.size())
	{
		switch(m_skeletonBar.m_axis)
		{
			case SkeletonBar::AT_X: {axis = SkeletonTuner::TA_X; m_dof[m_skeletonBar.m_jointIndex][0] = m_skeletonBar.m_angle; m_bases[m_skeletonBar.m_jointIndex][0] = m_skeletonBar.m_baseAngle;} break;
			case SkeletonBar::AT_Y: {axis = SkeletonTuner::TA_Y; m_dof[m_skeletonBar.m_jointIndex][1] = m_skeletonBar.m_angle; m_bases[m_skeletonBar.m_jointIndex][1] = m_skeletonBar.m_baseAngle;} break;
			case SkeletonBar::AT_Z: {axis = SkeletonTuner::TA_Z; m_dof[m_skeletonBar.m_jointIndex][2] = m_skeletonBar.m_angle; m_bases[m_skeletonBar.m_jointIndex][2] = m_skeletonBar.m_baseAngle;} break;
		}
	}
	
	//if(m_skeletonBar.m_setToIdentity)
	//{	
	//	m_skeletonBar.m_setToIdentity = false;
	//}

	m_skeletonTuner.ApplyAngles(m_dof);
	m_skeletonTuner.ChangeBaseAxis(m_bases);

	// duplicate basis for children
	if(m_skeletonBar.m_dupliForChild)
	{
		m_skeletonTuner.DuplicateConflictedOrientations();
		m_dof = vector<vector<float> >(m_skeletonTuner.GetRefSkeleton().m_animatedJoints.size(), vector<float>(3, 0.0f)); 
		m_bases = vector<vector<float> >(m_skeletonTuner.GetRefSkeleton().m_animatedJoints.size(), vector<float>(3, 0.0f)); 
		m_skeletonBar.m_dupliForChild = false;
	}
		
	// set dof
	if(m_skeletonBar.m_applyIn)
	{
		m_skeletonTuner.SetDOF(m_skeletonBar.m_jointIndex, axis, SkeletonTuner::TI_In, m_dof[m_skeletonBar.m_jointIndex][static_cast<UInt>(m_skeletonBar.m_axis)]);
		m_skeletonBar.m_applyIn = false;
	}
	if(m_skeletonBar.m_applyOut)
	{
		m_skeletonTuner.SetDOF(m_skeletonBar.m_jointIndex, axis, SkeletonTuner::TI_Out, m_dof[m_skeletonBar.m_jointIndex][static_cast<UInt>(m_skeletonBar.m_axis)]);
		m_skeletonBar.m_applyOut = false;
	}

	// save
	if(m_skeletonBar.m_save)
	{
		m_skeletonTuner.Writte(m_rep + m_name + ".skT");
		m_skeletonBar.m_save = false;
	}

	// load
	if(m_skeletonBar.m_load)
	{
		m_skeletonTuner.Read(m_rep + m_name + ".skT");
		m_dof = vector<vector<float> >(m_skeletonTuner.GetRefSkeleton().m_animatedJoints.size(), vector<float>(3, 0.0f)); 
		m_bases = vector<vector<float> >(m_skeletonTuner.GetRefSkeleton().m_animatedJoints.size(), vector<float>(3, 0.0f)); 
		m_skeletonBar.m_load = false;
		m_skeletonBar.m_baseAngle = 0.0f;
	}

	if(m_skeletonBar.m_jointIndex<m_skeletonTuner.GetSkeleton().m_animatedJoints.size())
	{
		ChainJoint* j(m_skeletonTuner.GetSkeleton().m_animatedJoints[m_skeletonBar.m_jointIndex].first);
		ChainJoint* jf(m_skeletonTuner.GetSkeleton().GetFather(j));
		std::string str;
		if(jf){	str +="Joint " +j->m_name+ " (father: " + jf->m_name + ") intervals:  "; str+= "\n";}
		else  { str +="Joint " +j->m_name+ " intervals:  "; str+= "\n";}
		str +="X: ["; str+= ToString(m_skeletonTuner.GetDOF(m_skeletonBar.m_jointIndex, SkeletonTuner::TA_X, SkeletonTuner::TI_In));  str+= ", "; str+= ToString(m_skeletonTuner.GetDOF(m_skeletonBar.m_jointIndex, SkeletonTuner::TA_X, SkeletonTuner::TI_Out)); str+= "]"; str+= "\n";
		str +="Y: ["; str+= ToString(m_skeletonTuner.GetDOF(m_skeletonBar.m_jointIndex, SkeletonTuner::TA_Y, SkeletonTuner::TI_In));  str+= ", "; str+= ToString(m_skeletonTuner.GetDOF(m_skeletonBar.m_jointIndex, SkeletonTuner::TA_Y, SkeletonTuner::TI_Out)); str+= "]"; str+= "\n";
		str +="Z: ["; str+= ToString(m_skeletonTuner.GetDOF(m_skeletonBar.m_jointIndex, SkeletonTuner::TA_Z, SkeletonTuner::TI_In));  str+= ", "; str+= ToString(m_skeletonTuner.GetDOF(m_skeletonBar.m_jointIndex, SkeletonTuner::TA_Z, SkeletonTuner::TI_Out)); str+= "]"; str+= "\n";
		//Sc::ScDisplayTextY(-100, str, 10);
	}



	//----------------------------------------------------------
	// Draw modificated skeleton
	//----------------------------------------------------------
	//RendererHelpers::DrawSkeleton(m_renderer, m_skeletonTuner.GetSkeleton(), RendererHelpers::SDS_Classic, Vec3(0.9f, 0.9f, 0.1f),  m_skeletonBar.m_skeletonRadius);
	m_renderer.DisplayAnimatedChain(&m_chain, 1, Vector3(0.4f, 0.9f, 0.9f), 0.3f, 1 );

	//----------------------------------------------------------
	// afficher le sol et axes de la scene
	//----------------------------------------------------------
	if(m_editionBar.m_ground)	m_renderer.DrawGround(500.0f);
	if(m_editionBar.m_axis)		m_renderer.DrawAxis(20.0f);

	//=================================================
	// load & save camera
	//=================================================
	//if(m_editionBar.m_loadScene){Application::LoadCamera(m_rep+m_name+".cam"); Application::LoadLight(m_rep+m_name+".light"); ToolsManager::GetInstance().GetTool("lightTool")->m_positon = ToVec3(Renderer::GetInstance().GetLight(0)->GetParameter(Light::LP_POSITION));m_editionBar.m_loadScene = false;}
	//if(m_editionBar.m_saveScene){Application::SaveCamera(m_rep+m_name+".cam"); Application::SaveLight(m_rep+m_name+".light"); m_editionBar.m_saveScene = false;}

	// save ani
	if(m_editionBar.m_saveAni)
	{
	//	m_editionBar.m_saveAni = false;
	}	

	//----------------------------------------------------------
	// UpdateSceneController
	//----------------------------------------------------------
	//UpdateSceneController();
	
};
