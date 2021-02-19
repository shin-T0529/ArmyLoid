#include "DirectSoundResource.h"

clsSoundResource::clsSoundResource()
	: m_hWnd(nullptr)
	, m_ppStaticSound(nullptr)
	, isound(0)
{

}

clsSoundResource::~clsSoundResource()
{
	for (int i = enStaticSound_Max - 1; i >= 0; i--)
	{
		ReleaseSound(i);
	}

	if (m_ppStaticSound != nullptr)
	{
		delete[] m_ppStaticSound;
		m_ppStaticSound = nullptr;
	}

}

HRESULT clsSoundResource::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	m_ppStaticSound = new clsDirectSound*[enStaticSound_Max];
	for (int i = 0; i < enStaticSound_Max; i++)
	{
		//������:NULL.
		m_ppStaticSound[i] = nullptr;
	}
	return S_OK;

}

HRESULT clsSoundResource::CreateSound(int enSound, char* FilePass)
{
	//�͈͓����ǂ����`�F�b�N����.
	if (CheckRangeStaticSound(enSound) == false)
	{
		return E_FAIL;
	}
	//���f�������łɑ��݂��Ă��邩.
	if (m_ppStaticSound[enSound] != nullptr)
	{
		return E_FAIL;
	}
	m_ppStaticSound[enSound] = new clsDirectSound;
	m_ppStaticSound[enSound]->Init(m_hWnd, FilePass);

	return S_OK;
}

HRESULT clsSoundResource::ReleaseSound(int enSound)
{
	if (CheckRangeStaticSound(enSound) == false)
	{
		return E_FAIL;
	}
	if (m_ppStaticSound[enSound] == nullptr)
	{
		return S_OK;
	}

	//�̈�̉��.
	delete m_ppStaticSound[enSound];	//�폜.
	m_ppStaticSound[enSound] = nullptr;
	return S_OK;
}

clsDirectSound* clsSoundResource::GetSound(int enSound)
{
	if (CheckRangeStaticSound(enSound) == false)
	{
		return NULL;
	}
	if (m_ppStaticSound[enSound] == NULL)
	{
		return NULL;
	}
	return m_ppStaticSound[enSound];
}

HRESULT clsSoundResource::LoadSoundDataAll()
{
	char* temp = new char[255];
	memcpy(temp, LoadData->LoadFilePass("Data\\csv\\SoundFilePass.csv", 1 + isound), sizeof(char) * 255);
	//���Ԃ�wav�t�@�C����Ǎ�.
	CreateSound(isound, temp);
	isound++;
	delete temp;
	return S_OK;
}


bool clsSoundResource::CheckRangeStaticSound(int enSound)
{
	//�͈͓����`�F�b�N.
	if (enStaticSound_Min <= enSound && enSound < enStaticSound_Max)
	{
		return true;
	}
	return false;
}

