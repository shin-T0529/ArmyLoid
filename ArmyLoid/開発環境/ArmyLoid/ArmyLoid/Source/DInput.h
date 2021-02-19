#ifndef __DIINPUT__
#define __DIINPUT__

#include<dinput.h>

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")	//�R���g���[���[����Ŏg�p

//�萔
#define N_OFFSET	500	//�W���C�X�e�B�b�N�̃j���[�g����

//DInput2�p
#define PadNumber	2	//�p�b�h�̐�

//�L�[���̗񋓑�
enum enPkey
{
	enPkey_U,		//Up
	enPkey_D,		//Down
	enPkey_L,		//Left
	enPkey_R,		//Right		

	enPkey_UR,		//UpR
	enPkey_DR,		//DownR
	enPkey_LR,		//LeftR
	enPkey_RR,		//RightR	

	enPkey_01,		
	enPkey_02,		
	enPkey_03,	
	enPkey_04,		
	enPkey_05,		
	enPkey_06,		
	enPkey_07,	
	enPkey_08,		
	enPkey_09,		
	enPkey_10,		

	enPkey_Max,		//���͂̍ő�l
};

class DInput
{
public:
	DInput();
	~DInput();

	//DirectInput �������ݒ�
	bool initDI(HWND hWnd);

	//���͏��X�V�֐�
	HRESULT UpdateInputState();
	HRESULT UpdateInputState2();

	//���̓`�F�b�N�֐�
	bool IsPressKey(enPkey key);

	LPDIRECTINPUTDEVICE8	m_pPad[2];//�f�o�C�X�I�u�W�F�N�g�i�R���g���[���[�j

private:
	LPDIRECTINPUT8			m_pDI;					//DxInput�I�u�W�F�N�g

	//LPDIRECTINPUTDEVICE8	m_pPad;					//�f�o�C�X�I�u�W�F�N�g�i�R���g���[���[�j

	DIJOYSTATE JoyState;

	unsigned int m_uInputState;	//���͏��̕ۊǗp

	//���͏��̒ǉ�
	void AddInputState(enPkey key);
	//���͏��̏�����
	void InitInputState();

};



#endif	//__DIINPUT__