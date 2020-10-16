//========================================================================
// TP Animation / Synthese image
//
//------------------------------------------------------------------------
// Copyright (c) 2010-2011 Equipe SAMSARA (T. LE NAOUR, N. COURTY, S. GIBET)
// VALORIA - UBS 
//========================================================================


//=====================================================================
// classe Renderer, s'occupe de gérer l'affichage de la scene
//=====================================================================
//-----------------------------------------------------------------
// Ici fichier des déclarations des fonctions. Une fonction ne peut être déclarée qu'une seule fois par le fichier principal (main.cpp)
// d'ou l'ajout d'une routine qui dit que si votre fichier à déjà été inclu avant pas la peine de le re-inclure
//-----------------------------------------------------------------
#ifndef __RENDERER_H__	// routine qui sert à pas inclure 2 fois le même fichier
#define __RENDERER_H__	//


//-----------------------------------------------------------------
// On inclu les fichiers .h qui contiennent les déclarations des fonctions dont a besion 
// On a besoin ici de la structure UserEvents contenant les déplacement de la souris à chaque frame
//-----------------------------------------------------------------
#include "Common.h"		// données liées à la souris
#include "AnimatedChain.h" // TP animation


static void GlNormal3f(const Vec3& n) { glNormal3f(n.x, n.y, n.z); }
static void GlVertex3f(const Vec3& v) { glVertex3f(v.x, v.y, v.z); }
static void GlColor3f(const Vec3& c) { glColor3f(c.x, c.y, c.z); }
static void GlTexCoord03f(const Vec3& c) { glTexCoord3f(c.x, c.y, c.z); }
static void GlTranslatef(const Vec3& v) { glTranslatef(v.x, v.y, v.z); }

////////////////////////////////////////////////////////////////////////
/// classe caméra: s'occupe de mettre à jour la position de la caméra dans la fonction gluLookAt
////////////////////////////////////////////////////////////////////////
class Camera
{
public:
	//----------------------------------------------------------
	// constructeur de l'objet
	//----------------------------------------------------------
	Camera()
		:m_UserEvents(0), m_XOffset(0.0f), m_YOffset(0.0f), m_ZOffset(100.0f), m_rotateX(0.0f), m_rotateY(45.0f){}

	//----------------------------------------------------------
	// Destructeur de l'objet
	//----------------------------------------------------------
	~Camera()
	{
		m_UserEvents = 0;
	}

	//----------------------------------------------------------
	// compute met à jour les coordonnées de poistion de la caméra
	//----------------------------------------------------------
	virtual glm::mat4& Compute() = 0;

	
	//----------------------------------------------------------
	// Données membres
	//----------------------------------------------------------
	UserEvents*		m_UserEvents				;		///< pointeur sur l'objet partagé mouse events mis à jour par le GUI
	glm::mat4		m_viewMatrix				;		///< matrice de transformation de la vue
	float			m_XOffset, m_YOffset, m_ZOffset,	///< x,y rotate
					m_rotateX, m_rotateY;				///< x,y,z offset
	glm::vec4		m_translate;
};


class CameraLookAt: public Camera
{
public:
	//----------------------------------------------------------
	// constructeur de l'objet
	//----------------------------------------------------------
	CameraLookAt()
		:m_posX(150.0f), m_posY(150.0f), m_posZ(150.0f), 
		 m_viewX(0.0f), m_viewY(0.0f), m_viewZ(0.0f),
		 m_upX(0.0f), m_upY(1.0f), m_upZ(0.0f), m_alpha(0.0f), m_type(CT_Default){}

	//----------------------------------------------------------
	// Destructeur de l'objet
	//----------------------------------------------------------
	~CameraLookAt() {}

	//----------------------------------------------------------
	// compute met à jour les coordonnées de poistion de la caméra
	//----------------------------------------------------------
	glm::mat4& Compute();

	enum CameraType
	{
		CT_Default,
		CT_X,
		CT_Y, 
		CT_Z
	};
	CameraType	m_type;
	float m_alpha;

	//----------------------------------------------------------
	// Données membres
	//----------------------------------------------------------
	float m_posX,	m_posY,	 m_posZ	;	///< look at variables (pos)
	float m_viewX,	m_viewY, m_viewZ;	///< look at variables (view)
	float m_upX,	m_upY,	 m_upZ	;	///< look at variables (up)
};



////////////////////////////////////////////////////////////////////////
// opengl helper
////////////////////////////////////////////////////////////////////////
/** \brief Noeud graphique représentant un cylindre. 
	Le cylindre est décrit par les rayons de lsa base et de son sommet, ainsi que par sa hauteur. 
	\warning : La hauteur se situe sur l'axe des Z.
*/
struct GraphicCylinder 
{     
	/** Gestion de l'affichage openGL
	  \param radiues : le rayon du cylindre
	  \param x,y, z : extremite du rayon
	*/
	static void Draw(float radius, float x, float y, float z, Vector3 color = Vector3(1.0f, 1.0f, 0.0f));
} ;


