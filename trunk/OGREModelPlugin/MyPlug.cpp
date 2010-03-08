#include "MyPlug.h"
#include "IORead.h"
#include "FileSystem.h"
//#include "MUBmd.h"
//#include "RenderSystem.h"
//#include "CSVFile.h"

CMyPlug::CMyPlug(void)
{

}

CMyPlug::~CMyPlug(void)
{

}

enum MeshChunkID {
	M_HEADER                = 0x1000,
	// char*          version           : Version number check
	M_MESH                = 0x3000,
	// bool skeletallyAnimated   // important flag which affects h/w buffer policies
	// Optional M_GEOMETRY chunk
	M_SUBMESH             = 0x4000, 
	// char* materialName
	// bool useSharedVertices
	// unsigned int indexCount
	// bool indexes32Bit
	// unsigned int* faceVertexIndices (indexCount)
	// OR
	// unsigned short* faceVertexIndices (indexCount)
	// M_GEOMETRY chunk (Optional: present only if useSharedVertices = false)
	M_SUBMESH_OPERATION = 0x4010, // optional, trilist assumed if missing
	// unsigned short operationType
	M_SUBMESH_BONE_ASSIGNMENT = 0x4100,
	// Optional bone weights (repeating section)
	// unsigned int vertexIndex;
	// unsigned short boneIndex;
	// float weight;
	// Optional chunk that matches a texture name to an alias
	// a texture alias is sent to the submesh material to use this texture name
	// instead of the one in the texture unit with a matching alias name
	M_SUBMESH_TEXTURE_ALIAS = 0x4200, // Repeating section
	// char* aliasName;
	// char* textureName;

	M_GEOMETRY          = 0x5000, // NB this chunk is embedded within M_MESH and M_SUBMESH
	// unsigned int vertexCount
	M_GEOMETRY_VERTEX_DECLARATION = 0x5100,
	M_GEOMETRY_VERTEX_ELEMENT = 0x5110, // Repeating section
	// unsigned short source;  	// buffer bind source
	// unsigned short type;    	// VertexElementType
	// unsigned short semantic; // VertexElementSemantic
	// unsigned short offset;	// start offset in buffer in bytes
	// unsigned short index;	// index of the semantic (for colours and texture coords)
	M_GEOMETRY_VERTEX_BUFFER = 0x5200, // Repeating section
	// unsigned short bindIndex;	// Index to bind this buffer to
	// unsigned short vertexSize;	// Per-vertex size, must agree with declaration at this index
	M_GEOMETRY_VERTEX_BUFFER_DATA = 0x5210,
	// raw buffer data
	M_MESH_SKELETON_LINK = 0x6000,
	// Optional link to skeleton
	// char* skeletonName           : name of .skeleton to use
	M_MESH_BONE_ASSIGNMENT = 0x7000,
	// Optional bone weights (repeating section)
	// unsigned int vertexIndex;
	// unsigned short boneIndex;
	// float weight;
	M_MESH_LOD = 0x8000,
	// Optional LOD information
	// unsigned short numLevels;
	// bool manual;  (true for manual alternate meshes, false for generated)
	M_MESH_LOD_USAGE = 0x8100,
	// Repeating section, ordered in increasing depth
	// NB LOD 0 (full detail from 0 depth) is omitted
	// float fromSquaredDepth;
	M_MESH_LOD_MANUAL = 0x8110,
	// Required if M_MESH_LOD section manual = true
	// String manualMeshName;
	M_MESH_LOD_GENERATED = 0x8120,
	// Required if M_MESH_LOD section manual = false
	// Repeating section (1 per submesh)
	// unsigned int indexCount;
	// bool indexes32Bit
	// unsigned short* faceIndexes;  (indexCount)
	// OR
	// unsigned int* faceIndexes;  (indexCount)
	M_MESH_BOUNDS = 0x9000,
	// float minx, miny, minz
	// float maxx, maxy, maxz
	// float radius

