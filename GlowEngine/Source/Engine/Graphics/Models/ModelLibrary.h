/*
/
// filename: ModelLibrary.h
// author: Callen Betts
// brief: defines ModelLibrary class
//
// description: Model library stores all of our model data (vertices, indices) so we can simply query the library
//  for a model's data to be rendered instead of loading a new one
/
*/

#pragma once

namespace Models
{

  class ModelLibrary
  {

  public:

    ModelLibrary();

    // initialize the model library with presets
    void init();

    // add a model to the map
    void add(std::string name, Models::Model* model);
    // get a model from the map
    Models::Model* get(std::string name);

  private:

    std::map<std::string, Models::Model*> models; // our models

  };

}