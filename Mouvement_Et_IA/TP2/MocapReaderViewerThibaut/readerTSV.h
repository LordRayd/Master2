#ifndef __readerTSV_H__	
#define __readerTSV_H__	



#include <string>
#include <iostream>
#include <fstream>
#include "Marker.h"
using namespace std;


struct FrameInfo
{
	unsigned int	m_frameIndex;
	double			m_time;
	string			m_smpte;
};

class readerTSV{
public:
	bool loadTSV(const char* fileName);
	vector<Marker*>		markers;
	vector<string>		marker_names;
	vector<FrameInfo>	m_frameInfos	;
	string header;

private:
	bool SkipCommentLine(istream& is);
	void SkipLine(istream& is);
	bool ProcessLine(istream& is);

	enum Task { none, readdataV1,  readdataV2};
    Task task;
};

#endif