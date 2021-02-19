//��۰��ٕϐ�.
//ø�����ڼ޽�� t(n).
Texture2D		g_texColor	: register(t0);
//����ׂ�ڼ޽�- s(n).
SamplerState	g_samLinear	: register(s0);

//�ݽ����ޯ̧.
cbuffer global
{
	matrix	g_mWVP;		//ܰ���,�ޭ�,��ۼު���݂̍����ϊ��s��.
	float4	g_vColor;	//�F.
	float4	g_Alpha;	//���ߒl.
};

//���_����ނ̏o�����Ұ�.
struct VS_OUTPUT
{
	float4	Pos	: SV_Position;	//�ʒu.
	float2	Tex	: TEXCOORD;		//ø���.
};

//���_�����.
VS_OUTPUT VS(float4 Pos : POSITION,
			 float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mWVP);
	output.Tex = Tex;

	return output;
}

//�߸�ټ����.
float4 PS( VS_OUTPUT input ) : SV_Target
{
	return (g_vColor * g_texColor.Sample(g_samLinear, input.Tex) )
	* g_Alpha.x;
}