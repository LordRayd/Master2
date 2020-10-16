#include "SkeletonTuner.h"
#include "MathIntersections.h"

using namespace std;

inline Vec4 Vec3ToVec4(const Vec3& v)
{
	return Vec4(v.x, v.y, v.z, 0.0f);
}

////////////////////////////////////////////////////////////
/// Constructeur à partir d'un modèle cube
///
/// \param le squelette à paramétrer
///
////////////////////////////////////////////////////////////
SkeletonTuner::SkeletonTuner(const Skeleton& skeleton)
{
	UInt i,j;

	//----------------------------------------
	// Skeleton
	//----------------------------------------
	// copy the skeleton
	m_firstSkeleton = skeleton;
	
	// set orientation to identity
	for(i=0; i<m_firstSkeleton.m_animatedJoints.size(); i++) m_firstSkeleton.m_animatedJoints[i].first->m_rotation = Quaternion();

	// build new orientations
	std::vector<Quaternion> refOrientations = std::vector<Quaternion>(m_firstSkeleton.m_animatedJoints.size());
	for(i=1; i<m_firstSkeleton.m_animatedJoints.size(); i++)
	{
		ChainJoint* fatherJoint(m_firstSkeleton.GetFather(m_firstSkeleton.m_animatedJoints[i].first));
		Vec3 fPos(fatherJoint->m_translation);
		Vec3 pos(m_firstSkeleton.m_animatedJoints[i].first->m_translation);
		for(j=0; j<m_firstSkeleton.m_animatedJoints[i].first->m_child.size(); j++)
		{
			Vec3 cPos(m_firstSkeleton.m_animatedJoints[i].first->m_child[j]->m_translation);
			
			Vec3 dir1(glm::normalize(fPos - pos)), dir2(glm::normalize(cPos - pos));
			Vec3 x(dir2), z(glm::normalize(glm::cross(dir1, dir2)));
			Vec3 y(glm::normalize(glm::cross(z, x)));
			
			Mat44f m(Vec3ToVec4(x), Vec3ToVec4(y), Vec3ToVec4(z), Vec4(0.0f,0,0,1));

			refOrientations[i] = ToQuaternion(m);
		}

		if(m_firstSkeleton.m_animatedJoints[i].first->m_child.size()>1)
			refOrientations[i] = Quaternion();
	}

	// set up skeleton with new orientations
	for(i=0; i<m_firstSkeleton.m_animatedJoints.size(); i++) 
		m_firstSkeleton.m_animatedJoints[i].first->m_rotation = refOrientations[i];
	

	m_firstSkeleton.m_transformationMode = Skeleton::TM_ABSOLUTE;
	m_firstSkeleton.ToRelativeMode();
	m_firstSkeleton.SetCurrentToBPValues();

	m_refSkeleton = m_firstSkeleton;

	//----------------------------------------
	// DOF
	//----------------------------------------
	m_dof = TDOF(m_refSkeleton.m_animatedJoints.size(), vector<TJointDOF>(3, TJointDOF(-3.145f, 3.145f) )); 

	//----------------------------------------
	// DOF
	//----------------------------------------
	m_skMapping = TSkMapping(m_firstSkeleton.m_animatedJoints.size());
	for(i=0; i<m_firstSkeleton.m_animatedJoints.size(); i++) m_skMapping[i] = i;
}


////////////////////////////////////////////////////////////
/// Constructeur à partir d'un modèle cube
///
/// \param le fichier contenant le squelette et les degré de liberté
///
////////////////////////////////////////////////////////////
SkeletonTuner::SkeletonTuner(const std::string& file)
{
	Read(file);
}


////////////////////////////////////////////////////////////
/// Destructeur 
///
////////////////////////////////////////////////////////////
SkeletonTuner::~SkeletonTuner()
{
}