	// Added By DrEvil
	// optional chunk that contains a table of submesh indexes and the names of
	// the sub-meshes.
	M_SUBMESH_NAME_TABLE = 0xA000,
	// Subchunks of the name table. Each chunk contains an index & string
	M_SUBMESH_NAME_TABLE_ELEMENT = 0xA100,
	// short index
	// char* name

	// Optional chunk which stores precomputed edge data					 
	M_EDGE_LISTS = 0xB000,
	// Each LOD has a separate edge list
	M_EDGE_LIST_LOD = 0xB100,
	// unsigned short lodIndex
	// bool isManual			// If manual, no edge data here, loaded from manual mesh
	// bool isClosed
	// unsigned long numTriangles
	// unsigned long numEdgeGroups
	// Triangle* triangleList
	// unsigned long indexSet
	// unsigned long vertexSet
	// unsigned long vertIndex[3]
	// unsigned long sharedVertIndex[3] 
	// float normal[4] 

	M_EDGE_GROUP = 0xB110,
	// unsigned long vertexSet
	// unsigned long triStart
	// unsigned long triCount
	// unsigned long numEdges
	// Edge* edgeList
	// unsigned long  triIndex[2]
	// unsigned long  vertIndex[2]
	// unsigned long  sharedVertIndex[2]
	// bool degenerate

	// Optional poses section, referred to by pose keyframes
	M_POSES = 0xC000,
	M_POSE = 0xC100,
	// char* name (may be blank)
	// unsigned short target	// 0 for shared geometry, 
	// 1+ for submesh index + 1
	M_POSE_VERTEX = 0xC111,
	// unsigned long vertexIndex
	// float xoffset, yoffset, zoffset
	// Optional vertex animation chunk
	M_ANIMATIONS = 0xD000, 
	M_ANIMATION = 0xD100,
	// char* name
	// float length
	M_ANIMATION_TRACK = 0xD110,
	// unsigned short type			// 1 == morph, 2 == pose
	// unsigned short target		// 0 for shared geometry, 
	// 1+ for submesh index + 1
	M_ANIMATION_MORPH_KEYFRAME = 0xD111,
	// float time
	// float x,y,z			// repeat by number of vertices in original geometry
	M_ANIMATION_POSE_KEYFRAME = 0xD112,
	// float time
	M_ANIMATION_POSE_REF = 0xD113, // repeat for number of referenced poses
	// unsigned short poseIndex 
	// float influence

	// Optional submesh extreme vertex list chink
	M_TABLE_EXTREMES = 0xE000,
	// unsigned short submesh_index;
	// float extremes [n_extremes][3];

