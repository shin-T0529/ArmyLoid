//��۰��ٕϐ�.
//ø�����ڼ޽� t(n).
Texture2D		g_texColor	: register(t0);
//����ׂ�ڼ޽� s(n).
SamplerState	g_samLinear	: register(s0);

//�ݽ����ޯ̧.
cbuffer global : register(b0)
{
	matrix	g_mW		: packoffset(c0);
	float	g_fViewPortW: packoffset(c4);
	float	g_fViewPortH: packoffset(c5);
	float	g_fAlpha	: packoffset(c6);
	float2	g_vUV		: packoffset(c7);
};

//���_����ނ̏o�����Ұ�.
struct VS_OUTPUT
{
	float4	Pos	: SV_Position;
	float2	UV	: TEXCOORD;
};

//���_�����.
VS_OUTPUT VS_Main(float4 Pos	: POSITION,
				  float2 UV		: TEXCOORD)
{
	//������.
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, g_mW);

	//��ذݍ��W�ɍ��킹��v�Z.
	output.Pos.x = (output.Pos.x / g_fViewPortW) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - (output.Pos.y / g_fViewPortH) * 2.0f;

	//UV�ݒ�.
	output.UV = UV;
	//UV���W�����炷.
	output.UV.x += g_vUV.x;
	output.UV.y += g_vUV.y;

	return output;
}

//�߸�ټ����.
float4 PS_Main( VS_OUTPUT input ) :SV_Target
{
	float4 color = g_texColor.Sample(g_samLinear, input.UV);
	color.a *= g_fAlpha;//��̧�l���|�����킹��.
	return color;
}