#include "pch.h"
#include "ExpressMonkey.h"

CExpressMonkey::CExpressMonkey(LPDIRECT3DDEVICE9 pGraphicDev)
    :CNPC(pGraphicDev)
{
}

CExpressMonkey::~CExpressMonkey()
{
}

HRESULT CExpressMonkey::Ready_GameObject()
{
    CNPC::Ready_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_tInfo.pName = L"숭숭 익스프레스";
    m_tInfo.pContent = L"익스프레스 이용권이 없으면 태워줄 수 없어요. 이용권을 가지고 다시 말 걸어주세요.";

    m_tQuestInfo.pQuestTitle = L"숭숭 익스프레스 이용권 찾기";
    m_tQuestInfo.pQuestContent = L"숭숭 익스프레스 이용권이 없으면 숭숭 익스프레스를 이용할 수 없다고?! 이용권을 찾아서 다시 오자!";

    _vec3 vMarkPos = m_pTransformCom->m_vInfo[INFO_POS];
    vMarkPos.y += 30.f;
    m_pMarkTransformCom->m_vInfo[INFO_POS] = vMarkPos;


    return S_OK;
}

void CExpressMonkey::LateReady_GameObject()
{
    CNPC::LateReady_GameObject();
}

_int CExpressMonkey::Update_GameObject(const _float& fTimeDelta)
{
    CNPC::Update_GameObject(fTimeDelta);

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    if (!m_bQuestClear && m_bQuestAccept)
    {
        //if (m_pInven->Find_Item(CItem::QUEST, CItem::EXTICKET));
        if (m_pInven->Find_Item(CItem::CONSUM, CItem::SMALL_FRUIT))
        {
            m_bQuestSucess = true;
        }
    }
    Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CExpressMonkey::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CExpressMonkey::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();
    m_pColliderCom->Render_Buffer();

    if (!m_bQuestClear)
    {
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pMarkTransformCom->Get_WorldMatrix());

        if (!m_bQuestAccept)
        {
            m_pMarkTextureCom[EXCLAMATION]->Set_Texture();
            m_pBufferCom->Render_Buffer();
            return;
        }
        if (m_bQuestSucess)
        {
            m_pMarkTextureCom[YELLOWQUEST]->Set_Texture();
            m_pBufferCom->Render_Buffer();

            return;
        }
        m_pMarkTextureCom[GRAYQUEST]->Set_Texture();
        m_pBufferCom->Render_Buffer();

    }

}

void CExpressMonkey::OnCollision(CGameObject* _pOther)
{
    // [S]버튼 출력.
    // [S]버튼 클릭 시 텍스트박스 출력 + 텍스트 출력
    if (Engine::GetKeyDown(DIK_S))
    {
        m_bConversation = m_bConversation ? false : true;
        // 대화중일때 S를 누르면 대화종료
        // 대화중이 아닐때 S를 누르면 대화시작
        if (!m_bConversation)
        {
            if (!m_bQuestAccept)
            {
                m_bConversation = m_bConversation ? false : true;
                // 여기에 new퀘스트 UI 띄우기
                m_bQuestAccept = true; // 조건 문 뒤에 true로 바꿔줌.
                m_pQuestUI->Add_Quest(m_tQuestInfo);

                m_pQuestAcceptUI->CallQuestAcceptUI(true);
                m_pQuestAcceptUI->Set_Text(m_tQuestInfo);
                //m_pInterButton->CallButton(true); // 대화중이 아닐 때 버튼 활성화

                return;
            }
        }

        if (m_bConversation)
        {
            m_pInterButton->CallButton(false); // 대화 중일 경우 버튼 출력이 필요 없음!!!!

            m_pTextBox->Set_Text(m_tInfo); //대화창 텍스트 세팅
            m_pTextBox->CallTextBox(true); //대화창 호출

            // 최초에는 기본 퀘스트 말풍을 보여줘야해서 아이템을 가지고 있더라도 false 상태로 출력하기 위해
            //if (!m_bQuestClear && m_bQuestAccept)
            if (m_bQuestSucess)
            {
                m_bQuestClear = true;
                m_tInfo.pContent = L"오..이용권을 가지고 계시는군요............... 숭숭!!! 어디로 이동할까요??";
                m_pTextBox->Set_Text(m_tInfo); //대화창 텍스트 세팅
                m_pQuestUI->Get_QuestArray()->pop_back();
            }
        }

    }

    if (!m_bConversation)
    {
        m_pInterButton->CallButton(true); // 대화중이 아닐 때 버튼 활성화
        // 대화하기[S]
    }

}

void CExpressMonkey::OnCollisionEnter(CGameObject* _pOther)
{
    // 풍선커지기 + 반가워하는 포즈 애니메이션
}

void CExpressMonkey::OnCollisionExit(CGameObject* _pOther)
{
    m_pInterButton->CallButton(false);
}



HRESULT CExpressMonkey::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ExpressMonkey"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 28.f, 21.f, 30.f };
    m_pTransformCom->Set_Pos(200.f, 20.f, 800.f);

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pColliderCom->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });


    return S_OK;
}

CExpressMonkey* CExpressMonkey::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CExpressMonkey* pNPC = new CExpressMonkey(pGraphicDev);

    if (FAILED(pNPC->Ready_GameObject()))
    {
        Safe_Release(pNPC);
        MSG_BOX("ExpressMonkey Create Failed");
        return nullptr;
    }

    return pNPC;
}

void CExpressMonkey::Free()
{
    Engine::CGameObject::Free();
}
