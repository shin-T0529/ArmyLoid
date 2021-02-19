#include "DInput.h"

LPDIRECTINPUT8			g_pDI;				//DirectInput�I�u�W�F�N�g
LPDIRECTINPUTDEVICE8	g_pPad[PadNumber];				//�f�o�C�X�I�u�W�F�N�g�i�R���g���[���[�j

//�N���X�֐��ł͂Ȃ��֐�
//�W���C�X�e�B�b�N�񋓊֐�
BOOL CALLBACK EnumJoysticksCallBack(const DIDEVICEINSTANCE *pDiDInstance,VOID *pContext)
{
		HRESULT hRlt;	//�֐��̕��A�n
		for (int i = 0; i < PadNumber; i++)
		{
			//�f�o�C�X�̍쐬
			hRlt = g_pDI->CreateDevice(
				pDiDInstance->guidInstance,	//�f�o�C�X�ԍ�
				&g_pPad[i],					//(out)�쐬���ꂽ�f�o�C�X�I�u�W�F�N�g
				NULL);
			//�G���[�`�F�b�N
			if (hRlt != DI_OK)
			{
				return DIENUM_CONTINUE;	//���̃f�o�C�X��v��
			}
		}

	return DIENUM_STOP;	//�񋓒�~
}

//�I�u�W�F�N�g�񋓊֐�
BOOL CALLBACK EnumObjectsCallBack(const DIDEVICEOBJECTINSTANCE *pDiDObjectInstance, VOID*pContext)
{
	for (int i = 0; i < PadNumber; i++)
	{
		//��(Axis,�A�i���O�X�e�B�b�N)�������Ă��邩�H
		if (pDiDObjectInstance->dwType & DIDFT_AXIS)
		{
			//���͏��͈̔͂�ݒ肷��
			DIPROPRANGE diprg;		//�͈͎w��\����

			diprg.diph.dwSize = sizeof(DIPROPRANGE);			//�\���̃T�C�Y
			diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);		//�w�b�_�[���̃T�C�Y
			diprg.diph.dwObj = pDiDObjectInstance->dwType;		//�I�u�W�F�N�g�^�C�v��ݒ肷��iAxis�^�C�v�j
			diprg.diph.dwHow = DIPH_BYID;						//dwObj�ɐݒ肳�ꂽ���̂��I�u�W�F�N�g�̎�ނ��H�C���X�^���X�ԍ����H���f

			diprg.lMax = +1000;			//���̍ő�l
			diprg.lMin = -1000;			//���̍ŏ��l

			//�͈͂̐ݒ�
			if (FAILED(g_pPad[i]->SetProperty(DIPROP_RANGE, &diprg.diph)))
			{
				return DIENUM_STOP;
			}
		}
	}
	return DIENUM_CONTINUE;
}

DInput::DInput()
{
	for (int i = 0; i < PadNumber; i++)
	{
		m_pPad[i] = NULL;

	}
	m_pDI = NULL;

}

DInput::~DInput()
{

}

