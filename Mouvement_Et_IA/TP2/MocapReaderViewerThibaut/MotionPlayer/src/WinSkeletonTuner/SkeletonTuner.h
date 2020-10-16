#ifndef __SkeletonTuner_H__
#define __SkeletonTuner_H__

#include "AnimatedChain.h"
#include <vector>

class SkeletonTuner
{
public:

	enum TAxis{TA_X, TA_Y, TA_Z };
	enum TInterval{TI_In, TI_Out};

	////////////////////////////////////////////////////////////
    /// Constructeur par défaut
    ///
    ////////////////////////////////////////////////////////////
	SkeletonTuner(){}

	////////////////////////////////////////////////////////////
    /// Constructeur à partir d'un modèle cube
    ///
	/// \param le squelette à paramétrer
	///
    ////////////////////////////////////////////////////////////
	SkeletonTuner(const Skeleton& skeleton);

	////////////////////////////////////////////////////////////
    /// Constructeur à partir d'un modèle cube
    ///
	/// \param le fichier contenant le squelette et les degré de liberté
	///
    ////////////////////////////////////////////////////////////
	SkeletonTuner(const std::string& file);

	////////////////////////////////////////////////////////////
    /// Destructeur 
    ///
    ////////////////////////////////////////////////////////////
	~SkeletonTuner();

	

	////////////////////////////////////////////////////////////
	/// modifie les repère donnés par défaut (Attention !! à faire avant de donner les dof)
	///
	/// \param liste des angles pour chaque articulation
	/// \return squelette courant
    ///
    ////////////////////////////////////////////////////////////
	void ChangeBaseAxis(const std::vector<std::vector<float> >& angles);

	////////////////////////////////////////////////////////////
	/// donne les degré de liberté d'une articulation associé à un axe (Attention !! à faire après ChangeBaseAxis)
	///
	/// \param liste des angles pour chaque articulation
	/// \return squelette courant
    ///
    ////////////////////////////////////////////////////////////
	const Skeleton& ApplyAngles(const std::vector<std::vector<float> >& angles);

	////////////////////////////////////////////////////////////
	/// duplique les articulation pour les articulation possédant plusieurs enfants 
    ///
    ////////////////////////////////////////////////////////////
	void DuplicateConflictedOrientations();

	////////////////////////////////////////////////////////////
	/// donne un degré de liberté d'une articulation associé à un axe
	///
	/// \param index de l'articulation
	/// \param axe
	/// \param interval in ou out
	/// \param valeur
    ///
    ////////////////////////////////////////////////////////////
	void SetDOF(UInt jointIndex, TAxis axis, TInterval interval, float value);

	////////////////////////////////////////////////////////////
	/// donne un degré de liberté d'une articulation associé à un axe
	///
	/// \param index de l'articulation
	/// \param axe
	/// \param interval in ou out
	/// \return valeur
    ///
    ////////////////////////////////////////////////////////////
	inline float GetDOF(UInt jointIndex, TAxis axis, TInterval interval){if(!(jointIndex<m_dof.size()))return 0.0f; if(interval == TI_In) return m_dof[jointIndex][static_cast<UInt>(axis)].first; else return m_dof[jointIndex][static_cast<UInt>(axis)].second;}

	////////////////////////////////////////////////////////////
	/// donne les orientations des articulations en relatif
    ///
	/// \param liste d'orientation à remplir
	///
    ////////////////////////////////////////////////////////////
	void GetAbsoluteOrientations(std::vector<std::pair<Vec3, Quaternion> >& orientations);

	////////////////////////////////////////////////////////////
	/// donne les orientations des articulations en relatif
    ///
	/// \return squelette courant
	///
    ////////////////////////////////////////////////////////////
	inline const Skeleton& GetSkeleton(){ return m_currentSkeleton;}

