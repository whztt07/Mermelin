/**
 * @file Earth_Shader.h
 * @brief A Shader for an earthclot-like effect.
 * @author Eduardo Hahn Paredes
 */

#ifndef EARTHSHADER_H
#define EARTHSHADER_H

#include "stdafx.h"
#include "Shaders/Shader.h"

namespace CotopaxiEngine {

    /**
     * @class EarthShader
     * @brief A Shader for an earthclot-like effect.
     */
    class EarthShader : public Shader
    {
    public:
        EarthShader(Entity* entity);
        void load();
    };
}

#endif