//DirectInput�̏�����
bool DInput::initDI(HWND hWnd)
{
	for (int i = 0; i < PadNumber; i++)
	{
		HRESULT hRlt;	//�֐��̕��A�n
		hRlt = DirectInput8Create(
			GetModuleHandle(NULL),	//DxInput���쐬����A�v���n���h��
			DIRECTINPUT_VERSION,	//�Œ�FDxInput�̃o�[�W����
			IID_IDirectInput8,		//�Œ�FUnicode��Ansi�̋�ʗp
			(VOID**)&m_pDI,			//(out)DxInput�I�u�W�F�N�g
			NULL);

		//�G���[�`�F�b�N
		if (hRlt != DI_OK)
		{
			MessageBox(NULL, "DxInput�̍쐬�Ɏ��s", "�G���[", MB_OK);
			return false;
		}

		g_pDI = m_pDI;

		//�g�p�\�ȃR���g���[���[��T��
		hRlt = (*m_pDI).EnumDevices(
			DI8DEVCLASS_GAMECTRL,//���ׂẴQ�[���R���g���[���[
			EnumJoysticksCallBack,//�R���g���[���[�̗񋓊֐�
			NULL,				 //�R�[���o�b�N�֐�����̒l
			DIEDFL_ATTACHEDONLY);//�q�����Ă�����̂���̂�

	//�G���[�`�F�b�N
		if (hRlt != DI_OK)
		{
			MessageBox(NULL, "�R���g���[���[�̊m�F�Ɏ��s", "�G���[", MB_OK);
			return false;
		}
		m_pPad[i] = g_pPad[i];

		//�R���g���[���[�̐ڑ��m�F
		//�ڑ�����Ă���ꍇ
		if (g_pPad[i] != NULL)
		{
			//�R���g���[���[�\���̂̃f�[�^�t�H�[�}�b�g���쐬
			hRlt = g_pPad[i]->SetDataFormat(&c_dfDIJoystick2);
			if (hRlt != DI_OK)
			{
				MessageBox(NULL, "�f�[�^�t�H�[�}�b�g���쐬�Ɏ��s", "�G���[", MB_OK);
			}

			//�ق��̃f�o�C�X�Ƃ̋������x����ݒ�
			hRlt = g_pPad[i]->SetCooperativeLevel(hWnd,
				DISCL_EXCLUSIVE |	//�r���A�N�Z�X
				DISCL_FOREGROUND);	//�t�H�A�O���E���h

			if (hRlt != DI_OK)
			{
				MessageBox(NULL, "�������x���̐ݒ�Ɏ��s", "�G���[", MB_OK);
			}

			//�g�p�\�ȃI�u�W�F�N�g�i�{�^���Ȃǁj�̗񋓂��쐬
			hRlt = g_pPad[i]->EnumObjects(
				EnumObjectsCallBack,	//�I�u�W�F�N�g�֐�
				(VOID*)hWnd,			//�R�[���o�b�N�֐��ɑ�����
				DIDFT_ALL);				//���ׂẴI�u�W�F�N�g 

			if (hRlt != DI_OK)
			{
				MessageBox(NULL, "�I�u�W�F�N�g�̗񋓂Ɏ��s", "�G���[", MB_OK);
			}

		}
	}
	return true;
}

//���͏��X�V
HRESULT DInput :: UpdateInputState()
{
	HRESULT hRlt;		//�֐����A�n
	DIJOYSTATE2 js;		//�W���C�X�e�B�b�N���\����

	for (int i = 0; i < PadNumber; i++)
	{
		//���͏��̏�����
		InitInputState();

		//�W���C�X�e�B�b�N�̐ڑ��m�F
		if (m_pPad[i] == NULL)
		{
			return S_OK;
		}

		//�W���C�X�e�B�b�N���ύX����Ă��邩���m�F
		hRlt = m_pPad[i]->Poll();

		if (hRlt != DI_OK && hRlt != DI_NOEFFECT)		//DI_NOEFFECT�F�m�F�s�v�ȃf�o�C�X
		{
			//�R���g���[���[�̃A�N�Z�X�����擾����
			hRlt = m_pPad[i]->Acquire();
			while (hRlt == DIERR_INPUTLOST)
			{
				hRlt = m_pPad[i]->Acquire();
			}
			return S_OK;
		}

		//�R���g���[���[���璼�ڃf�[�^���擾����
		hRlt = m_pPad[i]->GetDeviceState(
			sizeof(DIJOYSTATE2),			//�擾����T�C�Y
			&js);						//�iout�j�擾�����f�[�^�̌���
		if (hRlt == DIERR_INPUTLOST || hRlt == DIERR_NOTACQUIRED)
		{
			hRlt = m_pPad[i]->Acquire();
			while (hRlt == DIERR_INPUTLOST)
			{
				hRlt = m_pPad[i]->Acquire();
			}
		}
		if (hRlt != DI_OK)
		{
			return hRlt;
		}

		//���͌���.
		//�E
		if (js.lX > N_OFFSET){ AddInputState(enPkey_R); }
		//��
		else if (js.lX < -N_OFFSET){ AddInputState(enPkey_L); }
		//��
		if (js.lY > N_OFFSET){ AddInputState(enPkey_D); }
		//��
		else if (js.lY < -N_OFFSET){ AddInputState(enPkey_U); }

		//�{�^���iRGB Buttons�j
		//���[�v�J�n�ʒu���`�F�b�N�Ώۂ̃{�^���ɕύX����
		for (int i = enPkey_01; i < enPkey_Max; i++)
		{
			//�R���g���[���[�̃{�^���̔z��J�n�ԍ���0����Ȃ̂ŊJ�n�ʒu��߂�
			if (js.rgbButtons[i - enPkey_01] & 0x80)
			{
				//�L���X�g���s�����[�v�ϐ�����͒l�Ƃ��Ċi�[
				AddInputState((enPkey)i);
			}
		}
	}
	return S_OK;

}

