//��۰���.
//ø�����ڼ޽� t(n).
Texture2D		g_Texture	: register(t0);
//����ׂ�ڼ޽� s(n).
SamplerState	g_Sampler	: register(s0);

//�ݽ����ޯ̧.
cbuffer global
{
	matrix g_mWVP;	//World,View,Proj�̕ϊ������s��.
};

//�\����.
struct GS_INPUT
{
	float4	Pos	: SV_Position;
};

struct PS_INPUT
{
	float4 Pos	: SV_Position;
	float2 UV	: TEXCOORD;
};

//���_�����.
GS_INPUT VS(float4 Pos : POSITION)
{
	GS_INPUT output;
	output.Pos = Pos;	//�����ł͒��_��ϊ����Ȃ�.
						//���̂܂܂ż޵��ؼ���ނɓn��.
	return output;
}


//�޵��ؼ����.
//����غ�݂𕪊�����.
[maxvertexcount(4)]	//���_�̍ő吔.
void GS_Point(point GS_INPUT input[1],
	inout TriangleStream<PS_INPUT> TriStream)
	// TriangleStream:�޵��ؼ���ނō쐬�����O�p�`.
{
	float w = 0;//�S�̒��_�S�Ă�w�l��1�Ԗ�[0]�̒��_��w�ɍ��킹��.
				//���킹��͉̂��Ԗڂł�����.�S�Ă������Ȃ�.
				//��غ�݂��߰����|�����Ă��܂��̂�h�~.

	//�l�p�`�̒��_.
	PS_INPUT p = (PS_INPUT)0;
	p.Pos = mul(input[0].Pos, g_mWVP);
	w = p.Pos.w;
	p.UV = float2(0.0f, 0.0f);
	TriStream.Append(p);	//Append():���_�̒ǉ�.

	p.Pos = input[0].Pos + float4(6.0f, 0.0f, 0.0f, 0.0f);
	p.Pos = mul(p.Pos, g_mWVP);
	p.Pos.w = w;
	p.UV = float2(1.0f, 0.0f);
	TriStream.Append(p);

	p.Pos = input[0].Pos + float4(0.0f,-6.0f, 0.0f, 0.0f);
	p.Pos = mul(p.Pos, g_mWVP);
	p.Pos.w = w;
	p.UV = float2(0.0f, 1.0f);
	TriStream.Append(p);

	p.Pos = input[0].Pos + float4(6.0f,-6.0f, 0.0f, 0.0f);
	p.Pos = mul(p.Pos, g_mWVP);
	p.Pos.w = w;
	p.UV = float2(1.0f, 1.0f);
	TriStream.Append(p);

	TriStream.RestartStrip();	//�R�̒��_��ǉ�������A�Ăяo���Đݒ�.
}


//�߸�ټ����.
float4 PS(PS_INPUT input) : SV_Target
{
	//�w��ʒu��ø�����\��.
	return g_Texture.Sample(g_Sampler, input.UV);
}