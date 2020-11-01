
#include "readerTSV.h"
#include <fstream>
#include <sstream>
#if defined linux || defined __linux__ || defined __linux
	#include <string.h>
#endif

//----------------------------------------------------------
// split a string with space & tab  into word list
//	\param the string we want split
//	\return a string vector splited
//----------------------------------------------------------
static std::vector<std::string> Split(std::string& line)
{
	std::vector<std::string> v;
	std::istringstream ss1(line); 
	std::string w1, w2;
	while ( std::getline( ss1, w1, ' ') )
	{
		std::istringstream ss2(w1); 
		while( std::getline( ss2, w2, '\t') )
		{
			if(( w2 != "\t") && (w2 != ""))
				v.push_back( w2 );
		}
	}

	return v;
}


bool readerTSV::loadTSV(const char* fileName)
{
	//-------------------------------------------------------------------------
	// check if file is valid
	//-------------------------------------------------------------------------
	if(fileName == NULL) {std::cout << "file is invalid " << fileName << std::endl;  return false;}
	std::string strFileName(fileName);
	if(strFileName == "") {std::cout << "file is invalid " << fileName << std::endl;  return false;}

	ifstream ifs(fileName, ios::in|ios::ate);
	if (!ifs)
	{
		if (strlen(fileName) > 0)
		std::cout << "file not found: " << fileName << std::endl; 
		return false;
	}

	const int fileSize = (int) ifs.tellg();    //  get the file size (we started at the end)...
	ifs.seekg (0, ios::beg);			 //  ...then get back to start
	
	if (!fileSize)
	{
		std::cout << "file is empty: " << fileName << std::endl; 
		return false;
	}

	std::cout << "Be Sure that values of the missing trajectories have 0.0 and not NAN" << std::endl; 


	//-------------------------------------------------------------------------
	// second pass for header
	//-------------------------------------------------------------------------
	ifs.clear();
	ifs.seekg (0, ios::beg);
	header = "";
	unsigned int i = 0;
	while (!ifs.eof())
	{
		i++;
		string sLine = "";
		getline(ifs, sLine);
		header += sLine;
		header += "\n";
		if (i >= 10) 
		{
			sLine = "";
			getline(ifs, sLine);
			std::vector<std::string> words = Split(sLine);
			if(words.size() && words[0] == "Frame")
			{
				header += sLine;
				header += "\n";
				i++;
			}
			break;
		}
	}


	//-------------------------------------------------------------------------
	// first pass for data
	//-------------------------------------------------------------------------
	ifs.clear();
	ifs.seekg (0, ios::beg);
	markers.clear();
	task = none;
	unsigned int lineIndex(0);
	while(SkipCommentLine(ifs)) 
	{
		if(lineIndex == i && i == 10)
			task = readdataV1;
		if(lineIndex == i && i == 11)
			task = readdataV2;
		if (!ProcessLine(ifs))
			break;
		lineIndex++;
	}

	

	//-------------------------------------------------------------------------
	// set bind pose data
	//-------------------------------------------------------------------------
	for(i=0; i<markers.size() ;i++)
		if(markers[i]->pos.size())
			markers[i]->bindPose = markers[i]->pos[0];

	//-------------------------------------------------------------------------
	// close file
	//-------------------------------------------------------------------------
	ifs.close();

	return true; 
}

//	skip a comment line
bool readerTSV::SkipCommentLine(istream& is)
{
	char next;
	while( is >> std::skipws >> next ) 
    {
		is.putback(next);
		return true;
    }
    return false;
}

//	skip a line
void readerTSV::SkipLine(istream& is)
{
    char next;
	is >> std::noskipws;
    while( (is >> next) && ('\n' != next) );
}


// load a line
bool readerTSV::ProcessLine(istream& is)
{
	string currentString, str;
	float x,y,z;
	
	if (task == readdataV1)
	{
		for(unsigned int i = 0; i < markers.size(); i++){
			is >> x >> y >> z;
			markers[i]->pos.push_back(Vec3f(x,y,z));
			if((abs(x)<std::numeric_limits<float>::epsilon()) && (abs(y)<std::numeric_limits<float>::epsilon()) && (abs(z)<std::numeric_limits<float>::epsilon())){
				markers[i]->interpolation.push_back(TODO);
			}
			else{
				markers[i]->interpolation.push_back(NA);
			}
		}
		
	}

	else if (task == readdataV2)
	{
		FrameInfo fInfo;
		is >> fInfo.m_frameIndex >> fInfo.m_time >> fInfo.m_smpte;
		m_frameInfos.push_back(fInfo);
		for(unsigned int i = 0; i < markers.size(); i++)
		{
			is >> x >> y >> z;
			markers[i]->pos.push_back(Vec3f(x,y,z));
			if((abs(x)<std::numeric_limits<float>::epsilon()) && (abs(y)<std::numeric_limits<float>::epsilon()) && (abs(z)<std::numeric_limits<float>::epsilon())){
				markers[i]->interpolation.push_back(TODO);
			}
			else{
				markers[i]->interpolation.push_back(NA);
			}
		}
		
	}

	else if (!(is >> currentString))
	{
		return false;
	}
	
	else if ("NO_OF_MARKERS" == currentString) 
	{
		unsigned int nb_markers;
		is >> nb_markers;
		for(unsigned int i = 0; i < nb_markers; i++){
			markers.push_back(new Marker());
			markers[i]->index = i;
		}
	}

	else if ("MARKER_NAMES" == currentString)		
	{			
		for(unsigned int i = 0; i < markers.size(); i++)
		{
			is >> str;
			marker_names.push_back(str);
			markers[i]->m_name = str;
		}
		//SkipLine(is);
	}
	else
		SkipLine(is);

	return true;
}
