/**
 * @file Earth_Shader.h
 * @brief A Shader for a glassy, transparent effect.
 * @author Eduardo Hahn Paredes
 */

#ifndef AIRSHADER_H
#define AIRSHADER_H
#include "stdafx.h"
#include "Engine.h"
#include "Shaders/Shader.h"
#include "Entities/Camera.h"
#include "Modules/GraphicModule.h"

namespace CotopaxiEngine
{

    /**
     * @class AirShader
     * @brief A Shader for a glassy, transparent effect.
     */
    class AirShader : public Shader, public Ogre::RenderTargetListener,
    public Ogre::FrameListener
    {
    public:        
        AirShader(Entity* entity);
        virtual ~AirShader();
        void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
        void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
        void load();
        
    private:

        /**
         * @fn createCubeMap
         * Creates a cubemap that will be used in the intermediate version of the Shader.
         */
        void createCubeMap();
        Ogre::Camera* cubeCam;
        Ogre::RenderTarget* targets[6];
    };
}

#endif