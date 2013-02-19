/**
 * @file RotationShader.h
 * @brief This class enables the rotation of objects around an axis.
 * @author Eduardo Hahn Paredes
 */

#ifndef ROTATION_H
#define	ROTATION_H

#include "stdafx.h"
#include "Shaders/Shader.h"

namespace CotopaxiEngine
{
    /**
     * @class RotationShader
     * @brief This Shader enables the rotation of objects around an axis.
     * 
     * It uses the files rotation_basic.vert and rotation_basic.frag
     * to accomplish its work.
     */
    class RotationShader : public Shader
    {
    public:
        /**
         * Only the Entity* is mandatory for the constructor.
         * @param entity
         * The Entity on witch the rotation shall be applied.
         * @param speed
         * Sets the velocity of the rotation. Negative values will invert the rotation direction.
         * @param axis
         * Sets the axis for the rotation:
         * 0 = z-axis
         * 1 = y-axis
         * 2 = x-axis
         */
        RotationShader(Entity* entity, int speed = 1, int axis = 0, 
                std::string materialName = "default");
        
        void load();
    private:
        int axis;
        int speed;
    };
}

#endif

