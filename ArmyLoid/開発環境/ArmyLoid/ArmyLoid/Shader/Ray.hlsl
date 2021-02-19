//��۰���.
cbuffer global
{
	matrix	g_mWVP;		//World,View,Proj�̕ϊ��s��.
	float4	g_vColor;	//�F.
};

//�ްï�������.
float4 VS_Main( float4 Pos : POSITION ) : SV_Position
{
	Pos = mul(Pos, g_mWVP);
	return Pos;
}

//�߸�ټ����.
float4 PS_Main( float4 Pos : SV_Position ) : SV_Target
{
	return g_vColor;
}