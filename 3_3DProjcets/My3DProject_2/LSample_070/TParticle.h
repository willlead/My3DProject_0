#pragma once
#include "TBasisStd.h"
#include "TSpriteMgr.h"
#define MAX_PARTICLE 512
class TParticle 
{
public:
	D3DXVECTOR3		m_vPos;				// ��ƼŬ ��ġ
	D3DXVECTOR3		m_vAdd;				// ��ƼŬ ��ġ�� ������ ���� �������� ��
	D3DXVECTOR4		m_vColor;			// ��ƼŬ ����
	float			m_fTimer;			// ����ú��� �����Ǵ� �ð�
	float			m_fLifeTime;		// �����ֱ�
	int				m_iApplyIndex;		// �ؽ�ó �ε���
	INT				m_iSpriteIndex;		// ��������Ʈ Ÿ�� �ε���

	//bool m_bRender;
public:


	TParticle();
	virtual ~TParticle();
};

