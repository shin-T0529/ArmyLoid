//ｸﾞﾛｰﾊﾞﾙ変数.
//ﾃｸｽﾁｬはﾚｼﾞｽﾀ t(n).
Texture2D		g_texColor	: register(t0);
//ｻﾝﾌﾟﾗはﾚｼﾞｽﾀ s(n).
SamplerState	g_samLinear	: register(s0);

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer global : register(b0)
{
	matrix	g_mW		: packoffset(c0);
	float	g_fViewPortW: packoffset(c4);
	float	g_fViewPortH: packoffset(c5);
	float	g_fAlpha	: packoffset(c6);
	float2	g_vUV		: packoffset(c7);
};

//頂点ｼｪｰﾀﾞの出力ﾊﾟﾗﾒｰﾀ.
struct VS_OUTPUT
{
	float4	Pos	: SV_Position;
	float2	UV	: TEXCOORD;
};

//頂点ｼｪｰﾀﾞ.
VS_OUTPUT VS_Main(float4 Pos	: POSITION,
				  float2 UV		: TEXCOORD)
{
	//初期化.
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, g_mW);

	//ｽｸﾘｰﾝ座標に合わせる計算.
	output.Pos.x = (output.Pos.x / g_fViewPortW) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - (output.Pos.y / g_fViewPortH) * 2.0f;

	//UV設定.
	output.UV = UV;
	//UV座標をずらす.
	output.UV.x += g_vUV.x;
	output.UV.y += g_vUV.y;

	return output;
}

//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
float4 PS_Main( VS_OUTPUT input ) :SV_Target
{
	float4 color = g_texColor.Sample(g_samLinear, input.UV);
	color.a *= g_fAlpha;//ｱﾙﾌｧ値を掛け合わせる.
	return color;
}