//¸ŞÛ°ÊŞÙ.
cbuffer global
{
	matrix	g_mWVP;		//World,View,Proj‚Ì•ÏŠ·s—ñ.
	float4	g_vColor;	//F.
};

//ÊŞ°Ã¯¸½¼ª°ÀŞ.
float4 VS_Main( float4 Pos : POSITION ) : SV_Position
{
	Pos = mul(Pos, g_mWVP);
	return Pos;
}

//Ëß¸¾Ù¼ª°ÀŞ.
float4 PS_Main( float4 Pos : SV_Position ) : SV_Target
{
	return g_vColor;
}