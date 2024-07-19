/*
/
// filename: Window.cpp
// author: Callen Betts
// brief: implements Window.h
/
*/

#include "stdafx.h"
#include "Window.h"
#include "Engine/GlowEngine.h"


// construct the window
Graphics::Window::Window()
  : windowWidth(GetSystemMetrics(SM_CXSCREEN)/1.25f),
  windowHeight(GetSystemMetrics(SM_CYSCREEN)/1.25f)
{
  windowClassName = L"Otherglow Window";
  windowName = L"Otherglow";
}

// setup the window and register it in windows
bool Graphics::Window::setup()
{
  // register the window class
  hInstance = GetModuleHandle(nullptr);

  WNDCLASSEX wc = {};
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = windowProcStatic;
  wc.hInstance = hInstance;
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.lpszClassName = windowClassName.c_str();

  RegisterClassEx(&wc);

  // Create window handle
  windowHandle = CreateWindowEx(
    0, // window style
    windowClassName.c_str(), // class name
    windowName.c_str(), // window title
    WS_BORDER, // window style
    CW_USEDEFAULT, // initial horizontal position of the window
    CW_USEDEFAULT, // initial vertical position of the window
    windowWidth, // width
    windowHeight, // height
    nullptr, // window parent (none)
    nullptr, // window child (none)
    hInstance, // window instance handle
    this // Set the GWLP_USERDATA to the GlowEngine instance
  );

  if (windowHandle == nullptr)
  {
    return false;
  }

  // position the window to be in the centre of the screen
  RECT rect = { 0, 0, static_cast<LONG>(windowWidth), static_cast<LONG>(windowHeight) };
  AdjustWindowRect(&rect, GetWindowLong(windowHandle, GWL_STYLE), FALSE);
  SetWindowPos(windowHandle, nullptr, 0, 0, windowWidth, windowHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);

  return true;
}

// show and update the window
// update the window with it's system pointers 
void Graphics::Window::updateWindow()
{
  // window refresh
  UpdateWindow(windowHandle);
  ShowWindow(windowHandle, true);
  // system initialization
  engine = EngineInstance::getEngine();
  input = engine->getInputSystem();
}

// windows message callback
LRESULT Graphics::Window::windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
    return true;

  // handle input system key states
  if (input)
    input->updateKeyStates();

  switch (message)
  {
    // when a key is first triggered, set the keystate to active
  case WM_KEYDOWN:
    input->onKeyTriggered(wParam);
    break;

    // when a key is released, you should reset the keystate and call "released"
  case WM_KEYUP:
    input->onKeyRelease(wParam);
    break;

    // destroy the window and stop the engine
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// windows callback function
LRESULT Graphics::Window::windowProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  return EngineInstance::getEngine()->getWindow()->windowProc(hWnd, message, wParam, lParam);
}

void Graphics::Window::setFullscreen(bool val)
{
  if (val)
  {
    SetWindowLongPtr(windowHandle, GWL_STYLE, WS_BORDER | WS_VISIBLE);
    SetWindowPos(windowHandle, HWND_TOP, 0, 0, windowWidth, windowHeight, SWP_FRAMECHANGED);
    ShowWindow(windowHandle, SW_MAXIMIZE);
  }
  else
  {
    SetWindowLongPtr(windowHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
    SetWindowPos(windowHandle, nullptr, 0, 0, windowWidth, windowHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
    ShowWindow(windowHandle, SW_NORMAL);
  }
}

// update messages, return false if we quit the window
bool Graphics::Window::updateMessages()
{
  if (GetMessage(&msg, nullptr, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if (msg.message == WM_QUIT)
    {
      return false;
    }
  }
  return true;
}

int Graphics::Window::getWidth()
{
  return windowWidth;
}

int Graphics::Window::getHeight()
{
  return windowHeight;
}

// return the current message parameter (quit, create, etc)
int Graphics::Window::getMessageParam()
{
  return static_cast<int>(msg.wParam);
}
