//ｸﾞﾛｰﾊﾞﾙ変数.
//ﾃｸｽﾁｬ.
Texture2D		g_texColor	: register(t0);
//ｻﾝﾌﾟﾗ.
SamplerState	g_samLinear	: register(s0);

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
//ﾒｯｼｭ単位.
cbuffer per_mesh : register(b0)
{
	matrix	g_mW;	//ﾜｰﾙﾄﾞ行列.
	matrix	g_mWVP;	//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝの合成行列.
};
//ﾏﾃﾘｱﾙ単位.
cbuffer per_material : register(b1)
{
	float4	g_vAmbient = float4(0.0f, 0.0f, 0.0f, 0.0f);	//環境色.
	float4	g_vDiffuse = float4(1.0f, 0.0f, 0.0f, 0.0f);	//拡散反射色.
	float4	g_vSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);	//鏡面反射.
};
//ﾌﾚｰﾑ単位.
cbuffer per_frame : register(b2)
{
	float4	g_vEye;			//ｶﾒﾗ(視点).
	float4	g_vLightPos;	//ﾗｲﾄの位置.
	float4	g_vLightDir;	//ﾗｲﾄの方向ﾍﾞｸﾄﾙ.
	float4	g_vLightRot;	//ﾗｲﾄの回転行列.
	float4	g_fIntensity;	//ﾗｲﾄの強度(明るさ) x しか使用しない.

};
//頂点ｼｪｰﾀﾞの出力ﾊﾟﾗﾒｰﾀ.
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;
	float3	Normal		: TEXCOORD0;
	float2	Tex			: TEXCOORD1;
	float3	Light		: TEXCOORD2;
	float3	EyeVector	: TEXCOORD3;

};

//頂点ｼｪｰﾀﾞ.
VS_OUTPUT VS_Main(float4 Pos : POSITION,
				  float4 Norm : NORMAL,
				  float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mWVP);
	output.Normal = mul(Norm, (float3x3)g_mW);

	//ライト方向.
	//ﾃﾞｨﾚｸｼｮﾅﾙﾗｲﾄはどこでもおなじほうこう、いちは無関係.
	output.Light = normalize(g_vLightDir);

	float3 PosWorld = mul(Pos, g_mW);

	//視線ベクトル.
	//ワールド空間上で頂点から頂点へ向かうベクトル.
	output.EyeVector = normalize(g_vEye - PosWorld);

	output.Tex = Tex;

	return output;
}

//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	//環境光.
	float4 ambient = g_vAmbient;

	//拡散反射光.
	float NL = saturate(dot(input.Normal, input.Light));
	float4 diffuse
		= (g_vDiffuse / 2.0f + g_texColor.Sample(g_samLinear, input.Tex) / 2.0f)*NL;

	//鏡面反射光.
	float3 reflect = normalize(2.0f*NL*input.Normal - input.Light);
	float4 specular
		= pow(saturate(dot(reflect, input.EyeVector)), 4.0f)*g_vSpecular;

	//ﾌｫﾝﾓﾃﾞﾙ最終色　上記3つの合計.
	float4 color = ambient + diffuse + specular;

	//ライト強度を反映.
	color *= g_fIntensity.x;



	return color;
}

//ﾃｸｽﾁｬなしの場合.
//頂点ｼｪｰﾀﾞ.
VS_OUTPUT VS_NoTex(float4 Pos : POSITION,
				   float4 Norm : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mWVP);
	output.Normal = mul(Norm, (float3x3)g_mW);

	//ライト方向.
	//ﾃﾞｨﾚｸｼｮﾅﾙﾗｲﾄはどこでもおなじほうこう、いちは無関係.
	output.Light = normalize(g_vLightDir);

	float3 PosWorld = mul(Pos, g_mW);

	//視線ベクトル.
	//ワールド空間上で頂点から頂点へ向かうベクトル.
	output.EyeVector = normalize(g_vEye - PosWorld);

	return output;
}

//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	//環境光.
	float4 ambient = g_vAmbient;

	//拡散反射光.
	float NL = saturate(dot(input.Normal, input.Light));
	float4 diffuse = (g_vDiffuse)*NL;

	//鏡面反射光.
	float3 reflect = normalize(2.0f*NL*input.Normal - input.Light);
	float4 specular
		= pow(saturate(dot(reflect, input.EyeVector)), 4.0f)*g_vSpecular;

	//ﾌｫﾝﾓﾃﾞﾙ最終色　上記3つの合計.
	float4 color = ambient + diffuse + specular;

	//ライト強度を反映.
	color *= g_fIntensity.x;



	return color;
}
