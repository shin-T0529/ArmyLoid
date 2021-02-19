//��۰��ٕϐ�.
//ø���.
Texture2D		g_texColor	: register(t0);
//�����.
SamplerState	g_samLinear	: register(s0);

//�ݽ����ޯ̧.
//ү���P��.
cbuffer per_mesh : register(b0)
{
	matrix	g_mW;	//ܰ��ލs��.
	matrix	g_mWVP;	//ܰ���,�ޭ�,��ۼު���݂̍����s��.
};
//��رْP��.
cbuffer per_material : register(b1)
{
	float4	g_vAmbient = float4(0.0f, 0.0f, 0.0f, 0.0f);	//���F.
	float4	g_vDiffuse = float4(1.0f, 0.0f, 0.0f, 0.0f);	//�g�U���ːF.
	float4	g_vSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);	//���ʔ���.
};
//�ڰђP��.
cbuffer per_frame : register(b2)
{
	float4	g_vEye;			//���(���_).
	float4	g_vLightPos;	//ײĂ̈ʒu.
	float4	g_vLightDir;	//ײĂ̕����޸��.
	float4	g_vLightRot;	//ײẲ�]�s��.
	float4	g_fIntensity;	//ײĂ̋��x(���邳) x �����g�p���Ȃ�.

};
//���_����ނ̏o�����Ұ�.
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;
	float3	Normal		: TEXCOORD0;
	float2	Tex			: TEXCOORD1;
	float3	Light		: TEXCOORD2;
	float3	EyeVector	: TEXCOORD3;

};

//���_�����.
VS_OUTPUT VS_Main(float4 Pos : POSITION,
				  float4 Norm : NORMAL,
				  float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mWVP);
	output.Normal = mul(Norm, (float3x3)g_mW);

	//���C�g����.
	//�ިڸ����ײĂ͂ǂ��ł����Ȃ��ق������A�����͖��֌W.
	output.Light = normalize(g_vLightDir);

	float3 PosWorld = mul(Pos, g_mW);

	//�����x�N�g��.
	//���[���h��ԏ�Œ��_���璸�_�֌������x�N�g��.
	output.EyeVector = normalize(g_vEye - PosWorld);

	output.Tex = Tex;

	return output;
}

//�߸�ټ����.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	//����.
	float4 ambient = g_vAmbient;

	//�g�U���ˌ�.
	float NL = saturate(dot(input.Normal, input.Light));
	float4 diffuse
		= (g_vDiffuse / 2.0f + g_texColor.Sample(g_samLinear, input.Tex) / 2.0f)*NL;

	//���ʔ��ˌ�.
	float3 reflect = normalize(2.0f*NL*input.Normal - input.Light);
	float4 specular
		= pow(saturate(dot(reflect, input.EyeVector)), 4.0f)*g_vSpecular;

	//̫����ٍŏI�F�@��L3�̍��v.
	float4 color = ambient + diffuse + specular;

	//���C�g���x�𔽉f.
	color *= g_fIntensity.x;



	return color;
}

//ø����Ȃ��̏ꍇ.
//���_�����.
VS_OUTPUT VS_NoTex(float4 Pos : POSITION,
				   float4 Norm : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mWVP);
	output.Normal = mul(Norm, (float3x3)g_mW);

	//���C�g����.
	//�ިڸ����ײĂ͂ǂ��ł����Ȃ��ق������A�����͖��֌W.
	output.Light = normalize(g_vLightDir);

	float3 PosWorld = mul(Pos, g_mW);

	//�����x�N�g��.
	//���[���h��ԏ�Œ��_���璸�_�֌������x�N�g��.
	output.EyeVector = normalize(g_vEye - PosWorld);

	return output;
}

//�߸�ټ����.
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	//����.
	float4 ambient = g_vAmbient;

	//�g�U���ˌ�.
	float NL = saturate(dot(input.Normal, input.Light));
	float4 diffuse = (g_vDiffuse)*NL;

	//���ʔ��ˌ�.
	float3 reflect = normalize(2.0f*NL*input.Normal - input.Light);
	float4 specular
		= pow(saturate(dot(reflect, input.EyeVector)), 4.0f)*g_vSpecular;

	//̫����ٍŏI�F�@��L3�̍��v.
	float4 color = ambient + diffuse + specular;

	//���C�g���x�𔽉f.
	color *= g_fIntensity.x;



	return color;
}
