//========================================================================
// TP Animation / Synthese image
//
//------------------------------------------------------------------------
// Copyright (c) 2010-2011 Equipe SAMSARA (T. LE NAOUR, N. COURTY, S. GIBET)
// VALORIA - UBS 
//========================================================================


#include "AnimatedChain.h"
#include <iostream>
#include <fstream>

using namespace std;
#pragma warning( disable : 4996 )


enum  ChannelEnum
{
	X_ROTATION, Y_ROTATION, Z_ROTATION,
	X_POSITION, Y_POSITION, Z_POSITION
};

struct  Channel
{
	ChainJoint *         joint;
	ChannelEnum          type;
	int                  index;
};


//----------------------------------------------------------
// charge l'animation à partir d'un fichier bvh
//----------------------------------------------------------
AnimatedChain::AnimatedChain(const std::string& fileName)
{
	m_name = File::GetNameWithoutExtension(fileName);


	vector< vector<Channel *> >     channels;   

	#define  BUFFER_LENGTH  1024*16

	ifstream  			file;
	char      			line[ BUFFER_LENGTH ];
	char *    			token;
	char      			separater[] = " :,\t";
	vector< ChainJoint * >   joint_stack;
	ChainJoint *   			joint = NULL;
	ChainJoint *   			new_joint = NULL;
	bool      			is_site = false;
	float				x, y ,z;
	unsigned int       	i, j, k;


	std::string file_name = fileName;
	const char *  mn_first = fileName.c_str();
	const char *  mn_last = fileName.c_str() + strlen( fileName.c_str() );
	if ( strrchr( fileName.c_str(), '\\' ) != NULL )
		mn_first = strrchr( fileName.c_str(), '\\' ) + 1;
	else if ( strrchr( fileName.c_str(), '/' ) != NULL )
		mn_first = strrchr( fileName.c_str(), '/' ) + 1;
	if ( strrchr( fileName.c_str(), '.' ) != NULL )
		mn_last = strrchr( fileName.c_str(), '.' );
	if ( mn_last < mn_first )
		mn_last = fileName.c_str() + strlen( fileName.c_str() );

	std::string motion_name; motion_name.assign( mn_first, mn_last );


	file.open( fileName.c_str(), ios::in );
	if ( file.is_open() == 0 )  return;





	//------------------------------------------------------------------------
	// load Skeleton hierarchy
	//------------------------------------------------------------------------
	unsigned int jointIndex(0);
	while ( ! file.eof() )
	{
		if ( file.eof() )  return;

		file.getline( line, BUFFER_LENGTH );
		token = strtok( line, separater );

		if ( token == NULL )  continue;


		if ( strcmp( token, "{" ) == 0 )
		{
			joint_stack.push_back( joint );
			joint = new_joint;
			continue;
		}
		
		if ( strcmp( token, "}" ) == 0 )
		{
			joint = joint_stack.back();
			joint_stack.pop_back();
			is_site = false;
			continue;
		}

		if ( ( strcmp( token, "ROOT" ) == 0 ) ||
		     ( strcmp( token, "JOINT" ) == 0 ) )
		{
			new_joint = new ChainJoint();
			new_joint->m_index = jointIndex; jointIndex++;
			if ( joint ) joint->addChild(new_joint);
			else m_rootJoint = new_joint;

			token = strtok( NULL, "" );
			while ( *token == ' ' )  token ++;
			new_joint->m_name = token;

			m_animatedJoints.push_back(std::pair<ChainJoint*, Transformations>());
			m_animatedJoints[m_animatedJoints.size()-1].first = new_joint;
			channels.push_back(std::vector<Channel*>() );
			continue;
		}

		if ( ( strcmp( token, "End" ) == 0 ) )
		{
			//new_joint = joint;
			is_site = true;
			//continue;
		}

		if ( strcmp( token, "OFFSET" ) == 0 )
		{
			token = strtok( NULL, separater );
			x = float(token ? atof( token ) : 0.0f);
			token = strtok( NULL, separater );
			y = float(token ? atof( token ) : 0.0f);
			token = strtok( NULL, separater );
			z = float(token ? atof( token ) : 0.0f);
			
			if ( is_site )
			{
				//joint->m_BPtranslation.x = joint->m_translation.x = x;
				//joint->m_BPtranslation.y = joint->m_translation.y = y;
				//joint->m_BPtranslation.z = joint->m_translation.z = z;
			}
			else
			{
				joint->m_BPtranslation.x = joint->m_translation.x = x;
				joint->m_BPtranslation.y = joint->m_translation.y = y;
				joint->m_BPtranslation.z = joint->m_translation.z = z;
			}
			continue;
		}


		if ( strcmp( token, "CHANNELS" ) == 0 )
		{
	
			token = strtok( NULL, separater );
			//joint->channels.resize( token ? atoi( token ) : 0 );

			unsigned int channelCount(token ? atoi( token ) : 0);
			for ( i=0; i<channelCount; i++ )
			{
				Channel *  channel = new Channel();
				channel->joint = joint;
				channel->index = channels.size();
				channels[channels.size()-1].push_back( channel );

				token = strtok( NULL, separater );
				if ( strcmp( token, "Xrotation" ) == 0 )
					channel->type = X_ROTATION;
				else if ( strcmp( token, "Yrotation" ) == 0 )
					channel->type = Y_ROTATION;
				else if ( strcmp( token, "Zrotation" ) == 0 )
					channel->type = Z_ROTATION;
				else if ( strcmp( token, "Xposition" ) == 0 )
					channel->type = X_POSITION;
				else if ( strcmp( token, "Yposition" ) == 0 )
					channel->type = Y_POSITION;
				else if ( strcmp( token, "Zposition" ) == 0 )
					channel->type = Z_POSITION;
			}
		}

		if ( strcmp( token, "MOTION" ) == 0 )
			break;
	}

	


	//------------------------------------------------------------------------
	// load animation
	//------------------------------------------------------------------------
	file.getline( line, BUFFER_LENGTH );
	token = strtok( line, separater );
	if ( strcmp( token, "Frames" ) != 0 )  return;
	token = strtok( NULL, separater );
	if ( token == NULL )  return;
	
	unsigned int num_frame = atoi( token );

	file.getline( line, BUFFER_LENGTH );
	token = strtok( line, ":" );
	if ( strcmp( token, "Frame Time" ) != 0 )  return;
	token = strtok( NULL, separater );
	if ( token == NULL )  return;
	m_timeStep = atof( token );

	unsigned int num_channel = channels.size();
	//motion = new double[ num_frame * num_channel ];


	// pour toutes les frames
	for ( i=0; i<num_frame; i++ )
	{
		file.getline( line, BUFFER_LENGTH );
		token = strtok( line, separater );

		// pour tous les joints
		for ( j=0; j<num_channel; j++ )
		{
			// on ajoute une nouvelle transformation au joint
			m_animatedJoints[j].second.push_back(Transformation());

			float xRot(0.0f), yRot(0.0f), zRot(0.0f);
			float val(0.0f);
			bool pos(false);
			Quaternion q;

			// pour tous les channels de chaque joint
			for(k=0; k<channels[j].size(); k++)
			{
				if ( token == NULL ) 
					return;
				
				val = float(token ? atof( token ) : 0.0f);

				
				if(channels[j][k]->type == X_POSITION)
				{m_animatedJoints[j].second[m_animatedJoints[j].second.size()-1].m_translation.x = val; pos = true;}
				if(channels[j][k]->type == Y_POSITION)
				{m_animatedJoints[j].second[m_animatedJoints[j].second.size()-1].m_translation.y = val; pos = true;}
				if(channels[j][k]->type == Z_POSITION)
				{m_animatedJoints[j].second[m_animatedJoints[j].second.size()-1].m_translation.z = val; pos = true;}
					
				if(!pos)
				{
					m_animatedJoints[j].second[m_animatedJoints[j].second.size()-1].m_translation.x = m_animatedJoints[j].first->m_translation.x;
					m_animatedJoints[j].second[m_animatedJoints[j].second.size()-1].m_translation.y = m_animatedJoints[j].first->m_translation.y;
					m_animatedJoints[j].second[m_animatedJoints[j].second.size()-1].m_translation.z = m_animatedJoints[j].first->m_translation.z;
				}

				
				if(channels[j][k]->type == X_ROTATION) {xRot = val; q = q * Quaternion(Vec3(1.0,0.0,0.0), xRot*(float(3.14159265)/180.0f));}
				if(channels[j][k]->type == Y_ROTATION) {yRot = val; q = q * Quaternion(Vec3(0.0,1.0,0.0), yRot*(float(3.14159265)/180.0f));}
				if(channels[j][k]->type == Z_ROTATION) {zRot = val; q = q * Quaternion(Vec3(0.0,0.0,1.0), zRot*(float(3.14159265)/180.0f));}
				
				token = strtok( NULL, separater );
			}

			m_animatedJoints[j].second[m_animatedJoints[j].second.size()-1].m_rotation = q;
		}
	}

	for(i=0;i<channels.size(); i++)
	{
		std::for_each(channels[i].begin(), channels[i].end(), STLDelete());
		channels[i].clear();
	}
	channels.clear();

	file.close();
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// Export a motion to bvh file
// http://www.character-studio.net/bvh_file_specification.htm
//
///////////////////////////////////////////////////////////////////////////////////////////////
void AnimatedChain::ExportHierarchyToBvh(ofstream& outFile, ChainJoint* joint, unsigned int depth, const std::vector<unsigned int>& jointToExport, std::vector<unsigned int>& jointExported)
{
	unsigned int i;
	std::string escape;
	for(i=0;i<depth;i++)
		escape += "\t";

	if(Helpers::In(jointToExport, joint->m_index))
	{
		jointExported.push_back(joint->m_index);
		
		outFile << escape << "JOINT " << /*"BVH:"<<*/joint->m_name << endl;
		outFile << escape << "{" << endl;

		outFile << "\t" << escape << "OFFSET "	<< joint->m_BPtranslation.x << " " 
												<< joint->m_BPtranslation.y << " " 
												<< joint->m_BPtranslation.z << endl;

		outFile << "\t" << escape << "CHANNELS " << 3 << " Xrotation Yrotation Zrotation"<< endl;
	}
	

	for(i=0;i<joint->m_child.size();i++)
		ExportHierarchyToBvh(outFile, joint->m_child[i], depth+1, jointToExport,jointExported);
	
	if(Helpers::In(jointToExport, joint->m_index))
	{
		outFile << escape << "}" << endl;
	}
}

void AnimatedChain::ExportHierarchyToBvh(ofstream& outFile, ChainJoint* joint, unsigned int depth, std::vector<unsigned int>& jointExported)
{
	jointExported.push_back(joint->m_index);
	std::string escape;
	unsigned int i;
	for(i=0;i<depth;i++)
		escape += "\t";

	outFile << escape << "JOINT " << /*"BVH:"<<*/joint->m_name << endl;
	outFile << escape << "{" << endl;

	outFile << "\t" << escape << "OFFSET "	<< joint->m_BPtranslation.x << " " 
											<< joint->m_BPtranslation.y << " " 
											<< joint->m_BPtranslation.z << endl;

	outFile << "\t" << escape << "CHANNELS " << 3 << " Xrotation Yrotation Zrotation"<< endl;
	
	for(i=0;i<joint->m_child.size();i++)
		ExportHierarchyToBvh(outFile, joint->m_child[i], depth+1, jointExported);

	outFile << escape << "}" << endl;
}

unsigned int AnimatedChain::GetBestRoot(unsigned int currentRoot, const std::vector<unsigned int>& jointToExport)
{
	//unsigned int i, bestRoot(0);
	//ChainJoint* joint = m_animatedJoints[currentRoot].first;
	//for(i=0;i<joint->m_child.size();i++)
	//	if(bestRoot = GetBestRoot(joint->m_index, jointToExport))
	//		return bestRoot;
	return 0;
}

void ToEulerAngles( const Quaternion& q, float &_x, float &_y, float &_z ) 
{
	// Compute Y-axis angle
	_y = asin( 2.0f * ((q.x * q.z) + (q.w * q.y)) );

	// Compute cos and one over cos for optimization
	float cy = cos( _y );
	float oneOverCosY = 1.0f / cy;

	if( fabs( cy ) > 0.001 ) 
	{
		// No gimball lock
		_x = atan2( 2.0f *	((q.w * q.x) - (q.y * q.z)) * oneOverCosY,
							(1.0f - 2.0f * (q.x*q.x + q.y*q.y)) * oneOverCosY );
		_z = atan2( 2.0f *	((q.w * q.z) - (q.x * q.y)) * oneOverCosY,
							(1.0f - 2.0f * (q.y*q.y + q.z*q.z)) * oneOverCosY );
	}
	else 
	{
		// Gimbal lock case
		_x = 0.0f;
		_z = atan2( 2.0f * ((q.x * q.y) + (q.w * q.z)),
						1.0f - 2.0f * (q.x*q.x + q.z*q.z) );
	}
}

void AnimatedChain::ExportMotionToBvh(const string& filename, const std::vector<unsigned int>& jointToExport, unsigned int in, unsigned int out)
{
	unsigned int i, j;
	
	// defines a stream to be feed
	ofstream outFile(filename.c_str());


	// ------------------------------------------------------------------
	// First, export bind pose (RELATIVE & TRANSLATION FIRST)
	// ------------------------------------------------------------------
	// BVH HIERARCHY
	unsigned int jointIndex = jointToExport[0];
	ChainJoint* joint(m_animatedJoints[jointIndex].first);
	
	outFile << "HIERARCHY" << endl;
	outFile << "ROOT " << /*"BVH:"<<*/joint->m_name << endl;
	outFile << "{" << endl;
	outFile << "\t" << "OFFSET "<< joint->m_BPtranslation.x << " " << joint->m_BPtranslation.y << " " << joint->m_BPtranslation.z << endl;
	outFile << "\t" << "CHANNELS " << 6 << " Xposition Yposition Zposition Xrotation Yrotation Zrotation"<< endl;
	
	std::vector<unsigned int> jointExported;
	jointExported.push_back(joint->m_index);
	for(i=0;i<joint->m_child.size();i++)
		ExportHierarchyToBvh(outFile, joint->m_child[i], 1, jointToExport, jointExported);

	outFile << "}" << endl;


	// ------------------------------------------------------------------
	// Next, export motion
	// ------------------------------------------------------------------
	// BVH MOTION
	outFile << "MOTION" << endl ;
	outFile << "Frames: " << out - in<< endl ;
	outFile << "Frame Time: "<< m_timeStep << endl ;
	
	// declare tempory variable
	Quaternion		currentRotation	;
	Translationf		currentPosition	;
	float			rX(0.0f), rY(0.0f), rZ(0.0f);

	// export for each frame 
	for (i = in; i < out; i++)
	{
		// export for each joints 
		for ( j = 0; j<jointExported.size(); j++ )
		{
			// export for each channels
			currentRotation = m_animatedJoints[jointExported[j]].second[i].m_rotation		;
			currentPosition = m_animatedJoints[jointExported[j]].second[i].m_translation	;

			// for root joint: 6 channels
			if( !j ) 
			{
				outFile << currentPosition.x << " " << currentPosition.y << " " << currentPosition.z << " ";
			}
			
			ToEulerAngles(currentRotation, rX, rY, rZ);
			outFile << rX*180.0f/3.14159265 << " " << rY*180.0f/3.14159265 << " " << rZ*180.0f/3.14159265 << " ";
		}
		outFile << endl;
	}
}


void AnimatedChain::ExportMotionToBvh(const string& filename, unsigned int in, unsigned int out, unsigned int jointIndex)
{
	if( in < m_animatedJoints[0].second.size() && out < m_animatedJoints[0].second.size())
	{
		unsigned int i, j;
	
		// defines a stream to be feed
		ofstream outFile(filename.c_str());


		// ------------------------------------------------------------------
		// First, export bind pose (RELATIVE & TRANSLATION FIRST)
		// ------------------------------------------------------------------
		// BVH HIERARCHY
		ChainJoint* joint(m_animatedJoints[jointIndex].first);
	
		outFile << "HIERARCHY" << endl;
		outFile << "ROOT " << /*"BVH:"<<*/joint->m_name << endl;
		outFile << "{" << endl;
		outFile << "\t" << "OFFSET "<< joint->m_BPtranslation.x << " " << joint->m_BPtranslation.y << " " << joint->m_BPtranslation.z << endl;
		outFile << "\t" << "CHANNELS " << 6 << " Xposition Yposition Zposition Xrotation Yrotation Zrotation"<< endl;
	
		std::vector<unsigned int> jointExported;
		jointExported.push_back(joint->m_index);
		for(i=0;i<joint->m_child.size();i++)
			ExportHierarchyToBvh(outFile, joint->m_child[i], 1, jointExported);

		outFile << "}" << endl;


		// ------------------------------------------------------------------
		// Next, export motion
		// ------------------------------------------------------------------
		// BVH MOTION
		outFile << "MOTION" << endl ;
		outFile << "Frames: " << out - in<< endl ;
		outFile << "Frame Time: "<< m_timeStep << endl ;
	
		// declare tempory variable
		Quaternion		currentRotation	;
		Translationf		currentPosition	;
		float			rX(0.0f), rY(0.0f), rZ(0.0f);

		// export for each frame 
		for (i = in; i < out; i++)
		{
			// export for each joints 
			for ( j = 0; j<jointExported.size(); j++ )
			{
				// export for each channels
				currentRotation = m_animatedJoints[jointExported[j]].second[i].m_rotation		;
				currentPosition = m_animatedJoints[jointExported[j]].second[i].m_translation	;

				// for root joint: 6 channels
				if( !j ) 
				{
					outFile << currentPosition.x << " " << currentPosition.y << " " << currentPosition.z << " ";
				}
			
				ToEulerAngles(currentRotation, rX, rY, rZ);
				//currentRotation.toEulerAngles(rX, rY, rZ);
				outFile << rX*180.0f/3.14159265 << " " << rY*180.0f/3.14159265 << " " << rZ*180.0f/3.14159265 << " ";
			}
			outFile << endl;
		}
	}
}

//----------------------------------------------------------
//
//----------------------------------------------------------
void AnimatedChain::ComputeMocap(int timeStep)
{
	if( !(timeStep < m_animatedJoints[0].second.size()) )
		return ;

	unsigned int index;
	unsigned int i;

	if(timeStep<0)
	{
		for(i=0;i<m_animatedJoints.size(); i++)
		{
			if(i)
			{
				ChainJoint* father = GetFather(m_animatedJoints[i].first, index);
				Vec3 t = father->m_rotation.Rotate(m_animatedJoints[i].first->m_BPtranslation);
				//if( !(t == m_animatedJoints[i].second[timeStep].m_translation))
				//	cout << "error"<<endl;
				m_animatedJoints[i].first->m_translation = father->m_translation + father->m_rotation.Rotate(m_animatedJoints[i].first->m_translation);
				m_animatedJoints[i].first->m_rotation	= father->m_rotation * m_animatedJoints[i].first->m_BProtation;

				//TQuaternion<float> q = m_animatedJoints[i].first->m_rotation;
				//std::cout <<"q: " << i << "  "<< q.x << " " << q.y << " " << q.z << " " <<  q.w << std::endl;
		
			}
			else
			{
				m_animatedJoints[i].first->m_rotation		= m_animatedJoints[i].first->m_BProtation;
				m_animatedJoints[i].first->m_translation	= m_animatedJoints[i].first->m_translation;
			}
		}
	}
	else
	{
		for(i=0;i<m_animatedJoints.size(); i++)
		{
			if(i)
			{
				ChainJoint* father = GetFather(m_animatedJoints[i].first, index);
				Vec3 t = father->m_rotation.Rotate(m_animatedJoints[i].second[timeStep].m_translation);
				//if( !(t == m_animatedJoints[i].second[timeStep].m_translation))
				//	cout << "error"<<endl;
				m_animatedJoints[i].first->m_translation = father->m_translation + father->m_rotation.Rotate(m_animatedJoints[i].second[timeStep].m_translation);
				m_animatedJoints[i].first->m_rotation	= father->m_rotation * m_animatedJoints[i].second[timeStep].m_rotation;

				//TQuaternion<float> q = m_animatedJoints[i].first->m_rotation;
				//std::cout <<"q: " << i << "  "<< q.x << " " << q.y << " " << q.z << " " <<  q.w << std::endl;
		
			}
			else
			{
				m_animatedJoints[i].first->m_rotation		= m_animatedJoints[i].second[timeStep].m_rotation;
				m_animatedJoints[i].first->m_translation	= m_animatedJoints[i].second[timeStep].m_translation;
			}
		}
	}
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Flux d'entré et de sortie lié au graphe
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(std::ostream& Stream, const Skeleton& sk)
{
	unsigned int i, j, count(sk.m_animatedJoints.size());
	Stream << count << std::endl;

	for(i=0;i<count; i++)
	{
		Stream << sk.m_animatedJoints[i].first->m_index << " ";
		Stream << sk.m_animatedJoints[i].first->m_name << " ";
		Stream << sk.m_animatedJoints[i].first->m_BPtranslation << " ";
		Stream << sk.m_animatedJoints[i].first->m_BProtation << " ";
		Stream << sk.m_animatedJoints[i].first->m_translation << " ";
		Stream << sk.m_animatedJoints[i].first->m_rotation << " ";
		Stream << sk.m_animatedJoints[i].first->m_child.size() << " ";
		for(j=0;j<sk.m_animatedJoints[i].first->m_child.size(); j++)
			Stream << sk.m_animatedJoints[i].first->m_child[j]->m_index << " ";
		Stream << std::endl; 
	}
	Stream << std::endl;

	UInt skMode(static_cast<unsigned int>(sk.m_transformationMode));
	Stream << skMode ;

    return Stream;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Flux d'entré et de sortie lié au graphe
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::istream& operator >>(std::istream& Stream, Skeleton& sk)
{
	unsigned int i, j, count(0), childCount(0), index(0), skMode(0);

	Stream >> count ;

	std::vector<std::vector<UInt> > jointChilds(count);
	sk.m_animatedJoints.resize(count);
	for(i=0;i<count; i++)
	{
		sk.m_animatedJoints[i].first = new ChainJoint();

		Stream >> sk.m_animatedJoints[i].first->m_index;
		Stream >> sk.m_animatedJoints[i].first->m_name;
		Stream >> sk.m_animatedJoints[i].first->m_BPtranslation;
		Stream >> sk.m_animatedJoints[i].first->m_BProtation ;
		Stream >> sk.m_animatedJoints[i].first->m_translation;
		Stream >> sk.m_animatedJoints[i].first->m_rotation;
		Stream >> childCount;
		for(j=0;j<childCount; j++)
		{
			Stream >> index ;
			jointChilds[i].push_back(index);
		}
	}
	sk.m_rootJoint = sk.m_animatedJoints[0].first;

	Stream >> skMode;
	sk.m_transformationMode = Skeleton::TransformationMode(skMode);

	for(i=0;i<count; i++)
		for(j=0;j<jointChilds[i].size(); j++)
			sk.m_animatedJoints[i].first->addChild(sk.m_animatedJoints[jointChilds[i][j]].first);

    return Stream;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------
// calcule un nouveau squelette avec les transformations en relative
//----------------------------------------------------------
void AnimatedChain::ToRelativeMode()
{
	if(m_transformationMode == TM_RELATIVE)
		return;

	if(m_rotationOrder == RO_TranslationFirst)
	{
		ChainJoint*	joint		;
		Vec3		pca, pcr	;
		Quaternion	qca, qcr, 
					qf, qfInv	;

		// the transformation depends on the rotation order
		for( int i = m_animatedJoints.size() - 1 ; i >= 0; --i )
		{
			joint = m_animatedJoints[i].first;

			// get parent
			ChainJoint* fatherJoint(GetFather(joint));

			if(fatherJoint)
			{
				joint->m_rotation = joint->m_rotation.Normalize();
				fatherJoint->m_rotation = fatherJoint->m_rotation.Normalize();

				pca = joint->m_translation;
				qca = joint->m_rotation;

				qf = fatherJoint->m_rotation.Normalize();
				qfInv = Inverse(qf).Normalize();
				
				pcr = glm::rotate(qfInv, pca - fatherJoint->m_translation);
				qcr = (qfInv * qca).Normalize();

				//Vec3 ptmp = fatherJoint->m_transformation.m_position + glm::rotate(qf, pcr);
				//Quaternion qTmp = qf * qcr;
				//// test
				//if(glm::distance(ptmp, pca) > 0.01f)
				//	int fixme = 1.0f;
				
				joint->m_translation = pcr;
				joint->m_rotation = qcr;
			}
		}
	}
	else
	{
		ChainJoint*		joint		;
		Vec3 pos(0.0f);
		Quaternion quat;
		for( int i = m_animatedJoints.size() - 1 ; i >= 0; --i )
		{
			joint = m_animatedJoints[i].first;

			// get parent
			ChainJoint* fatherJoint(GetFather(joint));

			// has parent?
			if(fatherJoint)
			{
				pos = joint->m_translation;
	
				// compute new position
				pos =  pos - fatherJoint->m_translation;
				pos = Inverse(joint->m_rotation.Normalize()).Rotate(pos);
				joint->m_translation = pos;

				// compute new orientation
				quat =  fatherJoint->m_rotation;
				quat = Inverse(quat);
				fatherJoint->m_rotation = quat * joint->m_rotation.Normalize() ;

			}
			else
			{
				pos = joint->m_translation;
				pos = Inverse(joint->m_rotation.Normalize()).Rotate(pos);
				joint->m_translation = pos;
			}
		}
	}
	m_transformationMode = TM_RELATIVE;
}


//----------------------------------------------------------
// calcule un nouveau squelette avec les transformations en relative
//----------------------------------------------------------
void AnimatedChain::ToAbsoluteMode()
{
	if(m_transformationMode == TM_ABSOLUTE)
		return;
	if(m_rotationOrder == RO_TranslationFirst)
	{
		ComputeMocap(-1);
	}
	else
	{
		Vec3	pos		;
		ChainJoint* joint(0);
		for( unsigned int i = 0; i < m_animatedJoints.size(); ++i )
		{
			joint = m_animatedJoints[i].first;
			pos = joint->m_translation;

			// has parent?
			ChainJoint* parentJoint(0);
			if(parentJoint = GetFather(joint))
			{
				// get parent
				joint->m_rotation = ( ((parentJoint->m_rotation * joint->m_rotation).Normalize() ));
				pos = joint->m_rotation.Rotate( pos );
				joint->m_translation = pos + parentJoint->m_translation;
			} else
			{
				Inverse(joint->m_rotation).Rotate( pos );
				joint->m_translation = pos;
			}
		}
	}
	m_transformationMode = TM_ABSOLUTE;
}

void AnimatedChain::SetCurrentToBPValues()
{
	UInt i; 
	for(i=0; i<m_animatedJoints.size(); i++)
	{
		m_animatedJoints[i].first->m_BPtranslation = m_animatedJoints[i].first->m_translation;
		m_animatedJoints[i].first->m_BProtation = m_animatedJoints[i].first->m_rotation;
	}
}

//----------------------------------------------------------
// set bp values to current values and <->
//----------------------------------------------------------
void AnimatedChain::SetBPToCurrentValues()
{
	UInt i; 
	for(i=0; i<m_animatedJoints.size(); i++)
	{
		m_animatedJoints[i].first->m_translation = m_animatedJoints[i].first->m_BPtranslation;
		m_animatedJoints[i].first->m_rotation = m_animatedJoints[i].first->m_BProtation;
	}
}

//----------------------------------------------------------
// retrouve l index d un joint en fonction de son nom
//----------------------------------------------------------
int AnimatedChain::GetJointIndex(const std::string& name) const
{
	unsigned int i;
	for(i=0; i<m_animatedJoints.size();i++)
		if(m_animatedJoints[i].first->m_name == name)
			return i;
	return -1;
}


//----------------------------------------------------------
// 
//----------------------------------------------------------
AnimatedChain& AnimatedChain::Copy(const AnimatedChain & skeleleton)
{
	//for_each( m_joints.begin(), m_joints.end(), STLDelete());
	if(m_rootJoint)
		delete m_rootJoint;
	m_animatedJoints.clear();
	
	m_transformationMode = skeleleton.m_transformationMode;
	m_rotationOrder = skeleleton.m_rotationOrder;
	
	unsigned int i,j;
	for( i = 0; i < skeleleton.m_animatedJoints.size(); ++i )
		AddJoint(skeleleton.m_animatedJoints[i].first->Copy());
	
	for( i = 0; i < skeleleton.m_animatedJoints.size(); i++ )
		for( j = 0; j < skeleleton.m_animatedJoints[i].first->m_child.size(); j++ )
			m_animatedJoints[i].first->m_child.push_back(m_animatedJoints[skeleleton.GetJointIndex(skeleleton.m_animatedJoints[i].first->m_child[j]->m_name)].first);

	if(m_animatedJoints.size())
		m_rootJoint = m_animatedJoints[0].first;

	return (*this);
}
