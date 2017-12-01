// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
// by tiger.zhang

#pragma once

//
//  NOTE: 0x0400���µ�������UI���ڲ�ʹ�ã�˽�е��������WCEUI_CMD_USER��ʼ
// 
#define WCEUI_CMD_USER			0x0400

/*
* Listbox ѡ��ĳһ��ʱ
*/
// lParam:  high word ~ 0������ʾ�µ��ѡ��,1��ʾ�û������֮ǰ�Ѿ�ѡ����
//          low  word ~ ѡ���������ֵ
#define WCEUI_CMD_LISTBOX_SELCHANGE       0x0001
#define WCEUI_LB_SEL_NEW				  0
#define WCEUI_LB_SEL_OLD				  1
// Listbox Item State
#define WCEUI_LBIS_SELECTED			0x0002L

/*
* Button ���
* lParam: ����������������������ǳ������
*/
#define WCEUI_CMD_BUTTON_CLICK		       0x0002

#define WCEUI_BUTTON_CLICK					0
#define WCEUI_BUTTON_LONG_CLICK				1

/*
* CWceUiSButton ״̬�ı�ʱ
* lParam: ��ǰ��״̬
*/
#define WCEUI_CMD_SBUTTON_CLICK				0x0003
#define WCEUI_SBUTTON_OFF		0
#define WCEUI_SBUTTON_ON		1

/*
* CWceUiSlider, CWceUiVSlider״̬�ı�ʱ
* lParam: hight wordΪ��ǰ����״̬��low wordΪ��ǰ��λ��
*/
#define WCEUI_CMD_SLIDER					0x0004
#define SLIDER_MOUSE_DOWN	0
#define SLIDER_MOUSE_MOVE	1
#define SLIDER_MOUSE_UP		2

#define SLIDER_HORIZONTAL	0	// ����slider
#define SLIDER_VERTICAL		1	// ����slider



/*
* CWceUiPageLayerҳ״̬�ı�ʱ
* lParam: ��ǰҳ
*/
#define WCEUI_CMD_PAGELAYER_CHANGE		0x0005
