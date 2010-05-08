#pragma once
#include "RenderSystem.h"
#include "D3D9TextureMgr.h"
#include "D3D9HardwareBufferMgr.h"
#include "D3D9ShaderMgr.h"

#if defined(_DEBUG)
#pragma comment(lib, "rendersystemd.lib")
#else
#pragma comment(lib, "rendersystem.lib")
#endif

struct IDirect3DTexture9;
struct IDirect3DDevice9;
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;

class DLL_EXPORT CD3D9RenderSystem : public CRenderSystem
{
public:
	CD3D9RenderSystem();
	~CD3D9RenderSystem();
public:
	IDirect3DDevice9* GetD3D9Device(){return m_pD3D9Device;}
public:
	CTextureMgr& GetTextureMgr();
	CHardwareBufferMgr& GetHardwareBufferMgr();
	CShaderMgr& GetShaderMgr();
	CRenderWindow* CreateRenderWindow(WNDPROC pWndProc, const std::wstring& strWindowTitle, int32 nWidth, int32 nHeight, bool bFullScreen = false);
	// 创建设备
	//virtual HRESULT OnCreateDevice(LPDIRECT3DDEVICE9 pd3dDevice);
	// 重置D3D设备
	virtual HRESULT OnResetDevice();
	void OnLostDevice();
	void OnDestroyDevice();
	//
	CTexture* GetRenderTarget();
	void SetRenderTarget(CTexture* pRenderTarget);
	//
	CTexture* GetDepthStencil();
	void SetDepthStencil(CTexture* pDepthStencil);

	void OnFrameMove();

	bool BeginFrame();	// 帧渲染开始
	void EndFrame();	// 帧渲染结束

	void setViewport(const CRect<int>& rect);
	void getViewport(CRect<int>& rect);
	//
	void ClearBuffer(bool bZBuffer, bool bTarget, Color32 color);

	// 填充模式
	void SetFillMode(FillMode mode);

	// set matrix
	void setWorldMatrix(const Matrix& m);
	void setViewMatrix(const Matrix& m);
	void setProjectionMatrix(const Matrix& m);
	void setTextureMatrix(uint8 uTexChannel, TextureTransformFlag flag, const Matrix& m = Matrix::ZERO);
	// get matrix
	void getWorldMatrix(Matrix& m)const;
	void getViewMatrix(Matrix& m)const;
	void getProjectionMatrix(Matrix& m)const;
	void getTextureMatrix(uint8 uTexChannel, Matrix& m)const;

	// Func
	void SetDepthBufferFunc(bool bDepthTest = true, bool bDepthWrite = true,				// 深度检测
		CompareFunction depthFunction = CMPF_LESS_EQUAL );
	void SetAlphaTestFunc(bool bAlphaTest = true, CompareFunction func = CMPF_GREATER_EQUAL,// ALPHA检测
		unsigned char value = 0x80);
	void SetBlendFunc(bool bBlend = true, SceneBlendOperation op = BLENDOP_ADD,				// 混合参数
		SceneBlendFactor src = SBF_SOURCE_ALPHA, SceneBlendFactor dest = SBF_ONE_MINUS_SOURCE_ALPHA);

	//
	void SetCullingMode(CullingMode mode);	// 设置剔除模式
	void SetTextureFactor(Color32 color);	// 设置纹理因素颜色
	// TextureOP
	void setResultARGToTemp(size_t unit, bool bResultARGToTemp=true);
	void SetTextureColorOP(size_t unit, TextureBlendOperation op = TBOP_MODULATE,
		TextureBlendSource src1 = TBS_TEXTURE, TextureBlendSource src2 = TBS_DIFFUSE);
	void SetTextureAlphaOP(size_t unit, TextureBlendOperation op,
		TextureBlendSource src1 = TBS_TEXTURE, TextureBlendSource src2 = TBS_DIFFUSE);
	//
	void SetSamplerFilter(size_t unit, TextureFilterType MagFilter, TextureFilterType MinFilter, TextureFilterType MipFilter);
	void SetSamplerAddressUV(size_t unit, AddressUV addressU, AddressUV addressV);

	// 状态初始化
	void SetupRenderState();
	// 去色
	void SetTextureStageStateDecolor();
	// 设置shader
	void SetShader(CShader* pShader);
	void SetShader(uint32 id);
	//
	void SetMaterial(const Vec4D& vAmbient, const Vec4D& vDiffuse);
	// Light
	void SetDirectionalLight(uint32 uIndex,const DirectionalLight& light);
	void LightEnable(uint32 Index, bool bEnable);
	void SetLightingEnabled(bool bEnable);
	//
	void SetTexCoordIndex(uint32 stage, uint32 index);

	// 设置纹理
	void SetTexture(uint32 Stage, uint32 TextureID);
	void SetTexture(uint32 Stage, const CTexture* pTexture);
	// Get
	CTexture* GetTexture(uint32 Stage);