////////////////////////////////////////////////////////////
/// donne les orientations des articulations en relatif
///
/// \param liste d'orientation à remplir
///
////////////////////////////////////////////////////////////
void SkeletonTuner::GetAbsoluteOrientations(std::vector<std::pair<Vec3, Quaternion> >& _orientations)
{
	if(!m_refSkeleton.m_animatedJoints.size()) return;

	m_currentSkeleton = m_refSkeleton;
	m_currentSkeleton.ToRelativeMode();
	m_currentSkeleton.SetBPToCurrentValues();
	m_currentSkeleton.ToAbsoluteMode();

	UInt i,j;
	for(i=0; i<m_currentSkeleton.m_animatedJoints.size(); i++)
	{
		_orientations.push_back(pair<Vec3, Quaternion>(m_currentSkeleton.m_animatedJoints[i].first->m_translation, m_currentSkeleton.m_animatedJoints[i].first->m_rotation));
	}

	//for(i=0; i<orientations.size(); i++)
	//{
	//	for(j=0; j<orientations[i].size(); j++)
	//	{
	//		_orientations.push_back(pair<Vec3, Quaternion>(m_currentSkeleton.m_animatedJoints[i].first->m_translation, orientations[i][j]));
	//	}
	//}

	//for(i=0; i<m_refSkeleton.m_animatedJoints.size(); i++)
	//{
	//	if(i)
	//		_orientations.push_back(pair<Vec3, Quaternion>(m_currentSkeleton.GetFather(m_currentSkeleton.m_animatedJoints[i].first)->m_translation, m_currentSkeleton.m_animatedJoints[i].first->m_rotation));
	//	else
	//		_orientations.push_back(pair<Vec3, Quaternion>(m_currentSkeleton.m_animatedJoints[i].first->m_translation, m_currentSkeleton.m_animatedJoints[i].first->m_rotation));
	//}
}

////////////////////////////////////////////////////////////
/// modifie les repère donnés par défaut (Attention !! à faire avant de donner les dof)
///
/// \param liste des angles pour chaque articulation
/// \return squelette courant
///
////////////////////////////////////////////////////////////
void SkeletonTuner::ChangeBaseAxis(const std::vector<std::vector<float> >& angles)
{
	if(angles.size() != m_refSkeleton.m_animatedJoints.size()) return;
	if(m_refSkeleton.m_animatedJoints.size() == 0) return;

	m_refSkeleton = m_firstSkeleton;
	m_refSkeleton.SetCurrentToBPValues();
	m_refSkeleton.ToAbsoluteMode();
	
	
	UInt i,j;
	for(i=0; i<angles.size(); i++)
	{
		for(j=0; j<angles[i].size(); j++)
		{
			Vec3 axis(0.0f);
			switch(j)
			{
				case 0: axis = Vec3(1,0,0); break;
				case 1: axis = Vec3(0,1,0); break;
				case 2: axis = Vec3(0,0,1); break;
			}

			Quaternion q(axis, angles[i][j]);
			m_refSkeleton.m_animatedJoints[i].first->m_rotation = m_refSkeleton.m_animatedJoints[i].first->m_rotation * q;
		}
	}
	
	m_refSkeleton.SetCurrentToBPValues();
	m_refSkeleton.ToRelativeMode();
	m_refSkeleton.SetCurrentToBPValues();
}

////////////////////////////////////////////////////////////
/// applique un angle sur une articulation du squelette associé à un axe
///
/// \param liste des angles pour chaque articulation
///
////////////////////////////////////////////////////////////
const Skeleton& SkeletonTuner::ApplyAngles(const std::vector<std::vector<float> >& angles)
{
	if(angles.size() != m_refSkeleton.m_animatedJoints.size()) return m_currentSkeleton;
	if(m_refSkeleton.m_animatedJoints.size() == 0) return m_currentSkeleton;

	m_currentSkeleton = m_refSkeleton;
	m_currentSkeleton.ToRelativeMode();
	m_currentSkeleton.SetBPToCurrentValues();
	
	UInt i,j;
	for(i=0; i<angles.size(); i++)
	{
		for(j=0; j<angles[i].size(); j++)
		{
			Vec3 axis(0.0f);
			switch(j)
			{
				case 0: axis = Vec3(1,0,0); break;
				case 1: axis = Vec3(0,1,0); break;
				case 2: axis = Vec3(0,0,1); break;
			}

			Quaternion q(axis, angles[i][j]);
			m_currentSkeleton.m_animatedJoints[i].first->m_rotation = m_currentSkeleton.m_animatedJoints[i].first->m_rotation * q;
		}
	}
	
	m_currentSkeleton.SetCurrentToBPValues();
	m_currentSkeleton.ToAbsoluteMode();

	return m_currentSkeleton;
}

