//fucntion used to interpret position of mouse in relation to editor window and grab the pixel location of the desired texture.
int GraphicsEngine::GrabObjIDFromTexture(int &EditorViewWidth, int &EditorViewHeight, ImVec2 &MousePos, ImVec2 &RegionMin, const char* bufferName, int &bufferIndex, int &componentOffset)
{
	//calculate the user click position
	float mouseX = MousePos.x - RegionMin.x;
	float mouseY = MousePos.y - RegionMin.y;

	// Return invalid GameObject if the mouse is out of bounds.
	if (mouseX < 0 || mouseY < 0 || mouseX >= EditorViewWidth || mouseY >= EditorViewHeight)
	{
		return GameObjectID(0);
	}

	HRESULT result;

	//Get the geometry buffer render target array
	RenderTargetArray SourceArray = m_renderTargetManager.GetRenderTargetArrayByName(bufferName);
	//Get the list of render targets from the source list
	std::vector<ID3D11Texture2D*> SourceList = SourceArray.GetRenderTargetTextures(); // texture at index 2 is material texture
	//grab the material texture from the list
	ID3D11Texture2D* SourceTexture = SourceList[bufferIndex];

	//copy out the description of the material texture to make a similar staging texture
	D3D11_TEXTURE2D_DESC SourceDescription;
	SourceTexture->GetDesc(&SourceDescription);

	//setup the staging texture variables
	D3D11_MAPPED_SUBRESOURCE MapInfo;
	ID3D11Texture2D* stagingTexture = nullptr;

	//TEXTURE COPY
	{
		//setup a description for the staging texture that is exactly the same
		D3D11_TEXTURE2D_DESC StagingDescription;
		StagingDescription.SampleDesc = SourceDescription.SampleDesc;
		StagingDescription.MipLevels = SourceDescription.MipLevels;
		StagingDescription.ArraySize = SourceDescription.ArraySize;
		StagingDescription.Height = SourceDescription.Height;
		StagingDescription.Format = SourceDescription.Format;
		StagingDescription.Width = SourceDescription.Width;
		StagingDescription.BindFlags = 0;
		StagingDescription.MiscFlags = 0;
		//Change the flags here so the texture is CPU readable
		StagingDescription.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		StagingDescription.Usage = D3D11_USAGE_STAGING;

		//Create the new empty staging texture
		result = D3DApp().Instance().GetDevice()->CreateTexture2D(&StagingDescription, nullptr, &stagingTexture);
		ASSERT(result == S_OK);

		//copy the material texture into our staging texture
		D3DApp().Instance().GetContext()->CopyResource(stagingTexture, SourceTexture);
		//linearly map the memory into our mapped resource for reading
		result = D3DApp().Instance().GetContext()->Map(stagingTexture, 0, D3D11_MAP_READ_WRITE, 0, &MapInfo);
		ASSERT(result == S_OK);
	}

	/* USE FOR DEBUGGING
	result = DirectX::SaveDDSTextureToFile(D3DApp().Instance().GetContext(), SourceTexture, L"Source.DDS");
	ASSERT(result == S_OK);
	result = DirectX::SaveDDSTextureToFile(D3DApp().Instance().GetContext(), stagingTexture, L"CopiedTexture.DDS");
	ASSERT(result == S_OK);
	*/

	int EditorPositionX = (static_cast<float>(m_width) / EditorViewWidth) * mouseX;
	int EditorPositionY = (static_cast<float>(m_height) / EditorViewHeight) * mouseY;

	//size of four floats for rgba
	int bufferSize = sizeof(float) * 4;

	//calculate the offset into the texture by moving down by EditorPositionY and to the right by EditorPositionX
	size_t textureOffset = (EditorPositionY * bufferSize) * SourceDescription.Width + (EditorPositionX * bufferSize);
	//calculate the offsets in the given value
	int bit1 = textureOffset + componentOffset*4 + 0;
	int bit2 = textureOffset + componentOffset*4 + 1;
	int bit3 = textureOffset + componentOffset*4 + 2;
	int bit4 = textureOffset + componentOffset*4 + 3;

	//create a pointer with a c style cast from a void pointer to avoid memory allocation
	BYTE* TextureData = (BYTE*)MapInfo.pData;
	BYTE floatBuffer[4] = { TextureData[bit1], TextureData[bit2], TextureData[bit3], TextureData[bit4] }; //set the bytes to reassemble the g value
	//must reinterpret as a float since that is how the shader interpreted the data
	float ObjID = *reinterpret_cast<float*>(floatBuffer); //stick the data together

	//unmap the memory we previously mapped
	D3DApp().Instance().GetContext()->Unmap(stagingTexture, 0);
	//release the resource we allocated for
	RELEASE_RESOURCE(stagingTexture);

	/* 
	NOTE
	only grabs a single pixel
	could grab mips from the file and average out the id
	*/
	//cast the retreived float to an int
	return static_cast<int>(ObjID);
}

void Editor::ShowGameView(float dt)
// Game View
{
	...
			ImGuiIO& io = ImGui::GetIO();

		// Camera movement.
		if (mouseInScreen && io.MouseDown[1])
		{
			movingCamera = true;
		}
		else if (mouseInScreen && ImGui::IsMouseClicked(0) && !ImGuizmo::IsUsing() && !ImGuizmo::IsOver())
		{
			GameObjectID ObjID = GraphicsEngine::Instance().GrabObjIDFromTexture(bound.x, bound.y, ImGui::GetMousePos(), minPos + minBound, "G-Buffer", 2, 1);
			if(m_currentSpace->GetComponent<ObjectInfo>(ObjID) != nullptr)
			{
				Select(ObjID, io.KeyCtrl);
			}
		}
	...
}
