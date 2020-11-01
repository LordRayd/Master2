#ifndef __Font_Renderer_H__	// routine qui sert à pas inclure 2 fois le même fichier
#define __Font_Renderer_H__	//


#pragma comment(lib, "glu32.lib")

#include "Common.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

namespace Sc 
{


//==========================================================
// Macro d'exportation / importation
//==========================================================
#   define SC_EXPORT 
//#define SC_EXPORTS
//#ifdef SC_EXPORTS
//#   define SC_EXPORT 
//#else
//#   define SC_EXPORT
//#endif

//==========================================================
// Désactivation du warning 4251
// "T1 nécessite une interface DLL pour être utilisé(e) par
// les clients de class T2" 
//==========================================================
#ifdef _MSC_VER
#   pragma warning(disable : 4251)
#endif


#if defined _DEBUG || defined DEBUG || defined DEBUG_RELEASE 
	#define	LOG(msg)			std::cout << msg << std::endl;
#else	// _DEBUG
	#define	LOG(msg)			
#endif	// _DEBUG
#define RETURN(msg)				{LOG(msg) return;}
#define RETURN_VAL(msg, value)	{LOG(msg) return value;}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// classe define template singleton pattern 
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
class ScSingleton
{
public :
	static T& GetInstance()
    {
        if (!m_instance)
            m_instance = new T;
        return *m_instance;
    }

    static void Destroy()
    {
		delete m_instance;
        m_instance = 0;
    }

	static T* GetPointerInstance()
	{
		 if (!m_instance)
            m_instance = new T;
        return m_instance;
	}

protected :

    ScSingleton() {}
    ~ScSingleton() {}

private :
 
    static T* m_instance; 
    ScSingleton(ScSingleton&);
    void operator =(ScSingleton&);
};
template <class T> 
T* ScSingleton<T>::m_instance = 0;


template<class T>
struct Rect
{
	Rect():m_positionX(0), m_positionY(0), m_width(0), m_height(0){}
	Rect(const T& pX, const T& pY)
		:m_positionX(pX), m_positionY(pY), m_width(0), m_height(0){}
	Rect(const T& pX, const T& pY, const T& pW, const T& pH)
		:m_positionX(pX), m_positionY(pY), m_width(pW), m_height(pH){}

	T	m_positionX		;
	T	m_positionY		;
	T	m_width			;
	T	m_height		;

