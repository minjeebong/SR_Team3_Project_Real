#pragma once
#include "GameObject.h"

BEGIN(Engine)


class CTexture;
class CMapTex;
class CTransform;


class CElectriceelBossMap : public Engine::CGameObject
{

private:
	explicit CElectriceelBossMap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CElectriceelBossMap();


public:
	virtual HRESULT Ready_GameObject();
	virtual _int    Update_GameObject(const float& fTimeDelta);
	virtual void    LateUpdate_GameObject(const float& fTimeDelta);
	virtual void    Render_GameObject();


private:
	HRESULT   Add_Component();
	HRESULT   SetUp_Material();

private:
	Engine::CMapTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;


public:
	static CElectriceelBossMap* Create(LPDIRECT3DDEVICE9 pGraphicDev);	

private:
	virtual void Free();
};
END
