//========================================================================
// TP Animation / Synthese image
//
//------------------------------------------------------------------------
// Copyright (c) 2010-2011 Equipe SAMSARA (T. LE NAOUR, N. COURTY, S. GIBET)
// VALORIA - UBS 
//========================================================================


//-----------------------------------------------------------------
// On inclu les fichiers .h qui contiennent les déclarations des fonctions
// Les implémentations de nos fonctions ont besoin de leurs déclarations 
// ainsi que des déclarations des fonctions OPENGL
//-----------------------------------------------------------------
#include "Renderer.h"

#include <cmath>
#include <iostream>					// utiliser par la fonction "cout"
using namespace glm;
using namespace std;
	
/////////////////////////////////////////////////////////////
/// Initialise le renderer 
///
/// \return code d'erreur
////////////////////////////////////////////////////////////
bool Renderer::Init()
{
	// init glew
	GLenum error = glewInit();

	// Problem: glewInit failed, something is seriously wrong.
	if( GLEW_OK != error ) 
	{
		std::cout << "GLEW ERROR !" << std::endl;
		std::cout << glewGetErrorString( error ) << std::endl;
		return false;
	}

	// assurons nous que la caméra et la srtucture de config de la fenêtre a été bien intialisée
	if( (!m_camera->m_UserEvents) || (!m_windowSetup))
		return false;

	// intialisation de notre matrice de transformation de l'espace caméra à l'espace de projection
	glm::mat4 projection = glm::perspective(45.0f, static_cast<float>(m_windowSetup->m_width) / static_cast<float>(m_windowSetup->m_height), 0.2f, 5000.0f);
	LoadProjectionMatrix(projection);	

	// On donne une colueur à notre fond d'espace (background)
	glClearColor(m_rendererSetup.m_clearColor[0], m_rendererSetup.m_clearColor[1], 
				 m_rendererSetup.m_clearColor[2], m_rendererSetup.m_clearColor[3]);

	// OpenGL pour traiter les objets dans la scene a besoin de 2/3 routines, savoir dans quel ordre il affiche les éléments
	// affiche t'on les faces caché (normale dans le sens de la caméra) 
	// http://www.linuxgraphic.org/section3d/openGL/didacticiels/didac2/didac4.html
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// spécifie la couleur de remplissage des faces (soit opengl calcule un dégradé entre les différents sommet de la face, soit vous remplissez
	// la face par la couleur d'un des sommets(GL_FLAT): http://rvirtual.free.fr/programmation/OpenGl/Debut_3D.htm)
	glShadeModel(GL_SMOOTH);

	// dimension de la fenetre de sortie
	glViewport(0,0,m_windowSetup->m_width, m_windowSetup->m_height);

	return true;
}


/////////////////////////////////////////////////////////////
/// lance les méthodes communes à chaque frame
///
////////////////////////////////////////////////////////////
void Renderer::StartRender()
{
	glm::mat4 projection = glm::perspective(45.0f, static_cast<float>(m_windowSetup->m_width) / static_cast<float>(m_windowSetup->m_height), 0.2f, 5000.0f);
	LoadProjectionMatrix(projection);	

	glViewport(0,0,m_windowSetup->m_width, m_windowSetup->m_height);

	// remet à zeros les buffer précédents
	glClear(GL_COLOR_BUFFER_BIT |						// Efface le frame buffer (l'image précédente)
			GL_DEPTH_BUFFER_BIT);						// Efface le Z-buffer (le calcul de profondeur l'image précédente)

	// mise a jour des coordonnées de la caméra et on mets à jour la transformation de la caméra
	LoadViewMatrix(m_camera->Compute());
	
	// afficher le sol
	if( m_rendererSetup.m_drawGround )
		DrawGround(100.0f);
}

/////////////////////////////////////////////////////////////
/// charge la matrice de projection
///
/// \param matrice de projection
////////////////////////////////////////////////////////////
void Renderer::LoadProjectionMatrix(glm::mat4& projection)
{
	glMatrixMode(GL_PROJECTION);						// on spécifie qu'on se positionne dans l'espase de projection
	glLoadIdentity();									// on lui donne comme transformation la matrice identité
	
	// on envoit la matrice de projection à la carte graphique
	glLoadMatrixf(glm::value_ptr(projection));

	// Ancien code
	//-------------------------
	// on envoit la matrice de projection à la carte graphique
	//glm::mat4 projection = glm::perspective(45.0f, static_cast<float>(m_windowSetup->m_width) / static_cast<float>(m_windowSetup->m_height), 0.2f, 100.0f);
	//glLoadMatrixf(glm::value_ptr(projection));

	// Autre méthode: il existe une librairie directement intégré à OPENGL (glu) qui forunit directement une fonction 
	//gluPerspective(45.0f, 800.0f/640.0f, 0.2f, 100.0f);	// Angle de la caméra, clipping entre 1 et 100 (pas à partir de 0,
															// sinon on perd toute la précision du z-buffer)
}

