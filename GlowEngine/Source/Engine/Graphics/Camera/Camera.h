/*
/
// filename: Camera.h
// author: Callen Betts
// brief: defines the camera class for view and projection matrices
/
*/

#pragma once

namespace Graphics
{
  class Renderer;
}

namespace Visual
{

  class Camera
  {

  public:
    Camera(Graphics::Renderer* renderEngine);

    // update the camera properties
    void update();
    // camera controller
    void cameraController();

    // get the view matrix
    const Matrix& getViewMatrix();
    // get the perspective matrix
    const Matrix& getPerspecitveMatrix();

    // get the position
    XMVector getPosition() { return position; }

  private:

    // renderer
    Graphics::Renderer* renderer;
    Engine::GlowEngine* engine;
    Input::InputSystem* input;

    // camera properties
    XMVector position;
    XMVector target;
    XMVector upDirection;
    XMVector forward;
    XMVector right;
    float rotation;

    // controller properties
    float fov; // field of view
    float viewDistance; // how far we can see
    float yaw;
    float pitch;
    float cameraSpeed;
    float mouseSensitivity;

    // window properties
    float windowWidth;
    float windowHeight;
    float aspectRatio;
    HWND windowHandle;

    // camera contains both the view and perspective matrix
    Matrix viewMatrix;
    Matrix perspectiveMatrix;

  };

}