#if !defined NO_GLEW_INCLUDE
	#include <GL/glew.h>	// glew: librairie GPU
#endif	// NO_GLEW_INCLUDE

#include "FontRenderer.h"
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

namespace Sc
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ScFontRenderer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<ScFontRenderer::FontData*> ScFontRenderer::CreateFont()
{
	std::vector<ScFontRenderer::FontData*> vec(0);
	//vec[0]	= new FontData(PathManager::GetInstance().FindMedia("tahoma.ttf").c_str(), 12, glm::vec3(0.0f, 0.0f, 0.0f));
	return vec;
}
std::vector<ScFontRenderer::FontData*> ScFontRenderer::m_fontDatas = CreateFont();
std::map<float, ScFontRenderer::FontData*> ScFontRenderer::m_heigtFontDatas;

/////////////////////////////////////////////////////////////
/// FontRenderer::FontData::Init
///
/// param 
/// param
///
////////////////////////////////////////////////////////////
void ScFontRenderer::FontData::Init(const char* policyName, unsigned int height) 
{
	m_height = static_cast<float>(height);

	//Allocate some memory to store the texture ids.
	m_textures = new GLuint[128];

	//Create and initilize a freetype font library.
	FT_Library library;
	if (FT_Init_FreeType( &library )) 
		throw std::exception("FT_Init_FreeType failed");

	//The object in which Freetype holds information on a given
	//font is called a "face".
	FT_Face face;

	//This is where we load in the font information from the file.
	//Of all the places where the code might die, this is the most likely,
	//as FT_New_Face will die if the font file does not exist or is somehow broken.
	if (FT_New_Face( library, policyName, 0, &face )) 
		throw std::exception("FT_New_Face failed (there is probably a problem with your font file)");

	//For some twisted reason, Freetype measures font size
	//in terms of 1/64ths of pixels.  Thus, to make a font
	//h pixels high, we need to request a size of h*64.
	//(h << 6 is just a prettier way of writting h*64)
	FT_Set_Char_Size( face, height << 6, height << 6, 96, 96);

	//Here we ask opengl to allocate resources for
	//all the textures and displays lists which we
	//are about to create.  
	m_listBase = glGenLists(128);
	glGenTextures( 128, m_textures );

	//This is where we actually create each of the fonts display lists.
	for(unsigned char i=0;i<128;i++)
		CreateCharacterDisplayList(face, i, m_listBase, m_textures);

	m_faceTrX = static_cast<GLfloat>(face->glyph->advance.x >> 6);

	//We don't need the face information now that the display
	//lists have been created, so we free the assosiated resources.
	FT_Done_Face(face);

	//Ditto for the library.
	FT_Done_FreeType(library);
}


/////////////////////////////////////////////////////////////
/// FontRenderer::FontData::Clean
///
////////////////////////////////////////////////////////////
void ScFontRenderer::FontData::Clean() 
{
	glDeleteLists(m_listBase,128);
	glDeleteTextures(128,m_textures);
	delete[] m_textures;
}

