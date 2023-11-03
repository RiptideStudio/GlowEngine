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

  private:

    Vector3D position;
    Vector3D scale;
    Vector3D rotation;

    Matrix transformMatrix;

    bool dirty;

  };

}