//Not much has changed in this type of file since the previous project
//except for dynamic resizing and a few extra allocations for features
//my teammates and I are working on.

void D3DApp::ResizeSwapChain(unsigned width, unsigned height)
{
	if (m_swapChain)
	{
		m_width = width;
		m_height = height;

		GraphicsEngine::Instance().SetWidthHeight(width, height);
		GraphicsEngine::Instance().m_resizeDirty = true;

		m_deviceContext->OMSetRenderTargets(0, 0, 0);

		RELEASE_RESOURCE(m_renderTarget);
		RELEASE_RESOURCE(m_depthView);
		RELEASE_RESOURCE(m_depthDisabledState);
		RELEASE_RESOURCE(m_depthState);
		RELEASE_RESOURCE(m_depthTex);

		HRESULT result;
		result = m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		ASSERT(result == S_OK);

		CreateRenderTarget();
		SetDepthState();
	}
}
