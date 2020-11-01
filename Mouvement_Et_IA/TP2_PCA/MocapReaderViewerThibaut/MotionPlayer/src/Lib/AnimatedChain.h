//========================================================================
// TP Animation / Synthese image
//
//------------------------------------------------------------------------
// Copyright (c) 2010-2011 Equipe SAMSARA (T. LE NAOUR, N. COURTY, S. GIBET)
// VALORIA - UBS 
//========================================================================


//=====================================================================
// classe Animated chain, s'occupe de gérer l'animation d'une chaine (un squelette par exemple)
//=====================================================================

#ifndef __MY_ANIMATED_CHAIN_H__	//
#define __MY_ANIMATED_CHAIN_H__	//

#include "Common.h"				// opengl



// #define M_PI        3.14159265358979323846

typedef Vector3			Translationf	;
typedef Quaternion		Rotation		;


struct ChainJoint
{
	//----------------------------------------------------------
	// constructeur de l'objet
	//----------------------------------------------------------
	ChainJoint(){}

	//----------------------------------------------------------
	// Destructeur de l'objet
	//----------------------------------------------------------
	~ChainJoint()
	{
		for(unsigned int cpt=0 ; cpt<m_child.size() ; cpt++)
			delete m_child[cpt];

		m_child.clear();
	}

	/** \brief Ajout d'un fils au groupe d'objets. */
	void addChild(ChainJoint* child)
	{
		m_child.push_back(child) ;
	}

	bool IsAChild(unsigned int index)
	{
		unsigned int i;
		for(i=0;i<m_child.size(); i++)
			if(m_child[i]->m_index == index)
				return true;
		return false;
	}

	virtual ChainJoint* Copy() const
	{
		ChainJoint* cpy = new ChainJoint();
		cpy->m_name = m_name;
		cpy->m_translation = m_translation;
		cpy->m_rotation = m_rotation;
		cpy->m_BPtranslation = m_BPtranslation;
		cpy->m_BProtation = m_BProtation;
		cpy->m_index = m_index;
		//cpy->m_track = m_track; // à ne surtout pas faire !!
		return cpy;
	}

	std::string					m_name			;
	Translationf				m_translation	;
	Rotation					m_rotation		;
	Translationf				m_BPtranslation	;
	Rotation					m_BProtation	;
	std::vector<ChainJoint*>	m_child			;
	unsigned int				m_index			;
};

#include <iostream>
#include <fstream>
struct AnimatedChain
{
	enum TransformationMode{TM_RELATIVE, TM_ABSOLUTE};
	enum RotationOrder{RO_TranslationFirst /*classic*/, RO_RotationFirst};

	struct Transformation
	{
		Quaternion		m_rotation		;
		Translationf		m_translation	;
	};
	typedef std::vector<Transformation>	Transformations	;	// mocap value

	//----------------------------------------------------------
	// constructeur de l'objet
	//----------------------------------------------------------
	AnimatedChain()
		:m_rootJoint(0) {}

	//----------------------------------------------------------
	// constructeur de l'objet: construit un squelette à partir d'un fichier bvh
	//----------------------------------------------------------
	AnimatedChain(const std::string& file);

	//----------------------------------------------------------
	// Destructeur de l'objet
	//----------------------------------------------------------
	~AnimatedChain(){delete m_rootJoint; m_rootJoint = 0; m_animatedJoints.clear();}

	//----------------------------------------------------------
	// 
	//----------------------------------------------------------
	inline AnimatedChain & operator=(const AnimatedChain & skeleton)
	{
		return Copy(skeleton);
	}

	//----------------------------------------------------------
	// 
	//----------------------------------------------------------
	AnimatedChain& Copy(const AnimatedChain & skeleleton);


	//----------------------------------------------------------
	// export en bvh
	// joint index = root
	//----------------------------------------------------------
	void ExportHierarchyToBvh(std::ofstream& outFile, ChainJoint* joint, unsigned int depth, std::vector<unsigned int>& jointExported);
	void ExportMotionToBvh(const std::string& filename, unsigned int in, unsigned int out, unsigned int jointIndex = 0);
	void ExportHierarchyToBvh(std::ofstream& outFile, ChainJoint* joint, unsigned int depth, const std::vector<unsigned int>& jointToExport, std::vector<unsigned int>& jointExported);
	void ExportMotionToBvh(const std::string& filename, const std::vector<unsigned int>& jointToExport, unsigned int in, unsigned int out);
	unsigned int GetBestRoot(unsigned int currentRoot, const std::vector<unsigned int>& jointToExport);

