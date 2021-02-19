//ｸﾞﾛｰﾊﾞﾙ.
//ﾃｸｽﾁｬはﾚｼﾞｽﾀ t(n).
Texture2D		g_Texture	: register(t0);
//ｻﾝﾌﾟﾗはﾚｼﾞｽﾀ s(n).
SamplerState	g_Sampler	: register(s0);

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer global
{
	matrix g_mWVP;	//World,View,Projの変換合成行列.
};

//構造体.
struct GS_INPUT
{
	float4	Pos	: SV_Position;
};

struct PS_INPUT
{
	float4 Pos	: SV_Position;
	float2 UV	: TEXCOORD;
};

//頂点ｼｪｰﾀﾞ.
GS_INPUT VS(float4 Pos : POSITION)
{
	GS_INPUT output;
	output.Pos = Pos;	//ここでは頂点を変換しない.
						//そのままでｼﾞｵﾒﾄﾘｼｪｰﾀﾞに渡す.
	return output;
}


//ｼﾞｵﾒﾄﾘｼｪｰﾀﾞ.
//※ﾎﾟﾘｺﾞﾝを分割する.
[maxvertexcount(4)]	//頂点の最大数.
void GS_Point(point GS_INPUT input[1],
	inout TriangleStream<PS_INPUT> TriStream)
	// TriangleStream:ｼﾞｵﾒﾄﾘｼｪｰﾀﾞで作成される三角形.
{
	float w = 0;//４つの頂点全てのw値を1番目[0]の頂点のwに合わせる.
				//合わせるのは何番目でもいい.全てが同じなら.
				//ﾎﾟﾘｺﾞﾝにﾊﾟｰｽが掛かってしまうのを防止.

	//四角形の頂点.
	PS_INPUT p = (PS_INPUT)0;
	p.Pos = mul(input[0].Pos, g_mWVP);
	w = p.Pos.w;
	p.UV = float2(0.0f, 0.0f);
	TriStream.Append(p);	//Append():頂点の追加.

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

	TriStream.RestartStrip();	//３つの頂点を追加した後、呼び出して設定.
}


//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
float4 PS(PS_INPUT input) : SV_Target
{
	//指定位置にﾃｸｽﾁｬを貼る.
	return g_Texture.Sample(g_Sampler, input.UV);
}