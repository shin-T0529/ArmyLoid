//��۰��ٕϐ�.

//ø�����ڼ޽�� t(n).
Texture2D g_texColor	: register(t0);
//����ׂ�ڼ޽�� s(n).
SamplerState g_samLinear: register(s0);

//�ݽ����ޯ̧.
cbuffer global
{
	matrix g_mW;		//ܰ��ލs��.
	matrix g_mWVP;		//ܰ���,�ޭ�,�ˉe�̍����s��.
	float4 g_vLightDir;	//ײĕ���(�޸��).
	float4 g_vColor;	//�g�U����(�F).
	float4 g_vEye;		//���(���_).
};

//�\����.
struct VS_OUTPUT
{
	float4 Pos		: SV_Position;
	float4 Color	: COLOR;
	float3 Light	: TEXCOORD0;
	float3 Normal	: TEXCOORD1;
	float3 EyeVector: TEXCOORD2;
	float2 Tex		: TEXCOORD3;
};

//--------------------------------------
//	���_(�ްï��)�����.
//--------------------------------------
VS_OUTPUT VS(float4 Pos	  : POSITION,
			 float4 Normal: NORMAL,
		     float2 Tex	  : TEXCOORD )
{
	VS_OUTPUT output = (VS_OUTPUT)0;//�\���̂̏�����.

	// mul(A,B) : A��B�̊|���Z.
	output.Pos = mul(Pos, g_mWVP);

	//����.
	//����؂ɖ@�����|����.
	output.Normal = mul(Normal, (float3x3)g_mW);
	output.Light = g_vLightDir;

	float3 PosWorld = mul(Pos, g_mW);
	output.EyeVector = g_vEye - PosWorld;

	output.Tex = Tex;

	return output;
}

//**************************************
//	׽�ײ��(�G��Ȃ�).
//**************************************

//--------------------------------------
//	�߸�ټ����.
//--------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
	//����.
	float3 Normal = normalize(input.Normal);
	float3 LightDir = normalize(input.Light);
	float3 ViewDir = normalize(input.EyeVector);
	float4 NL = saturate(dot(Normal, LightDir));

	//Reflect:����.
	//�ʂ̔��˒l�����߂�.
	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = 2 * pow(saturate(dot(Reflect, ViewDir)), 2);

	float4 color = g_texColor.Sample(g_samLinear, input.Tex);//�F��Ԃ�.

	return color * NL + specular;
}

// normalize()		: ���K������.
// saturate()		: 0�`1�ɸ���߂���(�����I��1�Ɏ��߂�).
// dot( A, B )		: A��B�̓��ς����߂�.
// pow( A, B )		: A��B���Ԃ�.

// Specular(��߷��)	: ���ːF.
// Ambient(���޴��)	: ���F.
// Diffuse(�ި̭���): �g�U���ːF.
// Emissive(�Я���)	: ���Ȕ����F.