	/* Version 1.2 of the .mesh format (deprecated)
	enum MeshChunkID {
	M_HEADER                = 0x1000,
	// char*          version           : Version number check
	M_MESH                = 0x3000,
	// bool skeletallyAnimated   // important flag which affects h/w buffer policies
	// Optional M_GEOMETRY chunk
	M_SUBMESH             = 0x4000, 
	// char* materialName
	// bool useSharedVertices
	// unsigned int indexCount
	// bool indexes32Bit
	// unsigned int* faceVertexIndices (indexCount)
	// OR
	// unsigned short* faceVertexIndices (indexCount)
	// M_GEOMETRY chunk (Optional: present only if useSharedVertices = false)
	M_SUBMESH_OPERATION = 0x4010, // optional, trilist assumed if missing
	// unsigned short operationType
	M_SUBMESH_BONE_ASSIGNMENT = 0x4100,
	// Optional bone weights (repeating section)
	// unsigned int vertexIndex;
	// unsigned short boneIndex;
	// float weight;
	M_GEOMETRY          = 0x5000, // NB this chunk is embedded within M_MESH and M_SUBMESH
	*/
	// unsigned int vertexCount
	// float* pVertices (x, y, z order x numVertices)
	M_GEOMETRY_NORMALS = 0x5100,    //(Optional)
	// float* pNormals (x, y, z order x numVertices)
	M_GEOMETRY_COLOURS = 0x5200,    //(Optional)
	// unsigned long* pColours (RGBA 8888 format x numVertices)
	M_GEOMETRY_TEXCOORDS = 0x5300,    //(Optional, REPEATABLE, each one adds an extra set)
	// unsigned short dimensions    (1 for 1D, 2 for 2D, 3 for 3D)
	// float* pTexCoords  (u [v] [w] order, dimensions x numVertices)
	/*
	M_MESH_SKELETON_LINK = 0x6000,
	// Optional link to skeleton
	// char* skeletonName           : name of .skeleton to use
	M_MESH_BONE_ASSIGNMENT = 0x7000,
	// Optional bone weights (repeating section)
	// unsigned int vertexIndex;
	// unsigned short boneIndex;
	// float weight;
	M_MESH_LOD = 0x8000,
	// Optional LOD information
	// unsigned short numLevels;
	// bool manual;  (true for manual alternate meshes, false for generated)
	M_MESH_LOD_USAGE = 0x8100,
	// Repeating section, ordered in increasing depth
	// NB LOD 0 (full detail from 0 depth) is omitted
	// float fromSquaredDepth;
	M_MESH_LOD_MANUAL = 0x8110,
	// Required if M_MESH_LOD section manual = true
	// String manualMeshName;
	M_MESH_LOD_GENERATED = 0x8120,
	// Required if M_MESH_LOD section manual = false
	// Repeating section (1 per submesh)
	// unsigned int indexCount;
	// bool indexes32Bit
	// unsigned short* faceIndexes;  (indexCount)
	// OR
	// unsigned int* faceIndexes;  (indexCount)
	M_MESH_BOUNDS = 0x9000
	// float minx, miny, minz
	// float maxx, maxy, maxz
	// float radius

	// Added By DrEvil
	// optional chunk that contains a table of submesh indexes and the names of
	// the sub-meshes.
	M_SUBMESH_NAME_TABLE,
	// Subchunks of the name table. Each chunk contains an index & string
	M_SUBMESH_NAME_TABLE_ELEMENT,
	// short index
	// char* name

	*/
};
const long STREAM_OVERHEAD_SIZE = sizeof(uint16) + sizeof(uint32);

int CMyPlug::Execute(iModelData * pModelData, bool bShowDlg, bool bSpecifyFileName)
{
	return -1;
}

struct GeometryVertexElement
{
	unsigned short source;		// buffer bind source
	//VertexElementType vType;
	//VertexElementSemantic vSemantic;
	unsigned short vType;		// VertexElementType
	unsigned short vSemantic;	// VertexElementSemantic
	unsigned short offset;		// start offset in buffer in bytes
	unsigned short index;		// index of the semantic
};

void readGeometryVertexDeclaration(IOReadBase* pRead, std::vector<GeometryVertexElement>& setElement)
{
	// Find optional geometry streams
	if (!pRead->IsEof())
	{
		unsigned short streamID;
		unsigned int uLength;
		pRead->Read(&streamID,sizeof(unsigned short));
		pRead->Read(&uLength,sizeof(unsigned int));
		while(!!pRead->IsEof() &&(streamID == M_GEOMETRY_VERTEX_ELEMENT ))
		{
			switch (streamID)
			{
			case M_GEOMETRY_VERTEX_ELEMENT:
				{
					GeometryVertexElement element; 	
					pRead->Read(&element,sizeof(GeometryVertexElement));

					setElement.push_back(element);
					//dest->vertexDeclaration->addElement(source, offset, vType, vSemantic, index);

					// 	if (vType == VET_COLOUR)
					// 	{
					// 		LogManager::getSingleton().stream()
					// 			<< "Warning: VET_COLOUR element type is deprecated, you should use "
					// 			<< "one of the more specific types to indicate the byte order. "
					// 			<< "Use OgreMeshUpgrade on " << pMesh->getName() << " as soon as possible. ";
					// 	}
				}
				break;
			}
			// Get next stream
			if (!pRead->IsEof())
			{
				pRead->Read(&streamID,sizeof(unsigned short));
				pRead->Read(&uLength,sizeof(unsigned int));
			}
		}
		if (!pRead->IsEof())
		{
			// Backpedal back to start of non-submesh stream
			pRead->Move(-STREAM_OVERHEAD_SIZE);
		}
	}
}