/////////////////////////////////////////////////////////////
/// Create a display list coresponding to the give character.
///
/// param 
/// param
/// param
/// param
///
////////////////////////////////////////////////////////////
void ScFontRenderer::FontData::CreateCharacterDisplayList ( FT_Face face, char ch, unsigned int list_base, unsigned int* tex_base ) 
{
	//The first thing we do is get FreeType to render our character
	//into a bitmap.  This actually requires a couple of FreeType commands:

	//Load the Glyph for our character.
	if(FT_Load_Glyph( face, FT_Get_Char_Index( face, ch ), FT_LOAD_DEFAULT ))
		throw std::exception();//throw std::runtime_error("FT_Load_Glyph failed");

	//Move the face's glyph into a Glyph object.
    FT_Glyph glyph;
    if(FT_Get_Glyph( face->glyph, &glyph ))
		throw std::exception();//throw std::runtime_error("FT_Get_Glyph failed");

	//Convert the glyph to a bitmap.
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//This reference will make accessing the bitmap easier
	FT_Bitmap& bitmap=bitmap_glyph->bitmap;

	//Use our helper function to get the widths of
	//the bitmap data that we will need in order to create
	//our texture.
	int width = next_p2( bitmap.width );
	int height = next_p2( bitmap.rows );

	//Allocate memory for the texture data.
	GLubyte* expanded_data = new GLubyte[ 2 * width * height];

	//Here we fill in the data for the expanded bitmap.
	//Notice that we are using two channel bitmap (one for
	//luminocity and one for alpha), but we assign
	//both luminocity and alpha to the value that we
	//find in the FreeType bitmap. 
	//We use the ?: operator so that value which we use
	//will be 0 if we are in the padding zone, and whatever
	//is the the Freetype bitmap otherwise.
	for(int j=0; j <height;j++) 
	{
		for(int i=0; i < width; i++)
		{
			expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] = 
				(i>=bitmap.width || j>=bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width*j];
		}
	}


	//Now we just setup some texture paramaters.
    glBindTexture( GL_TEXTURE_2D, tex_base[ch]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	//Here we actually create the texture itself, notice
	//that we are using GL_LUMINANCE_ALPHA to indicate that
	//we are using 2 channel data.
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );

	//With the texture created, we don't need to expanded data anymore
    delete [] expanded_data;

	//So now we can create the display list
	glNewList(list_base+ch,GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D,tex_base[ch]);

	//first we need to move over a little so that
	//the character has the right amount of space
	//between it and the one before it.
	glTranslatef(static_cast<GLfloat>(bitmap_glyph->left),0.0f,0.0f);

	//Now we move down a little in the case that the
	//bitmap extends past the bottom of the line 
	//(this is only true for characters like 'g' or 'y'.
	glPushMatrix();
	glTranslatef(0.0f,static_cast<GLfloat>(bitmap_glyph->top-bitmap.rows),0.0f);

	//Now we need to account for the fact that many of
	//our textures are filled with empty padding space.
	//We figure what portion of the texture is used by 
	//the actual character and store that information in 
	//the x and y variables, then when we draw the
	//quad, we will only reference the parts of the texture
	//that we contain the character itself.
	float	x=(float)bitmap.width / (float)width,
			y=(float)bitmap.rows / (float)height;

	//Here we draw the texturemaped quads.
	//The bitmap that we got from FreeType was not 
	//oriented quite like we would like it to be,
	//so we need to link the texture to the quad
	//so that the result will be properly aligned.
	glBegin(GL_QUADS);
	glColor3f(m_color.x, m_color.y, m_color.z);
	glTexCoord2d(0.0f,0.0f);	glVertex2f(0.0f,static_cast<GLfloat>(bitmap.rows));
	glTexCoord2d(0.0f,y);		glVertex2f(0.0f,0.0f);
	glTexCoord2d(x,y);			glVertex2f(static_cast<GLfloat>(bitmap.width),0.0f);
	glTexCoord2d(x,0.0f);		glVertex2f(static_cast<GLfloat>(bitmap.width),static_cast<GLfloat>(bitmap.rows));
	glEnd();
	glPopMatrix();
	glTranslatef(static_cast<GLfloat>(face->glyph->advance.x >> 6) ,0.0f,0.0f);


	//increment the raster position as if we were a bitmap font.
	//(only needed if you want to calculate text length)
	//glBitmap(0,0,0,0,face->glyph->advance.x >> 6,0,NULL);

	//Finnish the display list
	glEndList();
}


