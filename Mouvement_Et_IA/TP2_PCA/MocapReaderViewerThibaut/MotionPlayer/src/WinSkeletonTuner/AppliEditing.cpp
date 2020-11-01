#include "AppliEditing.h"
#include "FontRenderer.h"

AppliEditing::MotionBar	AppliEditing::m_motionBar	;


bool GetFile(std::string& rep, std::string& name, std::string& extension, bool open = true)
{
#if defined _WIN32 || defined WIN32 || defined __NT__ || defined __WIN32__
	OPENFILENAME ofn;
    char szFileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
    ofn.hwndOwner = 0;
    //ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFilter = "All 3d files\0*.bvh;*.dump;*.data;\0";         
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
		
		return true;
    }
#endif
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AppliEditing::OpenFile()
{
	// get file with interface
	if(!GetFile(rep, name, extension)) return;

	if(!m_motionBar.m_multipleAnimation)
	{
		std::for_each(m_animatedChains.begin(), m_animatedChains.end(), STLDelete());
		m_animatedChains.clear();
	}

	m_animatedChains.push_back(new AnimatedChain(rep+name+"."+extension));


	//----------------------------------------------------------
	// m_motionBar initialisation
	//----------------------------------------------------------
	unsigned int i;
	vector<string> chains;
	for(i=0; i<m_animatedChains.size(); i++)
		chains.push_back(m_animatedChains[i]->m_name);

	m_modelsStr.clear();
	for(i=0; i<m_animatedChains.back()->m_animatedJoints.size(); i++)
		m_modelsStr.push_back(m_animatedChains.back()->m_animatedJoints[i].first->m_name);
	m_motionBar.Create(name, chains, m_modelsStr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Initialisation de notre appli (interfaces : Sc et AntTweakbar, chargement du fichier xml des sous mouvements
/// chargement d'une première animation et envoit du squelette à l'outil dédié aux graphes)
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AppliEditing::OnInit()
{
	vector<string> chains(0, "Load a bvh");
	m_modelsStr.clear();
	m_motionBar.Create("", chains, m_modelsStr);

	// int font controler
	Sc::ScFontRenderer::GetInstance();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// notre scène à dessiner: Mise à jour des donnée et affichage de la scène et tools
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AppliEditing::OnRender()
{
	if(m_motionBar.m_openFile)
	{
		OpenFile();
		m_motionBar.m_openFile = false;
	}

	//----------------------------------------------------------
	// update time step
	//----------------------------------------------------------
	if(m_motionBar.m_play) 
	{
		switch(m_motionBar.m_speedType)
		{
			case MotionBar::ST_01: if(m_stepCounter >= 9){m_motionBar.m_step++; m_stepCounter = 0;}else m_stepCounter++; break;
			case MotionBar::ST_02: if(m_stepCounter >= 4){m_motionBar.m_step++; m_stepCounter = 0;}else m_stepCounter++; break;
			case MotionBar::ST_05: if(m_stepCounter >= 2){m_motionBar.m_step++; m_stepCounter = 0;}else m_stepCounter++; break;
			case MotionBar::ST_1:  m_motionBar.m_step++; break;
			case MotionBar::ST_2:  m_motionBar.m_step+=2; break;
			case MotionBar::ST_5:  m_motionBar.m_step+=5; break;
		}
	}

	//----------------------------------------------------------
	// update motion bar
	//----------------------------------------------------------
	

	//----------------------------------------------------------
	// rotation de l'objet
	//----------------------------------------------------------
	Quaternion qUp, qRot( m_motionBar.m_rotation.w, Vec3(m_motionBar.m_rotation.x, m_motionBar.m_rotation.y, m_motionBar.m_rotation.z));
	if(m_motionBar.m_up == 1)
		qUp = Quaternion(Vec3(1.0f, 0.0f, 0.0f), 3.1415f/2.0f);
	else if(m_motionBar.m_up == 2)
		qUp = Quaternion(Vec3(1.0f, 0.0f, 0.0f), -3.1415f/2.0f);
	else if(m_motionBar.m_up == 2)
		qUp = Quaternion(Vec3(0.0f, 0.0f, 1.0f), 3.1415f/2.0f);
	else if(m_motionBar.m_up == 3)
		qUp = Quaternion(Vec3(0.0f, 0.0f, 1.0f), -3.1415f/2.0f);
	else if(m_motionBar.m_up == 4)
		qUp = Quaternion(Vec3(0.0f, 1.0f, 0.0f), 3.1415f/2.0f);
	else if(m_motionBar.m_up == 5)
		qUp = Quaternion(Vec3(0.0f, 1.0f, 0.0f), -3.1415f/2.0f);
	Quaternion q(qUp * qRot);

	//----------------------------------------------------------
	// compute motion
	//----------------------------------------------------------
	unsigned int i,j; 
	Vec3 p(0.0f);
	if(m_animatedChains.size() && (!(static_cast<unsigned int>(m_motionBar.m_step)<m_animatedChains[0]->m_animatedJoints[0].second.size())))
	{
		m_motionBar.m_step = 0;
		if(m_motionBar.m_playType == MotionBar::PT_Stop)
			m_motionBar.m_play = false;
	}

	for(j=0; j<m_animatedChains.size(); j++)
	{
		m_animatedChains[j]->ComputeMocap(static_cast<unsigned int>(m_motionBar.m_step));
		for(i=0;i< m_animatedChains[j]->m_animatedJoints.size(); i++)
		{
			p = m_motionBar.m_scale * m_animatedChains[j]->m_animatedJoints[i].first->m_translation; 
			p = q.Rotate(p);
			m_animatedChains[j]->m_animatedJoints[i].first->m_translation = p;
		}
		if(j == unsigned int(m_motionBar.m_chainType))
			m_renderer.DisplayAnimatedChain(m_animatedChains[j], m_motionBar.m_segments, Vector3(0.8f, 0.15f, 0.15f), m_motionBar.m_radius, m_motionBar.m_shadow);
		else
			m_renderer.DisplayAnimatedChain(m_animatedChains[j], m_motionBar.m_segments, Vector3(0.4f, 0.9f, 0.9f), m_motionBar.m_radius, m_motionBar.m_shadow );
	}

	if( (m_UserEvents->IsPressed(Key_d)||m_motionBar.m_extractPosRot) && m_animatedChains.size())
	{
		ExtractTransformations();
		m_motionBar.m_extractPosRot = false;
	}
		
		
	//----------------------------------------------------------
	// draw some tools
	//----------------------------------------------------------
	if(m_motionBar.m_ground)
		m_renderer.DrawGround(2500.0f);
		
	if(m_motionBar.m_axis)
		m_renderer.DrawAxis(20.0f);

	//----------------------------------------------------------
	// extract motion
	//----------------------------------------------------------
	if(m_motionBar.m_extract) 
	{
		Extract();
		m_motionBar.m_extract = false;
	}

	//----------------------------------------------------------
	// on met à jour le graphe avec les nouveau sommets (juste pour le premier squelette)
	//----------------------------------------------------------
	if(m_animatedChains.size())
	{
		unsigned int sIndex(m_motionBar.m_chainType);
		if(sIndex<m_animatedChains.size())
		{
			for(i=0;i< m_animatedChains[sIndex]->m_animatedJoints.size(); i++)
			{
				p =  m_animatedChains[sIndex]->m_animatedJoints[i].first->m_translation; 
				Sc::ScFontRenderer::GetInstance().AddStringToDisplay(Vec3(p.x+0.5f, p.y+0.5f, p.z+0.5f), Sc::ToString(i) + string(": ") + m_animatedChains[sIndex]->m_animatedJoints[i].first->m_name, 8, true);
			}
		}
	}

	// font manager 
	if(m_motionBar.m_displayJointNames)
		Sc::ScFontRenderer::GetInstance().Display();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sauvegarde des données et destructuion de notre appli 
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AppliEditing::OnRelease()
{
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Méthodes pour extraire une sous animation (clip)
/// 1. Dans un nouveau fichier bvh
/// 2. Avec les informations utilisateur dans un fichier xml
///////////////////////////////////////////////////////////////////////////////////////////////////////////


void GetChild(ChainJoint* father, ChainJoint* child, AnimatedChain* animatedChain, std::vector<unsigned int>& jointToExport, bool removeDouble)
{
	if(removeDouble && father && glm::length(child->m_BPtranslation) > 0.0001f)
		jointToExport.push_back(child->m_index);
	else if(!removeDouble || !father)
		jointToExport.push_back(child->m_index);

	unsigned int i;
	for(i=0; i<child->m_child.size(); i++)
		GetChild(child, child->m_child[i], animatedChain, jointToExport, removeDouble);
}

void AppliEditing::Extract()
{
	// first extract animation to bvh file
	std::string fileName = name + "_" + ToString(m_motionBar.m_fIn) + "." +  ToString(m_motionBar.m_fOut) + ".bvh";
	
	// get joints to export
	std::vector<unsigned int> jointToExport;
	ChainJoint* newRoot(m_animatedChains[0]->GetJoint(m_modelsStr[m_motionBar.m_modelType]));
	GetChild(0, newRoot, m_animatedChains[0], jointToExport, m_motionBar.m_removeDouble);

	unsigned int i;
	for(i=0; i<m_animatedChains.size();i++)
		if(i == unsigned int(m_motionBar.m_chainType))
			m_animatedChains[i]->ExportMotionToBvh(fileName, jointToExport, m_motionBar.m_fIn, m_motionBar.m_fOut);
}

void AppliEditing::ExtractTransformations()
{
	std::string myRep, myName, myExtension;
	GetFile(myRep, myName, myExtension, false);

	unsigned int i,j;
	// fichier translation
	{
		string		indexFileName(myRep+myName+ "_translationAbs."+myExtension);
		ofstream	indexFileStream;  
		indexFileStream.open(indexFileName.c_str(), std::ios_base::binary);

		cout << "extraction des translations, fichier: " << rep+name+ "_translationAbs." << myExtension << endl;
		
		indexFileStream << "# Frame time (time step)  en seconde" << endl;
		indexFileStream << m_animatedChains[0]->m_timeStep << endl;
		indexFileStream << endl;

		indexFileStream << "# une ligne -> une frame" << endl;
		indexFileStream << "# position x,y,z articulation 1, ..., position x,y,z articulation n " << endl;
		indexFileStream << endl;
		

		for(j=0; j<m_animatedChains[0]->m_animatedJoints[0].second.size(); j++)
		{
			m_animatedChains[0]->ComputeMocap(j);
			for(i=0; i<m_animatedChains[0]->m_animatedJoints.size(); i++)
			{
				Vector3 p = m_animatedChains[0]->m_animatedJoints[i].first->m_translation;
				indexFileStream << p.x << " " << p.y << " " << p.z << " ";
			}
			indexFileStream << endl;
		}
			
		indexFileStream.close();
	}

	// les fichiers translation
	for(i=0; i<m_animatedChains[0]->m_animatedJoints.size(); i++)
	{
		string		indexFileName(myRep+m_animatedChains[0]->m_animatedJoints[i].first->m_name+ "_translationAbs."+myExtension);
		ofstream	indexFileStream;  
		indexFileStream.open(indexFileName.c_str(), std::ios_base::binary);

		indexFileStream << "# Frame time (time step) en seconde" << endl;
		indexFileStream << m_animatedChains[0]->m_timeStep << endl;
		indexFileStream << endl;

		cout << "extraction des translations, fichier: " << rep+m_animatedChains[0]->m_animatedJoints[i].first->m_name+ "_translationAbs." << myExtension << endl;
		indexFileStream << "# une ligne -> position x,y,z pour une frame" << endl;
		indexFileStream << endl;

		for(j=0; j<m_animatedChains[0]->m_animatedJoints[0].second.size(); j++)
		{
			m_animatedChains[0]->ComputeMocap(j);
			{
				Vector3 p = m_animatedChains[0]->m_animatedJoints[i].first->m_translation;
				indexFileStream << p.x << " " << p.y << " " << p.z << " " << endl;
			}
		}
			
		indexFileStream.close();
	}

	// rotations
	for(i=0; i<m_animatedChains[0]->m_animatedJoints.size(); i++)
	{
		string		indexFileName(myRep+m_animatedChains[0]->m_animatedJoints[i].first->m_name+ "_quaternionRel."+myExtension);
		ofstream	indexFileStream;  
		indexFileStream.open(indexFileName.c_str(), std::ios_base::binary);

		indexFileStream << "# Frame time (time step)  en seconde" << endl;
		indexFileStream << m_animatedChains[0]->m_timeStep << endl;
		indexFileStream << endl;

		cout << "extraction des quaternions, fichier: " << rep+m_animatedChains[0]->m_animatedJoints[i].first->m_name+ "_quaternionRel."<< myExtension << endl;
		indexFileStream << "# une ligne -> quaternion x,y,z et w pour une frame" << endl;
		indexFileStream << endl;

		for(j=0; j<m_animatedChains[0]->m_animatedJoints[0].second.size(); j++)
		{
			{
				Quaternion q = m_animatedChains[0]->m_animatedJoints[i].second[j].m_rotation;
				indexFileStream << q.x << " " << q.y << " " << q.z << " " << q.w << " " << endl;
			}
		}
			
		indexFileStream.close();
	}

	for(i=0; i<m_animatedChains[0]->m_animatedJoints.size(); i++)
	{
		string		indexFileName(myRep+m_animatedChains[0]->m_animatedJoints[i].first->m_name+ "_eulerRel."+myExtension);
		ofstream	indexFileStream;  
		indexFileStream.open(indexFileName.c_str(), std::ios_base::binary);

		indexFileStream << "# Frame time (time step)  en seconde" << endl;
		indexFileStream << m_animatedChains[0]->m_timeStep << endl;
		indexFileStream << endl;

		cout << "extraction des angles d'Euler, fichier: " << rep+m_animatedChains[0]->m_animatedJoints[i].first->m_name+ "_eulerRel." << myExtension << endl;
		indexFileStream << "# une ligne -> angles Euler x,y,z pour une frame" << endl;
		indexFileStream << endl;

		for(j=0; j<m_animatedChains[0]->m_animatedJoints[0].second.size(); j++)
		{
			{
				Quaternion q = m_animatedChains[0]->m_animatedJoints[i].second[j].m_rotation;
				Vector3 p = glm::eularAngles(q);
				indexFileStream << p.x << " " << p.y << " " << p.z << " " << endl;
			}
		}
			
		indexFileStream.close();
	}
}
	