	T GetCenterX(){return	m_positionX+(m_width/2);	}
	T GetCenterY(){return	m_positionY+(m_height/2);	}
};
typedef Rect<float>	Rectf;
typedef Rect<int>	Recti;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Structure qui regroupe des petites fonctions utiles sur les fichiers
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct File
{
	//----------------------------------------------------------
	// Fonction qui donne l'extension du fichier 
	//----------------------------------------------------------
	static std::string GetExtension(const std::string& fileName) 
	{
		std::string::size_type Pos = fileName.find_last_of(".");
		if (Pos != std::string::npos)
			return fileName.substr(Pos + 1, std::string::npos);
		else
			return "";
	}


	//----------------------------------------------------------
	//	Fonction qui donne le nom du fichier avec extension sans le chemin
	//----------------------------------------------------------
	static const std::string GetName(const std::string& fileName) 
	{
		std::string::size_type Pos = fileName.find_last_of("\\/");

		if (Pos != std::string::npos)
			return fileName.substr(Pos + 1, std::string::npos);
		else
			return fileName;
	}

	//----------------------------------------------------------
	//	Vérifie si le fichier existe
	//----------------------------------------------------------
	static bool Exists(const std::string& fileName)
	{
		std::ifstream file(fileName.c_str());  // on ouvre le fichier 
		return file.is_open();
	}

	//----------------------------------------------------------
	//	Donne le chemin du fichier
	//----------------------------------------------------------
	static std::string GetFilePath(const std::string& fileName) 
	{
		std::string name(GetName(fileName)), str("");

		int pos;
		if( (pos = fileName.find(name)) && ( (pos+name.size()) == fileName.size() ) )
			str = fileName.substr(0,pos);

		return str;
	}

	//----------------------------------------------------------
	//	Donne le nom du fichier sans l'extension
	//----------------------------------------------------------
	static const std::string GetNameWithoutExtension(const std::string& fileName) 
	{
		std::string name = fileName;

		std::string::size_type Pos = name.find_last_of("\\/");
		if (Pos != std::string::npos)
			name = name.substr(Pos + 1, std::string::npos);

		Pos = name.find_last_of("/");
		if (Pos != std::string::npos)
			name = name.substr(Pos + 1, std::string::npos);

		Pos = name.find_last_of(".");
		if (Pos != std::string::npos)
			return name.substr(0, Pos);

		return name;
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PathManager
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PathManager: public ScSingleton<PathManager>
{
friend class ScSingleton<PathManager>;
public:
	/////////////////////////////////////////////////////////////
	///
	/////////////////////////////////////////////////////////////
	void AddRootPath( const std::string & path)
	{
		if (path.empty() || (*path.rbegin() == '\\') || (*path.rbegin() == '/'))
			m_rootPath.push_back(path);
		else
			m_rootPath.push_back(path + "/");
	}

	/////////////////////////////////////////////////////////////
	///
	/////////////////////////////////////////////////////////////
	void AddPath( const std::string & path)
	{
		if (path.empty() || (*path.rbegin() == '\\') || (*path.rbegin() == '/'))
			m_path.push_back(path);
		else
			m_path.push_back(path + "/");
	}

    /////////////////////////////////////////////////////////////
	/// Cherche un fichier dans les répertoires de recherche
	///
	/// \param filename : Chemin du media
	///
	/// \return Chemin complet du media, exception si non trouvé
	///
	////////////////////////////////////////////////////////////
	std::string FindMedia(const std::string & fileName) const
	{
		std::string result = File::GetName(fileName);
		if (File::Exists(result))
			return result;

		// Parcours de la liste des chemins de recherche
		unsigned int j;
		for( j=0; j < m_rootPath.size(); j++)
		{
			for (std::vector<std::string>::const_iterator i = m_path.begin(); i != m_path.end(); ++i)
			{
				result = m_rootPath[j] + *i + fileName;
				if (File::Exists(result))
					return result;
			}
		}

#if defined _DEBUG || defined DEBUG || defined DEBUG_RELEASE 
		// Si le fichier est introuvable, on lance une exception
		//throw LoadingFailed(std::string(filename.GetFileName()), "Fichier introuvable dans les répertoires de recherche");
		std::string warning = "Fichier " + fileName + " introuvable dans les repertoires de recherche";
		LOG(warning);
#endif	// _DEBUG

		throw std::exception();

		return fileName;
	}
	

protected:
	/////////////////////////////////////////////////////////////
	/// 
	/////////////////////////////////////////////////////////////
	inline PathManager( void )
	{
		m_path.push_back("");
		m_rootPath.push_back("");
	};

	/////////////////////////////////////////////////////////////
	///destructor
	/////////////////////////////////////////////////////////////
	inline ~PathManager( void ){}

	//  a vector of string representing path to search files from
	std::vector<std::string>		m_path		;
	std::vector<std::string>		m_rootPath	;
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// liste des touches du clavier (la même que SDL)
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum Key
{
	/* The keyboard syms have been cleverly chosen to map to ASCII */
	KT_UNKNOWN		= 0,
	KT_FIRST		= 0,
	KT_BACKSPACE		= 8,
	KT_TAB		= 9,
	KT_CLEAR		= 12,
	KT_RETURN		= 13,
	KT_PAUSE		= 19,
	KT_ESCAPE		= 27,
	KT_SPACE		= 32,
	KT_EXCLAIM		= 33,
	KT_QUOTEDBL		= 34,
	KT_HASH		= 35,
	KT_DOLLAR		= 36,
	KT_AMPERSAND		= 38,
	KT_QUOTE		= 39,
	KT_LEFTPAREN		= 40,
	KT_RIGHTPAREN		= 41,
	KT_ASTERISK		= 42,
	KT_PLUS		= 43,
	KT_COMMA		= 44,
	KT_MINUS		= 45,
	KT_PERIOD		= 46,
	KT_SLASH		= 47,
	KT_0			= 48,
	KT_1			= 49,
	KT_2			= 50,
	KT_3			= 51,
	KT_4			= 52,
	KT_5			= 53,
	KT_6			= 54,
	KT_7			= 55,
	KT_8			= 56,
	KT_9			= 57,
	KT_COLON		= 58,
	KT_SEMICOLON		= 59,
	KT_LESS		= 60,
	KT_EQUALS		= 61,
	KT_GREATER		= 62,
	KT_QUESTION		= 63,
	KT_AT			= 64,
	/* 
	   Skip uppercase letters
	 */
	KT_LEFTBRACKET	= 91,
	KT_BACKSLASH		= 92,
	KT_RIGHTBRACKET	= 93,
	KT_CARET		= 94,
	KT_UNDERSCORE		= 95,
	KT_BACKQUOTE		= 96,
	KT_a			= 97,
	KT_b			= 98,
	KT_c			= 99,
	KT_d			= 100,
	KT_e			= 101,
	KT_f			= 102,
	KT_g			= 103,
	KT_h			= 104,
	KT_i			= 105,
	KT_j			= 106,
	KT_k			= 107,
	KT_l			= 108,
	KT_m			= 109,
	KT_n			= 110,
	KT_o			= 111,
	KT_p			= 112,
	KT_q			= 113,
	KT_r			= 114,
	KT_s			= 115,
	KT_t			= 116,
	KT_u			= 117,
	KT_v			= 118,
	KT_w			= 119,
	KT_x			= 120,
	KT_y			= 121,
	KT_z			= 122,
	KT_DELETE		= 127,
	/* End of ASCII mapped keysyms */

	/* International keyboard syms */
	KT_WORLD_0		= 160,		/* 0xA0 */
	KT_WORLD_1		= 161,
	KT_WORLD_2		= 162,
	KT_WORLD_3		= 163,
	KT_WORLD_4		= 164,
	KT_WORLD_5		= 165,
	KT_WORLD_6		= 166,
	KT_WORLD_7		= 167,
	KT_WORLD_8		= 168,
	KT_WORLD_9		= 169,
	KT_WORLD_10		= 170,
	KT_WORLD_11		= 171,
	KT_WORLD_12		= 172,
	KT_WORLD_13		= 173,
	KT_WORLD_14		= 174,
	KT_WORLD_15		= 175,
	KT_WORLD_16		= 176,
	KT_WORLD_17		= 177,
	KT_WORLD_18		= 178,
	KT_WORLD_19		= 179,
	KT_WORLD_20		= 180,
	KT_WORLD_21		= 181,
	KT_WORLD_22		= 182,
	KT_WORLD_23		= 183,
	KT_WORLD_24		= 184,
	KT_WORLD_25		= 185,
	KT_WORLD_26		= 186,
	KT_WORLD_27		= 187,
	KT_WORLD_28		= 188,
	KT_WORLD_29		= 189,
	KT_WORLD_30		= 190,
	KT_WORLD_31		= 191,
	KT_WORLD_32		= 192,
	KT_WORLD_33		= 193,
	KT_WORLD_34		= 194,
	KT_WORLD_35		= 195,
	KT_WORLD_36		= 196,
	KT_WORLD_37		= 197,
	KT_WORLD_38		= 198,
	KT_WORLD_39		= 199,
	KT_WORLD_40		= 200,
	KT_WORLD_41		= 201,
	KT_WORLD_42		= 202,
	KT_WORLD_43		= 203,
	KT_WORLD_44		= 204,
	KT_WORLD_45		= 205,
	KT_WORLD_46		= 206,
	KT_WORLD_47		= 207,
	KT_WORLD_48		= 208,
	KT_WORLD_49		= 209,
	KT_WORLD_50		= 210,
	KT_WORLD_51		= 211,
	KT_WORLD_52		= 212,
	KT_WORLD_53		= 213,
	KT_WORLD_54		= 214,
	KT_WORLD_55		= 215,
	KT_WORLD_56		= 216,
	KT_WORLD_57		= 217,
	KT_WORLD_58		= 218,
	KT_WORLD_59		= 219,
	KT_WORLD_60		= 220,
	KT_WORLD_61		= 221,
	KT_WORLD_62		= 222,
	KT_WORLD_63		= 223,
	KT_WORLD_64		= 224,
	KT_WORLD_65		= 225,
	KT_WORLD_66		= 226,
	KT_WORLD_67		= 227,
	KT_WORLD_68		= 228,
	KT_WORLD_69		= 229,
	KT_WORLD_70		= 230,
	KT_WORLD_71		= 231,
	KT_WORLD_72		= 232,
	KT_WORLD_73		= 233,
	KT_WORLD_74		= 234,
	KT_WORLD_75		= 235,
	KT_WORLD_76		= 236,
	KT_WORLD_77		= 237,
	KT_WORLD_78		= 238,
	KT_WORLD_79		= 239,
	KT_WORLD_80		= 240,
	KT_WORLD_81		= 241,
	KT_WORLD_82		= 242,
	KT_WORLD_83		= 243,
	KT_WORLD_84		= 244,
	KT_WORLD_85		= 245,
	KT_WORLD_86		= 246,
	KT_WORLD_87		= 247,
	KT_WORLD_88		= 248,
	KT_WORLD_89		= 249,
	KT_WORLD_90		= 250,
	KT_WORLD_91		= 251,
	KT_WORLD_92		= 252,
	KT_WORLD_93		= 253,
	KT_WORLD_94		= 254,
	KT_WORLD_95		= 255,		/* 0xFF */

	/* Numeric keypad */
	KT_KP0		= 256,
	KT_KP1		= 257,
	KT_KP2		= 258,
	KT_KP3		= 259,
	KT_KP4		= 260,
	KT_KP5		= 261,
	KT_KP6		= 262,
	KT_KP7		= 263,
	KT_KP8		= 264,
	KT_KP9		= 265,
	KT_KP_PERIOD		= 266,
	KT_KP_DIVIDE		= 267,
	KT_KP_MULTIPLY	= 268,
	KT_KP_MINUS		= 269,
	KT_KP_PLUS		= 270,
	KT_KP_ENTER		= 271,
	KT_KP_EQUALS		= 272,

	/* Arrows + Home/End pad */
	KT_UP			= 273,
	KT_DOWN		= 274,
	KT_RIGHT		= 275,
	KT_LEFT		= 276,
	KT_INSERT		= 277,
	KT_HOME		= 278,
	KT_END		= 279,
	KT_PAGEUP		= 280,
	KT_PAGEDOWN		= 281,

	/* Function keys */
	KT_F1			= 282,
	KT_F2			= 283,
	KT_F3			= 284,
	KT_F4			= 285,
	KT_F5			= 286,
	KT_F6			= 287,
	KT_F7			= 288,
	KT_F8			= 289,
	KT_F9			= 290,
	KT_F10		= 291,
	KT_F11		= 292,
	KT_F12		= 293,
	KT_F13		= 294,
	KT_F14		= 295,
	KT_F15		= 296,

	/* Key state modifier keys */
	KT_NUMLOCK		= 300,
	KT_CAPSLOCK		= 301,
	KT_SCROLLOCK		= 302,
	KT_RSHIFT		= 303,
	KT_LSHIFT		= 304,
	KT_RCTRL		= 305,
	KT_LCTRL		= 306,
	KT_RALT		= 307,
	KT_LALT		= 308,
	KT_RMETA		= 309,
	KT_LMETA		= 310,
	KT_LSUPER		= 311,		/* Left "Windows" key */
	KT_RSUPER		= 312,		/* Right "Windows" key */
	KT_MODE		= 313,		/* "Alt Gr" key */
	KT_COMPOSE		= 314,		/* Multi-key compose key */

	/* Miscellaneous function keys */
	KT_HELP		= 315,
	KT_PRINT		= 316,
	KT_SYSREQ		= 317,
	KT_BREAK		= 318,
	KT_MENU		= 319,
	KT_POWER		= 320,		/* Power Macintosh power key */
	KT_EURO		= 321,		/* Some european keyboards */
	KT_UNDO		= 322,		/* Atari keyboard has Undo */

	/* Add any other keys here */
	KT_KP_DECIMAL = 323, 
	KT_LAST
} ;



};





namespace Sc
{

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// srtucture contenant les evenements claviers
// on regroupe les évenements liés à la souris dans une structure  
// la structure UserEvents est utilisée par l'objet interface utilisateur pour récupérer les données et
// exploitée par la caméra du renderer (la caméra bouge en fonction de la souris)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ScUserEvents: public ScSingleton<ScUserEvents>
{
friend class ScSingleton<ScUserEvents >;
public:
	int mousex,mousey		;
	int mouselx,mousely		;
	int mouseXrel,mouseYrel	;
	/* rappel :
		SDL_BUTTON_LEFT			1
		SDL_BUTTON_MIDDLE		2
		SDL_BUTTON_RIGHT		3
		SDL_BUTTON_WHEELUP		4
		SDL_BUTTON_WHEELDOWN	5
	*/
	std::vector<unsigned int> mousebuttons			;
	std::vector<unsigned int> mousebuttonsRelease	;


	//----------------------------------------------------------
	// constructeur
	//----------------------------------------------------------
	ScUserEvents()
		:mousebuttons(6), mousebuttonsRelease(6)
	{
		mousebuttons[0] = mousebuttons[1] = mousebuttons[2] = mousebuttons[3] = mousebuttons[4] = mousebuttons[5] = 0;
		mousebuttonsRelease[0] = mousebuttonsRelease[1] = mousebuttonsRelease[2] = mousebuttonsRelease[3] = mousebuttonsRelease[4] = mousebuttonsRelease[5] = 0;

		m_keyBoardEvents.resize(KT_LAST);
		m_keyBoardTimes = std::vector<unsigned int>(KT_LAST, 0);

		CreateChar();
	}

	void MouseButtonClear()
	{
		mousebuttonsRelease[0] = mousebuttonsRelease[1] = mousebuttonsRelease[2] = mousebuttonsRelease[3] = mousebuttonsRelease[4] = mousebuttonsRelease[5] = 0;
	}

	//----------------------------------------------------------
	// on vide la liste d'évenements a chaque frame 
	//----------------------------------------------------------
	inline void Clear()
	{
		m_keys.clear();
		m_charKeys.clear();
	}

	//----------------------------------------------------------
	// on enleve un élément de la liste
	//----------------------------------------------------------
	void Remove(Key key)
	{
		if( m_keys.size() )
		{
			std::vector<Key>::iterator it;
			for(it = m_keys.begin(); it != m_keys.end(); it++)
				if((*it) == key){ m_keys.erase(it); return;}
		}
	}

	//----------------------------------------------------------
	// retire un char de la liste
	//----------------------------------------------------------
	void RemoveChar(char key)
	{
		if( m_charKeys.size() )
		{
			std::vector<char>::iterator it;
			for(it = m_charKeys.begin(); it != m_charKeys.end(); it++)
				if((*it) == key){ m_charKeys.erase(it); return;}
		}
	}

	//----------------------------------------------------------
	// Ajout d'un nouvel événemzent
	//----------------------------------------------------------
	inline void Add(Key key)
	{
		m_keys.push_back(key);
	}

	//----------------------------------------------------------
	// une touche a t elle ete presse entre 2 frames
	//----------------------------------------------------------
	bool IsPressed(Key key) const
	{
		unsigned int i;
		for(i = 0 ; i< m_keys.size(); i++)
		{
			if(key == m_keys[i])
				return true;
		}

		return (m_keyBoardEvents[(unsigned int)(key)] && m_keyBoardTimes[(unsigned int)(key)]<1 || m_keyBoardTimes[(unsigned int)(key)]>20) ;
	}

	void ClearKeyBardEvents()
	{
		unsigned int i;
		for(i=0;i<m_keyBoardEvents.size(); i++)
		{
			if(m_keyBoardEvents[i] == false)
				m_keyBoardTimes[i] = 0;
			else
				m_keyBoardTimes[i]++;
		}
	}

	void CreateChar()
	{
		m_characters[KT_a] = 'a';
		m_characters[KT_b] = 'b';
		m_characters[KT_c] = 'c';
		m_characters[KT_d] = 'd';
		m_characters[KT_e] = 'e';
		m_characters[KT_f] = 'f';
		m_characters[KT_g] = 'g';
		m_characters[KT_h] = 'h';
		m_characters[KT_i] = 'i';
		m_characters[KT_j] = 'j';
		m_characters[KT_k] = 'k';
		m_characters[KT_l] = 'l';
		m_characters[KT_m] = 'm';
		m_characters[KT_n] = 'n';
		m_characters[KT_o] = 'o';
		m_characters[KT_p] = 'p';
		m_characters[KT_q] = 'q';
		m_characters[KT_r] = 'r';
		m_characters[KT_s] = 's';
		m_characters[KT_t] = 't';
		m_characters[KT_u] = 'u';
		m_characters[KT_v] = 'v';
		m_characters[KT_w] = 'w';
		m_characters[KT_x] = 'x';
		m_characters[KT_y] = 'y';
		m_characters[KT_z] = 'z';
		
		m_characters[KT_KP0] = '0';
		m_characters[KT_KP1] = '1';
		m_characters[KT_KP2] = '2';
		m_characters[KT_KP3] = '3';
		m_characters[KT_KP4] = '4';
		m_characters[KT_KP5] = '5';
		m_characters[KT_KP6] = '6';
		m_characters[KT_KP7] = '7';
		m_characters[KT_KP8] = '8';
		m_characters[KT_KP9] = '9';

		m_characters[KT_SPACE] = ' ';
		m_characters[KT_SEMICOLON] = '.';
		m_characters[KT_KP_DECIMAL] = '.';
	}	
	std::map<Key, char> m_characters;

	bool CharExist(Key k)
	{
		std::map<Key, char>::const_iterator it = m_characters.find(k);
		if(it != m_characters.end())
			return true;
		return false;
	}

	//----------------------------------------------------------
	// variable membre
	//----------------------------------------------------------
	std::vector<char>			m_charKeys		;	///< chars correspondants aux touches préssées
	std::vector<Key>			m_keys			;	///< touche pressée pour l'image actuelle
	std::vector<unsigned int>	m_keyBoardTimes	;
	std::vector<bool>			m_keyBoardEvents;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// la structure WindowSetup est utilisée par l'objet interface utilisateur pour récupérer les données et
// exploitée par le renderer (on calcul la matrice de projection en fonction de la taille de la fenêtre)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ScWindowSetup: public ScSingleton<ScWindowSetup>
{
friend class ScSingleton<ScWindowSetup >;
public:
	ScWindowSetup()
		:m_width(800), m_height(640) {}
	ScWindowSetup(int width, int height)
		:m_width(width), m_height(height) {}

	inline void SetSize(int width, int height){m_width = width; m_height = height;}

	int			m_width, m_height	;
	bool		m_videoResize		;	///< if window has been resized during scene
};


//----------------------------------------------------------
// fonction d'aide qui convertit tout en string
//----------------------------------------------------------
template<class T>
static std::string ToString(const T& value)
{
	std::stringstream ss; 
	ss << value; 

	return ss.str();
}

//----------------------------------------------------------
// décompose un string en une sous liste de string 
//----------------------------------------------------------
static void Split(const std::string& String, const std::string& Delim, std::vector<std::string>& Tokens)
{
	// Effacement du vector dans lequel on va mettre les résultats
	//Tokens.clear();
	std::string tmp;
	// Parcours de la chaîne et extraction des tokens un à un
	for (std::string::size_type p1 = 0, p2 = 0; p1 != std::string::npos; )
	{
		p1 = String.find_first_not_of(Delim, p1);
		if (p1 != std::string::npos)
		{
			p2 = String.find_first_of(Delim , p1);
			tmp = String.substr(p1, p2 - p1);
			if(tmp != " ")
				Tokens.push_back(tmp);
			p1 = p2;
		}
	}
}

//----------------------------------------------------------
// décompose un string en une sous liste de string 
//----------------------------------------------------------
static void Split(const std::string& String, const std::vector<std::string>& Delim, std::vector<std::string>& Tokens)
{
	// Effacement du vector dans lequel on va mettre les résultats
	Tokens.clear();
	std::vector<std::string> TokensTmp;
	TokensTmp.push_back(String);
	// Parcours de la chaîne et extraction des tokens un à un
	unsigned int i,j;
	for(i=0; i<Delim.size(); i++)
	{
		Tokens.clear();
		for(j=0; j<TokensTmp.size(); j++)
			Split(TokensTmp[j], Delim[i], Tokens);
		TokensTmp = Tokens;
	}
}


};




//FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>




namespace Sc 
{

////////////////////////////////////////////////////////////////////////
/// classe Demo:
////////////////////////////////////////////////////////////////////////
class ScFontRenderer: public ScSingleton<ScFontRenderer>
{
friend class ScSingleton<ScFontRenderer >;
public:
	//----------------------------------------------------------
	// constructeur 
	//----------------------------------------------------------
	ScFontRenderer()
		:m_init(false)
	{
		
		try{
			Create();
			m_init = true;
		}
		catch(std::exception e){}
	}

	//----------------------------------------------------------
	// Destructeur
	//----------------------------------------------------------
	virtual ~ScFontRenderer()
	{	
		Release();
	}
	
	void AddStringToDisplay(const Vec3& pos, const std::string& str, unsigned int height, bool To2D = false, Vec3& displacement = Vec3(0.0f))
	{
		if(!m_init) return;
		if(!To2D)
		{
			std::vector<std::string> tokens, delim;
			delim.push_back("\n"); delim.push_back("/"); 
			Split(str, delim, tokens);

			float y(pos.y), x(pos.x), z(pos.z);
			if(x<0) x += ScWindowSetup::GetInstance().m_width ;
			if(y<0) y += ScWindowSetup::GetInstance().m_height;
	
			unsigned int i;
			for(i=0; i<tokens.size(); i++)
				m_toDisplayList.push_back(ToDisplay(Vec3(x, y + float(i)*(height+height/2.0f), z), height, tokens[i], To2D));
		}
		else
			m_toDisplayList.push_back(ToDisplay(pos, height, str, To2D, displacement));
	}

	void Display();

	void AddLegendRectangle(unsigned int x, unsigned int y, unsigned w, unsigned h, const Vec3& color)
	{
		m_legends.push_back(std::pair<Rect<unsigned int>, Vec3>(Rect<unsigned int>(x,y,w,h), color));
	}

private:

	//----------------------------------------------------------
	//  
	//----------------------------------------------------------
	void Create();

	//----------------------------------------------------------
	//  
	//----------------------------------------------------------
	inline unsigned int CreateFont(const char* policyName, unsigned int height, glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f))
	{
		m_fontDatas.push_back(new FontData(PathManager::GetInstance().FindMedia(policyName).c_str(), height, color));
		m_heigtFontDatas[float(height)] = m_fontDatas[m_fontDatas.size()-1];
		return m_fontDatas.size()-1;
	}

	void Release()
	{	
		std::for_each(m_fontDatas.begin(), m_fontDatas.end(), STLDelete());
		m_fontDatas.clear();
	}

	//----------------------------------------------------------
	// Display directly info to  
	//----------------------------------------------------------
	void DisplayWord(float x, float y, const std::string& text, unsigned int fontIndex);

	//----------------------------------------------------------
	//This holds all of the information related to any
	//freetype font that we want to create.  
	//----------------------------------------------------------
	class FontData 
	{
	public:
		float			m_height	;	///< Holds the height of the font.
		unsigned int	m_listBase	;	///< Holds the first display list id
		float			m_faceTrX	;

		//----------------------------------------------------------
		// constructeur 
		//----------------------------------------------------------
		FontData(const char* policyName, unsigned int height, const glm::vec3& color)
			:m_color(color)
		{
			Init(policyName, height);
		}

		//----------------------------------------------------------
		// constructeur 
		//----------------------------------------------------------
		~FontData()
		{
			delete[] m_textures;
			m_textures = 0;
		}

		//----------------------------------------------------------
		//The init function will create a font of the height 'height' 
		//from the file policyName.
		//----------------------------------------------------------
		void Init(const char* policyName, unsigned int height);

		//----------------------------------------------------------
		//Free all the resources assosiated with the font.
		//----------------------------------------------------------
		void Clean();

	private:
		unsigned int*	m_textures;		///< Holds the texture id's 
		glm::vec3		m_color;		///< font color

		//----------------------------------------------------------
		// Create a display list coresponding to the give character.
		//----------------------------------------------------------
		void CreateCharacterDisplayList( FT_Face face, char ch, unsigned int list_base, unsigned int* tex_base );

		//----------------------------------------------------------
		///This function gets the first power of 2 >= the
		///int that we pass it.
		//----------------------------------------------------------
		inline int next_p2 ( int a )
		{
			int rval=1;
			while(rval<a) rval<<=1;
			return rval;
		}
	};

	bool	m_init;
	static	std::vector<ScFontRenderer::FontData*> CreateFont();
	static	std::vector<FontData*>		m_fontDatas; ///< index of fonts
	static	std::map<float, FontData*>	m_heigtFontDatas; ///< index of fonts

	struct ToDisplay
	{
		ToDisplay(const Vec3& pos, unsigned int height, const std::string& str, bool To2D)
			:m_position(pos), m_height(height), m_string(str), m_To2D(To2D)  {}
		ToDisplay(const Vec3& pos, unsigned int height, const std::string& str, bool To2D, const Vec3& displacement2d)
			:m_position(pos), m_height(height), m_string(str), m_To2D(To2D), m_2dDisplacement(displacement2d)  {}
		~ToDisplay(){}

		Vec3			m_position		;
		unsigned int	m_height		; 
		std::string		m_string		;
		bool			m_To2D			;
		Vec3			m_2dDisplacement;
	};
	std::vector<ToDisplay>		m_toDisplayList;

	std::vector<std::pair<Rect<unsigned int>, Vec3> > m_legends;
};


};

#endif // __SC_COMMON_H__ fin de la routine