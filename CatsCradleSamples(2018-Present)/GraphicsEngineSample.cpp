//This system didn't do anything out of the ordinary. However, this is 
//my first time dealing with the winproc more in depth and interfacing 
//with the operating cleanly. I've learned quite a bit about how Windows
//talks to applications through this.

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GraphicsEngine::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, umessage, wparam, lparam))
	{
		return 1;
	}

  switch (umessage)
  {
  case WM_DESTROY:
  {
    PostQuitMessage(0);
    break;
  }
  case WM_CLOSE:
  {
	ENGINE->StopRunning();
    break;
  }
  case WM_SIZE:
  {
	RECT Window;
	GetWindowRect(D3DApp::Instance().GetWindowHandle(), &Window);
	D3DApp::Instance().ResizeSwapChain(Window.right - Window.left, Window.bottom - Window.top);
	GraphicsEngine::Instance().GetRenderTargetManager().ResizeAllRenderTargets(D3DApp::Instance().GetDevice(), D3DApp::Instance().GetContext(), Window.right - Window.left, Window.bottom - Window.top);
	
    break;
  }
  case WM_DEVICECHANGE:
	  {
	  InputCore::Instance().CheckControllerState();
	  }
  case WM_KEYDOWN:
	  {
			//set key state as triggered
	  InputCore::Instance().SetKeyPressed(wparam);
	  break;
	  }
  case WM_KEYUP:
	  {
  		//set key state as up
	  InputCore::Instance().SetKeyReleased(wparam);
	  break;
	  }
  case WM_SYSKEYDOWN:
  {
	  break;
  }
  default:
    //return the default wndproc response
    return DefWindowProc(hwnd, umessage, wparam, lparam);
  }
  return 0;
}