////////////////////////////////////////////////////////////
/// duplique les articulation pour les articulation possédant plusieurs enfants 
///
////////////////////////////////////////////////////////////
void SkeletonTuner::DuplicateConflictedOrientations()
{
	UInt i,j;

	// déjà dupliqué ?
	for(i=0; i<m_skMapping.size(); i++)
		if(m_skMapping[i] != i)
			return;

	// create new skeleton
	Skeleton newSkeleton;
	
	// skeleton to copy
	Skeleton toCopy = m_currentSkeleton;
	toCopy.SetCurrentToBPValues(); 
	toCopy.ToAbsoluteMode();
	newSkeleton.m_transformationMode = toCopy.m_transformationMode;
	newSkeleton.m_rotationOrder = toCopy.m_rotationOrder;

	m_skMapping.clear();
	std::vector<ChainJoint*> fathers(toCopy.m_animatedJoints.size());
	TDOF dof;

	for(i=0; i<toCopy.m_animatedJoints.size(); i++)
	{
		// create new joint
		ChainJoint* newJoint = toCopy.m_animatedJoints[i].first->Copy();
		newJoint->m_index = newSkeleton.m_animatedJoints.size();

		// and add it
		newSkeleton.AddJoint(newJoint);
		dof.push_back(m_dof[i]);

		// mapping
		m_skMapping.push_back(i);

		// hierarchy
		if(i != 0)
		{
			fathers[toCopy.m_animatedJoints[i].first->m_index]->m_child.push_back(newJoint);
		}

		// create new joints
		if(toCopy.m_animatedJoints[i].first->m_child.size()>1)
		{
			for(j=0; j<toCopy.m_animatedJoints[i].first->m_child.size(); j++)
			{
				// create joint and copy informations
				ChainJoint*  newChild = new ChainJoint();
				newChild->m_name = toCopy.m_animatedJoints[i].first->m_name + "_" + ToString(j);
				newChild->m_rotation = toCopy.m_animatedJoints[i].first->m_rotation;
				newChild->m_translation = toCopy.m_animatedJoints[i].first->m_translation;
				newChild->m_BPtranslation = toCopy.m_animatedJoints[i].first->m_BPtranslation;
				newChild->m_BProtation = toCopy.m_animatedJoints[i].first->m_BProtation;
				newChild->m_index = m_skMapping.size();

				// mapping
				m_skMapping.push_back(i);
				
				// and add it
				newSkeleton.AddJoint(newChild);
				dof.push_back(m_dof[i]);

				// hierarchy
				newJoint->m_child.push_back(newChild);

				// give links with newt joints
				fathers[toCopy.m_animatedJoints[i].first->m_child[j]->m_index] = newChild;
			}
		}
		else if(toCopy.m_animatedJoints[i].first->m_child.size())
		{
			// give links with newt joints
			fathers[toCopy.m_animatedJoints[i].first->m_child[0]->m_index] = newJoint;
		}
	}
	newSkeleton.m_rootJoint = newSkeleton.m_animatedJoints[0].first;

	// set to relative
	newSkeleton.m_transformationMode = Skeleton::TM_ABSOLUTE;
	newSkeleton.ToRelativeMode();
	newSkeleton.SetCurrentToBPValues();

	m_firstSkeleton = m_bpSkeleton = m_refSkeleton = newSkeleton;

	m_dof = dof;
}


////////////////////////////////////////////////////////////
/// donne un degré de liberté d'une articulation associé à un axe
///
/// \param index de l'articulation
/// \param axe
/// \param interval in ou out
/// \param valeur
///
////////////////////////////////////////////////////////////
void SkeletonTuner::SetDOF(UInt jointIndex, TAxis axis, TInterval interval, float value)
{
	if(interval == TI_In)
		m_dof[jointIndex][static_cast<UInt>(axis)].first = value;
	if(interval == TI_Out)
		m_dof[jointIndex][static_cast<UInt>(axis)].second = value;
}
















