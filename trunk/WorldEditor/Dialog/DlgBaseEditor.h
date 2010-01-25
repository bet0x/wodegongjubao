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

enum USER_DEFINED_MSG_TYPE
{
	USER_DEFINED_MSG_TYPE_OBJECT_POS_CHANGED=10000,
};

class CDlgBaseEditor : public CUIDialog
{
public:
	CDlgBaseEditor();
	~CDlgBaseEditor();
protected:
	CTerrainBrush& getBrush();
	virtual CUIWorldEditorDisplay& getDisplay();
};