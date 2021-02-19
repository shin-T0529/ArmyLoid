#include "Charactor.h"

clsChara::clsChara()
	:m_pStaticMesh(NULL)
	, m_pStaticSkinMesh(NULL)
	, m_vPos(0.0f,0.0f,0.0f)
	, m_vRot(0.0f, 0.0f, 0.0f)
	, m_vCRot(0.0f, 0.0f, 0.0f)
	, m_Yaw(0.0f),m_Pitch(0.0f),m_Roll(0.0f)
	, m_Scale(1.0f)
	, m_ScaleSize(1.0f)
	, m_SSphere()
	//HP.
	, m_HP(100), m_DamageHP(m_HP)
	//制御用フラグ.
	, m_DeadFlag(false), m_Stopping(false)
	//アニメーション.
	//, ActState(0), ActState2(0)
{

}

clsChara::~clsChara()
{

}

void clsChara::Update()
{
	if (m_pStaticMesh == NULL)
	{ return; }
	m_pStaticMesh->m_Scale = m_Scale;
	m_pStaticMesh->m_Yaw = m_vRot.y;
	m_pStaticMesh->m_Pitch = m_vRot.x;
	m_pStaticMesh->m_Roll = m_vRot.z;
	m_pStaticMesh->m_vPos = m_vPos;
}
void clsChara::UpdateSkin()
{
	if (m_pStaticSkinMesh == NULL)
	{
		return;
	}
	m_pStaticSkinMesh->m_vScale = m_SkinScale;
	m_pStaticSkinMesh->m_fYaw = m_vRot.y;
	m_pStaticSkinMesh->m_fPitch = m_vRot.x;
	m_pStaticSkinMesh->m_fRoll = m_vRot.z;
	m_pStaticSkinMesh->m_vPos = m_vPos;
}

void clsChara::Render(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye)
{
	if (m_pStaticMesh == NULL)
	{
		return;
	}
	//更新.
	Update();
	//メッシュファイルのレンダリング関数を呼ぶ,
	m_pStaticMesh->Render(mView, mProj, vLight, vEye);
}
void clsChara::SkinRender(D3DXMATRIX &mView, D3DXMATRIX &mProj, D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye)
{
	if (m_pStaticSkinMesh == NULL)
	{
		return;
	}
	//更新.
	UpdateSkin();
	//メッシュファイルのレンダリング関数を呼ぶ,
	m_pStaticSkinMesh->Render(mView, mProj, vLight, vEye,0);
}

//モデルデータ受け渡し(関連付け).
void clsChara::AttachModel(clsDx9Mesh* pModel)
{
	if (pModel != NULL)
	{
		m_pStaticMesh = pModel;
	}
}
void clsChara::AttachSkinModel(clsD3DXSKINMESH* pSkinModel)
{
	if (pSkinModel != NULL)
	{
		m_pStaticSkinMesh = pSkinModel;
	}
}

//モデルデータ関連付け解除.
void clsChara::DetatchModel()
{
	if (m_pStaticMesh != NULL)
	{
		m_pStaticMesh = NULL;
	}
}
void clsChara::DetatchSkinModel()
{
	if (m_pStaticSkinMesh != NULL)
	{
		m_pStaticSkinMesh = NULL;
	}
}

//バウンディングスフィア(判定用球)作成.
HRESULT clsChara::InitSphere(clsDx9Mesh* pMesh)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;//頂点ﾊﾞｯﾌｧ.
	void*		pVertices = NULL;//頂点.
	D3DXVECTOR3	vCenter;	//中心.
	float		Radius;		//半径.

	if (pMesh == NULL){	pMesh = m_pStaticMesh; }

	//頂点ﾊﾞｯﾌｧを取得.
	if (FAILED(pMesh->m_pMesh->GetVertexBuffer(&pVB))){	return E_FAIL; }
	//ﾒｯｼｭの頂点ﾊﾞｯﾌｧをﾛｯｸする.
	if (FAILED(pVB->Lock(0, 0, &pVertices, 0)))
	{
		SAFE_RELEASE(pVB);
		return E_FAIL;
	}

	//ﾒｯｼｭの外接円の中心と半径を計算する.
	D3DXComputeBoundingSphere((D3DXVECTOR3*)pVertices,pMesh->m_pMesh->GetNumVertices(),
		D3DXGetFVFVertexSize(pMesh->m_pMesh->GetFVF()),	&vCenter,&Radius);

	//ｱﾝﾛｯｸ.
	pVB->Unlock();
	SAFE_RELEASE(pVB);

	//中心と半径を構造体に設定.
	pMesh->m_Sphere.vCenter = vCenter;
	pMesh->m_Sphere.Radius = Radius;

	//判定用に記録.
	m_SSphere.vCenter = pMesh->m_Sphere.vCenter;
	m_SSphere.Radius = pMesh->m_Sphere.Radius;

	return S_OK;
}