////////////////////////////////////////////////////////////
/// Trouve la bonne configuration du squelette à partir d'une posture
/// Attention: la méthode est itérative, elle part de la configuration de la frame précédente et corrige
///
/// \param valeurs des positions en absolut
/// \return squelette corrigé
///
////////////////////////////////////////////////////////////
void SkeletonTuner::FitSkeleton(const Quaternion& rootOrientation, const std::vector<Vec3>& jointPositions, std::vector<Vec3>& newPositions)
{
	UInt i,j;

	//----------------------------------------
	// correct positions
	//----------------------------------------
	Skeleton aSkeleton = m_refSkeleton;
	std::vector<Vec3> correctedPositions(aSkeleton.m_animatedJoints.size());
	correctedPositions[0] = jointPositions[0];
	for(i=0; i<aSkeleton.m_animatedJoints.size(); i++) 
	{
		aSkeleton.m_animatedJoints[i].first->m_translation = correctedPositions[i];
		if(i==0)
			aSkeleton.m_animatedJoints[i].first->m_rotation = rootOrientation;
		for(j=0; j<aSkeleton.m_animatedJoints[i].first->m_child.size(); j++) 
		{
			float d = glm::length(m_refSkeleton.m_animatedJoints[i].first->m_child[j]->m_translation);
			Vec3 pos(jointPositions[m_skMapping[i]]);
			Vec3 childPos( jointPositions[m_skMapping[aSkeleton.m_animatedJoints[i].first->m_child[j]->m_index]] );
			if(glm::length(childPos - pos) < 0.001f)
				correctedPositions[aSkeleton.m_animatedJoints[i].first->m_child[j]->m_index] = correctedPositions[i];
			else
				correctedPositions[aSkeleton.m_animatedJoints[i].first->m_child[j]->m_index] = correctedPositions[i] + d * glm::normalize(childPos - pos);
		}
	}
	
	// next compute the fitting of the skeleton
	Skeleton sk = FitCorrectedSkeleton(aSkeleton);

	//
	newPositions.resize(jointPositions.size());
	for(i=0; i<sk.m_animatedJoints.size(); i++) newPositions[m_skMapping[i]] = sk.m_animatedJoints[i].first->m_translation;
}

////////////////////////////////////////////////////////////
/// Trouve la bonne configuration du squelette à partir d'une posture
/// Attention: la méthode est itérative, elle part de la configuration de la frame précédente et corrige
///
/// \param valeurs des positions des articulations d'un squelette à corriger ou vérifier (en ABSOLUT)
/// \return squelette corrigé (en ABSOLUT)
///
////////////////////////////////////////////////////////////
const Skeleton& SkeletonTuner::FitSkeleton(const Skeleton& absoluteSkeleton)
{
	UInt i,j;

	//----------------------------------------
	// correct positions
	//----------------------------------------
	Skeleton aSkeleton = m_refSkeleton;
	std::vector<Vec3> correctedPositions(aSkeleton.m_animatedJoints.size());
	correctedPositions[0] = absoluteSkeleton.m_animatedJoints[0].first->m_translation;
	for(i=0; i<aSkeleton.m_animatedJoints.size(); i++) 
	{
		aSkeleton.m_animatedJoints[i].first->m_translation = correctedPositions[i];
		aSkeleton.m_animatedJoints[i].first->m_rotation = (m_refSkeleton.m_animatedJoints[m_skMapping[i]].first->m_rotation) * absoluteSkeleton.m_animatedJoints[m_skMapping[i]].first->m_rotation;
		for(j=0; j<aSkeleton.m_animatedJoints[i].first->m_child.size(); j++) 
		{
			float d = glm::length(m_refSkeleton.m_animatedJoints[i].first->m_child[j]->m_translation);
			Vec3 pos(absoluteSkeleton.m_animatedJoints[m_skMapping[i]].first->m_translation);
			Vec3 childPos( absoluteSkeleton.m_animatedJoints[m_skMapping[aSkeleton.m_animatedJoints[i].first->m_child[j]->m_index]].first->m_translation );
			if(glm::length(childPos - pos) < 0.001f)
				correctedPositions[aSkeleton.m_animatedJoints[i].first->m_child[j]->m_index] = correctedPositions[i];
			else
				correctedPositions[aSkeleton.m_animatedJoints[i].first->m_child[j]->m_index] = correctedPositions[i] + d * glm::normalize(childPos - pos);
		}
	}
	
	// next compute the fitting of the skeleton
	return FitCorrectedSkeleton(aSkeleton);
	
	//aSkeleton.m_transformationMode = Skeleton::TM_ABSOLUTE;
	//return m_currentSkeleton = (aSkeleton);
}


