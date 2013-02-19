/**
 * @file WaterShader.h
 * @brief A Shader for water effects.
 * 
 * @author Eduardo Hahn Paredes
 */

#ifndef WATERSHADER_H
#define WATERSHADER_H
#include "stdafx.h"
#include "Shaders/Shader.h"

namespace CotopaxiEngine {

    /**
     * @class WaterShader
     * @brief A Shader for water effects.
     */
    class WaterShader : public Shader
    {
    public:
        WaterShader(Entity* entity);
        void load();
    };
    
}

#endif