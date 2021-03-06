#pragma once
#include "TBasisStd.h"
#include "TSpriteMgr.h"
#define MAX_PARTICLE 512
class TParticle 
{
public:
	D3DXVECTOR3		m_vPos;				// 파티클 위치
	D3DXVECTOR3		m_vAdd;				// 파티클 위치에 프레임 마다 더해지는 값
	D3DXVECTOR4		m_vColor;			// 파티클 색상
	float			m_fTimer;			// 실행시부터 누적되는 시간
	float			m_fLifeTime;		// 생명주기
	int				m_iApplyIndex;		// 텍스처 인덱스
	INT				m_iSpriteIndex;		// 스프라이트 타입 인덱스

	//bool m_bRender;
public:


	TParticle();
	virtual ~TParticle();
};

