/**
 * @file Shader.h
 * @brief This class is the door to GLSL.
 *
 * @author Eduardo Hahn Paredes
 */

#ifndef SHADER_H
#define	SHADER_H
#include "stdafx.h"

namespace CotopaxiEngine {

    // forward declaration
    class Entity;

    /**
     * @class Shader
     * @brief This class has is the door to the "OpenGL Shader Language" (GLSL)
     * 
     * It generates a material in the background. On that materials GPU-programs can be attached.
     */
    class Shader
    {
    public:

        /**
         * @enum ShaderLevel
         * Three levels of shaders are supported
         * - LEVEL_BASIC
         * - LEVEL_INTERMEDIATE
         * - LEVEL_ADVANCED
         */
        enum ShaderLevel
        {
            LEVEL_BASIC = 0,
            LEVEL_INTERMEDIATE = 1,
            LEVEL_ADVANCED = 2
        };

        /**
         * The construtor needs at least a pointer to the Entity and a name. This name will be
         * looked up combined with the shader level.
         * 
         * If multiple shaders shall be used with 
         * different attributes its very important to provide also a name for the material. If not
         * the las specified attributes will be applied because the same material is used.
         * 
         * The shader files are stored in the /media/shaders folder. There are the following files:
         * - .frag              GLSL Fragment Shader files
         * - .geom              GLSL Geometry Shader files
         * - .vert              GLSL Vertex Shader files
         * 
         * Every file must have one of the following endings (before filetype)
         * - _basic             Basic-level shader (default)
         * - _intermediate      Intermediate-level shader
         * - _advanced          Advanced-level shader
         * 
         * Those endings are chowsen depending on the ShaderLevel. This would be for example
         * 
         * air_intermediate.frag 
         * 
         * if ShaderLevel is INTERMEDIATE and "air" is chowsen as name.
         * 
         * Then the private @link load @endlink function checks if the resource exists and loads
         * the parts that are present. This can be from one to three shaders. Not every part has to 
         * be present. It's absolutely ok to have only a fragent shader for example.
         * 
         * @param entity The Entity on which the shader shall take effect
         * @param shaderName The name of the Shader. This is would be "air" in the example.
         * @param materialName A unique name for the material.
         */
        Shader(Entity* entity, std::string shaderName, std::string materialName = "default");
        virtual ~Shader();

        /**
         * @fn setTexture
         * Creates a texture unit state in the material. It will put the given image on a stack.
         * @param name The name of the texture.
         * @param parameterName A texture name alias
         */
        void setTexture(std::string name, std::string parameterName = "");

        /**
         * @fn setGeometryShaderParameters
         * This is only needed if a geometry shader is used. But when a geometry shader is there, 
         * it's mandatory.
         * @param in Tells what RenderOperation (like OT_TRIANGLE_LIST) comes in
         * @param out Tells what RenderOperation (like OT_TRIANGLE_LIST) comes out
         * @param max Defines the maximum of output vertices.
         */
        void setGeometryShaderParameters(Ogre::RenderOperation::OperationType in,
                Ogre::RenderOperation::OperationType out, int max);

        /**
         * @fn getVertex
         * Has to be called to pass variables from the program to the vertex shader.
         * @return A shared pointer to a GpuProgramParameters reference
         */
        Ogre::GpuProgramParametersSharedPtr getVertex();

        /**
         * @fn getGeometry
         * Has to be called to pass variables from the program to the geometry shader.
         * @return A shared pointer to a GpuProgramParameters reference
         */
        Ogre::GpuProgramParametersSharedPtr getGeometry();

        /**
         * @fn getFragment
         * Has to be called to pass variables from the program to the fragment shader.
         * @return A shared pointer to a GpuProgramParameters reference
         */
        Ogre::GpuProgramParametersSharedPtr getFragment();
        /**
         * @fn getPass
         * Return the pass 0 from the generated material
         * @return A pointer to the Ogre::Pass
         */
        Ogre::Pass* getPass() const
        {
            return pass;
        }
        static void setShaderLevel(ShaderLevel level)
        {
            shaderLevel = level;
        }
        static int getShaderLevel()
        {
            return shaderLevel;
        }
        
        /**
         * @fn reload
         * Calls the @link load @endlink fuction of the subclass, making use of the template
         * design pattern.
         */
        void reload();

    protected:

        /**
         * @fn load
         * Overwritten Abstract function that has to be implemented by the subclass defining the 
         * shader. Is used
         * Should be called by its contructor.
         */
        virtual void load() = 0;        
        Entity* entity;

    private:
        static int shaderLevel;
        Ogre::HighLevelGpuProgramPtr geometry;
        Ogre::MaterialPtr material;
        Ogre::Pass* pass;
        std::string shaderName;
        std::string materialName;
        
        void prepare();
    };
}

#endif