	//----------------------------------------------------------
	// Calcul l'animation en fonction d'un pas de temps
	//----------------------------------------------------------
	void ComputeMocap(int timeStep);

	//----------------------------------------------------------
	// Ajoute un nouveau joint compris dans l'arborescence dans la liste de joint
	//----------------------------------------------------------
	void AddJoint(ChainJoint* joint)
	{
		m_animatedJoints.push_back(std::pair<ChainJoint*, Transformations>(joint, Transformations(0)));
	}

	ChainJoint* GetJoint(const std::string& name)
	{
		unsigned int i;
		for(i=0;i<m_animatedJoints.size();i++)
			if(m_animatedJoints[i].first->m_name == name)
				return m_animatedJoints[i].first;
		return 0;
	}

	ChainJoint* GetFather(ChainJoint* child, unsigned int& index)
	{
		unsigned int i,j;
		for(i=0;i<m_animatedJoints.size();i++)
		{
			for(j=0;j<m_animatedJoints[i].first->m_child.size();j++)
			{
				if(m_animatedJoints[i].first->m_child[j]->m_name ==  child->m_name)
				{
					index = i;
					return m_animatedJoints[i].first;
				}
			}
		}
		return 0;
	}
	

	enum AXIS
	{
		AXIS_X,
		AXIS_Y,
		AXIS_Z
	};


	//----------------------------------------------------------
	// Translate animation
	//----------------------------------------------------------
	void Translate(const Translationf& t) 
	{
		unsigned int i;
		for(i=0; i<m_animatedJoints[0].second.size(); i++)
			m_animatedJoints[0].second[i].m_translation += t;
	}

	//----------------------------------------------------------
	// Scale animation
	//----------------------------------------------------------
	void Scale(float s) 
	{
		unsigned int i,j;
		for(i=0; i<m_animatedJoints.size(); i++)
		{
			m_animatedJoints[i].first->m_BPtranslation *= s;
			for(j=0; j<m_animatedJoints[i].second.size(); j++)
				m_animatedJoints[i].second[j].m_translation *= s;
		}
	}

	//----------------------------------------------------------
	// enlève les données d'animation
	//----------------------------------------------------------
	void ResetAnimation() 
	{
		unsigned int i;
		for(i=0; i<m_animatedJoints.size(); i++)
			m_animatedJoints[i].second.clear();
	}

	//----------------------------------------------------------
	// Ajoute une frame
	//----------------------------------------------------------
	void AddFrame(const Transformations& transformation) 
	{
		if(transformation.size() <m_animatedJoints.size())
		{
			std::cout << "error dans AddFrame" << std::endl;
			return;
		}

		unsigned int i;
		for(i=0; i<m_animatedJoints.size(); i++)
			m_animatedJoints[i].second.push_back(transformation[i]);
	}


	//----------------------------------------------------------
	// calcule un nouveau squelette avec les transformations en relative
	//----------------------------------------------------------
	void ToRelativeMode();

	//----------------------------------------------------------
	// calcule un nouveau squelette avec les transformations en absolut
	//----------------------------------------------------------
	void ToAbsoluteMode();


	//----------------------------------------------------------
	// set bp values to current values and <->
	//----------------------------------------------------------
	void SetBPToCurrentValues();
	void SetCurrentToBPValues();
	
	ChainJoint* GetFather(ChainJoint* child) const
	{
		unsigned int i,j;
		for(i=0;i<m_animatedJoints.size(); i++)
			for(j=0;j<m_animatedJoints[i].first->m_child.size(); j++)
				if(m_animatedJoints[i].first->m_child[j]->m_name == child->m_name)
					return m_animatedJoints[i].first;
		return 0;
	}

	int GetJointIndex(const std::string& name) const;

	//----------------------------------------------------------
	// data member
	//----------------------------------------------------------
	TransformationMode										m_transformationMode;
	RotationOrder											m_rotationOrder		;
	
	std::vector<std::pair<ChainJoint*, Transformations> >	m_animatedJoints	;	// utilisé pour la mocap
	ChainJoint*												m_rootJoint			;	// (hierarchie) utilisé pour le rendu
	double													m_timeStep			;
	std::string												m_name				;
}; 
typedef AnimatedChain Skeleton;

extern std::ostream& operator <<(std::ostream& Stream, const Skeleton& graph);
extern std::istream& operator >>(std::istream& Stream, Skeleton& graph);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SkeletonIO
{
	static void Read(const std::string& fileName, Skeleton& skeleton)
	{
		std::ifstream ifstream(fileName.c_str(), std::ios::in);
		ifstream >> skeleton;
		ifstream.close();
	}
};


#endif // __GRAPHIC_CHAIN_H__ 