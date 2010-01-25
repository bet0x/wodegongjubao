#include "DlgBaseEditor.h"
#include "DlgToolbar.h"
CDlgBaseEditor::CDlgBaseEditor()
{
}

CDlgBaseEditor::~CDlgBaseEditor()
{	
}

CTerrainBrush& CDlgBaseEditor::getBrush()
{
	return getDisplay().getTerrain().GetBrushDecal();
}

CUIWorldEditorDisplay& CDlgBaseEditor::getDisplay()
{
	assert(GetParentDialog());
	return ((CDlgBaseEditor*)GetParentDialog())->getDisplay();
}