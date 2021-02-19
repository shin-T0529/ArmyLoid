//ｸﾞﾛｰﾊﾞﾙ変数.
//ﾃｸｽﾁｬ.
Texture2D		g_texColor	: register(t0);
//ｻﾝﾌﾟﾗ.
SamplerState	g_samLinear	: register(s0);

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer global_0 : register(b0)
{
	matrix	g_mW;	//ﾜｰﾙﾄﾞ行列.
	matrix	g_mWVP;	//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝの合成行列.
	float4	g_vLightDir;	//ﾗｲﾄの方向ﾍﾞｸﾄﾙ.
	float4	g_vEye;			//ｶﾒﾗ(視点).
};
cbuffer global_1 : register(b1)
{
	float4	g_vAmbient  = float4(0.0f,0.0f,0.0f,0.0f);	//環境色.
	float4	g_vDiffuse  = float4(1.0f,0.0f,0.0f,0.0f);	//拡散反射色.
	float4	g_vSpecular = float4(1.0f,1.0f,1.0f,1.0f);	//鏡面反射.
};

//頂点ｼｪｰﾀﾞの出力ﾊﾟﾗﾒｰﾀ.
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;
	float4	Color		: COLOR;
	float3	Light		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	EyeVector	: TEXCOORD2;
	float2	Tex			: TEXCOORD3;
};

//頂点ｼｪｰﾀﾞ.
VS_OUTPUT VS_Main(float4 Pos : POSITION,
				  float4 Norm: NORMAL,
				  float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//法線をﾜｰﾙﾄﾞ空間に.
	Norm.w = 0;//w=0 で移動成分を反映させない.
	output.Normal = mul(Norm, g_mW);

	output.Pos = mul(Pos, g_mWVP);
	//ﾗｲﾄ方向.
	output.Light = g_vLightDir;
	//視線ﾍﾞｸﾄﾙ.
	float3 PosWorld = mul(Pos, g_mW);
	output.EyeVector = g_vEye - PosWorld;

	float3 Normal	= normalize( output.Normal );
	float3 LightDir = normalize( output.Light );
	float3 ViewDir	= normalize( output.EyeVector );
	float4 NL		= saturate( dot( Normal, LightDir ) );

	float3 Reflect	= normalize( 2 * NL * Normal - LightDir );
	float4 Specular = pow( saturate( dot( Reflect, ViewDir )), 4);

	output.Color = g_vDiffuse * NL + Specular * g_vSpecular; 
	//ﾃｸｽﾁｬ座標.
	output.Tex = Tex;

	return output;
}

//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	float4 color
	= g_texColor.Sample(g_samLinear, input.Tex)/2.0f + input.Color/2.0f;
	return color;
}

//================================================
//	ﾃｸｽﾁｬ無し用.
//================================================
//頂点ｼｪｰﾀﾞ.
VS_OUTPUT VS_NoTex(float4 Pos : POSITION,
				   float4 Norm: NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//法線をﾜｰﾙﾄﾞ空間に.
	Norm.w = 0;//w=0 で移動成分を反映させない.
	output.Normal = mul(Norm, g_mW);

	output.Pos = mul(Pos, g_mWVP);
	//ﾗｲﾄ方向.
	output.Light = g_vLightDir;
	//視線ﾍﾞｸﾄﾙ.
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

//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
float4 PS_NoTex( VS_OUTPUT input ) : SV_Target
{
	float4 color = input.Color;
	return color;
}

//================================================
//	以降、板ﾎﾟﾘｺﾞﾝ用ｺｰﾄﾞ.
//================================================
//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer Ita_Global	: register(b2)	//ﾚｼﾞｽﾀを数分追加.
{
	matrix	g_WVP;		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝの変換行列.
	float4	g_Color;	//色.
	float4	g_UV;		//UV座標.
};

//構造体.
struct VS_ItaOUT
{
	float4	Pos	: SV_Position;
	float2	Tex	: TEXCOORD;
};

//頂点ｼｪｰﾀﾞ.
VS_ItaOUT VS_Ita( float4 Pos : POSITION,
				  float2 Tex : TEXCOORD)
{
	VS_ItaOUT output = (VS_ItaOUT)0;

	output.Pos = mul(Pos, g_WVP);
	output.Tex = Tex;

	//ﾃｸｽﾁｬのUV座標を操作する.
	output.Tex.x += g_UV.x;

	return output;
}

//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
float4 PS_Ita( VS_ItaOUT input ) : SV_Target
{
	return g_texColor.Sample(g_samLinear, input.Tex);
}