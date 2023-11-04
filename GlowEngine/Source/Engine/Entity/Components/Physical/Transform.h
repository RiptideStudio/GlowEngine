/*
/
// filename: Transform.h
// author: Callen Betts
// brief: defines the Transform class, holds vertices and Transform data
/
*/

#pragma once
#include "Engine/Entity/Components/Component.h"

namespace Components
{

  class Transform : public Component
  {

  public:
    Transform();

    // we want to avoid doing this as much as possible
    void recalculateMatrix();
    // check if dirty flag is set for matrix recalculation
    bool isDirty();
    // get the transform matrix
    const Matrix& getTransformMatrix();
    // update the transform - all this does is determine if we need to update the matrix
    void update();

    // getters and setters
    void setPosition(Vector3D pos);

  private:

    Vector3D position;
    Vector3D scale;
    Vector3D rotation;

    Matrix transformMatrix;

    bool dirty;

  };

}