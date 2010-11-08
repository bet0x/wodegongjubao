#pragma once
#include "D3D9HardwareBufferMgr.h"
#include "D3D9HardwareIndexBuffer.h"
#include "D3D9HardwareVertexBuffer.h"
#include "D3D9RenderSystem.h"

CD3D9HardwareBufferMgr::CD3D9HardwareBufferMgr() {}
CD3D9HardwareBufferMgr::~CD3D9HardwareBufferMgr() {}

CHardwareVertexBuffer* CD3D9HardwareBufferMgr::CreateVertexBuffer(size_t numVerts, size_t vertexSize,
															 CHardwareBuffer::Usage usage)
{
	IDirect3DDevice9* m_pD3D9Device = GetD3D9RenderSystem().GetD3D9Device();
	CD3D9HardwareVertexBuffer* vbuf = new CD3D9HardwareVertexBuffer(
		vertexSize, numVerts, usage, m_pD3D9Device, false);
	if (vbuf->getD3D9VertexBuffer()==NULL)
	{
		S_DEL(vbuf);
		return NULL;
	}
	vbuf->setHardwareBufferMgr(this);
	m_setVertexHardwareBuffer.insert(vbuf);
	return vbuf;
}

CHardwareIndexBuffer* CD3D9HardwareBufferMgr::CreateIndexBuffer(size_t numIndexes, CHardwareIndexBuffer::IndexType itype,
														   CHardwareBuffer::Usage usage)
{
	IDirect3DDevice9* m_pD3D9Device = GetD3D9RenderSystem().GetD3D9Device();
	CD3D9HardwareIndexBuffer* idx = new CD3D9HardwareIndexBuffer(
		itype, numIndexes, usage, m_pD3D9Device, false);
	if (idx->getD3DIndexBuffer()==NULL)
	{
		S_DEL(idx);
		return NULL;
	}
	idx->setHardwareBufferMgr(this);
	m_setIndexHardwareBuffer.insert(idx);
	return idx;
}