void ScFontRenderer::Create()
{ 
	CreateFont("tahoma.ttf", 1); /* add & create default font*/
	CreateFont("tahoma.ttf", 1); /* add & create default font*/
	CreateFont("tahoma.ttf", 2); /* add & create default font*/
	CreateFont("tahoma.ttf", 3); /* add & create default font*/
	CreateFont("tahoma.ttf", 4); /* add & create default font*/
	CreateFont("tahoma.ttf", 5); /* add & create default font*/
	CreateFont("tahoma.ttf", 6); /* add & create default font*/
	CreateFont("tahoma.ttf", 7); /* add & create default font*/
	CreateFont("tahoma.ttf", 8); /* add & create default font*/
	CreateFont("tahoma.ttf", 9); /* add & create default font*/
	CreateFont("tahoma.ttf", 10); /* add & create default font*/
	CreateFont("tahoma.ttf", 11); /* add & create default font*/
	CreateFont("tahoma.ttf", 12); /* add & create default font*/
	CreateFont("tahoma.ttf", 13); /* add & create default font*/
	CreateFont("tahoma.ttf", 14); /* add & create default font*/
}


void ScFontRenderer::DisplayWord(float x, float y, const std::string& text, unsigned int fontIndex)
{
	GLuint font = m_fontDatas[fontIndex]->m_listBase;
	glListBase(font);
	glPushMatrix();
		//glLoadIdentity();
		glTranslatef(x,y,0.0f);
		glCallLists(text.length(), GL_UNSIGNED_BYTE, text.c_str());
		glTranslatef(-x,-y,0.0f);
	glPopMatrix();
}

void ScFontRenderer::Display()
{
	// arrays to hold matrix information
	GLdouble model_view[16], projection[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
			glLoadIdentity();
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			glOrtho(viewport[0],viewport[2],viewport[1],viewport[3], 0.0, -1.0);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
				glLoadIdentity();

				unsigned int i;
				glBegin(GL_QUADS);
				for(i=0;i<m_legends.size(); i++)
				{
					glColor3f(m_legends[i].second.x, m_legends[i].second.y, m_legends[i].second.z);
					glVertex2i(m_legends[i].first.m_positionX, m_legends[i].first.m_positionY);
					glVertex2i(m_legends[i].first.m_positionX, m_legends[i].first.m_positionY + m_legends[i].first.m_height);
					glVertex2i(m_legends[i].first.m_positionX + m_legends[i].first.m_width, m_legends[i].first.m_positionY+ m_legends[i].first.m_height);
					glVertex2i(m_legends[i].first.m_positionX + m_legends[i].first.m_width, m_legends[i].first.m_positionY);
				}
				glEnd();
				m_legends.clear();

				glDisable(GL_LIGHTING);
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				
				
				for(i=0;i<m_toDisplayList.size(); i++)
				{
					bool to2D = m_toDisplayList[i].m_To2D;
					if(!to2D)
						DisplayWord(m_toDisplayList[i].m_position.x, viewport[3]-m_toDisplayList[i].m_position.y, m_toDisplayList[i].m_string, m_toDisplayList[i].m_height);
				}
				
				{
					GLdouble pos3D_x, pos3D_y, pos3D_z;
					for(i=0;i<m_toDisplayList.size(); i++)
					{
						bool to2D = m_toDisplayList[i].m_To2D;
						if(to2D)
						{
							// get 3D coordinates based on window coordinates
							gluProject(m_toDisplayList[i].m_position.x, m_toDisplayList[i].m_position.y, m_toDisplayList[i].m_position.z, model_view, projection, viewport, &pos3D_x, &pos3D_y, &pos3D_z);
							
							std::vector<std::string> tokens, delim;
							delim.push_back("\n"); delim.push_back("/"); 
							Split(m_toDisplayList[i].m_string, delim, tokens);

							float y(static_cast<float>(pos3D_y)), x(static_cast<float>(pos3D_x));
							unsigned int w;
							for(w=0; w<tokens.size(); w++)
								DisplayWord(x + m_toDisplayList[i].m_2dDisplacement.x, y+ + m_toDisplayList[i].m_2dDisplacement.y + float(w)*(m_toDisplayList[i].m_height+m_toDisplayList[i].m_height/2.0f), tokens[w], m_toDisplayList[i].m_height);
						}
					}
				}
				
				m_toDisplayList.clear();

				glDisable(GL_BLEND);

				
				
				glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	glPopAttrib();	
}

};