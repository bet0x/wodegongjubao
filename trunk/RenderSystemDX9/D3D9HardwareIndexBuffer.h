#pragma once
#include "HardwareIndexBuffer.h"
#include "D3D9RenderSystemCommon.h"

class CD3D9HardwareIndexBuffer : public CHardwareIndexBuffer
{
protected:
	LPDIRECT3DINDEXBUFFER9 mlpD3DBuffer;
	D3DPOOL mD3DPool;
	/** See HardwareBuffer. */
	void* lockImpl(size_t offset, size_t length, LockOptions options);
	/** See HardwareBuffer. */
	void unlockImpl(void);
public:
	CD3D9HardwareIndexBuffer(IndexType idxType, size_t numIndexes, 
		CHardwareBuffer::Usage usage, LPDIRECT3DDEVICE9 pDev, bool useSystemMem);
	~CD3D9HardwareIndexBuffer();
	/** See HardwareBuffer. */
	void readData(size_t offset, size_t length, void* pDest);
	/** See HardwareBuffer. */
	void writeData(size_t offset, size_t length, const void* pSource,
		bool discardWholeBuffer = false);

	// 
	void releaseBuffer();
	/// For dealing with lost devices - release the resource if in the default pool
	bool releaseIfDefaultPool();
	/// For dealing with lost devices - recreate the resource if in the default pool
	bool recreateIfDefaultPool(LPDIRECT3DDEVICE9 pDev);

	/// Get the D3D-specific index buffer
	LPDIRECT3DINDEXBUFFER9 getD3DIndexBuffer(void) { return mlpD3DBuffer; }
};