////////////////////////////////////////////////////////////
/// Trouve la bonne configuration du squelette à partir d'une posture
/// Attention: la méthode est itérative, elle part de la configuration de la frame précédente et corrige
///
/// \param valeurs des positions des articulations d'un squelette à corriger ou vérifier (en ABSOLUT)
/// \return squelette corrigé (en ABSOLUT)
///
////////////////////////////////////////////////////////////
Vec3 ComputePosition(const Skeleton& relativeSkeleton, const Vec3& rootPos, const std::vector<Quaternion>& relativeRotation, UInt index)
{
	Skeleton sk = relativeSkeleton;
	sk.ToRelativeMode();
	sk.SetBPToCurrentValues();
	sk.m_animatedJoints[0].first->m_translation = rootPos;
	UInt i;
	for(i=0; i<sk.m_animatedJoints.size(); i++) 
		sk.m_animatedJoints[i].first->m_rotation = relativeRotation[i];
	sk.SetCurrentToBPValues();
	sk.ToAbsoluteMode();

	return sk.m_animatedJoints[index].first->m_translation;
}

Quaternion GetRotationBetweenPoints(const Quaternion& centerRotation, const Vec3& centerPosition, const Vec3& from, const Vec3& to)
{
	Quaternion r;

	if(glm::length(from) < 0.001f) return r;

	Vec3 pn(to - centerPosition);
	Vec3 po(from);
	pn = glm::rotate(Inverse( centerRotation ), pn);
	
	// find the rotation to apply
	Vec3 oldAxis(glm::normalize(po)), newAxis(glm::normalize(pn));
	Vec3 axis(glm::cross(oldAxis, newAxis));
	axis = glm::normalize(axis);
	double angle(acos(glm::dot(oldAxis, newAxis))) ;
	r = Quaternion(axis, angle);

	return r;
}

