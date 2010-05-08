#pragma once
#include "HardwareVertexBuffer.h"
#include "D3D9RenderSystemCommon.h"

class CD3D9HardwareVertexBuffer : public CHardwareVertexBuffer 
{
protected:
	LPDIRECT3DVERTEXBUFFER9 mlpD3DBuffer;
	D3DPOOL mD3DPool;
	/** See HardwareBuffer. */
	void* lockImpl(size_t offset, size_t length, LockOptions options);
	/** See HardwareBuffer. */
	void unlockImpl(void);
public:
	CD3D9HardwareVertexBuffer(size_t vertexSize, size_t numVertices, 
		CHardwareBuffer::Usage usage, LPDIRECT3DDEVICE9 pDev, bool useSystemMem);
	~CD3D9HardwareVertexBuffer();
	/** See HardwareBuffer. */
	void readData(size_t offset, size_t length, void* pDest);
	/** See HardwareBuffer. */
	void writeData(size_t offset, size_t length, const void* pSource,
		bool discardWholeBuffer = false);
	//
	void releaseBuffer();
	/// For dealing with lost devices - release the resource if in the default pool
	bool releaseIfDefaultPool(void);
	/// For dealing with lost devices - recreate the resource if in the default pool
	bool recreateIfDefaultPool(LPDIRECT3DDEVICE9 pDev);

	/// Get D3D9-specific vertex buffer
	LPDIRECT3DVERTEXBUFFER9 getD3D9VertexBuffer(void) const { return mlpD3DBuffer; }
};