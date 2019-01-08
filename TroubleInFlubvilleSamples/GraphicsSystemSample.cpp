//This is an example of my rendering system for this project.
//the top of the function I also sample the pause code I wrote for a
//technical requirement where the game had to pause in the background.

void GraphicsSystem::Update(float dt)
{
	...
	
  while (PeekMessage(&msg, windowHandle, 0, 0, PM_REMOVE))
  {
    WindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
  }

  if (windowHandle != GetFocus())
  {
	  AudioEngine::Mute();
	  std::string name = Factory::GetCurrentLevelName();

	  if (name.find("Lvl") == std::string::npos
		  && name.find("Transition") == std::string::npos
		  && name.find("Pause") == std::string::npos)
		  return;

	  if (!Factory::IsPaused())
		  Factory::Pause();
  }
  else
  {
	  if (AudioEngine::CheckIfMuted())
	  {
		  AudioEngine::UnMute();
	  }
	  
	...
	std::set<RenderInfo*>::iterator infoIter = renderingInfo.begin();

	while (infoIter != renderingInfo.end())
	{
	ColorBuffer CBuffer;
	//reset the color buffer
	CBuffer.r = 0.0f;
	CBuffer.b = 0.0f;
	CBuffer.g = 0.0f;
	CBuffer.a = 0.0f;

	//send the new color buffer
	Device.CBManager.UpdateBuffer<BUFFER_COLOR>(&CBuffer);
	Device.CBManager.SetBuffer(SHADER_PIXEL, BUFFER_COLOR, 2);

	Device.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	RenderObjInfo(*infoIter);
	  
  }
  
  
