#include "pch.h"
#include "ExamineButton.h"

CExamineButton::CExamineButton(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev), m_bCall(false)
{
}

CExamineButton::~CExamineButton()
{
}

HRESULT CExamineButton::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CExamineButton::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (m_bCall)
		Engine::Add_RenderGroup(RENDER_UI, this);

	return iExit;
}

void CExamineButton::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CExamineButton::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CExamineButton::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ExamineButton"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->m_vScale = { 100.5f, 40.f, 0.2f };
	m_pTransformCom->Set_Pos(500.f, 400.f, 1.f);

	return S_OK;
}


CExamineButton* CExamineButton::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CExamineButton* pButton = new CExamineButton(pGraphicDev);

	if (FAILED(pButton->Ready_GameObject()))
	{
		Safe_Release(pButton);
		MSG_BOX("ExamineButton Create Failed");
		return nullptr;
	}

	return pButton;
}

void CExamineButton::Free()
{
	Engine::CGameObject::Free();
}
