#include "DlgBaseEditor.h"
#include "..\MainRoot.h"

CDlgBaseEditor::CDlgBaseEditor()
{
}

CDlgBaseEditor::~CDlgBaseEditor()
{	
}

CDlgMainEditor& CDlgBaseEditor::getMainDialog()
{
	return CMainRoot::getInstance().getMainDialog();
}

CUIWorldEditorDisplay& CDlgBaseEditor::getDisplay()
{
	return getMainDialog().getDisplay();
}

CModelDisplay& CDlgBaseEditor::getModelDisplay()
{
	return getMainDialog().getModelDisplay();
}

CTerrainBrush& CDlgBaseEditor::getBrush()
{
	return getDisplay().getTerrain().GetBrushDecal();
}