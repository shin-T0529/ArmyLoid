#include "cCommon.h"

//����(�A���t�@�u�����h)�ݒ�̐؂�ւ�
void clsCommon::SetBlend(bool Flag)
{
	//�A���t�@�u�����h�p�u�����h�X�e�[�g�\����
	//png�t�@�C�����ɃA���t�@��񂪂���̂œ��߂���悤�Ƀu�����h�X�e�[�g�Őݒ肷��
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));//������

	blendDesc.IndependentBlendEnable = false;
	//false:RenderTarget[0]�̃����o�[�̂ݎg�p�ł���
	//true :RenderTarget[1]~[7]���g�p�ł���
	//(�����_�[�^�[�Q�b�g���ɓƗ������u�����h����)
	blendDesc.AlphaToCoverageEnable = false;
	//true  : �A���t�@�g�D�J�o���b�W���g�p����
	blendDesc.RenderTarget[0].BlendEnable = Flag;
	//true  : �A���t�@�u�����h���g�p����

	//���f�ނɑ΂���ݒ�
	blendDesc.RenderTarget[0].SrcBlend
		= D3D11_BLEND_SRC_ALPHA;	//�A���t�@�u�����h�w��

	//�d�˂�f�ނɑ΂���ݒ�
	blendDesc.RenderTarget[0].DestBlend
		= D3D11_BLEND_INV_SRC_ALPHA;	//�A���t�@�u�����h�w��

	//�u�����h�I�v�V����
	blendDesc.RenderTarget[0].BlendOp
		= D3D11_BLEND_OP_ADD;	//ADD:���Z����

	//���f�ނ̃A���t�@�ɑ΂���ݒ�
	blendDesc.RenderTarget[0].SrcBlendAlpha
		= D3D11_BLEND_ONE;	//���̂܂܎g�p

	//�d�˂�f�ނ̃A���t�@�ɑ΂���ݒ�
	blendDesc.RenderTarget[0].DestBlendAlpha
		= D3D11_BLEND_ZERO;	//�������Ȃ�

	//�A���t�@�̃u�����h�I�v�V����
	blendDesc.RenderTarget[0].BlendOpAlpha
		= D3D11_BLEND_OP_ADD;	//ADD:���Z����

	//�s�N�Z�����Ƃ̏������݃}�X�N
	blendDesc.RenderTarget[0].RenderTargetWriteMask
		= D3D11_COLOR_WRITE_ENABLE_ALL;	//���ׂĂ̐���(RGBA)�ւ̃f�[�^�̊i�[������


	//�u�����h�X�e�[�g�̍쐬
	if (FAILED(	m_pDevice11->CreateBlendState(&blendDesc, &m_pBlendState)))
	{
		MessageBox(NULL, "�u�����h�X�e�[�g�쐬���s", "error", MB_OK);
	}

	//�u�����h�X�e�[�g�̐ݒ�
	UINT mask = 0xffffffff;			//�}�X�N�l
	m_pContext11->OMSetBlendState(m_pBlendState, NULL, mask);
}