void readGeometryVertexBuffer(IOReadBase* pRead, iLodMesh * pMesh,std::vector<GeometryVertexElement>& setElement)
{
	unsigned short bindIndex, vertexSize;
	// unsigned short bindIndex;	// Index to bind this buffer to
	pRead->Read(&bindIndex,sizeof(unsigned short));
	// unsigned short vertexSize;	// Per-vertex size, must agree with declaration at this index
	pRead->Read(&vertexSize,sizeof(unsigned short));

	// Check for vertex data header
	unsigned short streamID;
	unsigned int uLength;
	pRead->Read(&streamID,sizeof(unsigned short));
	pRead->Read(&uLength,sizeof(unsigned int));
	if (streamID != M_GEOMETRY_VERTEX_BUFFER_DATA)
	{
		MessageBoxW(NULL, L"Can't find vertex buffer data area",	L"MeshSerializerImpl::readGeometryVertexBuffer",0);
	}

	//pRead->read(pBuf, dest->vertexCount * vertexSize);

}

void readGeometry(IOReadBase* pRead, iLodMesh * pMesh)
{
	unsigned int vertexCount = 0;
	pRead->Read(&vertexCount,sizeof(unsigned int));

	unsigned short streamID;
	unsigned int uLength;
	pRead->Read(&streamID,sizeof(unsigned short));
	pRead->Read(&uLength,sizeof(unsigned int));

	std::vector<GeometryVertexElement> setElement; 
	while(!pRead->IsEof() && (streamID == M_GEOMETRY_VERTEX_DECLARATION ||streamID == M_GEOMETRY_VERTEX_BUFFER ))
	{
		switch (streamID)
		{
		case M_GEOMETRY_VERTEX_DECLARATION:
			readGeometryVertexDeclaration(pRead, setElement);
			break;
		case M_GEOMETRY_VERTEX_BUFFER:
			readGeometryVertexBuffer(pRead, pMesh, setElement);
			break;
		}
		// Get next stream
		if (!pRead->IsEof())
		{
			pRead->Read(&streamID,sizeof(unsigned short));
			pRead->Read(&uLength,sizeof(unsigned int));
		}
	}
	if (!pRead->IsEof())
	{
		// Backpedal back to start of non-submesh stream
		pRead->Move(-STREAM_OVERHEAD_SIZE);
	}
}