const Skeleton& SkeletonTuner::FitCorrectedSkeleton(const Skeleton& aSkeleton)
{
	UInt i,j;

	//----------------------------------------
	// copy original relative skeleton
	//----------------------------------------
	m_currentSkeleton = m_refSkeleton;
	m_currentSkeleton.ToRelativeMode();
	m_currentSkeleton.SetBPToCurrentValues();

	// save relative orientation
	std::vector<Quaternion> relativeOrientations(m_currentSkeleton.m_animatedJoints.size());

	//----------------------------------------
	// realignement du squelette
	//----------------------------------------
	for(i=0; i<m_currentSkeleton.m_animatedJoints.size(); i++) 
	{
		ChainJoint* joint(m_currentSkeleton.m_animatedJoints[i].first);

		// si root
		if( i == 0 )
		{
			// translation et rotation du root
			joint->m_translation = aSkeleton.m_animatedJoints[m_skMapping[i]].first->m_translation;
			joint->m_rotation = aSkeleton.m_animatedJoints[m_skMapping[i]].first->m_rotation;

			// compute relative rotation
			relativeOrientations[i] = joint->m_rotation;
		}

		// si autre articulations
		else
		{
			// rotation courante
			Quaternion q(joint->m_rotation);
			
			// trouver la rotation qui permet de passer de l'ancien fils au nouveau
			Quaternion r;
			for(j=0; j<joint->m_child.size(); j++)
				r = GetRotationBetweenPoints(joint->m_rotation, joint->m_translation, joint->m_child[j]->m_translation, aSkeleton.m_animatedJoints[i].first->m_child[j]->m_translation);
		
			// correct the rotations
			float x, y, z, x2, y2, z2;
			Quaternion qRot(r);
			qRot.toEulerAngles(x, y, z);

			x2 = Clamp(x, m_dof[i][0].first, m_dof[i][0].second); //if(x2-x>0.1f) cout << "x j " << i << " corrected" << endl;
			y2 = Clamp(y, m_dof[i][1].first, m_dof[i][1].second); //if(y2-y>0.1f) cout << "y j " << i << " corrected" << endl;
			z2 = Clamp(z, m_dof[i][2].first, m_dof[i][2].second); //if(z2-z>0.1f) cout << "z j " << i << " corrected" << endl;

			r = Quaternion(Vec3(1.0, 0.0, 0.0), x2) * Quaternion(Vec3(0.0, 1.0, 0.0), y2) * Quaternion(Vec3(0.0, 0.0, 1.0), z2);

			// set rotation
			// si valide
			if(!(IsIndeterminate(r.x)|| !IsFiniteNumber(r.x)))
			{
				joint->m_rotation = joint->m_rotation * r;
			}
			
			// compute relative rotation
			relativeOrientations[i] = Inverse(m_currentSkeleton.GetFather(joint)->m_rotation) * joint->m_rotation;
		
			//si on arrive sur une butée, on essaye d'améliorer l'atteinte de la cible en utilisant les parents
			if(m_smartDOF)
			{
				// sélection d'une articulation en extrémité
				if(joint->m_child.size() && joint->m_child[0]->m_child.size() == 0)
				{
					// à la recherche du bon papa
					ChainJoint* jointFather(m_currentSkeleton.GetFather(joint));
					UInt count(0);
					bool notFound(true);
					while(notFound)
					{
						if(!jointFather)
						{
							notFound = false; 
							break;
						}

						ChainJoint* jointFatherNext = m_currentSkeleton.GetFather(jointFather);

						if(jointFatherNext->m_child.size()>1 || !jointFatherNext)  
						{
							notFound = false; 
							break;
						}

						jointFather = jointFatherNext;

						count++;
						if(count>10) cout << "pb" << endl;
					}

					// le père trouvé est il ok ?
					if(jointFather && jointFather->m_child.size() == 1)
					{
						Vec3 targetPos(aSkeleton.m_animatedJoints[i].first->m_child[0]->m_translation);
						Vec3 currentPos(ComputePosition(m_refSkeleton, aSkeleton.m_animatedJoints[0].first->m_translation, relativeOrientations, joint->m_child[0]->m_index));

						r = Quaternion();

						if(glm::distance(targetPos, currentPos)>0.001f)
						{
							Vec3 centerPos(jointFather->m_translation);

							// find the rotation to apply
							Vec3 oldAxis(glm::normalize(currentPos-centerPos)), newAxis(glm::normalize(targetPos-centerPos));
							Vec3 axis(glm::cross(oldAxis, newAxis));
							axis = glm::normalize(axis);
							double angle(acos(glm::dot(oldAxis, newAxis))) ;
							r = Quaternion(axis, angle);

							// si valide
							if(!(IsIndeterminate(r.x)|| !IsFiniteNumber(r.x)))
							{
								relativeOrientations[jointFather->m_index] = relativeOrientations[jointFather->m_index] * r;
							}
						}
					}
				}
			}
		}

		// apply to hierarchy (-> ABSOLUT)
		for(j=0; j<joint->m_child.size(); j++)
		{
			Vec3 pos(glm::rotate(joint->m_rotation, joint->m_child[j]->m_translation));
			Quaternion nextRotation = joint->m_child[j]->m_rotation;
			Quaternion q(
			(joint->m_rotation.w * nextRotation.w) - (joint->m_rotation.x * nextRotation.x) - (joint->m_rotation.y * nextRotation.y) - (joint->m_rotation.z * nextRotation.z),
			(joint->m_rotation.w * nextRotation.x) + (joint->m_rotation.x * nextRotation.w) + (joint->m_rotation.y * nextRotation.z) - (joint->m_rotation.z * nextRotation.y),
			(joint->m_rotation.w * nextRotation.y) + (joint->m_rotation.y * nextRotation.w) + (joint->m_rotation.z * nextRotation.x) - (joint->m_rotation.x * nextRotation.z),
			(joint->m_rotation.w * nextRotation.z) + (joint->m_rotation.z * nextRotation.w) + (joint->m_rotation.x * nextRotation.y) - (joint->m_rotation.y * nextRotation.x))  ;
	
			joint->m_child[j]->m_translation = pos;
			joint->m_child[j]->m_rotation = q;
		}
	}
	
	// 
	m_currentSkeleton = m_refSkeleton;
	m_currentSkeleton.ToRelativeMode();
	m_currentSkeleton.SetBPToCurrentValues();
	m_currentSkeleton.m_animatedJoints[0].first->m_translation = aSkeleton.m_animatedJoints[m_skMapping[0]].first->m_translation;
	for(i=0; i<m_currentSkeleton.m_animatedJoints.size(); i++) 
		m_currentSkeleton.m_animatedJoints[i].first->m_rotation = relativeOrientations[i];
	m_currentSkeleton.SetCurrentToBPValues();
	m_currentSkeleton.ToAbsoluteMode();

	// the skeleton is in absolute mode
	//m_currentSkeleton.m_transformationMode = Skeleton::TM_ABSOLUTE;

	//m_refSkeleton = m_currentSkeleton;

	return m_currentSkeleton;
}