//バウンディングボックス(判定用箱)作成.
HRESULT	clsChara::InitBBox(clsDx9Mesh* pMesh)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	VOID* pVertices = NULL;
	D3DXVECTOR3 Max, Min;
	if (pMesh == NULL){	pMesh = m_pStaticMesh; }

	//ﾒｯｼｭの頂点バッファをロックする.
	if (FAILED(pMesh->m_pMesh->GetVertexBuffer(&pVB)))
	{
		MessageBox(NULL, "頂点バッファ取得失敗", "error", MB_OK);
		return E_FAIL;
	}
	if (FAILED(pVB->Lock(0, 0, &pVertices, 0)))
	{
		MessageBox(NULL, "頂点バッファロック失敗", "error", MB_OK);
		return E_FAIL;
	}
	
	//メッシュ内の頂点位置の最大と最小を検索する
	D3DXComputeBoundingBox(
		(D3DXVECTOR3*)pVertices,pMesh->m_pMesh->GetNumVertices(),D3DXGetFVFVertexSize(pMesh->m_pMesh->GetFVF()),
		&Min, &Max);


	pVB->Unlock();//アンロック.
	SAFE_RELEASE(pVB);

	m_BBox.vPosMax = Max;
	m_BBox.vPosMin = Min;
	m_BBox.vScale = m_Scale;

	//モデルのサイズで最大値・最小値を制限する.
	m_BBox.vPosMax.x = m_BBox.vPosMax.x * m_Scale;
	m_BBox.vPosMin.x = m_BBox.vPosMin.x * m_Scale;
	m_BBox.vPosMax.y = m_BBox.vPosMax.y * m_Scale;
	m_BBox.vPosMin.y = m_BBox.vPosMin.y * m_Scale;
	m_BBox.vPosMax.z = m_BBox.vPosMax.z * m_Scale;
	m_BBox.vPosMin.z = m_BBox.vPosMin.z * m_Scale;

	//軸ベクトル・軸の長さ(この場合、ボックスの各半径)を初期化する.
	m_BBox.LengthX = (Max.x - Min.x) / 2.0f;
	m_BBox.LengthY = (Max.y - Min.y) / 2.0f;
	m_BBox.LengthZ = (Max.z - Min.z) / 2.0f;

	return S_OK;
}

//●判定.
bool clsChara::SphereCollision(clsChara* pTarget, SSPHERE *pSSphere)
{
	D3DXVECTOR3 tempPos(0.0f,0.0f,0.0f);
	float		tempRadius = 0.0f;
	if (pSSphere != NULL)
	{
		tempPos = pSSphere->vPos + m_vPos;
		tempRadius = pSSphere->Radius;
	}
	else
	{
		tempPos = m_SSphere.vPos + m_vPos;
		tempRadius = m_SSphere.Radius;
	}
	//２つの物体の中心間の距離を求める.
	D3DXVECTOR3 vLength = pTarget->m_vPos - tempPos;

	//長さに変換する.
	float Length = D3DXVec3Length(&vLength);

	//２つの物体の距離が、２つの物体の半径をたしたものより小さいということは.
	//ｽﾌｨｱ同士が重なっている(衝突している)ということ.
	if (Length <= tempRadius + pTarget->m_SSphere.Radius)
	{
		return true;//衝突.
	}
	return false;//衝突していない.
}

