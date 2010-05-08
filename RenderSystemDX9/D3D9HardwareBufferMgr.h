#pragma once
#include "Common.h"
#include "HardwareBufferMgr.h"

class DLL_EXPORT CD3D9HardwareBufferMgr: public CHardwareBufferMgr
{
public:
	CD3D9HardwareBufferMgr();
	~CD3D9HardwareBufferMgr();
	// ´´½¨VB IB
	CHardwareVertexBuffer* CreateVertexBuffer(size_t numVerts, size_t vertexSize,
		CHardwareBuffer::Usage usage=CHardwareBuffer::HBU_STATIC);
	CHardwareIndexBuffer* CreateIndexBuffer(size_t numIndexes, CHardwareIndexBuffer::IndexType itype = CHardwareIndexBuffer::IT_16BIT,
		CHardwareBuffer::Usage usage=CHardwareBuffer::HBU_STATIC);
	void _notifyVertexBufferDestroyed(CHardwareVertexBuffer* pHardwareBuffer);
	void _notifyIndexBufferDestroyed(CHardwareIndexBuffer* pHardwareBuffer);
	void OnResetDevice();
	void OnLostDevice();
	void OnDestroyDevice();
protected:
};