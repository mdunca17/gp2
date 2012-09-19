#include "GameApplication.h"

CGameApplication::CGameApplication(void)
{
   m_pWindow=NULL;
   m_pD3D10Device=NULL;
   m_pRenderTargetView=NULL;
   m_pSwapChain=NULL;
}

CGameApplication::~CGameApplication(void)
{
	 if(m_pD3D10Device)
		 m_pD3D10Device->ClearState();

	 if(m_pRenderTargetView)
		 m_pRenderTargetView->Release();
	 if(m_pSwapChain)
		 m_pSwapChain->Release();
	 if(m_pD3D10Device)
		 m_pD3D10Device->Release();

	 if(m_pWindow)
	 {
		 delete m_pWindow;
		 m_pWindow=NULL;
	 }
}

bool CGameApplication::init()
{
	if(!initWindow())
		  return false;

	if(!initGraphics())
		  return false;

	return true;
}

void CGameApplication::run()
{
	while(m_pWindow->running())
	{
		if (!m_pWindow->checkForWindowMessages())
		{
			update();
			render();
		}
	}
}

void CGameApplication::render()
{

}

void CGameApplication::update()
{

}

bool CGameApplication::initGraphics()
{
	RECT windowRect;
	GetClientRect(m_pWindow->getHandleToWindow(),&windowRect);

	UINT width = windowRect.right-windowRect.left;
	UINT height = windowRect.bottom-windowRect.top;

	UINT createDeviceFlags=0;
#ifdef _DEBUG
	createDeviceFlags|=D3D10_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd,sizeof(sd));

	if(m_pWindow->isFullScreen())
		sd.BufferCount = 2;
	else 
		sd.BufferCount = 1;

	sd.OutputWindow = m_pWindow->getHandleToWindow();
	sd.Windowed = (BOOL)(!m_pWindow->isFullScreen());
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;

	if(FAILED(D3D10CreateDeviceAndSwapChain(NULL,
		D3D10_DRIVER_TYPE_HARDWARE,NULL,createDeviceFlags,
		&m_pD3D10Device)))
		return false;

	ID3D10Texture2D*pBackBuffer;
	if (FAILED (m_pSWAPChain->GetBuffer(0,_uuidof (ID3D10Texture2D),
		(void**)&pBackBuffer)))
		return false;
	return true;
}

bool CGameApplication::initWindow()
{
	m_pWindow=new CWin32Window();
	if(!m_pWindow->init(TEXT("Matthew"), 800, 640,false))
		return false;

	return true;
}
