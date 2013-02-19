/**
 * @file FireShader.h
 * @brief A Shader for fire effects.
 * 
 * @author Eduardo Hahn Paredes
 */

#ifndef FIRESHADER_H
#define FIRESHADER_H

#include "stdafx.h"
#include "Shaders/Shader.h"

namespace CotopaxiEngine {

    /**
     * @class FireShader
     * @brief A Shader for lava-like effects.
     */
    class FireShader : public Shader
    {
    public:
        FireShader(Entity* entity);
        void load();
    };
}

#endif