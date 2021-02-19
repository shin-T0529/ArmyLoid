#include "CEffects.h"
#include "Global.h"

//�`��p�ݽ�ݽ�ő吔.
const int g_RenderSpriteMax = 2000;
//�̪�ĊǗ��p�ݽ�ݽ�ő吔.
const int g_EffectInstanceMax = 1000;

//�ݽ�׸�.
clsEffects::clsEffects()
	: m_pManager(nullptr)
	, m_pRender(nullptr)
	, m_pEffect(nullptr)
	, m_Handle(0)
{
}

//�޽�׸�.
clsEffects::~clsEffects()
{

}


//�������֐�.
HRESULT clsEffects::Init()
{
	//COM�̏�����.
	CoInitializeEx(NULL, NULL);

	return S_OK;
}

//�\�z�֐�.
HRESULT clsEffects::Create(
	ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11)
{
	//�`��p�ݽ�ݽ�̐���.
	m_pRender =
		::EffekseerRendererDX11::Renderer::Create(
		pDevice11, pContext11, g_RenderSpriteMax);

	//�̪�ĊǗ��p�ݽ�ݽ�̐���.
	m_pManager = ::Effekseer::Manager::Create(g_EffectInstanceMax);

	//�`��p�ݽ�ݽ����`��@�\��ݒ�.
	m_pManager->SetSpriteRenderer(m_pRender->CreateSpriteRenderer());
	m_pManager->SetRibbonRenderer(m_pRender->CreateRibbonRenderer());
	m_pManager->SetRingRenderer(m_pRender->CreateRingRenderer());
	m_pManager->SetModelRenderer(m_pRender->CreateModelRenderer());

	//�`��p�ݽ�ݽ����ø����̓Ǎ��@�\��ݒ�.
	//�Ǝ��g���\�A���݂�̧�ق���ǂݍ���ł���.
	m_pManager->SetTextureLoader(m_pRender->CreateTextureLoader());
	m_pManager->SetModelLoader(m_pRender->CreateModelLoader());
	return S_OK;
}


//�j���֐�.
HRESULT clsEffects::Destroy()
{
	//�̪���ް��̉��.

	//��ɴ̪�ĊǗ��p�ݽ�ݽ��j��.
	m_pManager->Destroy();
	//���ɕ`��p�ݽ�ݽ��j��.
	m_pRender->Destroy();
	//COM�̏I������.
	CoUninitialize();

	return S_OK;
}

//�ް��Ǎ��֐�.
HRESULT clsEffects::LoadData(const char* cFilepass)
{
	const char* const end = cFilepass + sizeof(cFilepass);

	char16_t c16s[32];
	mbstate_t mbstate = { 0 };
	int mbi = 0;
	int c16i = 0;

	for (;; ) {
		const int result = (int)mbrtoc16(&c16s[c16i], &cFilepass[mbi], end - &cFilepass[mbi], &mbstate);
		if (result > 0) {
			c16i++;
			mbi += result;
		}
		else if (result == 0) {
			// �k�������ɓ��B
			break;
		}
		else if (result == -1 || result == -2) {
			c16s[c16i] = u'/';
			c16i++;
			mbi += 1;
//			fputs("�����ȃo�C�g��ł��B\n", stderr);
		}
		else if (result == -3) {
			// char16_t �̕������\������c��̕����𓾂��B
			// �}���`�o�C�g�������̃o�C�g�͏���Ă��Ȃ��B
			c16i++;
		}
	}

	//�̪�Ă̓ǂݍ���.
	m_pEffect = Effekseer::Effect::Create(m_pManager, c16s);
	if (m_pEffect == nullptr) {
		_ASSERT_EXPR(false, L"�t�@�C���ǂݍ��ݎ��s");
		return E_FAIL;
	}

	return S_OK;
}

//�ް�����֐�.
HRESULT clsEffects::ReleaseData()
{
	//�̪�Ẳ��.
	ES_SAFE_RELEASE(m_pEffect);

	return S_OK;
}

//�`��.
void clsEffects::Render(
	const D3DXMATRIX& mView,
	const D3DXMATRIX& mProj,
	const D3DXVECTOR3& vCamPos)
{
	//�ޭ��s���ݒ�.
	SetViewMatrix(mView);

	//��ۼު���ݍs���ݒ�.
	SetProjectionMatrix(mProj);

	//�̪�Ă̍X�V����.
	m_pManager->Update();

	//------------------------------------------------
	//	Effekseer�����ݸ�.
	//------------------------------------------------
	//�̪�Ă̕`��J�n�������s��.
	m_pRender->BeginRendering();

	//�̪�Ă̕`����s��.
	m_pManager->Draw();

	//�̪�Ă̕`��I�����s��.
	m_pRender->EndRendering();
}

//�ޭ��s���ݒ�.
void clsEffects::SetViewMatrix(const D3DXMATRIX& mView)
{
	Effekseer::Matrix44 tmpEsCamMat;
	tmpEsCamMat = ConvertToMatrixEfk(mView);

	//��׍s���ݒ�.
	m_pRender->SetCameraMatrix(tmpEsCamMat);
}
//��ۼު���ݍs���ݒ�.
void clsEffects::SetProjectionMatrix(const D3DXMATRIX& mProj)
{
	Effekseer::Matrix44 tmpEsPrjMat;
	tmpEsPrjMat = ConvertToMatrixEfk(mProj);

	//��ۼު���ݍs���ݒ�.
	m_pRender->SetProjectionMatrix(tmpEsPrjMat);
}

//---------------------------------------------------------------------
//	DirectX Vector3 �� Effekseer Vector3 �ɕϊ�����.
//---------------------------------------------------------------------
Effekseer::Vector3D clsEffects::ConvertToVector3Efk(const D3DXVECTOR3& SrcVec3Dx)
{
	return Effekseer::Vector3D(SrcVec3Dx.x, SrcVec3Dx.y, SrcVec3Dx.z);
}
//---------------------------------------------------------------------
//	Effekseer Vector3 �� DirectX Vector3 �ɕϊ�����.
//---------------------------------------------------------------------
D3DXVECTOR3 clsEffects::ConvertToVector3Dx(const Effekseer::Vector3D& SrcVec3Efk)
{
	return D3DXVECTOR3(SrcVec3Efk.X, SrcVec3Efk.Y, SrcVec3Efk.Z);
}
//---------------------------------------------------------------------
//	�s��ϊ��֐�.
//---------------------------------------------------------------------
Effekseer::Matrix44 clsEffects::ConvertToMatrixEfk(const D3DXMATRIX& SrcMatDx)
{
	Effekseer::Matrix44 OutMatEfk;
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			OutMatEfk.Values[i][j] = SrcMatDx.m[i][j];
		}
	}
	return OutMatEfk;
}
//---------------------------------------------------------------------
//	�s��ϊ��֐�.
//---------------------------------------------------------------------
D3DXMATRIX clsEffects::ConvertToMatrixDx(const Effekseer::Matrix44& SrcMatEfk)
{
	D3DXMATRIX OutMatDx;
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			OutMatDx.m[i][j] = SrcMatEfk.Values[i][j];
		}
	}
	return OutMatDx;
}