/////////////////////////////////////////////////////////////
/// charge la matrice de vue
///
/// \param matrice de vue
////////////////////////////////////////////////////////////
void Renderer::LoadViewMatrix(glm::mat4& view)
{
	glMatrixMode(GL_MODELVIEW);							// on se place dans le repere de la camera
	glLoadIdentity();									// on lui donne comme transformation la matrice identité

	// matrice de vue 	
	glLoadMatrixf(glm::value_ptr(view));

	// Ancien code
	//-------------------------
	// matrice de vue 		
	//glm::mat4 view = glm::lookAt(glm::vec3(m_camera.m_posX, m_camera.m_posY, m_camera.m_posZ),		// position de la camera
	//							 glm::vec3(m_camera.m_viewX, m_camera.m_viewY, m_camera.m_viewZ),	// endroit de la scene vers ou la caméra pointe (regarde)
	//							 glm::vec3(m_camera.m_upX, m_camera.m_upY, m_camera.m_upZ));		// orientation verticale
	//glLoadMatrixf(glm::value_ptr(view));

	// Autre méthode: il existe une librairie directement intégré à OPENGL (glu) qui forunit directement une fonction 
	//gluLookAt(m_camera.m_posX, m_camera.m_posY, m_camera.m_posZ,		// position de la camera
	//			m_camera.m_viewX, m_camera.m_viewY, m_camera.m_viewZ,	// endroit de la scene vers ou la caméra pointe (regarde)
	//			m_camera.m_upX, m_camera.m_upY, m_camera.m_upZ);		// orientation verticale

}


/////////////////////////////////////////////////////////////
/// dessine un sol
///
/// \param _size : Taille d'un carreau
///
////////////////////////////////////////////////////////////
void Renderer::DrawGround(float _size)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);					// Sauvegarde contexte
	float step = _size/100.0f; 
	
	glDisable(GL_LIGHTING);
	glColor4f(100.0f/255.0f, 106.0f/255.0f,118.0f/255.0f, 0.8f);
	unsigned int count = 0;
	for (float i = 0 ; i <= 2.0*_size ; i=i+step, count++)
	{
		if (!(count%5)) glLineWidth(3.0f);
		else glLineWidth(1.0f);
		glBegin(GL_LINES);
			glVertex3d(-_size+i,0.001,-_size);
			glVertex3d(-_size+i,0.001,_size);
			glVertex3d(-_size,0.001,-_size+i);
			glVertex3d(_size,0.001,-_size+i);
		glEnd();
	}

	// draw some plane
	// activation de la transparence
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
		glColor4f(112.0f/255.0f,117.0f/255.0f,130.0f/255.0f, 0.5f);
		//glColor4f(0.4f,0.4f,0.4f, 0.5f);
		glVertex3d(-_size,-0.01,-_size);glVertex3d(-_size,-0.01,_size);
		glVertex3d(_size,-0.01,_size);glVertex3d(_size,-0.01,-_size);
	glEnd();
	glDisable(GL_BLEND);
	glPopAttrib();										// Retour au contexte d'origine
}
 

void DrawArrow(float length, float radius, int nbSubdivisions = 12)
{
	static GLUquadric* quadric = gluNewQuadric();

	if (radius < 0.0)
		radius = 0.05f * length;

	const float head = 2.5f*(radius / length) + 0.1f;
	const float coneRadiusCoef = 4.0f - 5.0f * head;

	gluCylinder(quadric, radius, radius, length * (1.0f - head/coneRadiusCoef), nbSubdivisions, 1);
	glTranslatef(0.0f, 0.0f, length * (1.0f - head));
	gluCylinder(quadric, coneRadiusCoef * radius, 0.0f, head * length, nbSubdivisions, 1);
	glTranslatef(0.0f, 0.0f, -length * (1.0f - head));
}

