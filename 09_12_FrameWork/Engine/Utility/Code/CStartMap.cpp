#include "CStartMap.h"

CStartMap::CStartMap()
{
}

CStartMap::CStartMap(LPDIRECT3DDEVICE9 pGraphicDev)
    :CVIBuffer(pGraphicDev)
{
}

CStartMap::CStartMap(const CStartMap& rhs)
    :CVIBuffer(rhs)
{
}

CStartMap::~CStartMap()
{
}

HRESULT CStartMap::Ready_Buffer()
{
    m_dwTriCnt = 2; 
    m_dwVtxCnt = 4; 
    m_dwVtxSize = sizeof(VTXTEX);
    m_dwFVF = FVF_TEX;

    m_dwIdxSize = sizeof(INDEX32);
    m_IdxFmt = D3DFMT_INDEX32;

    FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

    VTXTEX* pVertex = NULL; 

    m_pVB->Lock(0, 0, (void**)&pVertex, 0);

    pVertex[0].vPosition = { 0.f, 0.f, 1032.f };
    pVertex[0].vTexUV = { 0.f,0.f };

    pVertex[1].vPosition = { 1208.f,0.f,1032.f };
    pVertex[1].vTexUV = { 1.f, 0.f };

    pVertex[2].vPosition = { 1208.f, 0.f, 0.f };
    pVertex[2].vTexUV = { 1.f, 1.f };

    pVertex[3].vPosition = { 0.f, 0.f, 0.f };
    pVertex[3].vTexUV = { 0.f , 1.f };

    m_pVB->Unlock();

    INDEX32* pIndex = nullptr;

    m_pIB->Lock(0, 0, (void**)&pIndex, 0);

    // 오른쪽 위
    pIndex[0]._0 = 0;
    pIndex[0]._1 = 1;
    pIndex[0]._2 = 2;

    // 왼쪽 아래
    pIndex[1]._0 = 0;
    pIndex[1]._1 = 2;
    pIndex[1]._2 = 3;

    m_pIB->Unlock();

    return S_OK;

}

void CStartMap::Render_Buffer()
{
    CVIBuffer::Render_Buffer();
}

CStartMap* CStartMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CStartMap* pInstance = new  CStartMap(pGraphicDev);

    if (FAILED(pInstance->Ready_Buffer()))
    {
        Safe_Release(pInstance);
        MSG_BOX(" CStartMap Create Failed");
        return nullptr;
    }

    return pInstance;
}

CComponent* CStartMap::Clone()
{
    return new CStartMap(*this);
}

void CStartMap::Free()
{
    CVIBuffer::Free();
}