////////////////////////////////////////////////////////////
/// Trouve la bonne configuration du squelette à partir d'une posture
/// Attention: la méthode est itérative, elle part de la configuration de la frame précédente et corrige
///
/// \param valeurs des positions en absolut
/// \return squelette corrigé
///
////////////////////////////////////////////////////////////
const Skeleton& SkeletonTuner::FitSkeleton(const std::vector<Vec3>& absoluteSkeletonPositions)
{
	UInt i,j;

	//----------------------------------------
	// copy original relative skeleton
	//----------------------------------------
	m_currentSkeleton = m_refSkeleton;
	m_currentSkeleton.ToRelativeMode();
	m_currentSkeleton.SetBPToCurrentValues();

	//----------------------------------------
	// realignement du squelette
	//----------------------------------------
	for(i=0; i<m_currentSkeleton.m_animatedJoints.size(); i++) 
	{
		ChainJoint* joint(m_currentSkeleton.m_animatedJoints[i].first);

		// root
		if(i == 0)
			joint->m_translation = absoluteSkeletonPositions[m_skMapping[0]];

		// rotation courante
		Quaternion q(joint->m_rotation);
		
		// trouver la rotation qui permet de passer passer de l'ancien fils au nouveau
		Quaternion r;
		for(j=0; j<joint->m_child.size(); j++)
		{
			Vec3 pn(absoluteSkeletonPositions[m_skMapping[joint->m_child[j]->m_index]] - joint->m_translation);
			Vec3 po(joint->m_child[j]->m_translation);
			pn = glm::rotate(Inverse( joint->m_rotation ), pn);

			
			// find the rotation to apply
			Vec3 oldAxis(glm::normalize(po)), newAxis(glm::normalize(pn));
			Vec3 axis(glm::cross(oldAxis, newAxis));
			axis = glm::normalize(axis);
			double angle(acos(glm::dot(oldAxis, newAxis))) ;
			r = Quaternion(axis, angle);
		}
	
	
		// correct the rotations
		float x, y, z, x2, y2, z2;
		Quaternion qRot(r);
		qRot.toEulerAngles(x, y, z);

		x2 = Clamp(x, m_dof[i][0].first, m_dof[i][0].second); //if(x2-x>0.1f) cout << "x j " << i << " corrected" << endl;
		y2 = Clamp(y, m_dof[i][1].first, m_dof[i][1].second); //if(y2-y>0.1f) cout << "y j " << i << " corrected" << endl;
		z2 = Clamp(z, m_dof[i][2].first, m_dof[i][2].second); //if(z2-z>0.1f) cout << "z j " << i << " corrected" << endl;

		r = Quaternion(Vec3(1.0, 0.0, 0.0), x2) * Quaternion(Vec3(0.0, 1.0, 0.0), y2) * Quaternion(Vec3(0.0, 0.0, 1.0), z2);

		// set rotation
		joint->m_rotation = joint->m_rotation * r;
		
		// apply to hierarchy (-> ABSOLUT)
		for(j=0; j<joint->m_child.size(); j++)
		{
			Vec3 pos(glm::rotate(joint->m_rotation, joint->m_child[j]->m_translation));
			Quaternion nextRotation = joint->m_child[j]->m_rotation;
			Quaternion q(
			(joint->m_rotation.w * nextRotation.w) - (joint->m_rotation.x * nextRotation.x) - (joint->m_rotation.y * nextRotation.y) - (joint->m_rotation.z * nextRotation.z),
			(joint->m_rotation.w * nextRotation.x) + (joint->m_rotation.x * nextRotation.w) + (joint->m_rotation.y * nextRotation.z) - (joint->m_rotation.z * nextRotation.y),
			(joint->m_rotation.w * nextRotation.y) + (joint->m_rotation.y * nextRotation.w) + (joint->m_rotation.z * nextRotation.x) - (joint->m_rotation.x * nextRotation.z),
			(joint->m_rotation.w * nextRotation.z) + (joint->m_rotation.z * nextRotation.w) + (joint->m_rotation.x * nextRotation.y) - (joint->m_rotation.y * nextRotation.x))  ;
	
			joint->m_child[j]->m_translation = pos;
			joint->m_child[j]->m_rotation = q;
		}
	}
	
	// the skeleton is in absolute mode
	m_currentSkeleton.m_transformationMode = Skeleton::TM_ABSOLUTE;

	return m_currentSkeleton;
}












