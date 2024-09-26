#include "pch.h"
#include "DynamicCamera.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"

//IMPLEMENT_SINGLETON(CDynamicCamera)

//CDynamicCamera::CDynamicCamera()
//{
//}

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
    :CCamera(pGraphicDev)
    , m_Player(nullptr)
    , m_playerTransform(nullptr)
    , m_eCameraState(CAMERASTATE::PLAYER)
    , m_fZoomDeltaTime(0.f)
    , m_bZoomTrigger(false)
{


}

CDynamicCamera::~CDynamicCamera()
{
}

HRESULT CDynamicCamera::Ready_GameObject(const _vec3* pEye
    , const _vec3* pAt
    , const _vec3* pUp
    , const _float& _fFov
    , const _float& _fAspect
    , const _float& _fNear
    , const _float& _fFar)
{
    m_vEye = *pEye;
    m_vAt = *pAt;
    m_vUp = *pUp;

    m_fFov = _fFov;
    m_fAspect = _fAspect;
    m_fNear = _fNear;
    m_fFar = _fFar;

    FAILED_CHECK_RETURN(CCamera::Ready_GameObject(), E_FAIL);

    return S_OK;
}

void CDynamicCamera::LateReady_GameObject()
{
    m_Player = dynamic_cast<CPlayer*>(Engine::Get_GameObject(
        L"Layer_GameLogic", L"Player"));
    m_Player->SetCamera(this);

    m_playerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(
        ID_DYNAMIC, L"Layer_GameLogic", L"Player", L"Com_Transform"));
    NULL_CHECK_RETURN(m_playerTransform, );

    m_vIntervalPos.y = 100;
    m_vIntervalPos.z = -100;
    m_vOriginInterval = m_vIntervalPos;

    GetPlayerInfo();
    m_vEye = m_vPlayerPos;
    m_vEye.y += m_vIntervalPos.y;
    m_vEye.z += m_vIntervalPos.z;
    m_vAt = { 0.f, -1.f, 1.f };
    m_vAt += m_vEye;

    CCamera::LateReady_GameObject();
}

_int CDynamicCamera::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CCamera::Update_GameObject(fTimeDelta);
    D3DXMatrixInverse(&m_matCameraWorld, 0, &m_matView);

    Key_Input(fTimeDelta);
    if (m_bZoomTrigger)
        ZoomToTrigger(fTimeDelta);

    Add_RenderGroup(RENDER_UI, this);
    return iExit;
}

void CDynamicCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_eCameraState == CAMERASTATE::PLAYER)
    {
        GetPlayerInfo();
        MoveToPlayer(fTimeDelta);
    }

    else if (m_eCameraState == CAMERASTATE::DEBUG)
    {
        Mouse_Move(fTimeDelta);
    }


    CCamera::LateUpdate_GameObject(fTimeDelta);
}

