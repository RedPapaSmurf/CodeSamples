//These are the important features from my constant buffer manager
//which allowed me to make sending data to shaders simple.

template <ConstantBuffer buffer, typename T>
void BufferManager::CreateBuffer()
{
    HRESULT result;
    D3D11_BUFFER_DESC bufferDesc;

    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;

    bufferDesc.ByteWidth = sizeof(T);

    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    
    //other stuff
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    result = m_device->CreateBuffer(&bufferDesc, nullptr, &m_bufferData[ buffer ]);

    ASSERT(result == S_OK);

    m_rawBufferData[buffer] = new float[ sizeof(T) / sizeof(float)];
    m_rawBufferSize[buffer] = sizeof(T) / sizeof(float);
}

template <ConstantBuffer buffer, typename T>
void BufferManager::UpdateBuffer(T *data, bool isMainData)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;

    T *dataPtr;

    //int x = buffer;

    //lock the buffer
    result = m_deviceContext->Map(m_bufferData[ buffer ], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    //ASSERT(result == S_OK);

    //grab data
    dataPtr = reinterpret_cast<T*>(mappedResource.pData);

    //set data
    memcpy(dataPtr, data, sizeof(T));

    //unlock buffer
    m_deviceContext->Unmap(m_bufferData[ buffer ], 0);

    if(isMainData)
    {
        memcpy(m_rawBufferData[ buffer ], data, sizeof(T));
    }
}

void BufferManager::SetBuffer(ShaderType shaderType, ConstantBuffer buffer, unsigned location)
{
    switch (shaderType)
    {
    case SHADER_PIXEL:
        m_deviceContext->PSSetConstantBuffers(location, 1, &m_bufferData[ buffer ]);
        break;
    default: // vertex shader
        m_deviceContext->VSSetConstantBuffers(location, 1, &m_bufferData[ buffer ]);
        break;
    }
}
