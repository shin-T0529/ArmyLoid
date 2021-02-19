#ifndef GLOBAL_H	//�ݸٰ�޶ް��.
#define GLOBAL_H

//�x���ɂ��Ă̺��ޕ��͂𖳌��ɂ���. 4005:�Ē�`.
#pragma warning(disable:4005)
//#pragma warning(disable:3206)

#include <Windows.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>
#include <D3D10.h>

//ײ����.
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx10.lib" )


//�萔(�ǂ̃\�[�X�ɂ��g���\���̂�����̂̂�).
#define WND_W	1280	//����޳��.
#define WND_H	720		//����޳����.
#define WND_W_X	1300	//����޳��.
const int  ZERO	= 0;
const float ChangeSpeed = 0.5f;
const float ChangeManualSpeed = 0.3f;
const int PlayerMAX = 2;		//�v���C���[�l��.
const int BulletMax = 4;
//�����_���[�W.
const int BoostDamageG = 5;
const int BoostDamageM = 15;
const int BoostDamageB = 25;

const int GatlingReload = 50;
const double AnimPlaySpeed = 0.01;

//�Ǎ��p.
const int	MeshMax = 2;		//���b�V�����f���̐�.
const int	SkinMax = 2;		//���b�V�����f���̐�.

const float	WRepulsion = 1.4f;	//�ǂ̔���̔�������.

//�ǂݍ��ݎ��̃T�C�Y�ݒ�.
const D3DXVECTOR3 SkinSize = D3DXVECTOR3(0.06f, 0.06f, 0.06f);
const D3DXVECTOR3 AnotherSkinSize = D3DXVECTOR3(0.04f, 0.04f, 0.04f);

//���U���g��ʋ��ʂŎg�����W.
const D3DXVECTOR3 LeftCursor(200.0f, 568.0f, 0.0f);
const D3DXVECTOR3 RightCursor(810.0f, 568.0f, 0.0f);

//std::���Ȃ�.
using namespace std;

enum class SceneIndex
{
	Nothing = -1,
	TITLE = 0,
	MANUAL = 1,
	PLAY = 2,
	P1WIN = 3,
	P2WIN = 4,
	DRAW = 5,
	CREDIT = 6,

	Start = TITLE,
	End = CREDIT,
};


#endif//#ifndef GLOBAL_H