std::string readString(IOReadBase* pRead)
{
	char	c;
	std::string str;
	pRead->Read(&c,sizeof(char));
	while (c!='\n')
	{
		str.push_back(c);
		pRead->Read(&c,sizeof(char));
	}
	return str;
} 
void readSubMesh(IOReadBase* pRead, iLodMesh * pMesh)
{
//	SubMesh* sm = pMesh->createSubMesh();

	// char* materialName
	std::string materialName = readString(pRead);
// 	if(listener)
// 		listener->processMaterialName(pMesh, &materialName);
// 	sm->setMaterialName(materialName);

	// bool useSharedVertices
	bool useSharedVertices;
	pRead->Read(&useSharedVertices,sizeof(bool));

	// sm->indexData->indexStart = 0;
	unsigned int indexCount = 0;
	pRead->Read(&indexCount,sizeof(unsigned int));
	//sm->indexData->indexCount = indexCount;

	// bool indexes32Bit
	bool idx32bit;
	pRead->Read(&idx32bit,sizeof(bool));
	if (idx32bit)
	{
		MessageBoxW(0,L"Can't read idx32bit",L"Error",0);
		FaceIndex faceIndex;
		for (size_t i=0;i<indexCount;++i)
		{
			unsigned int uVertexIndex;
			pRead->Read(&uVertexIndex,sizeof(unsigned int));

			size_t n = i%3;
			faceIndex.v[n]=uVertexIndex;
			faceIndex.n[n]=uVertexIndex;
			faceIndex.c[n]=uVertexIndex;
			faceIndex.uv1[n]=uVertexIndex;
			if (2==n)
			{
				pMesh->addFaceIndex(faceIndex);
			}
		}
	}
	else // 16-bit
	{
		FaceIndex faceIndex;
		for (size_t i=0;i<indexCount;++i)
		{
			unsigned short uVertexIndex;
			pRead->Read(&uVertexIndex,sizeof(unsigned short));

			size_t n = i%3;
			faceIndex.v[n]=uVertexIndex;
			faceIndex.n[n]=uVertexIndex;
			faceIndex.c[n]=uVertexIndex;
			faceIndex.uv1[n]=uVertexIndex;
			if (2==n)
			{
				pMesh->addFaceIndex(faceIndex);
			}
		}
	}

	// M_GEOMETRY stream (Optional: present only if useSharedVertices = false)
	if (!useSharedVertices)
	{
		unsigned short streamID;
		unsigned int uLength;
		pRead->Read(&streamID,sizeof(unsigned short));
		pRead->Read(&uLength,sizeof(unsigned int));
		if (streamID != M_GEOMETRY)
		{
			MessageBoxW(0,L"Missing geometry data in mesh file",L"readSubMesh",0);
		}
		readGeometry(pRead, pMesh);
	}


	// Find all bone assignments, submesh operation, and texture aliases (if present)
	if (!pRead->IsEof())
	{
		unsigned short streamID;
		unsigned int uLength;
		pRead->Read(&streamID,sizeof(unsigned short));
		pRead->Read(&uLength,sizeof(unsigned int));
		while(!pRead->IsEof() &&
			(streamID == M_SUBMESH_BONE_ASSIGNMENT ||
			streamID == M_SUBMESH_OPERATION ||
			streamID == M_SUBMESH_TEXTURE_ALIAS))
		{
			switch(streamID)
			{
			case M_SUBMESH_OPERATION:
			//	readSubMeshOperation(stream, pMesh, sm);
				break;
			case M_SUBMESH_BONE_ASSIGNMENT:
				//readSubMeshBoneAssignment(stream, pMesh, sm);
				break;
			case M_SUBMESH_TEXTURE_ALIAS:
				//readSubMeshTextureAlias(stream, pMesh, sm);
				break;
			}

			if (!pRead->IsEof())
			{
				pRead->Read(&streamID,sizeof(unsigned short));
				pRead->Read(&uLength,sizeof(unsigned int));
			}

		}
		if (!pRead->IsEof())
		{
			// Backpedal back to start of non-submesh stream
			pRead->Move(-STREAM_OVERHEAD_SIZE);
		}
	}
}

void readHeader(IOReadBase* pRead)
{
	unsigned short headerID;
	pRead->Read(&headerID,sizeof(unsigned short));

	if (headerID == M_HEADER)// OK
	{
		// Read version
		std::string strVersion = readString(pRead);
		//if (ver != mVersion)
		//{
		//	OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, 
		//		"Invalid file: version incompatible, file reports " + String(ver) +
		//		" Serializer is version " + mVersion,
		//		"Serializer::readFileHeader");
		//}
	}
	else
	{
		MessageBoxW(NULL,L"Invalid file: no header",0,0);
	}
}

