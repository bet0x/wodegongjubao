#pragma once
#include "Manager.h"
#include "UIDialog.h"
#include "UIControl.h"
//#include "ControlID.h"

#include "UIStatic.h"
#include "UIButton.h"
#include "UICheckBox.h"
#include "UIComboBox.h"
#include "UIRadioButton.h"
#include "UIImage.h"
#include "UIListBox.h"
//#include "UIPropertyGrid.h"
#include "UIScrollBar.h"
#include "UISlider.h"
#include "UIEditBox.h"
#include "UIIMEEditBox.h"

#include "Scene.h"
#include "TerrainEditor.h"

#include "UIWorldEditorDisplay.h"

//////////////////////////////////////////////////////////////////////////
#include "UIDialog.h"
#include "UIControl.h"

#include "UIStatic.h"
#include "UIButton.h"
#include "UICheckBox.h"
#include "UIComboBox.h"
#include "UIRadioButton.h"
#include "UIImage.h"
#include "UIListBox.h"
#include "UIScrollBar.h"
#include "UISlider.h"
#include "UIEditBox.h"
#include "UIIMEEditBox.h"
#include "UIComboNumber.h"
#include "UIComboColor.h"

enum USER_DEFINED_MSG_TYPE
{
	USER_DEFINED_MSG_TYPE_OBJECT_POS_CHANGED=10000,
};

class CModelDisplay;
class CUIWorldEditorDisplay;
class CDlgMainEditor;
class CTerrainBrush;

class CDlgBaseEditor : public CUIDialog
{
public:
	CDlgBaseEditor();
	~CDlgBaseEditor();
protected:
	CTerrainBrush& getBrush();
	CUIWorldEditorDisplay& getDisplay();
	CDlgMainEditor& getMainDialog();
	CModelDisplay& getModelDisplay();
};