	CVertexDeclaration* CreateVertexDeclaration();
	// 创建VB IB
	CHardwareVertexBuffer* CreateVertexBuffer(size_t numVerts, size_t vertexSize,
		CHardwareBuffer::Usage usage=CHardwareBuffer::HBU_STATIC);
	CHardwareIndexBuffer* CreateIndexBuffer(size_t numIndexes, CHardwareIndexBuffer::IndexType itype = CHardwareIndexBuffer::IT_16BIT,
		CHardwareBuffer::Usage usage=CHardwareBuffer::HBU_STATIC);
	// 顶点
	void SetVB(int nVBID);
	// 设置FVF顶点格式
	void SetFVF(uint32 FVF);
	//
	void SetVertexDeclaration(CVertexDeclaration* pDecl);
	//
	void SetStreamSource(uint32 StreamNumber, CHardwareVertexBuffer* pStreamData,uint32 OffsetInBytes,uint32 Stride);
	void SetIndices(CHardwareIndexBuffer* pIndexData);

	// 绘制
	void DrawPrimitive(VertexRenderOperationType PrimitiveType,uint32 StartVertex,uint32 PrimitiveCount);
	void DrawIndexedPrimitive(VertexRenderOperationType PrimitiveType,int32 BaseVertexIndex,uint32 MinVertexIndex,uint32 NumVertices,uint32 startIndex,uint32 primCount);
	void DrawPrimitiveUP(VertexRenderOperationType PrimitiveType,uint32 PrimitiveCount,const void* pVertexStreamZeroData,uint32 VertexStreamZeroStride);
	void DrawIndexedPrimitiveUP(VertexRenderOperationType PrimitiveType,uint32 MinVertexIndex,uint32 NumVertices,uint32 PrimitiveCount,const void* pIndexData,const void* pVertexStreamZeroData,uint32 VertexStreamZeroStride);
	
	void drawIndexedSubset(const IndexedSubset& subset);

	void setFog(const Fog& fog);
	void setFogEnable(bool bEnable);

	void StretchRect(CTexture* pSource,const CRect<int>* pSourceRect,CTexture* pDest,const CRect<int>* pDestRect,TextureFilterType filter);
protected:
	void SetTexture(uint32 Stage, IDirect3DTexture9* pD3D9Texture);
	// 设置顶点采样器状态
	void SetSamplerState(uint32 Sampler, uint32 Type, uint32 Value);
	//
	//void SetVertexShader(IDirect3DVertexShader9* pShader);
	//void SetPixelShader(IDirect3DPixelShader9* pShader);
	// 设置顶点纹理状态
	void SetTextureStageState(uint32 Stage, uint32 Type, uint32 Value);
	// 提交
	bool commitRenderState();
	bool commitSamplerstate();
	bool commitTextureStageState();
	bool commitTexture();
	bool commitLight();
	bool commitOther();
	bool commitStreamSource();
	//
	//void SetTransform(D3DTRANSFORMSTATETYPE State,const D3DMATRIX* pMatrix);
	//void GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix);
	bool commit();
	void SetRenderState(uint32 State, uint32 Value);// 设置顶点渲染状态
	//
protected:
	CD3D9TextureMgr				m_D3D9TextureMgr;
	CD3D9HardwareBufferMgr		m_D3D9HardwareBufferMgr;
	CD3D9ShaderMgr				m_D3D9ShaderMgr;
	IDirect3DDevice9*			m_pD3D9Device;

	//D3DLIGHT9					m_Lights[8];
	//bool						m_LightEnable[8];

	struct D3D9StreamSource 
	{
		IDirect3DVertexBuffer9* pStreamData;
		uint32 uOffsetInBytes;
		uint32 uStride;
		bool operator!= (const D3D9StreamSource &s) const
		{
			return pStreamData != s.pStreamData||
				uOffsetInBytes != s.uOffsetInBytes||
				uStride != s.uStride;
		}
	};
	std::map<uint32,uint32>						m_mapChangeRenderState;
	std::map<uint32,uint32>						m_mapChangeSamplerState;
	std::map<uint32,uint32>						m_mapChangeTextureStage;
	std::map<uint32,IDirect3DTexture9*>			m_mapChangeTexture;
	std::map<uint32,D3D9StreamSource>			m_mapChangeStreamSource;
	uint32										m_uChangeFVF;
	IDirect3DIndexBuffer9*						m_pChangeIB;
	CShader*									m_pChangeShader;
	CShader*									m_pOldShader;

	//std::map<uint32,D3DLIGHT9>					m_LightsChangeMap;
	//std::map<uint32,bool>						m_LightEnableChangeMap;

	//////////////////////////////////////////////////////////////////////////

	int m_nShaderID;
	int	m_nChangeShaderID;

	//D3DMATERIAL9	m_Material;
	//D3DMATERIAL9	m_ChangeMaterial;

	//////////////////////////////////////////////////////////////////////////
};

CD3D9RenderSystem& GetD3D9RenderSystem();