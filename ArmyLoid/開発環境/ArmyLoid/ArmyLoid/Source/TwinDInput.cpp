#include "TwinDInput.h"

// �}�N��
// Joypad�p�ݒ�l
#define DEADZONE		2500			// �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX		1000			// �L���͈͂̍ő�l
#define RANGE_MIN		-1000			// �L���͈͂̍ŏ��l

//-----------------------------------------------------------------------------
// �O���[�o���ϐ��i���̃\�[�X��������p�������́j
//-----------------------------------------------------------------------------
/* �W���C�p�b�h�p */
DWORD					g_diJoyState = 0x00000000L;	// �W���C�p�b�h���
DWORD					g_diJoyState2[JOYPADMAX];	// �W���C�p�b�h���i�����Ή��j
int						g_diJoyCount = 0;			// ���o�����W���C�p�b�h�̐�

/* DirectInput���� */
static LPDIRECTINPUT8		g_lpDInput;				// DirectInput�I�u�W�F�N�g
/* �W���C�p�b�h�p */
static LPDIRECTINPUTDEVICE8	g_lpDIJoypad;			// �W���C�p�b�h�f�o�C�X
static LPDIRECTINPUTDEVICE8	g_lpDIJoypad2[JOYPADMAX];// �W���C�p�b�h�f�o�C�X


//-----------------------------------------------------------------------------
//	�W���C�X�e�B�b�N�E�f�o�C�X��񋓁i�R�[���o�b�N�֐��j
//-----------------------------------------------------------------------------
BOOL CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE lpddi, LPVOID /*pvRef*/)
{
	HRESULT hr;

	hr = g_lpDInput->CreateDevice(lpddi->guidInstance, &g_lpDIJoypad, NULL);
	if (FAILED(hr))
		return DIENUM_CONTINUE;	// ���̃f�o�C�X���

	return DIENUM_STOP;	// �f�o�C�X�̗񋓂𒆎~

}

//-----------------------------------------------------------------------------
//	�����̃W���C�X�e�B�b�N�E�f�o�C�X��񋓁i�R�[���o�b�N�֐��j
//-----------------------------------------------------------------------------
BOOL CALLBACK GetJoystickCallback2(LPDIDEVICEINSTANCE lpddi, LPVOID /*pvRef*/)
{
	HRESULT hr;

	hr = g_lpDInput->CreateDevice(lpddi->guidInstance, &g_lpDIJoypad2[g_diJoyCount++], NULL);
	return DIENUM_CONTINUE;	// ���̃f�o�C�X���

}


clsDInput::clsDInput()
	: g_diJoyState(NULL)
{
	for (int i = 0; i < JOYPADMAX; i++)
	{
		g_diJoyState2[i] = NULL;
	}

}

clsDInput::~clsDInput()
{
	for (int i = 0; i < JOYPADMAX; i++)
	{
		g_diJoyState2[i] = NULL;
	}

}


//-----------------------------------------------------------------------------
// Direct Input ������
//-----------------------------------------------------------------------------
bool clsDInput::InitDInput(HINSTANCE hInstApp)
{
	HRESULT		hr;

	hr = DirectInput8Create(hInstApp, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_lpDInput, NULL);
	if ( FAILED(hr) )
		return false;	// DirectInput8�̍쐬�Ɏ��s

	return true;

}

//-----------------------------------------------------------------------------
// Direct Input �J������
//-----------------------------------------------------------------------------
void clsDInput::ReleaseDInput(void)
{
	// DirectInput�̃f�o�C�X���J��
	if ( g_lpDIJoypad ) g_lpDIJoypad->Unacquire();
	SAFE_RELEASE(g_lpDIJoypad);
	for (int i=0 ; i<JOYPADMAX ; i++) {
		if ( g_lpDIJoypad2[i] ) g_lpDIJoypad2[i]->Unacquire();
		SAFE_RELEASE(g_lpDIJoypad2[i]);
	}
	SAFE_RELEASE(g_lpDInput);

}


//-----------------------------------------------------------------------------
// Joypad�p�I�u�W�F�N�g�쐬
//-----------------------------------------------------------------------------
bool clsDInput::InitJoypad(void)
{
	HRESULT		hr;

	// �W���C�p�b�h��T��
	g_lpDInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL, 
		(LPDIENUMDEVICESCALLBACK)GetJoystickCallback,
		NULL, 
		DIEDFL_ATTACHEDONLY);
	if ( g_lpDIJoypad == NULL )
		return false;

	// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	hr = g_lpDIJoypad->SetDataFormat(&c_dfDIJoystick);
	if ( FAILED(hr) )
		return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

	// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
