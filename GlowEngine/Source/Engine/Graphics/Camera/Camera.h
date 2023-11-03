/*
/
// filename: Camera.h
// author: Callen Betts
// brief: defines the camera class for view and projection matrices
/
*/

#pragma once

namespace Visual
{

  class Camera
  {

  public:
    Camera(HWND handle);

    void update();

    // get the view matrix
    const Matrix& getViewMatrix();
    // get the perspective matrix
    const Matrix& getPerspecitveMatrix();

  private:

    // needs a handle to the window
    HWND windowHandle;

    Vector position;
    float rotation;

    // camera contains both the view and perspective matrix
    Matrix viewMatrix;
    Matrix perspectiveMatrix;

  };

}