//■判定.
bool clsChara::BBoxCollision(clsChara* pTarget, BBOX *pBBox)
{
	//攻撃用に2つの頂点を用意し初期化する.
	D3DXVECTOR3 vecAtkMax, vecAtkMin;	//最大、最小頂点.

	if (pBBox != NULL)
	{
		vecAtkMax = pBBox->vPosMax + m_vPos;
		vecAtkMin = pBBox->vPosMin + m_vPos;
	}
	else
	{
		vecAtkMax = m_BBox.vPosMax + m_vPos;
		vecAtkMin = m_BBox.vPosMin + m_vPos;
	}

	//目標用用に2つの頂点を用意し初期化する.
	D3DXVECTOR3 vecTrgMax, vecTrgMin;	//最大、最小頂点.
	vecTrgMax = pTarget->m_BBox.vPosMax + pTarget->m_vPos;
	vecTrgMin = pTarget->m_BBox.vPosMin + pTarget->m_vPos;

	//2つのメッシュそれぞれの最大、最小位置を使う.
	//4つの情報があれば、衝突を検出できる.
	if( vecAtkMin.x<vecTrgMax.x &&vecAtkMax.x>vecTrgMin.x
	  &&vecAtkMin.y<vecTrgMax.y &&vecAtkMax.y>vecTrgMin.y
	  &&vecAtkMin.z<vecTrgMax.z &&vecAtkMax.z>vecTrgMin.z)
	{
		return true;	//衝突している.
	}
	return false;		//衝突していない.
}

//線分判定.
bool clsChara::RayCollsion(clsChara* pAttacker, clsChara* pTarget)
{
	FLOAT	fDistance;	//距離.

	//現在位置のコピー.
	pAttacker->m_vRay = pAttacker->m_vPos;
	//レイの高さを自機の位置より上にする.
	pAttacker->m_vRay.y = pAttacker->m_vPos.y + 3.0f;
	//軸ベクトルは垂直で下向き.
	pAttacker->m_vAxis = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	bHit = Intersect(pAttacker, pTarget, &fDistance, &vIntersect);

	if (bHit == true)
	{
		return true;
	}
	return false;


	//交点の座標からy座標としてセット.
	pAttacker->m_vPos.y = vIntersect.y + 1.0f;
	return bHit;

 }

