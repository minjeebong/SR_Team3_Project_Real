#include "InputDev.h"

IMPLEMENT_SINGLETON(CInputDev)

CInputDev::CInputDev(void)
{
	ZeroMemory(m_byPrevKeyState, sizeof(m_byPrevKeyState));
	ZeroMemory(m_byCurrKeyState, sizeof(m_byCurrKeyState));
}

CInputDev::~CInputDev(void)
{
	Free();
}

_bool CInputDev::GetAnyActionKeyDown()
{
	_bool returnValue = false;
	int keyArray[] = { PLY_LEFTKEY, PLY_RIGHTKEY, PLY_UPKEY, PLY_DOWNKEY,
					   PLY_DASHKEY, PLY_SWINGKEY, PLY_LIFTKEY, PLY_ROLLKEY,
					   PLY_DANCEKEY, PLY_SMASHKEY };

	for (int i = 0; i < CONTROLKEY::PLY_ENDKEY; i++)
	{
		returnValue |= GetKeyDown(keyArray[i]);
	}

	return returnValue;
}

HRESULT CInputDev::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	//DInput 컴객체를 생성하는 함수 
	FAILED_CHECK_RETURN(DirectInput8Create(hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		NULL), E_FAIL);


	//키보드 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);



	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠 있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();




	// 마우스 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수 
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠 있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();


	return S_OK;
}

void CInputDev::Update_InputDev(void)
{
	memcpy(m_byPrevKeyState, m_byCurrKeyState, sizeof(m_byCurrKeyState));

	m_pKeyBoard->GetDeviceState(256, m_byCurrKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
}

void CInputDev::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}