	////////////////////////////////////////////////////////////
	/// donne les orientations des articulations en relatif
    ///
	/// \param liste d'orientation à remplir
	///
    ////////////////////////////////////////////////////////////
	inline const Skeleton& GetRefSkeleton(){ return m_refSkeleton;}
	



	////////////////////////////////////////////////////////////
	/// Trouve la bonne configuration du squelette à partir d'une posture
	/// Attention: la méthode est itérative, elle part de la configuration de la frame précédente et corrige
	///
	/// \param valeurs des positions en absolut
	/// \return squelette corrigé
	///
	////////////////////////////////////////////////////////////
	const Skeleton& FitSkeleton(const Skeleton& absoluteSkeleton);

	////////////////////////////////////////////////////////////
	/// Trouve la bonne configuration du squelette à partir d'une posture
	/// Attention: la méthode est itérative, elle part de la configuration de la frame précédente et corrige
	///
	/// \param valeurs des positions en absolut
	/// \return squelette corrigé
	///
	////////////////////////////////////////////////////////////
	void FitSkeleton(const Quaternion& rootOrientation, const std::vector<Vec3>& jointPositions, std::vector<Vec3>& newPositions);

	////////////////////////////////////////////////////////////
	/// Trouve la bonne configuration du squelette à partir d'une posture
	/// Attention: la méthode est itérative, elle part de la configuration de la frame précédente et corrige
	///
	/// \param valeurs des positions en absolut
	/// \return squelette corrigé
	///
	////////////////////////////////////////////////////////////
	const Skeleton& FitSkeleton(const std::vector<Vec3>& absoluteSkeletonPositions);




	////////////////////////////////////////////////////////////
	/// Sauvegarde le squelette avec ses degrés de liberté (extension sk)
    ///
	/// \param nom du fichier 
	///
    ////////////////////////////////////////////////////////////
	void Writte(const std::string& name);

	////////////////////////////////////////////////////////////
    /// Sauvegarde le squelette avec ses degrés de liberté (extension sk)
     ///
	/// \param nom du fichier 
	///
    ////////////////////////////////////////////////////////////
	void Read(const std::string& name);

	////////////////////////////////////////////////////////////
    /// adapte les parents si la butée est atteinte sur une articulation
    ///
	/// \param valeur
	///
    ////////////////////////////////////////////////////////////
	inline void SetSmartDOF(bool smartDOF) {m_smartDOF = smartDOF;}

private:

	////////////////////////////////////////////////////////////
	// 
	////////////////////////////////////////////////////////////
	typedef std::pair<float, float>					TJointDOF	;
	typedef std::vector<std::vector<TJointDOF> >	TDOF		;
	typedef std::vector<UInt>						TSkMapping	;

	////////////////////////////////////////////////////////////
	// Données membres
	////////////////////////////////////////////////////////////
	Skeleton	m_firstSkeleton		;	///< le squelette de référence (le tout premier)
	Skeleton	m_bpSkeleton		;	///< le squelette de référence (le premier) <- a degager
	Skeleton	m_refSkeleton		;	///< le squelette de référence édité
	Skeleton	m_currentSkeleton	;	///< le squelette courant respectant les contraintes de dol
	TDOF		m_dof				;	///< les degrés de liberté pour chaque articulations en X,Y,Z
	TSkMapping	m_skMapping			;	///< table de correspondance dans le cas ou on modifie le squelette courant pour lui insérerer des repère supplémentaires
	bool		m_smartDOF			;	///< adapte les parents si la butée est atteinte sur une articulation

	////////////////////////////////////////////////////////////
	/// Trouve la bonne configuration du squelette à partir d'une posture
	/// Attention: la méthode est itérative, elle part de la configuration de la frame précédente et corrige
	///
	/// \param valeurs des positions en absolut
	/// \return squelette corrigé
	///
	////////////////////////////////////////////////////////////
	const Skeleton& FitCorrectedSkeleton(const Skeleton& absoluteSkeleton);
};


#endif //__SkeletonTuner_H__
