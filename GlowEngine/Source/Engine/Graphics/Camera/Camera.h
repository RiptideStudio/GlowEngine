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

    // camera properties
    XMVector position;
    XMVector target;
    XMVector upDirection;
    float rotation;

    // our field of view
    float fov; 
    // how far we can see
    float viewDistance; 

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