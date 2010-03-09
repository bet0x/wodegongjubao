#include "UIComboObjListTile.h"
#include "Scene.h"
#include "Terrain.h"
#include "TerrainEditor.h"
#include "Graphics.h"

CUIComboObjListTile::CUIComboObjListTile()
{
}

CUIComboObjListTile::~CUIComboObjListTile()
{	
}

void CUIComboObjListTile::OnListBoxObjectSelection()
{
	if (NULL==m_pScene)
	{
		return;
	}
	uint8 uTileIndex = (uint8)m_ListBoxObject.GetSelectedData();
	((CTerrainEditor*)(m_pScene->getTerrain()))->GetBrushDecal().SetTileID(uTileIndex);

	CTerrain::MAP_TILES& tiles =((CTerrain*)(m_pScene->getTerrain()))->GetTiles();
	if (tiles.find(getSelectedObjectID())!=tiles.end())
	{
		const Tile& tile = tiles[getSelectedObjectID()];
		m_uTileTexID = tile.material.uDiffuse;
	}
}

void CUIComboObjListTile::initObject(CScene& scene)
{
	m_pScene=&scene;
	m_ListBoxObject.RemoveAllItems();
	static const int nNullTile = 255;
	m_ListBoxObject.AddItem(L"NULL",(LPVOID)nNullTile);
	CTerrain::MAP_TILES& tiles =((CTerrain*)(scene.getTerrain()))->GetTiles();
	for (CTerrain::MAP_TILES::iterator it=tiles.begin(); it!=tiles.end(); it++)
	{
		m_ListBoxObject.AddItem(s2ws(it->second.strName),(LPVOID)it->first);
	}
}

void CUIComboObjListTile::SelectObjectByObjectID(size_t id)
{
	if (NULL==m_pScene)
	{
		return;
	}

	int nIndex = m_ListBoxObject.getItemIndexByData((void*)(id));
	m_ListBoxObject.SelectItem(nIndex);

	CTerrain::MAP_TILES& tiles =((CTerrain*)(m_pScene->getTerrain()))->GetTiles();
	if (tiles.find(getSelectedObjectID())!=tiles.end())
	{
		const Tile& tile = tiles[getSelectedObjectID()];
		m_uTileTexID = tile.material.uDiffuse;
	}
}

size_t CUIComboObjListTile::getSelectedObjectID()
{
	return (size_t)m_ListBoxObject.GetSelectedData();
}

void CUIComboObjListTile::OnObjectListReload()
{
	//std::string strObjectListFilename = getDisplay().getScene().getObjectListFilename();
	//getDisplay().getScene().loadObjectList(strObjectListFilename,strObjectListFilename);
	//initObject();
	//std::string strTileListFilename = getDisplay().getTerrain()().getTileListFilename();
	////getDisplay().getTerrain()().LoadTiles(strTileListFilename);
	//initTiles();
}

void CUIComboObjListTile::OnObjectListEdit()
{
	//std::string strObjectListFilename = getDisplay().getScene().getObjectListFilename();
	//ShellExecuteW(NULL,L"open",s2ws(strObjectListFilename).c_str(),NULL,NULL,SW_SHOWNORMAL); 
	//std::string strTileListFilename = getDisplay().getTerrain()().getTileListFilename();
	//ShellExecuteW(NULL,L"open",s2ws(strTileListFilename).c_str(),NULL,NULL,SW_SHOWNORMAL); 
}

void CUIComboObjListTile::OnFrameMove(double fTime, float fElapsedTime)
{
	CUIComboObjList::OnFrameMove(fTime, fElapsedTime);
}

void CUIComboObjListTile::OnFrameRender(double fTime, float fElapsedTime)
{
	CUIComboObjList::OnFrameRender(fTime, fElapsedTime);
	CRenderSystem& R = GetRenderSystem();
	R.setViewport(m_ListBoxDisplay.GetBoundingBox());
	GetGraphics().DrawTex(m_ListBoxDisplay.GetBoundingBox(),m_uTileTexID,0xFFFFFFFF);
	R.SetupRenderState();
	R.setViewport(GetParentDialog()->GetParentDialog()->GetParentDialog()->GetBoundingBox());
}