//	hr = g_lpDIJoypad->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//	if (FAILED(hr))
//		return false; // ���[�h�̐ݒ�Ɏ��s

	// ���̒l�͈̔͂�ݒ�
	// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
	// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
	// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�́A�֌W�Ȃ�)
	DIPROPRANGE				diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize		= sizeof(diprg); 
	diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
	diprg.diph.dwHow		= DIPH_BYOFFSET; 
	diprg.lMin				= RANGE_MIN;
	diprg.lMax				= RANGE_MAX;
	// X���͈̔͂�ݒ�
	diprg.diph.dwObj		= DIJOFS_X; 
	g_lpDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Y���͈̔͂�ݒ�
	diprg.diph.dwObj		= DIJOFS_Y;
	g_lpDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

	// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
	// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
	// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
	DIPROPDWORD				dipdw;
	dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
	dipdw.diph.dwHow		= DIPH_BYOFFSET;
	dipdw.dwData			= DEADZONE;
	//X���̖����]�[����ݒ�
	dipdw.diph.dwObj		= DIJOFS_X;
	g_lpDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	//Y���̖����]�[����ݒ�
	dipdw.diph.dwObj		= DIJOFS_Y;
	g_lpDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		
	//�W���C�X�e�B�b�N���͐���J�n
	g_lpDIJoypad->Acquire();
		
	return true;

}

//-----------------------------------------------------------------------------
// �֐����@�F�@GetJoypadState()
// �@�\�T�v�F�@�W���C�p�b�h�̏�Ԃ��擾
//-----------------------------------------------------------------------------
void clsDInput::GetJoypadState(void)
{
	HRESULT			hr;
	DIJOYSTATE2		dijs;

	g_diJoyState = 0x00000000l;	// ������

	hr = g_lpDIJoypad->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
	if ( FAILED(hr) ) {
		hr = g_lpDIJoypad->Acquire();
		while ( hr == DIERR_INPUTLOST )
			hr = g_lpDIJoypad->Acquire();
	}

	hr = g_lpDIJoypad->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
	if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) {
		hr = g_lpDIJoypad->Acquire();
		while ( hr == DIERR_INPUTLOST )
			hr = g_lpDIJoypad->Acquire();
	}

	// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
	//* y-axis (forward)
	if ( dijs.lY < 0 )					g_diJoyState |= BUTTON_UP;
	//* y-axis (backward)
	if ( dijs.lY > 0 )					g_diJoyState |= BUTTON_DOWN;
	//* x-axis (left)
	if ( dijs.lX < 0 )					g_diJoyState |= BUTTON_LEFT;
	//* x-axis (right)
	if ( dijs.lX > 0 )					g_diJoyState |= BUTTON_RIGHT;
	//* �`�{�^��
	if ( dijs.rgbButtons[0] & 0x80 )	g_diJoyState |= BUTTON_A;
	//* �a�{�^��
	if ( dijs.rgbButtons[1] & 0x80 )	g_diJoyState |= BUTTON_B;
	//* �b�{�^��
	if ( dijs.rgbButtons[2] & 0x80 )	g_diJoyState |= BUTTON_X;
	//* �w�{�^��
	if ( dijs.rgbButtons[3] & 0x80 )	g_diJoyState |= BUTTON_Y;
	//* �x�{�^��
	if ( dijs.rgbButtons[4] & 0x80 )	g_diJoyState |= BUTTON_LB;
	//* �y�{�^��
	if ( dijs.rgbButtons[5] & 0x80 )	g_diJoyState |= BUTTON_RB;
    //* �k�{�^��
    if ( dijs.rgbButtons[6] & 0x80 )	g_diJoyState |= BUTTON_L;
	//* �q�{�^��
	if ( dijs.rgbButtons[7] & 0x80 )	g_diJoyState |= BUTTON_R;
	//* �r�s�`�q�s�{�^��
	if ( dijs.rgbButtons[8] & 0x80 )	g_diJoyState |= BUTTON_START;
	//* �l�{�^��
	if ( dijs.rgbButtons[9] & 0x80 )	g_diJoyState |= BUTTON_M;

}