void Renderer::DrawAxis(float length, const glm::mat4& m, float radius)
{
	glPushMatrix();
	glMultMatrixf(glm::value_ptr(m));

	//const float charWidth = length / 40.0f;
	//const float charHeight = length / 30.0f;
	//const float charShift = 1.04f * length;

	GLboolean lighting, colorMaterial;
	glGetBooleanv(GL_LIGHTING, &lighting);
	glGetBooleanv(GL_COLOR_MATERIAL, &colorMaterial);

	glDisable(GL_LIGHTING);
/*
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	// The X
	glVertex3f(charShift,  charWidth, -charHeight);
	glVertex3f(charShift, -charWidth,  charHeight);
	glVertex3f(charShift, -charWidth, -charHeight);
	glVertex3f(charShift,  charWidth,  charHeight);
	// The Y
	glVertex3f( charWidth, charShift, charHeight);
	glVertex3f(0.0f,        charShift, 0.0f);
	glVertex3f(-charWidth, charShift, charHeight);
	glVertex3f(0.0f,        charShift, 0.0f);
	glVertex3f(0.0f,        charShift, 0.0f);
	glVertex3f(0.0f,        charShift, -charHeight);
	// The Z
	glVertex3f(-charWidth,  charHeight, charShift);
	glVertex3f( charWidth,  charHeight, charShift);
	glVertex3f( charWidth,  charHeight, charShift);
	glVertex3f(-charWidth, -charHeight, charShift);
	glVertex3f(-charWidth, -charHeight, charShift);
	glVertex3f( charWidth, -charHeight, charShift);
	glEnd();
*/
	//glEnable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	float color[4];
	color[0] = 0.6f;  color[1] = 0.6f;  color[2] = 1.0f;  color[3] = 1.0f;
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glColor3f(color[0], color[1], color[2]);
	DrawArrow(length, radius*length, 12);

	color[0] = 1.0f;  color[1] = 0.6f;  color[2] = 0.6f;  color[3] = 1.0f;
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glColor3f(color[0], color[1], color[2]);
	glPushMatrix();
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	DrawArrow(length, radius*length, 12);
	glPopMatrix();

	color[0] = 0.6f;  color[1] = 1.0f;  color[2] = 0.6f;  color[3] = 1.0f;
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glColor3f(color[0], color[1], color[2]);
	glPushMatrix();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	DrawArrow(length, radius*length, 12);
	glPopMatrix();

	if (colorMaterial)
		glEnable(GL_COLOR_MATERIAL);
	if (!lighting)
		glDisable(GL_LIGHTING);

	glPopMatrix();
}

void Renderer::DrawAxis(float length)
{
	glm::mat4 m;
	DrawAxis(length, m, 0.01f);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Partie Animation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GraphicCylinder::Draw(float radius, float x, float y, float z, Vector3 color)
{
	glPushMatrix() ;
	glLineWidth(radius);
	glColor3f(color.x, color.y, color.z) ;
	glBegin(GL_LINES);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(x, y, z) ;
	glEnd();
	    
	glPopMatrix() ;
}

//////////////////////////////////////////////////////////////////////////////////////////////
///	 specific functions for drawing skeletons
//////////////////////////////////////////////////////////////////////////////////////////////
void renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric)
{
	float vx = x2-x1;
	float vy = y2-y1;
	float vz = z2-z1;

	//handle the degenerate case of z1 == z2 with an approximation
	if(vz == 0)
		vz = .0001f;

	float v = sqrt( vx*vx + vy*vy + vz*vz );
	float ax = 57.2957795f*acos( vz/v );
	if ( vz < 0.0 )
		ax = -ax;
	float rx = -vy*vz;
	float ry = vx*vz;
	glPushMatrix();

	//draw the cylinder body
	glTranslatef( x1,y1,z1 );
	glRotatef(ax, rx, ry, 0.0);
	gluQuadricOrientation(quadric,GLU_OUTSIDE);
	gluCylinder(quadric, radius, radius, v, subdivisions, 1);

	//draw the first cap
	gluQuadricOrientation(quadric,GLU_INSIDE);
	gluDisk( quadric, 0.0, radius, subdivisions, 1);
	glTranslatef( 0,0,v );

	//draw the second cap
	gluQuadricOrientation(quadric,GLU_OUTSIDE);
	gluDisk( quadric, 0.0, radius, subdivisions, 1);
	glPopMatrix();
}

void GraphicSphere::Draw(float radius, Vector3 color)
{
	GLUquadric * quadric=gluNewQuadric() ;
	gluQuadricOrientation(quadric, GLU_OUTSIDE) ;
	glColor3f(color.x, color.y, color.z) ;
	gluSphere(quadric, radius, 10, 10) ;
	gluDeleteQuadric(quadric) ;
}