////////////////////////////////////////////////////////////
/// Sauvegarde le squelette avec ses degrés de liberté (extension sk)
///
/// \param nom du fichier 
///
////////////////////////////////////////////////////////////
void SkeletonTuner::Writte(const std::string& name)
{
	ofstream stream(name.c_str(), ios::out | ios::trunc);
	{
		m_refSkeleton.SetCurrentToBPValues();

		//----------------------------------------------------------
		// skeleton
		//----------------------------------------------------------
		stream << m_refSkeleton << std::endl;

		//----------------------------------------------------------
		// dof
		//----------------------------------------------------------
		UInt i,j, count(m_dof.size());
		
		stream << count << std::endl;
		for(i=0; i<count; i++)
		{
			stream << i << " " << m_refSkeleton.m_animatedJoints[i].first->m_name << " ";
			for(j=0; j<3; j++)
			{
				stream << m_dof[i][j].first << " " << m_dof[i][j].second << " ";
			}
			stream << endl;
		}

		//----------------------------------------------------------
		// sk mapping
		//----------------------------------------------------------
		stream << m_skMapping.size() << std::endl;
		for(i=0; i<count; i++)
		{
			stream << m_skMapping[i] << " ";
		}
		stream << endl;
	}
}


////////////////////////////////////////////////////////////
/// Sauvegarde le squelette avec ses degrés de liberté (extension sk)
 ///
/// \param nom du fichier 
///
////////////////////////////////////////////////////////////
void SkeletonTuner::Read(const std::string& name)
{
	std::string str;
	std::ifstream stream(name.c_str(), ios::in);
	
	if(!stream.is_open())
	{
		cout << ">>> File " << name << " not found !!"<<endl;
		return;
	}
	{
		//----------------------------------------------------------
		// skeleton
		//----------------------------------------------------------
		stream >> m_refSkeleton;

		//----------------------------------------------------------
		// dof
		//----------------------------------------------------------
		UInt i,j, count(0), id;
		string str;
		
		stream >> count;
		m_dof = TDOF(count, vector<TJointDOF>(3, TJointDOF(-3.145f, 3.145f) )); 
		for(i=0; i<count; i++)
		{
			stream >> id >> str;
			for(j=0; j<3; j++)
				stream >> m_dof[i][j].first >> m_dof[i][j].second;
		}

		//----------------------------------------------------------
		// sk mapping
		//----------------------------------------------------------
		stream >> count;
		m_skMapping.resize(count);
		for(i=0; i<count; i++)
		{
			stream >> m_skMapping[i];
		}
	}

	m_firstSkeleton = m_refSkeleton;
}