void CDynamicCamera::Render_GameObject()
{
    _tchar buf[128];
    _vec2 position = { 1000,100 };

    switch (m_eCameraState)
    {
    case CAMERASTATE::DEBUG:
        lstrcpy(buf, L"카메라 상태 : DEBUG");
        break;
    case CAMERASTATE::PLAYER:
        lstrcpy(buf, L"카메라 상태 : PLAYER");
        break;
    case CAMERASTATE::EVENT:
        lstrcpy(buf, L"카메라 상태 : EVENT");
        break;
    default:
        break;
    }
    Engine::Render_Font(L"Font_Ogu24", buf, &position, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev
    , const _vec3* pEye
    , const _vec3* pAt
    , const _vec3* pUp
    , const _float& _fFov
    , const _float& _fAspect
    , const _float& _fNear
    , const _float& _fFar)
{

    CDynamicCamera* pCDynamicCamera = new CDynamicCamera(pGraphicDev);
    if (FAILED(pCDynamicCamera->Ready_GameObject(pEye, pAt, pUp, _fFov, _fAspect, _fNear, _fFar)))
    {

        Safe_Release(pCDynamicCamera);
        MSG_BOX("Dynamic Camera Create Failed");
        return nullptr;
    }

    return pCDynamicCamera;
}


void CDynamicCamera::Free()
{
    CCamera::Free();
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{
    if (Engine::GetKeyDown(DIK_F1))
    {
        m_eCameraState = CAMERASTATE::PLAYER;
        m_vEye = m_vPlayerPos;
        //  m_vAt = m_vToPlayerAt;
    }

    if (Engine::GetKeyDown(DIK_F2))
    {
        m_eCameraState = CAMERASTATE::DEBUG;
    }

    if (Engine::GetKeyDown(DIK_F3))
        m_eCameraState = CAMERASTATE::EVENT;

    if (m_eCameraState != CAMERASTATE::DEBUG)
        return;

    // 뷰행렬을 역행렬 하면 카메라 월드 행렬을 가져올 수 있다.
    _matrix		matCamWorld;
    D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

    // 카메라 이동
    _vec3	vRight;
    memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

    _vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * 5.f;
    if (Engine::Get_DIKeyState(DIK_D) & 0x80)
    {
        m_vEye += vLength;
        m_vAt += vLength;
    }
    if (Engine::Get_DIKeyState(DIK_A) & 0x80)
    {
        m_vEye -= vLength;
        m_vAt -= vLength;
    }

    _vec3	vFront{ 0.f, 0.f, 1.f };
    vLength = *D3DXVec3Normalize(&vFront, &vFront) * fTimeDelta * 5.f;

    if (Engine::Get_DIKeyState(DIK_W) & 0x80)
    {
        m_vEye += vLength;
        m_vAt += vLength;
    }
    if (Engine::Get_DIKeyState(DIK_S) & 0x80)
    {
        m_vEye -= vLength;
        m_vAt -= vLength;
    }


}

void CDynamicCamera::Mouse_Move(const _float& fTimeDelta)
{
    _matrix		matCamWorld;
    D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

    _long	dwMouseMove(0);

    // 카메라 줌 인/아웃
    if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Z))
    {
        _vec3	vLook;
        memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

        _vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook)
            * fTimeDelta * dwMouseMove / 5;

        m_vEye += vLength;
        m_vAt += vLength;
    }

    if (Engine::Get_DIMouseState(DIM_MB))
    {
        // 카메라 상하좌우 이동
        _vec3	vUp{ 0.f, 1.f, 0.f };
        _vec3 vLength = *D3DXVec3Normalize(&vUp, &vUp) * fTimeDelta;
        if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
        {
            m_vEye += vLength * dwMouseMove;
            m_vAt += vLength * dwMouseMove;
        }

        _vec3	vRight;
        memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));
        vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta;
        if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
        {
            m_vEye += vLength * -dwMouseMove;
            m_vAt += vLength * -dwMouseMove;
        }
    }

    if (Engine::Get_DIMouseState(DIM_RB))
    {
        // 카메라 회전
        if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
        {
            _vec3	vRight;
            memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

            _vec3	vLook = m_vAt - m_vEye;

            _matrix		matRot;
            D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
            D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

            m_vAt = m_vEye + vLook;
        }
        if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
        {
            _vec3	vUp{ 0.f, 1.f, 0.f };

            _vec3	vLook = m_vAt - m_vEye;

            _matrix		matRot;
            D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
            D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

            m_vAt = m_vEye + vLook;
        }
    }
}

void CDynamicCamera::MoveToPlayer(const _float& fTimeDelta)
{
    _vec3 vTargetPos = m_vPlayerPos;
    vTargetPos.y += m_vIntervalPos.y;
    vTargetPos.z += m_vIntervalPos.z;

    _vec3 vDir = vTargetPos - m_vEye;
    if (abs(vDir.x) < 0.1f) vDir.x = 0;
    if (abs(vDir.y) < 0.1f) vDir.y = 0;
    if (abs(vDir.z) < 0.1f) vDir.z = 0;
    _vec3 vMoveDir = *D3DXVec3Normalize(&vDir, &vDir) * fTimeDelta * 5.f;

    m_vEye += vMoveDir;
    m_vAt += vMoveDir;
}

void CDynamicCamera::ZoomTo(_float fRatio, _float fDuration)
{
    m_bZoomTrigger = true;
    m_fZoomDeltaTime = 0.f;
    m_fZoomRatio = fRatio;
    m_fZoomDuration = fDuration;

    // 최종으로 위치할 값 
    m_vTowardMove = m_vOriginInterval * (m_fZoomRatio / 100.f);
    // 초당 이동속도
    m_vTowardMove /= m_fZoomDuration;
}

void CDynamicCamera::SetCamera_Dash(const _float& fTimeDelta, _bool _opt)
{
    if (_opt && m_vIntervalPos.y >= 12)
    {
        return;
    }

    if (!_opt && m_vIntervalPos.y <= m_vOriginInterval.y)
    {
        return;
    }

    int dir = _opt * 2 - 1;
    float fSpeed = 3.f * dir;

    _vec3 vLook;
    memcpy(&vLook, &m_matCameraWorld.m[2][0], sizeof(_vec3));
    D3DXVec3Normalize(&vLook, &vLook);

    _vec3 vLength = vLook * fTimeDelta * fSpeed;
    m_vIntervalPos -= vLength;

}

void CDynamicCamera::ZoomToTrigger(const _float& fTimeDelta)
{
    // timedelta당 이동속도
    m_vIntervalPos += (m_vTowardMove * fTimeDelta);

    m_fZoomDeltaTime += fTimeDelta;
    if (m_fZoomDeltaTime >= m_fZoomDuration)
        m_bZoomTrigger = false;
}

void CDynamicCamera::ResetCamera(const _float& fTimeDelta)
{
}

void CDynamicCamera::GetPlayerInfo()
{
    m_playerTransform->Get_Info(INFO_POS, &m_vPlayerPos);
}