/////////////////////////////////////////////////////////////
/// dessine une chaine animée
///
/// \param la chaine a afficher
///
////////////////////////////////////////////////////////////
void Renderer::DisplayAnimatedChain(AnimatedChain* chain, bool drawSegment, Vector3 color, float radius, bool shadow)
{
	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);

	glEnable(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set light
	//---------------------------------------------
	float v[4]; // will be used to set light parameters
	// Light parameter
	float g_LightMultiplier = 1.0f;
	float g_LightDirection[] = { -0.57735f, -0.57735f, -0.57735f };
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	v[0] = v[1] = v[2] = g_LightMultiplier*0.4f; v[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_AMBIENT, v);
	v[0] = v[1] = v[2] = g_LightMultiplier*0.8f; v[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_DIFFUSE, v);
	v[0] = -g_LightDirection[0]; v[1] = -g_LightDirection[1]; v[2] = -g_LightDirection[2]; v[3] = 0.0f;
	glLightfv(GL_LIGHT0, GL_POSITION, v);

	// Set material
	// Shapes material
	float g_MatAmbient[] = { 0.5f, 0.0f, 0.0f, 1.0f };
	float g_MatDiffuse[] = { color.x, color.y, color.z, 1.0f };
		
	//if(false)
	//{
	//	g_MatAmbient[0] = 0.5f; g_MatAmbient[1] = 0.4f; g_MatAmbient[2] = 0.4f; //{ 0.5f, 0.4f, 0.4f, 1.0f };
	//	g_MatDiffuse[0] = 0.5f; g_MatDiffuse[1] = 0.5f; g_MatDiffuse[2] = 0.5f; //{ 0.5f, 0.5f, 0.5f, 1.0f };
	//}
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, g_MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, g_MatDiffuse);



	static GLUquadric* quadric = gluNewQuadric();
	
	Vec3 *p, *pc;
	unsigned int i,j;
	for(i=0; i<chain->m_animatedJoints.size(); i++)
	{
		p = &chain->m_animatedJoints[i].first->m_translation;
	
		if(!i)
		{
			glTranslatef(p->x, p->y, p->z);
				gluSphere(quadric, radius, 10, 6);
			glTranslatef(-p->x, -p->y, -p->z);
		}
		for(j=0; j<chain->m_animatedJoints[i].first->m_child.size(); j++)
		{
			pc = &chain->m_animatedJoints[i].first->m_child[j]->m_translation;
			if(drawSegment)
				renderCylinder(	p->x,p->y, p->z,  pc->x, pc->y, pc->z, radius,  50, quadric);
			glTranslatef(pc->x, pc->y, pc->z);
				gluSphere(quadric, radius, 10, 6);
			glTranslatef(-pc->x, -pc->y, -pc->z);
		}
	}
	glPopAttrib();

	// Render shadow
	if(shadow)
	{
		glColor4f(0.1f, 0.2f, 0.3f, 0.8f);
		glPushMatrix();
			const static float shadowMat[]={ 1,0,0,0, 0,0,0,0, 0,0,1,0, 0,0,0,1	};
			glMultMatrixf(shadowMat);
			for(i=0; i<chain->m_animatedJoints.size(); i++)
			{
				p = &chain->m_animatedJoints[i].first->m_translation;
			
				for(j=0; j<chain->m_animatedJoints[i].first->m_child.size(); j++)
				{
					pc = &chain->m_animatedJoints[i].first->m_child[j]->m_translation;
					renderCylinder(	p->x,p->y, p->z,  pc->x, pc->y, pc->z, radius,  50, quadric);
					glTranslatef(p->x, p->y, p->z);
						gluSphere(quadric, radius, 10, 6);
					glTranslatef(-p->x, -p->y, -p->z);
				}
			}
		glPopMatrix();
	}
}