void readMesh(IOReadBase* pRead, iLodMesh * pMesh)
{
	bool skeletallyAnimated;
	pRead->Read(&skeletallyAnimated,sizeof(bool));
	// Find all substreams
	if (!pRead->IsEof())
	{
		unsigned short streamID;
		unsigned int uLength;
		pRead->Read(&streamID,sizeof(unsigned short));
		pRead->Read(&uLength,sizeof(unsigned int));

		while(!pRead->IsEof() &&
			(streamID == M_GEOMETRY ||
			streamID == M_SUBMESH ||
			streamID == M_MESH_SKELETON_LINK ||
			streamID == M_MESH_BONE_ASSIGNMENT ||
			streamID == M_MESH_LOD ||
			streamID == M_MESH_BOUNDS ||
			streamID == M_SUBMESH_NAME_TABLE ||
			streamID == M_EDGE_LISTS ||
			streamID == M_POSES ||
			streamID == M_ANIMATIONS ||
			streamID == M_TABLE_EXTREMES))
		{
			switch(streamID)
			{
			case M_GEOMETRY:
				{
					readGeometry(pRead, pMesh);
				}
				break;
			case M_SUBMESH:
				//readSubMesh(stream, pMesh, listener);
				break;
			case M_MESH_SKELETON_LINK:
				//readSkeletonLink(stream, pMesh, listener);
				break;
			case M_MESH_BONE_ASSIGNMENT:
				//readMeshBoneAssignment(stream, pMesh);
				break;
			case M_MESH_LOD:
				//readMeshLodInfo(stream, pMesh);
				break;
			case M_MESH_BOUNDS:
				//readBoundsInfo(stream, pMesh);
				break;
			case M_SUBMESH_NAME_TABLE:
				//readSubMeshNameTable(stream, pMesh);
				break;
			case M_EDGE_LISTS:
				//readEdgeList(stream, pMesh);
				break;
			case M_POSES:
				//readPoses(stream, pMesh);
				break;
			case M_ANIMATIONS:
				//readAnimations(stream, pMesh);
				break;
			case M_TABLE_EXTREMES:
				//readExtremes(stream, pMesh);
				break;
			}

			if (!pRead->IsEof())
			{
				pRead->Read(&streamID,sizeof(unsigned short));
				pRead->Read(&uLength,sizeof(unsigned int));
			}
		}
		if (!pRead->IsEof())
		{
			// Backpedal back to start of non-submesh stream
			pRead->Move(-STREAM_OVERHEAD_SIZE);
		}
	}
}

int CMyPlug::importData(iModelData * pModelData, const std::string& strFilename)
{
	assert(pModelData);
	IOReadBase* pRead = IOReadBase::autoOpen(strFilename);
	if (pRead)
	{
		// header
		readHeader(pRead);

		if (!pRead->IsEof())
		{
			unsigned short streamID;
			unsigned int uLength;
			pRead->Read(&streamID,sizeof(unsigned short));
			pRead->Read(&uLength,sizeof(unsigned int));

			switch (streamID)
			{
			case M_MESH:
				{
					readMesh(pRead,&pModelData->getMesh());
					break;
				}
			}
		}
	}

	//mesh.update();

	//m_bbox = mesh.getBBox();
	std::string strMyPath ="Data\\"+GetFilename(GetParentPath(strFilename))+"\\";
	std::string strMatFilename = ChangeExtension(strFilename,".mat.csv");
	std::string strParFilename = ChangeExtension(strFilename,".par.csv");
	if (!IOReadBase::Exists(strMatFilename))
	{
		strMatFilename=strMyPath+ChangeExtension(GetFilename(strFilename),".mat.csv");
	}
	if (!IOReadBase::Exists(strParFilename))
	{
		strParFilename=strMyPath+ChangeExtension(GetFilename(strFilename),".par.csv");
	}

	pModelData->loadMaterial(strMatFilename,GetParentPath(strFilename));
	pModelData->loadParticleEmitters(strParFilename,GetParentPath(strFilename));
	return true;
}

int CMyPlug::exportData(iModelData * pModelData, const std::string& strFilename)
{
	return true;
}

void CMyPlug::Release()
{
	delete this;
}