#ifndef NKC_DINPUT_H
#define NKC_DINPUT_H

#include<dinput.h>
#include "cCommon.h"

/* Joypad��� */
#define BUTTON_UP		0x00000001l	// �����L�[��(dijs.IY<0)
#define BUTTON_DOWN		0x00000002l	// �����L�[��(dijs.IY>0)
#define BUTTON_LEFT		0x00000004l	// �����L�[��(dijs.IX<0)
#define BUTTON_RIGHT	0x00000008l	// �����L�[�E(dijs.IX>0)
#define BUTTON_A		0x00000010l	// �`�{�^��(dijs.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// �a�{�^��(dijs.rgbButtons[1]&0x80) ����{�^��.
#define BUTTON_X		0x00000040l	// �b�{�^��(dijs.rgbButtons[2]&0x80)	X
#define BUTTON_Y		0x00000080l	// �w�{�^��(dijs.rgbButtons[3]&0x80)	Y
#define BUTTON_LB		0x00000100l	// �x�{�^��(dijs.rgbButtons[4]&0x80)	LB
#define BUTTON_RB		0x00000200l	// �y�{�^��(dijs.rgbButtons[5]&0x80)	RB
#define BUTTON_L		0x00000400l	// �k�{�^��(dijs.rgbButtons[6]&0x80)	7
#define BUTTON_R		0x00000800l	// �q�{�^��(dijs.rgbButtons[7]&0x80)	8
#define BUTTON_START	0x00001000l	// �r�s�`�q�s�{�^��(dijs.rgbButtons[8]&0x80)
#define BUTTON_M		0x00002000l	// �l�{�^��(dijs.rgbButtons[9]&0x80)	�E�X�e�B�b�N����
#define BUTTON_R		0x00003000l	// �l�{�^��(dijs.rgbButtons[9]&0x80)	���X�e�B�b�N����

#define JOYPADMAX		2			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g

/*
Xinput : Dinput
B:X
X:A
B:A
M:Start
Start:Select


*/
class clsDInput
{
public:
	clsDInput();
	~clsDInput();

	/* �W���C�p�b�h�p */
	DWORD				g_diJoyState;
	DWORD				g_diJoyState2[JOYPADMAX];
	//int					g_diJoyCount = 0;

	/* DirectInput���� */
	bool InitDInput(HINSTANCE);
	void ReleaseDInput(void);

	/* �W���C�p�b�h�p */
	bool InitJoypad(void);
	void GetJoypadState(void);
	bool InitJoypad2(void);
	void GetJoypadState2(void);

};

#endif // !NKC_DINPUT_H