//���͏��X�V
HRESULT DInput::UpdateInputState2()
{
	HRESULT hRlt;		//�֐����A�n
	DIJOYSTATE2 js;		//�W���C�X�e�B�b�N���\����

	for (int i = 0; i < PadNumber; i++)
	{
		//���͏��̏�����
		InitInputState();

		//�W���C�X�e�B�b�N�̐ڑ��m�F
		if (m_pPad[i] == NULL)
		{
			return S_OK;
		}

		//�W���C�X�e�B�b�N���ύX����Ă��邩���m�F
		hRlt = m_pPad[i]->Poll();

		if (hRlt != DI_OK && hRlt != DI_NOEFFECT)		//DI_NOEFFECT�F�m�F�s�v�ȃf�o�C�X
		{
			//�R���g���[���[�̃A�N�Z�X�����擾����
			hRlt = m_pPad[i]->Acquire();
			while (hRlt == DIERR_INPUTLOST)
			{
				hRlt = m_pPad[i]->Acquire();
			}
			return S_OK;
		}

		//�R���g���[���[���璼�ڃf�[�^���擾����
		hRlt = m_pPad[i]->GetDeviceState(
			sizeof(DIJOYSTATE2),			//�擾����T�C�Y
			&js);							//�iout�j�擾�����f�[�^�̌���
		if (hRlt == DIERR_INPUTLOST || hRlt == DIERR_NOTACQUIRED)
		{
			hRlt = m_pPad[i]->Acquire();
			while (hRlt == DIERR_INPUTLOST)
			{
				hRlt = m_pPad[i]->Acquire();
			}
		}
		if (hRlt != DI_OK)
		{
			return hRlt;
		}

		//���͌���.
			//�E
			if (js.lX > N_OFFSET) { AddInputState(enPkey_R); }
			//��
			else if (js.lX < -N_OFFSET) { AddInputState(enPkey_L); }
			//��
			if (js.lY > N_OFFSET) { AddInputState(enPkey_D); }
			//��
			else if (js.lY < -N_OFFSET) { AddInputState(enPkey_U); }



		//�{�^���iRGB Buttons�j
		//���[�v�J�n�ʒu���`�F�b�N�Ώۂ̃{�^���ɕύX����
		for (int i = enPkey_01; i < enPkey_Max; i++)
		{
			//�R���g���[���[�̃{�^���̔z��J�n�ԍ���0����Ȃ̂ŊJ�n�ʒu��߂�
			if (js.rgbButtons[i - enPkey_01] & 0x80)
			{
				//�L���X�g���s�����[�v�ϐ�����͒l�Ƃ��Ċi�[
				AddInputState((enPkey)i);
			}
		}
	}
	return S_OK;

}

//���͏���ǉ�
void DInput :: AddInputState(enPkey key)
{
	//�V�t�g���Z�q�F<<�܂���>>
	m_uInputState |= 1 << key;

}

//���͏��̏�����
void DInput :: InitInputState()
{
	m_uInputState = 0;
}

//���̓`�F�b�N
bool DInput :: IsPressKey(enPkey key)
{
	if((m_uInputState >> key) & 1)
	{
		return true;
	}

	return false;
}