bool clsChara::Intersect(
	clsChara*	 pAttacker,		//基準の物体.
	clsChara*	 pTarget,		//対象の物体.
	float*		 pfDistance,	//(出力)距離.
	D3DXVECTOR3* pIntersect)	//(出力)交差座標.
{
	D3DXMATRIXA16	matRot;		//回転行列.

	//回転行列を計算.
	D3DXMatrixRotationY(&matRot, pAttacker->m_Yaw);

	//軸ベクトルの用意.
	D3DXVECTOR3	vecAxisZ;

	//Zベクトルそのものを現在の回転状態により変換する.
	D3DXVec3TransformCoord
	(&vecAxisZ, &pAttacker->m_vAxis, &matRot);

	D3DXVECTOR3	vecStart, vecEnd, vecDistance;
	vecStart = vecEnd = pAttacker->m_vRay;	//レイの位置を設定.

	vecEnd += vecAxisZ * 1.0f;

	//基準キャラの座標に回転座標を合成する.
	//逆光列を用いれば正しくレイが当たる.
	D3DXMATRIX matWorld, matScale, matTrans;
	D3DXMATRIX matYaw, matPitch, matRoll;
	D3DXMatrixScaling(&matScale,
		pTarget->m_Scale, pTarget->m_Scale, pTarget->m_Scale);

	D3DXMatrixRotationY(&matYaw, pTarget->m_Yaw);
 	D3DXMatrixRotationX(&matPitch, pTarget->m_Pitch);
	D3DXMatrixRotationZ(&matRoll, pTarget->m_Roll);

	D3DXMatrixTranslation(&matTrans,
		pTarget->m_vPos.x, pTarget->m_vPos.y, pTarget->m_vPos.z);


	//matWorld =
	//	matScale * matYaw * matPitch * matRoll * matTrans;

	matWorld= matScale * matYaw * matTrans;

	//逆行列を求める.
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformCoord(&vecStart, &vecStart, &matWorld);
	D3DXVec3TransformCoord(&vecEnd, &vecEnd, &matWorld);

	//距離を求める.
	vecDistance = vecEnd - vecStart;

	BOOL	bHit = false;//命中フラグ.

	DWORD	dwIndex = 0;	//インデックス番号.
	D3DXVECTOR3 vVertex[3];	//頂点座標.
	FLOAT U = 0, V = 0;		//(out)重心ヒット座標.

	//レイとメッシュの交差を調べる.
		D3DXIntersect(pTarget->m_pStaticMesh->m_pMeshForRay,	//対象のメッシュ.
			&vecStart,							//レイの開始位置.
			&vecDistance,						//レイの距離.
			&bHit,								//(out)判定結果
			&dwIndex,							//(out)bHitがtrue時、
			//レイの視点に最も近くの面のｲﾝﾃﾞｯｸｽ値へのポインタ.
			&U, &V,								//(out)重心ヒット座標.
			pfDistance,							//ターゲットとの距離.
			NULL, NULL);


	if (bHit == TRUE)
	{
		//命中時.
		FindVerticesOnPoly
		(pTarget->m_pStaticMesh->m_pMeshForRay, dwIndex, vVertex);
		//重心座標から交点を算出する.
		//ローカル交点はv0 + U*(v1-v0)+v*(v2-v0)で求められる/
		*pIntersect = vVertex[0]
			+ U * (vVertex[1] - vVertex[0])
			+ V * (vVertex[2] - vVertex[0]);

		D3DXMATRIX matHit;
		D3DXMatrixTranslation(&matHit,
			pIntersect->x, pIntersect->y, pIntersect->z);

		//matWorld =
		//	matHit * matScale * matYaw * matPitch * matRoll * matTrans;

		matWorld = matHit * matScale * matYaw * matTrans;

		pIntersect->x = matWorld._41;
		pIntersect->y = matWorld._42;
		pIntersect->z = matWorld._43;

		return true;
	}
	return false;
}


//交差位置のポリゴンの頂点を見つける.
//(頂点座標はローカル座標)
HRESULT	clsChara::FindVerticesOnPoly(
	LPD3DXMESH	pMeshForRay, DWORD	dwPolyIndex,
	D3DXVECTOR3* pVecVertices)
{
	//頂点毎のバイト数を取得.
	DWORD dwStride = pMeshForRay->GetNumBytesPerVertex();
	//頂点数を取得する.
	DWORD dwVertexAmt = pMeshForRay->GetNumVertices();
	//面数取得.
	DWORD dwPolyAmt = pMeshForRay->GetNumFaces();

	WORD* pwPoly = NULL;

	//インデックスバッファをロック(読込モード)
	pMeshForRay->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&pwPoly);
	BYTE* pbVertices = NULL;
	FLOAT* pfVertices = NULL;
	LPDIRECT3DVERTEXBUFFER9 VB = NULL;
	pMeshForRay->GetVertexBuffer(&VB);

	if (SUCCEEDED(VB->Lock(0, 0, (VOID**)&pbVertices, 0)))
	{
		//ポリゴンの頂点のひとつ目を取得.
		pfVertices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3]];
		pVecVertices[0].x = pfVertices[0];
		pVecVertices[0].y = pfVertices[1];
		pVecVertices[0].z = pfVertices[2];

		//ポリゴンの頂点のふたつ目を取得.
		pfVertices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 1]];
		pVecVertices[1].x = pfVertices[0];
		pVecVertices[1].y = pfVertices[1];
		pVecVertices[1].z = pfVertices[2];

		//ポリゴンの頂点のみっつ目を取得.
		pfVertices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 2]];
		pVecVertices[2].x = pfVertices[0];
		pVecVertices[2].y = pfVertices[1];
		pVecVertices[2].z = pfVertices[2];

		//ロック解除.
		pMeshForRay->UnlockIndexBuffer();
		VB->Unlock();

	}

	VB->Release();

	return S_OK;

}



