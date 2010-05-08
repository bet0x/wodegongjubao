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

void CD3D9HardwareBufferMgr::_notifyVertexBufferDestroyed(CHardwareVertexBuffer* pHardwareBuffer)
{
	std::set<CHardwareVertexBuffer*>::iterator it = m_setVertexHardwareBuffer.find(pHardwareBuffer); 
	if (it!=m_setVertexHardwareBuffer.end())
	{
		m_setVertexHardwareBuffer.erase(it);
	}
}

void CD3D9HardwareBufferMgr::_notifyIndexBufferDestroyed(CHardwareIndexBuffer* pHardwareBuffer)
{
	std::set<CHardwareIndexBuffer*>::iterator it = m_setIndexHardwareBuffer.find(pHardwareBuffer); 
	if (it!=m_setIndexHardwareBuffer.end())
	{
		m_setIndexHardwareBuffer.erase(it);
	}
}


void CD3D9HardwareBufferMgr::OnResetDevice()
{
	IDirect3DDevice9* pD3D9Device = GetD3D9RenderSystem().GetD3D9Device();
	for (std::set<CHardwareVertexBuffer*>::iterator it=m_setVertexHardwareBuffer.begin();
		it!=m_setVertexHardwareBuffer.end();it++)
	{
		((CD3D9HardwareVertexBuffer*)*it)->recreateIfDefaultPool(pD3D9Device);
	}
	for (std::set<CHardwareIndexBuffer*>::iterator it=m_setIndexHardwareBuffer.begin();
		it!=m_setIndexHardwareBuffer.end();it++)
	{
		((CD3D9HardwareIndexBuffer*)*it)->recreateIfDefaultPool(pD3D9Device);
	}
}

void CD3D9HardwareBufferMgr::OnLostDevice()
{
	for (std::set<CHardwareVertexBuffer*>::iterator it=m_setVertexHardwareBuffer.begin();
		it!=m_setVertexHardwareBuffer.end();it++)
	{
		((CD3D9HardwareVertexBuffer*)*it)->releaseIfDefaultPool();
	}
	for (std::set<CHardwareIndexBuffer*>::iterator it=m_setIndexHardwareBuffer.begin();
		it!=m_setIndexHardwareBuffer.end();it++)
	{
		((CD3D9HardwareIndexBuffer*)*it)->releaseIfDefaultPool();
	}
}

void CD3D9HardwareBufferMgr::OnDestroyDevice()
{
	for (std::set<CHardwareVertexBuffer*>::iterator it=m_setVertexHardwareBuffer.begin();
		it!=m_setVertexHardwareBuffer.end();it++)
	{
		((CD3D9HardwareVertexBuffer*)*it)->releaseBuffer();
	}
	for (std::set<CHardwareIndexBuffer*>::iterator it=m_setIndexHardwareBuffer.begin();
		it!=m_setIndexHardwareBuffer.end();it++)
	{
		((CD3D9HardwareIndexBuffer*)*it)->releaseBuffer();
	}
}
