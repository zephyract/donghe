// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
// by tiger.zhang

#pragma once

//
//  NOTE: 0x0400以下的命令码UI库内部使用，私有的命令码从WCEUI_CMD_USER开始
// 
#define WCEUI_CMD_USER			0x0400

/*
* Listbox 选中某一项时
*/
// lParam:  high word ~ 0用来表示新的项被选中,1表示用户点击了之前已经选中项
//          low  word ~ 选中项的索引值
#define WCEUI_CMD_LISTBOX_SELCHANGE       0x0001
#define WCEUI_LB_SEL_NEW				  0
#define WCEUI_LB_SEL_OLD				  1
// Listbox Item State
#define WCEUI_LBIS_SELECTED			0x0002L

/*
* Button 点击
* lParam: 用来描述是正常点击，还是长按点击
*/
#define WCEUI_CMD_BUTTON_CLICK		       0x0002

#define WCEUI_BUTTON_CLICK					0
#define WCEUI_BUTTON_LONG_CLICK				1

/*
* CWceUiSButton 状态改变时
* lParam: 当前的状态
*/
#define WCEUI_CMD_SBUTTON_CLICK				0x0003
#define WCEUI_SBUTTON_OFF		0
#define WCEUI_SBUTTON_ON		1

/*
* CWceUiSlider, CWceUiVSlider状态改变时
* lParam: hight word为当前鼠标的状态，low word为当前的位置
*/
#define WCEUI_CMD_SLIDER					0x0004
#define SLIDER_MOUSE_DOWN	0
#define SLIDER_MOUSE_MOVE	1
#define SLIDER_MOUSE_UP		2

#define SLIDER_HORIZONTAL	0	// 横向slider
#define SLIDER_VERTICAL		1	// 纵向slider



/*
* CWceUiPageLayer页状态改变时
* lParam: 当前页
*/
#define WCEUI_CMD_PAGELAYER_CHANGE		0x0005