/** \brief Noeud de géométrie représentant une sphère.
*/
struct GraphicSphere 
{
	/** \brief Gestion de l'affichage OpenGL 
		 \param radius : le rayon de la sphère
	*/
	static void Draw(float radius, Vector3 color = Vector3(1.0f, 0.0f, 0.0f));
};


////////////////////////////////////////////////////////////////////////
///  tutorial sympa pour commencer : http://www-evasion.imag.fr/Membres/Antoine.Bouthors/teaching/opengl/
///
/// le renderer est notre classe qui va 
///		- intialiser notre matrice de transformation de l'espace caméra à l'espace de projection
///		- initialiser notre matrice de transformation de l'espace monde à l'espace caméra
///		- intialiser notre scene dans l'espace monde
///		- dessiner des objets 3D, des gémométries
////////////////////////////////////////////////////////////////////////
enum PrimitiveType
{
	PT_Face = 1, 
	PT_Edge = 10, 
	PT_Vertex = 100
};

class Renderer
{
public:		// les fonctions suivantes peuvent être appelées par d'autres classes / programmes
	//----------------------------------------------------------
	// constructeur de l'objet (appeler automatiquement à la déclaration (si l'objet n'est pas un pointeur))
	// rien à mettre dedans pour le moment, notre programme n'a pas besoin d'objets temporaires
	//----------------------------------------------------------
	Renderer();

	//----------------------------------------------------------
	// constructeur de l'objet 
	//----------------------------------------------------------
	Renderer(WindowSetup* windowSetup, UserEvents* UserEvents, Camera* camera);

	//----------------------------------------------------------
	// Autre façon de créer le renderer
	//----------------------------------------------------------
	void Create(WindowSetup* windowSetup, UserEvents* UserEvents, Camera* camera);

	//----------------------------------------------------------
	// Destructeur de l'objet appeler automatiquement à la fin de la fonction main (si l'objet n'est pas un pointeur))
	// rien à mettre dedans pour le moment, notre programme n'a pas besoin d'objets temporaires
	//----------------------------------------------------------
	~Renderer();

	//----------------------------------------------------------
	// fonction d'initialisation, initialie OPENGL	
	//----------------------------------------------------------
	bool Init();

	//----------------------------------------------------------
	// fonction d'affichage
	//----------------------------------------------------------
	void StartRender();

	//----------------------------------------------------------
	// fonction qui permet de configurer les options propres au renderer
	//----------------------------------------------------------
	RendererSetup*	SetUpRenderer();

	//----------------------------------------------------------
	// fonction d'initialisation de la transformation de projection
	//----------------------------------------------------------
	void LoadProjectionMatrix(glm::mat4& projection);

	//----------------------------------------------------------
	// fonction dde mise à jour de la transformation de vue
	//----------------------------------------------------------
	void LoadViewMatrix(glm::mat4& view);

	//----------------------------------------------------------
	// Afficher d'une chaine animée (un squelette par ex)
	//----------------------------------------------------------
	void DisplayAnimatedChain(AnimatedChain* chain, bool drawSegment = true, Vector3 color = Vector3(0.8f, 0.15f, 0.15f), float radius = 0.5f, bool shadow = 1);
	
	//----------------------------------------------------------
	// fonction qui affiche un plan dans l'espace 3d
	//----------------------------------------------------------
	void DrawGround(float _size);

	static void DrawAxis(float length);
	static void DrawAxis(float length, const glm::mat4& m, float radius = 0.01f);

protected:	// les fonctions suivantes peuvent être appelées seulement pas cettte même classe et les classes filles de cette classe

	//----------------------------------------------------------
	// Données membres
	//----------------------------------------------------------
	Camera*			m_camera		;	// la camera
	WindowSetup*	m_windowSetup	;	// config et evenement liés à la fenêtre
	RendererSetup	m_rendererSetup	;	// config et evenement du renderer

	static std::vector<glm::vec3> CreateColors();
	static std::vector<glm::vec3> m_colors	;
};


// les fonctions inline permette une optimisation du code. Brievement, le corps des fonctions suivantes
// remplace les valeurs appelés dans le code par ces mêmes fonctions 
inline	Renderer::Renderer() :m_camera() {}
inline	Renderer::~Renderer(){m_windowSetup = 0;}
inline	Renderer::Renderer(WindowSetup* windowSetup, UserEvents* UserEvents,  Camera* camera) 
	:m_windowSetup(windowSetup), m_camera(camera)
{
	m_camera->m_UserEvents = UserEvents;
}

inline void Renderer::Create(WindowSetup* windowSetup, UserEvents* UserEvents, Camera* camera)
{
	m_windowSetup = windowSetup;
	m_camera = camera;
	m_camera->m_UserEvents = UserEvents;
}
inline RendererSetup*	Renderer::SetUpRenderer()	{ return &m_rendererSetup;}


#endif // __RENDERER_H__ fin de la routine