//壁判定.
void clsChara::WallJudge(clsChara* pTackle, clsChara* pWall, float WSPACE)
{
	FLOAT		fDistance[4];	//距離.
	D3DXVECTOR3	vIntersect[4];	//交点座標.
	float		fDis, fYaw;		//距離と回転.

	pTackle->m_vRay = pTackle->m_vPos;
	pTackle->m_vRay.y = 0.0f;	//ﾚｲのy位置を調整(壁を出来るだけ低くして壁感無くすために低め).

	//ﾚｲの向きにより当たる壁までの距離を求める.
	pTackle->m_vAxis = D3DXVECTOR3(0.0f, 0.0f, 3.0f);	//軸ﾍﾞｸﾄﾙ前.
	Intersect(pTackle, pWall, &fDistance[0], &vIntersect[0]);
	pTackle->m_vAxis = D3DXVECTOR3(0.0f, 0.0f, -3.0f);	//軸ﾍﾞｸﾄﾙ後.
	Intersect(pTackle, pWall, &fDistance[1], &vIntersect[1]);
	pTackle->m_vAxis = D3DXVECTOR3(3.0f, 0.0f, 0.0f);	//軸ﾍﾞｸﾄﾙ右.
	Intersect(pTackle, pWall, &fDistance[2], &vIntersect[2]);
	pTackle->m_vAxis = D3DXVECTOR3(-3.0f, 0.0f, 0.0f);	//軸ﾍﾞｸﾄﾙ左.
	Intersect(pTackle, pWall, &fDistance[3], &vIntersect[3]);

#ifdef _DEBUG
	for (int i = 0; i < 4; i++)
	{
		m_tagHitPos.vPos[i] = vIntersect[i];
	}
#endif // _DEBUG

	fYaw = fabsf(pTackle->m_Yaw);	//fabs:絶対値(flaot版).
	dirCheck(&fYaw);	//0～2πの間に収める.

	//前が壁に接近.
	fDis = fDistance[0];
	if (0.01f < fDis && fDis < WSPACE)
	{
		//時計回り.
		if (pTackle->m_Yaw < 0.0f)
		{
			if (0.785f <= fYaw && fYaw < 2.355f) 
			{		//右から.
				pTackle->m_vPos.x += WSPACE - fDis;
			}
			else if (2.355f <= fYaw && fYaw < 3.925f) 
			{	//前から.
				pTackle->m_vPos.z += WSPACE - fDis;
			}
			else if (3.925f <= fYaw && fYaw < 5.495f)
			{	//左から.
				pTackle->m_vPos.x -= WSPACE - fDis;
			}
			else
			{	//奥から.
				pTackle->m_vPos.z -= WSPACE - fDis;
			}
		}
		//反時計回り.
		else {
			if (0.785f <= fYaw && fYaw < 2.355f)
			{		//右から.
				pTackle->m_vPos.x -= WSPACE - fDis;
			}
			else if (2.355f <= fYaw && fYaw < 3.925f)
			{	//前から.
				pTackle->m_vPos.z += WSPACE - fDis;
			}
			else if (3.925f <= fYaw && fYaw < 5.495f) 
			{	//左から.
				pTackle->m_vPos.x += WSPACE - fDis;
			}
			else
			{	//奥から.
				pTackle->m_vPos.z -= WSPACE - fDis;
			}
		}
	}

	//後ろが壁に接近.
	fDis = fDistance[1];
	if (0.01f < fDis && fDis < WSPACE) 
	{
		//時計回り.
		if (pTackle->m_Yaw < 0.0f) 
		{
			if (0.785f <= fYaw && fYaw < 2.355f)
			{		//右から.
				pTackle->m_vPos.x -= WSPACE - fDis;
			}
			else if (2.355f <= fYaw && fYaw < 3.925f)
			{	//前から.
				pTackle->m_vPos.z -= WSPACE - fDis;
			}
			else if (3.925f <= fYaw && fYaw < 5.495f) 
			{	//左から.
				pTackle->m_vPos.x += WSPACE - fDis;
			}
			else 
			{										//奥から.
				pTackle->m_vPos.z += WSPACE - fDis;
			}
		}
		//反時計回り.
		else 
		{
			if (0.785f <= fYaw && fYaw < 2.355f)
			{		//右から.
				pTackle->m_vPos.x += WSPACE - fDis;
			}
			else if (2.355f <= fYaw && fYaw < 3.925f)
			{	//前から.
				pTackle->m_vPos.z -= WSPACE - fDis;
			}
			else if (3.925f <= fYaw && fYaw < 5.495f)
			{	//左から.
				pTackle->m_vPos.x -= WSPACE - fDis;
			}
			else
			{	//奥から.
				pTackle->m_vPos.z += WSPACE - fDis;
			}
		}
	}

	//右が壁に接近.
	fDis = fDistance[2];
	if (0.01f < fDis && fDis < WSPACE) 
	{
		//時計回り.
		if (pTackle->m_Yaw < 0.0f) {
			if (0.785f <= fYaw && fYaw < 2.355f) {		//右から.
				pTackle->m_vPos.z -= WSPACE - fDis;
			}
			else if (2.355f <= fYaw && fYaw < 3.925f) {	//前から.
				pTackle->m_vPos.x += WSPACE - fDis;
			}
			else if (3.925f <= fYaw && fYaw < 5.495f) {	//左から.
				pTackle->m_vPos.z += WSPACE - fDis;
			}
			else {										//奥から.
				pTackle->m_vPos.x -= WSPACE - fDis;
			}
		}
		//反時計回り.
		else {
			if (0.785f <= fYaw && fYaw < 2.355f) {		//右から.
				pTackle->m_vPos.z += WSPACE - fDis;
			}
			else if (2.355f <= fYaw && fYaw < 3.925f) {	//前から.
				pTackle->m_vPos.x += WSPACE - fDis;
			}
			else if (3.925f <= fYaw && fYaw < 5.495f) {	//左から.
				pTackle->m_vPos.z -= WSPACE - fDis;
			}
			else {										//奥から.
				pTackle->m_vPos.x -= WSPACE - fDis;
			}
		}
	}

	//左が壁に接近.
	fDis = fDistance[3];
	if (0.01f < fDis && fDis < WSPACE)
	{
		//時計回り.
		if (pTackle->m_Yaw < 0.0f) {
			if (0.785f <= fYaw && fYaw < 2.355f) {		//右から.
				pTackle->m_vPos.z += WSPACE - fDis;
			}
			else if (2.355f <= fYaw && fYaw < 3.925f) {	//前から.
				pTackle->m_vPos.x -= WSPACE - fDis;
			}
			else if (3.925f <= fYaw && fYaw < 5.495f) {	//左から.
				pTackle->m_vPos.z -= WSPACE - fDis;
			}
			else {										//奥から.
				pTackle->m_vPos.x += WSPACE - fDis;
			}
		}
		//反時計回り.
		else {
			if (0.785f <= fYaw && fYaw < 2.355f) {		//右から.
				pTackle->m_vPos.z -= WSPACE - fDis;
			}
			else if (2.355f <= fYaw && fYaw < 3.925f) {	//前から.
				pTackle->m_vPos.x -= WSPACE - fDis;
			}
			else if (3.925f <= fYaw && fYaw < 5.495f) {	//左から.
				pTackle->m_vPos.z += WSPACE - fDis;
			}
			else {										//奥から.
				pTackle->m_vPos.x += WSPACE - fDis;
			}
		}
	}

}

//回転値調整.
//(Yawが1周以上している時の調整).
void clsChara::dirCheck(float *fYaw)
{
	if (*fYaw > static_cast<float>(D3DX_PI * 2.0f)) {
		//1周以上している.
		*fYaw -= static_cast<float>(D3DX_PI * 2.0f);//2π(360°)分引く.
	}

	//再帰関数.
	if (*fYaw > static_cast<float>(D3DX_PI * 2.0f)) {
		dirCheck(fYaw);
	}
}

