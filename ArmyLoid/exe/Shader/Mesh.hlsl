//��۰��ٕϐ�.
//ø���.
Texture2D		g_texColor	: register(t0);
//�����.
SamplerState	g_samLinear	: register(s0);

//�ݽ����ޯ̧.
cbuffer global_0 : register(b0)
{
	matrix	g_mW;	//ܰ��ލs��.
	matrix	g_mWVP;	//ܰ���,�ޭ�,��ۼު���݂̍����s��.
	float4	g_vLightDir;	//ײĂ̕����޸��.
	float4	g_vEye;			//���(���_).
};
cbuffer global_1 : register(b1)
{
	float4	g_vAmbient  = float4(0.0f,0.0f,0.0f,0.0f);	//���F.
	float4	g_vDiffuse  = float4(1.0f,0.0f,0.0f,0.0f);	//�g�U���ːF.
	float4	g_vSpecular = float4(1.0f,1.0f,1.0f,1.0f);	//���ʔ���.
};

//���_����ނ̏o�����Ұ�.
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;
	float4	Color		: COLOR;
	float3	Light		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	EyeVector	: TEXCOORD2;
	float2	Tex			: TEXCOORD3;
};

//���_�����.
VS_OUTPUT VS_Main(float4 Pos : POSITION,
				  float4 Norm: NORMAL,
				  float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//�@����ܰ��ދ�Ԃ�.
	Norm.w = 0;//w=0 �ňړ������𔽉f�����Ȃ�.
	output.Normal = mul(Norm, g_mW);

	output.Pos = mul(Pos, g_mWVP);
	//ײĕ���.
	output.Light = g_vLightDir;
	//�����޸��.
	float3 PosWorld = mul(Pos, g_mW);
	output.EyeVector = g_vEye - PosWorld;

	float3 Normal	= normalize( output.Normal );
	float3 LightDir = normalize( output.Light );
	float3 ViewDir	= normalize( output.EyeVector );
	float4 NL		= saturate( dot( Normal, LightDir ) );

	float3 Reflect	= normalize( 2 * NL * Normal - LightDir );
	float4 Specular = pow( saturate( dot( Reflect, ViewDir )), 4);

	output.Color = g_vDiffuse * NL + Specular * g_vSpecular; 
	//ø������W.
	output.Tex = Tex;

	return output;
}

//�߸�ټ����.
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	float4 color
	= g_texColor.Sample(g_samLinear, input.Tex)/2.0f + input.Color/2.0f;
	return color;
}

//================================================
//	ø��������p.
//================================================
//���_�����.
VS_OUTPUT VS_NoTex(float4 Pos : POSITION,
				   float4 Norm: NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//�@����ܰ��ދ�Ԃ�.
	Norm.w = 0;//w=0 �ňړ������𔽉f�����Ȃ�.
	output.Normal = mul(Norm, g_mW);

	output.Pos = mul(Pos, g_mWVP);
	//ײĕ���.
	output.Light = g_vLightDir;
	//�����޸��.
	float3 PosWorld = mul(Pos, g_mW);
	output.EyeVector = g_vEye - PosWorld;

	float3 Normal	= normalize( output.Normal );
	float3 LightDir = normalize( output.Light );
	float3 ViewDir	= normalize( output.EyeVector );
	float4 NL		= saturate( dot( Normal, LightDir ) );

	float3 Reflect	= normalize( 2 * NL * Normal - LightDir );
	float4 Specular = pow( saturate( dot( Reflect, ViewDir )), 4);

	output.Color = g_vDiffuse * NL + Specular * g_vSpecular; 

	return output;
}

//�߸�ټ����.
float4 PS_NoTex( VS_OUTPUT input ) : SV_Target
{
	float4 color = input.Color;
	return color;
}

//================================================
//	�ȍ~�A����غ�ݗp����.
//================================================
//�ݽ����ޯ̧.
cbuffer Ita_Global	: register(b2)	//ڼ޽��𐔕��ǉ�.
{
	matrix	g_WVP;		//ܰ���,�ޭ�,��ۼު���݂̕ϊ��s��.
	float4	g_Color;	//�F.
	float4	g_UV;		//UV���W.
};

//�\����.
struct VS_ItaOUT
{
	float4	Pos	: SV_Position;
	float2	Tex	: TEXCOORD;
};

//���_�����.
VS_ItaOUT VS_Ita( float4 Pos : POSITION,
				  float2 Tex : TEXCOORD)
{
	VS_ItaOUT output = (VS_ItaOUT)0;

	output.Pos = mul(Pos, g_WVP);
	output.Tex = Tex;

	//ø�����UV���W�𑀍삷��.
	output.Tex.x += g_UV.x;

	return output;
}

//�߸�ټ����.
float4 PS_Ita( VS_ItaOUT input ) : SV_Target
{
	return g_texColor.Sample(g_samLinear, input.Tex);
}