//-----------------------------------------------------------------------------
//  ������Joypad�p�I�u�W�F�N�g���쐬
//-----------------------------------------------------------------------------
bool clsDInput::InitJoypad2(void)
{
	HRESULT		hr;
	int			i;

	// �W���C�p�b�h��T��
	g_lpDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback2, NULL, DIEDFL_ATTACHEDONLY);

	for ( i=0 ; i<g_diJoyCount ; i++ ) {
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		hr = g_lpDIJoypad2[i]->SetDataFormat(&c_dfDIJoystick);
		if ( FAILED(hr) )
			return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

		// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
//		hr = g_lpDIJoypad2[i]->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//		if ( FAILED(hr) )
//			return false; // ���[�h�̐ݒ�Ɏ��s

		// ���̒l�͈̔͂�ݒ�
		// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
		// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
		// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�́A�֌W�Ȃ�)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize		= sizeof(diprg); 
		diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
		diprg.diph.dwHow		= DIPH_BYOFFSET; 
		diprg.lMin				= RANGE_MIN;
		diprg.lMax				= RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_X; 
		g_lpDIJoypad2[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_Y;
		g_lpDIJoypad2[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwHow		= DIPH_BYOFFSET;
		dipdw.dwData			= DEADZONE;
		//X���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_X;
		g_lpDIJoypad2[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		//Y���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_Y;
		g_lpDIJoypad2[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
			
		//�W���C�X�e�B�b�N���͐���J�n
		g_lpDIJoypad2[i]->Acquire();
	}
		
	return true;

}

//-----------------------------------------------------------------------------
// �֐����@�F�@GetJoypadState2()
// �@�\�T�v�F�@�����̃W���C�p�b�h�̏�Ԃ��擾
//-----------------------------------------------------------------------------
void clsDInput::GetJoypadState2(void)
{
	HRESULT			hr = NULL;
	DIJOYSTATE2		dijs;
	int				i;

	for ( i = 0 ; i < g_diJoyCount; i++ ) {
		g_diJoyState2[i] = 0x00000000l;	// ������

		g_lpDIJoypad2[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if ( FAILED(hr) ) 
		{
			hr = g_lpDIJoypad2[i]->Acquire();
			while ( hr == DIERR_INPUTLOST )
				hr = g_lpDIJoypad2[i]->Acquire();
		}

		hr = g_lpDIJoypad2[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
		if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) 
		{
			hr = g_lpDIJoypad2[i]->Acquire();
			while (hr == DIERR_INPUTLOST)
			{
				hr = g_lpDIJoypad2[i]->Acquire();
			}
		}

		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
		//* y-axis (forward)
		if ( dijs.lY < 0 )
			g_diJoyState2[i] |= BUTTON_UP;
		//* y-axis (backward)
		if ( dijs.lY > 0 )
			g_diJoyState2[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if ( dijs.lX < 0 )
			g_diJoyState2[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if ( dijs.lX > 0 )
			g_diJoyState2[i] |= BUTTON_RIGHT;
		//* �`�{�^��
		if ( dijs.rgbButtons[0] & 0x80 )
			g_diJoyState2[i] |= BUTTON_A;
		//* �a�{�^��
		if ( dijs.rgbButtons[1] & 0x80 )
			g_diJoyState2[i] |= BUTTON_B;
		//* �b�{�^��
		if ( dijs.rgbButtons[2] & 0x80 )
			g_diJoyState2[i] |= BUTTON_X;
		//* �w�{�^��
		if ( dijs.rgbButtons[3] & 0x80 )
			g_diJoyState2[i] |= BUTTON_Y;
		//* �x�{�^��
		if ( dijs.rgbButtons[4] & 0x80 )
			g_diJoyState2[i] |= BUTTON_LB;
		//* �y�{�^��
		if ( dijs.rgbButtons[5] & 0x80 )
			g_diJoyState2[i] |= BUTTON_RB;
		//* �k�{�^��
		if ( dijs.rgbButtons[6] & 0x80 )
			g_diJoyState2[i] |= BUTTON_L;
		//* �q�{�^��
		if ( dijs.rgbButtons[7] & 0x80 )
			g_diJoyState2[i] |= BUTTON_R;
		//* �r�s�`�q�s�{�^��
		if ( dijs.rgbButtons[8] & 0x80 )
			g_diJoyState2[i] |= BUTTON_START;
		//* �l�{�^��
		if ( dijs.rgbButtons[9] & 0x80 )
			g_diJoyState2[i] |= BUTTON_M;
	}

}
