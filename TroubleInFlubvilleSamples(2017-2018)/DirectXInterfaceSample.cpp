//Most of the setup for DirectX 11 in this particular project was boiler plate
//To display the functionality of the LoadShaders function I have reduced it
//to just two shaders.

//set the render target to the window
void DXDevice::CreateRenderTarget()
{
	HRESULT result;
	ID3D11Texture2D *backBufferPtr;

	// grab the buffer
	result = m_swapchain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&backBufferPtr
	);

	ASSERT(result == S_OK);

	// create render target
	result = m_device->CreateRenderTargetView(
		backBufferPtr,
		nullptr,
		&m_renderTarget
	);

	ASSERT(result == S_OK);

	// set render target
	m_deviceContext->OMSetRenderTargets(1, &m_renderTarget, m_depthView);
}

void DXDevice::LoadShaders()
{
	HRESULT result = S_OK;

	ID3D11PixelShader *tempPS;
	ID3D11VertexShader *tempVS;
  //compile from file
#ifdef _DEBUG
	ID3D10Blob* pixelErrorMessage, *vertexErrorMessage;
	ID3D10Blob* pixelShaderBuffer;
	ID3D10Blob* tempVSBlob;
	// compiling our vertex shader
	result |= D3DCompileFromFile(L"VertexShader.hlsl", NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&tempVSBlob, &vertexErrorMessage);
	ASSERT(result == S_OK);
	if (result != S_OK)
	{
		char *err = reinterpret_cast<char*>(vertexErrorMessage->GetBufferPointer());

    TraceMsgConsole() << err << "\n";
	}

	// compiling our pixel shader
	result |= D3DCompileFromFile(L"PixelShader.hlsl", NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &pixelErrorMessage);
	ASSERT(result == S_OK);
	if (result != S_OK)
	{
		char *err = reinterpret_cast<char*>(vertexErrorMessage->GetBufferPointer());
    TraceMsgConsole() << err << "\n";
	}

	if (pixelShaderBuffer != nullptr && tempVSBlob != nullptr)
	{
		// create the shaders
		result |= m_device->CreateVertexShader(tempVSBlob->GetBufferPointer(), tempVSBlob->GetBufferSize(), NULL, &tempVS);
		ASSERT(result == S_OK);
		result |= m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &tempPS);
		ASSERT(result == S_OK);

		if (result == S_OK)
		{
			// release if already existing
			RELEASE_RESOURCE(m_pixelShader);
			RELEASE_RESOURCE(m_vertexShader);

			// copy them over
			m_vertexShader = tempVS;
			m_pixelShader = tempPS;

			m_vertexShaderBlob = tempVSBlob;

			// set shaders
			m_deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
			m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);

			// recreate our input layout
			CreateInputLayout();
		}
	}
#else //compile from binary
  std::ifstream Vertex("VertexShader.cso", std::ios::in | std::ios::binary | std::ios::ate);
  if (!Vertex.good())
  {
    result = E_FAIL;
    ASSERT(result == S_OK);
  }
  size_t length = 0;
  void* buffer = nullptr;

  Vertex.seekg(0, std::ios::end);
  length = size_t(Vertex.tellg());
  buffer = new char[length];
  Vertex.seekg(0, std::ios::beg);
  Vertex.read(reinterpret_cast<char*>(buffer), length);
  Vertex.close();

  m_buffer = buffer;
  m_length = length;

  // create the shaders
  result |= m_device->CreateVertexShader(buffer, length, NULL, &tempVS);
  ASSERT(result == S_OK);
  Vertex.close();

  std::ifstream Pixel("PixelShader.cso", std::ios::in | std::ios::binary | std::ios::ate);
  if (!Pixel.good())
  {
    result = E_FAIL;
    ASSERT(result == S_OK);
  }

  length = 0;
  void* buffer2 = nullptr;

  Pixel.seekg(0, std::ios::end);
  length = size_t(Pixel.tellg());
  buffer2 = new char[length];
  Pixel.seekg(0, std::ios::beg);
  Pixel.read(reinterpret_cast<char*>(buffer2), length);
  Pixel.close();

  result |= m_device->CreatePixelShader(buffer2, length, NULL, &tempPS);
  ASSERT(result == S_OK);
  Pixel.close();

  if (result == S_OK)
  {
    // release if already existing
    RELEASE_RESOURCE(m_pixelShader);
    RELEASE_RESOURCE(m_vertexShader);

    // copy them over
    m_vertexShader = tempVS;
    m_pixelShader = tempPS;

    
    //is this gonna break stuffs
    //m_vertexShaderBlob = tempVSBlob;

    // set shaders
    m_deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
    m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);

    // recreate our input layout
    CreateInputLayout();
  }
#endif