/////////////////////////////////////////////////////////////
/// mets à jour les attributs de la caméra en fonction des évenements souris
///
////////////////////////////////////////////////////////////
glm::mat4& CameraLookAt::Compute()
{
	if(m_UserEvents->m_mode != UserEvents::MT_DEFAULT)
		return m_viewMatrix;


	if(m_UserEvents->mousebuttons[0] && m_UserEvents->mousebuttons[1]) 
	{
		if(m_type == CT_Y){}
		else
			m_viewY += m_UserEvents->mouseyrel;
	}

	if( m_UserEvents->mousebuttons[1]) // bouton droit
	{
		float mg(m_posX*m_posX + m_posY*m_posY + m_posZ*m_posZ);
		float x(m_posX/mg);
		float y(m_posY/mg);
		float z(m_posZ/mg);

		float coeff(sqrt(mg) + 1.0f);

		m_posX = m_UserEvents->mouseyrel * coeff * x + m_posX;
		m_posY = m_UserEvents->mouseyrel * coeff * y + m_posY;
		m_posZ = m_UserEvents->mouseyrel * coeff * z + m_posZ;
	}

	// si boutton de droite on fait tourner la caméra autour du point vers lequelle elle pointe
	if(m_UserEvents->mousebuttons[2]) // boutton molette 
	{
		if(m_type == CT_Default)
		{
			// axe entre la position de la caméra et la position sur laquelle elle "regarde"
			Vec3  CameraToView ( Vec3(m_viewX,m_viewY,m_viewZ) - Vec3(m_posX,m_posY,m_posZ) ); 

			// axe projeté sur le plan XZ !
			Vec3 CameraToViewInPlane(CameraToView.x, 0.0f, CameraToView.z);
			CameraToViewInPlane = glm::normalize(CameraToViewInPlane);

			// axe perpendiculaire à CameraToViewInPlan dans le plan XZ !
			Vec3 planeAxis = glm::cross(CameraToViewInPlane, Vec3(0.0f, 1.0f, 0.0f));

			// quaternion qPlan (= rotation autour de l'axe dans le plan XZ perpendiculaire à CameraToView projeté sur XZ) 
			Quaternion qPlan = Quaternion(planeAxis, -m_UserEvents->mouseyrel*0.005f);
			
			// quaternion qy (= rotation autor de l'axe Y=(0,1,0)
			Quaternion qy = Quaternion(Vec3(0,1,0), -m_UserEvents->mousexrel*0.005f);
			
			// calcul de la position finale (= qPlan * pY appliqué à la position intiale = rotationPlan * rotationT appliqué à la position intiale)
			Vec3 newPos = (qPlan*qy).Rotate(Vec3(m_posX,m_posY,m_posZ));
			
			// on affecte les valeurs
			m_posX = newPos.x; 
			m_posY = newPos.y; 
			m_posZ = newPos.z; 
		}
		else if(m_type == CT_X)
		{
			float alpha(m_alpha + 0.01f * static_cast<float>(m_UserEvents->mouseyrel));
			Quaternion q(Vec3(0.0f, 0.0f, 1.0f), alpha);
			Vec3 pos(m_posX, m_posY, m_posZ), view(m_viewX, m_viewY, m_viewZ);
			pos = q.Rotate(pos-view) + view;
			m_posX = pos.x; m_posY = pos.y; m_posZ = pos.z; 
		}
		else if(m_type == CT_Y)
		{
			float alpha(m_alpha + 0.01f * static_cast<float>(m_UserEvents->mousexrel));
			Quaternion q(Vec3(0.0f, 1.0f, 0.0f), alpha);
			Vec3 pos(m_posX, m_posY, m_posZ), view(m_viewX, m_viewY, m_viewZ);
			pos = q.Rotate(pos-view) + view;
			m_posX = pos.x; m_posY = pos.y; m_posZ = pos.z; 
		}
		else if(m_type == CT_Z)
		{
			float alpha(m_alpha + 0.01f * static_cast<float>(m_UserEvents->mouseyrel));
			Quaternion q(Vec3(1.0f, 0.0f, 0.0f), alpha);
			Vec3 pos(m_posX, m_posY, m_posZ), view(m_viewX, m_viewY, m_viewZ);
			pos = q.Rotate(pos-view) + view;
			m_posX = pos.x; m_posY = pos.y; m_posZ = pos.z; 
		}
	}	

	m_viewMatrix = glm::lookAt(	Vec3(m_posX, m_posY, m_posZ),		// position de la camera
								Vec3(m_viewX, m_viewY, m_viewZ),	// endroit de la scene vers ou la caméra pointe (regarde)
								Vec3(m_upX, m_upY, m_upZ));		// orientation verticale
	
	// si boutton de droite on fait tourner la caméra autour du point vers lequelle elle pointe
	if(m_UserEvents->mousebuttons[0] && m_UserEvents->mousebuttons[1]) 
	{
		float mg(m_posX*m_posX + m_posY*m_posY + m_posZ*m_posZ);
		float coeff(sqrt(mg) + 1.0f);

		m_translate.x += 0.002f * coeff * m_UserEvents->mousexrel;
		m_translate.y -= 0.002f * coeff * m_UserEvents->mouseyrel;
	}	

	m_viewMatrix[3] += m_translate;
	glm::mat4 viewMatrix(glm::inverse(m_viewMatrix));


	return m_viewMatrix;
}	

