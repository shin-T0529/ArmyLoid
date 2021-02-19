//ｸﾞﾛｰﾊﾞﾙ変数.
//ﾃｸｽﾁｬはﾚｼﾞｽﾀｰ t(n).
Texture2D		g_texColor	: register(t0);
//ｻﾝﾌﾟﾗはﾚｼﾞｽﾀ- s(n).
SamplerState	g_samLinear	: register(s0);

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer global
{
	matrix	g_mWVP;		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝの合成変換行列.
	float4	g_vColor;	//色.
	float4	g_Alpha;	//透過値.
};

//頂点ｼｪｰﾀﾞの出力ﾊﾟﾗﾒｰﾀ.
struct VS_OUTPUT
{
	float4	Pos	: SV_Position;	//位置.
	float2	Tex	: TEXCOORD;		//ﾃｸｽﾁｬ.
};

//頂点ｼｪｰﾀﾞ.
VS_OUTPUT VS(float4 Pos : POSITION,
			 float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mWVP);
	output.Tex = Tex;

	return output;
}

//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
float4 PS( VS_OUTPUT input ) : SV_Target
{
	return (g_vColor * g_texColor.Sample(g_samLinear, input.Tex) )
	* g_Alpha.x;
}