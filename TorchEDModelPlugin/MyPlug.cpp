#include "MyPlug.h"
#include "IORead.h"
#include "FileSystem.h"
#include "MUBmd.h"
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

int CMyPlug::Execute(iModelData * pModelData, bool bShowDlg, bool bSpecifyFileName)
{
	return -1;
}

const long STREAM_OVERHEAD_SIZE = sizeof(uint16) + sizeof(uint32);

int CMyPlug::importData(iModelData * pModelData, const std::string& strFilename)
{
	assert(pModelData);
	IOReadBase* pRead = IOReadBase::autoOpen(strFilename);
	if (pRead)
	{
		{ // header
			unsigned short headerID;
			pRead->Read(&headerID,sizeof(unsigned short));

			if (headerID == M_HEADER)// OK
			{
				// Read version
				char	c;
				std::string strVersion;
				pRead->Read(&c,sizeof(char));
				while (c!=0)
				{
					strVersion.push_back(c);
					pRead->Read(&c,sizeof(char));
				}
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
		{
			unsigned short streamID;
			unsigned int uLength;

			if (!pRead->IsEof())
			{
				pRead->Read(&streamID,sizeof(unsigned short));
				pRead->Read(&uLength,sizeof(unsigned int));


				switch (streamID)
				{
				case M_MESH:
					{
						bool skeletallyAnimated;
						pRead->Read(&skeletallyAnimated,sizeof(bool));
						// Find all substreams
						if (!pRead->IsEof())
						{
							pRead->Read(&streamID,sizeof(unsigned short));
							pRead->Read(&uLength,sizeof(unsigned int));

							switch(streamID)
							{
							case M_GEOMETRY:
								{
									unsigned int vertexCount = 0;
									pRead->Read(&vertexCount,sizeof(unsigned int));

									pRead->Read(&streamID,sizeof(unsigned short));
									pRead->Read(&uLength,sizeof(unsigned int));

									while(!pRead->IsEof() && (streamID == M_GEOMETRY_VERTEX_DECLARATION ||streamID == M_GEOMETRY_VERTEX_BUFFER ))
									{
										switch (streamID)
										{
										case M_GEOMETRY_VERTEX_DECLARATION:
											// Find optional geometry streams
											if (!pRead->IsEof())
											{
												pRead->Read(&streamID,sizeof(unsigned short));
												pRead->Read(&uLength,sizeof(unsigned int));
												while(!!pRead->IsEof() &&(streamID == M_GEOMETRY_VERTEX_ELEMENT ))
												{
													switch (streamID)
													{
													case M_GEOMETRY_VERTEX_ELEMENT:
														readGeometryVertexElement(stream, pMesh, dest);
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
											break;
										case M_GEOMETRY_VERTEX_BUFFER:
											readGeometryVertexBuffer(stream, pMesh, dest);
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
								break;
							case M_SUBMESH:
								readSubMesh(stream, pMesh, listener);
								break;
							case M_MESH_SKELETON_LINK:
								readSkeletonLink(stream, pMesh, listener);
								break;
							case M_MESH_BONE_ASSIGNMENT:
								readMeshBoneAssignment(stream, pMesh);
								break;
							case M_MESH_LOD:
								readMeshLodInfo(stream, pMesh);
								break;
							case M_MESH_BOUNDS:
								readBoundsInfo(stream, pMesh);
								break;
							case M_SUBMESH_NAME_TABLE:
								readSubMeshNameTable(stream, pMesh);
								break;
							case M_EDGE_LISTS:
								readEdgeList(stream, pMesh);
								break;
							case M_POSES:
								readPoses(stream, pMesh);
								break;
							case M_ANIMATIONS:
								readAnimations(stream, pMesh);
								break;
							case M_TABLE_EXTREMES:
								readExtremes(stream, pMesh);
								break;
							}

							if (!pRead->IsEof())
							{
								pRead->Read(&streamID,sizeof(unsigned short));
								pRead->Read(&uLength,sizeof(unsigned int));
							}
						}
						break;
					}
				}
			}
		}

	static CMUBmd* pPlayerBmd;
	CMUBmd bmd;
	bool bIsPlayerPart = false;
	if (!bmd.LoadFile(strFilename))
	{
		return false;
	}
	if (GetFilename(strFilename)!="player.bmd")
	{
		std::string str = GetFilename(GetParentPath(strFilename));
		if (str=="player")
		{
			if (pPlayerBmd)
			{
				bIsPlayerPart= true;
			}
			else
			{
				pPlayerBmd = new CMUBmd;
				bIsPlayerPart = pPlayerBmd->LoadFile(GetParentPath(strFilename)+"player.bmd");
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	iLodMesh& mesh = pModelData->getMesh();
	//m_Mesh.m_Lods.resize(1);
	if (bmd.nFrameCount>1)// if there one frame only, free the animlist
	{
		int nFrameCount = 0;
		for (size_t i=0; i<bmd.head.uAnimCount; ++i)
		{
			long timeStart = nFrameCount*MU_BMD_ANIM_FRAME_TIME;
			nFrameCount+=bmd.bmdSkeleton.setBmdAnim[i].uFrameCount+1;
			long timeEnd = (nFrameCount-1)*MU_BMD_ANIM_FRAME_TIME;
			pModelData->addAnimation(timeStart,timeEnd);
		}
	}

	for (size_t i=0;  i<bmd.setBmdSub.size(); ++i)
	{
		CMUBmd::BmdSub& bmdSub = bmd.setBmdSub[i];
		iLodMesh& mesh = pModelData->getMesh();
		for(std::vector<CMUBmd::BmdSub::BmdTriangle>::iterator it=bmdSub.setTriangle.begin(); it!=bmdSub.setTriangle.end(); it++)
		{
			FaceIndex faceIndex;
			faceIndex.uSubID=i;
			for (size_t j=0; j<3; ++j)
			{
				faceIndex.v[j]	= mesh.getPosCount()+it->indexVertex[2-j];
				faceIndex.b[j]	= mesh.getBoneCount()+it->indexVertex[2-j];
				faceIndex.w[j]	= mesh.getWeightCount()+it->indexVertex[2-j];
				faceIndex.n[j]	= mesh.getNormalCount()+it->indexNormal[2-j];
				faceIndex.uv1[j]= mesh.getTexcoordCount()+it->indexUV[2-j];
			}
			mesh.addFaceIndex(faceIndex);
		}
		for(std::vector<CMUBmd::BmdSub::BmdPos>::iterator it=bmdSub.setVertex.begin(); it!=bmdSub.setVertex.end(); it++)
		{
			Vec3D vPos = fixCoordSystemPos(it->vPos);
			if (bIsPlayerPart)
			{
				vPos = pPlayerBmd->bmdSkeleton.getLocalMatrix(it->uBones)*vPos;
			}
			else
			{
				vPos = bmd.bmdSkeleton.getLocalMatrix(it->uBones)*vPos;
			}
			if (1<bmd.nFrameCount||bIsPlayerPart)
			{
				uint8 uBone = it->uBones&0xFF;
				if (bmd.bmdSkeleton.setBmdBone.size()<=uBone||bmd.bmdSkeleton.setBmdBone[uBone].bEmpty)
				{
					mesh.addBone(0);
				}
				else
				{
					mesh.addBone(it->uBones);
				}
				//assert((it->uBones&0xFFFFFF00)==0);
				mesh.addWeight(0x000000FF);
			}
			mesh.addPos(vPos);
		}
		for(std::vector<CMUBmd::BmdSub::BmdNormal>::iterator it=bmdSub.setNormal.begin(); it!=bmdSub.setNormal.end(); it++)
		{
			Vec3D n = fixCoordSystemNormal(it->vNormal);
			n = bmd.bmdSkeleton.getRotateMatrix(it->uBones)*n;
			mesh.addNormal(n);
		}
		for(std::vector<Vec2D>::iterator it=bmdSub.setUV.begin(); it!=bmdSub.setUV.end(); it++)
		{
			mesh.addTexcoord(*it);
		}
		{
// 			ModelRenderPass pass;
// 			pass.nSubID = i;
// 			pass.material.bCull = false;
// 			pass.material.bAlphaTest = true;
// 			pass.material.uAlphaTestValue = 0x80;
			std::string strTexFileName = GetParentPath(strFilename) + bmdSub.szTexture;
			{
				std::string strExtension = GetExtension(bmdSub.szTexture); 
				if (".jpg"==strExtension)
					strTexFileName = ChangeExtension(strTexFileName,".ozj");
				else if (".tga"==strExtension)
					strTexFileName = ChangeExtension(strTexFileName,".ozt");
				else if (".bmp"==strExtension)
					strTexFileName = ChangeExtension(strTexFileName,".ozb");
			}
			pModelData->setRenderPass(i,"",strTexFileName,"","","","","",0,false,true,0,0);
		}
	}

	iSkeleton& skeleton = pModelData->getSkeleton();
	skeleton.m_BoneAnims.resize(bmd.head.uBoneCount);

	std::vector<BoneAnim>::iterator itBoneAnim = skeleton.m_BoneAnims.begin();
	for (std::vector<CMUBmd::BmdSkeleton::BmdBone>::iterator itBmdBone=bmd.bmdSkeleton.setBmdBone.begin();itBmdBone!=bmd.bmdSkeleton.setBmdBone.end();itBmdBone++)
	{
		if (!itBmdBone->bEmpty)
		{
			itBoneAnim->strName = itBmdBone->szName;
			Matrix	mInvLocal = itBmdBone->mLocal;
			mInvLocal.Invert();
			itBoneAnim->mSkin = mInvLocal;
			unsigned int uTime =0;
			for (std::vector<Vec3D>::iterator it= itBmdBone->setTrans.begin();
				it!=itBmdBone->setTrans.end(); it++)
			{
				if (GetFilename(strFilename)=="player.bmd"&&itBmdBone==bmd.bmdSkeleton.setBmdBone.begin())
				{
					/*if (uTime>m_AnimList[15].timeStart&&uTime<m_AnimList[23].timeStart)
					{
					Vec3D vPos = fixCoordSystemPos(*it);
					vPos.x=0;vPos.z=0;
					itBoneAnim->trans.addValue(uTime,vPos);
					uTime += MU_BMD_ANIM_FRAME_TIME;
					continue;
					}*/
				}
				itBoneAnim->trans.addValue(uTime,fixCoordSystemPos(*it));
				uTime += MU_BMD_ANIM_FRAME_TIME;
			}
			uTime =0;
			for (std::vector<Vec3D>::iterator it= itBmdBone->setRotate.begin();
				it!=itBmdBone->setRotate.end(); it++)
			{
				itBoneAnim->rot.addValue(uTime,fixCoordSystemRotate(*it));
				uTime += MU_BMD_ANIM_FRAME_TIME;
			}
			int nParent = itBmdBone->nParent;
			if (nParent<0||nParent>255)
			{
				nParent = 255;
			}
			itBoneAnim->parent=nParent;	
		}
		itBoneAnim++;
	}

	mesh.update();

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