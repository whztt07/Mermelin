/*
 * Mermelin, Copyright © 2013 by Eduardo Hahn Paredes (mermelin@outlook.com)
 * 
 * Mermelin is free software: You can redistribute it and/or modify it under the terms of the 
 * GNU General Public License as published by the Free Software Foundation, either version 3 of 
 * the License, or (at your option) any later version.
 * Mermelin is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without 
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with Mermelin. 
 * Look for a file named COPYING. If it's not there, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file Shader.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Shaders/Shader.h"
#include "Entities/Entity.h"

using namespace CotopaxiEngine;
using namespace Ogre;

// inintalizing static variable
int Shader::shaderLevel = Shader::LEVEL_INTERMEDIATE;

Shader::Shader(CotopaxiEngine::Entity* entity, std::string shaderName, std::string materialName)
: entity(entity),
shaderName(shaderName),
materialName(materialName)
{
    prepare();
}

Shader::~Shader() {}

GpuProgramParametersSharedPtr Shader::getVertex()
{
    return pass->getVertexProgramParameters();
}

GpuProgramParametersSharedPtr Shader::getFragment()
{
    return pass->getFragmentProgramParameters();
}

GpuProgramParametersSharedPtr Shader::getGeometry()
{
    return pass->getGeometryProgramParameters();
}

void Shader::setTexture(std::string name, std::string parameterName)
{
    TextureUnitState* texture = getPass()->createTextureUnitState(name);
    texture->setTextureNameAlias(parameterName);
    texture->setName(parameterName);
}

void Shader::setGeometryShaderParameters(Ogre::RenderOperation::OperationType in,
        Ogre::RenderOperation::OperationType out, int max)
{
    GLSLProgram* t = static_cast<GLSLProgram*> (geometry.getPointer());
    t->setInputOperationType(in);
    t->setOutputOperationType(out);
    t->setMaxOutputVertices(max);
}

void Shader::reload()
{    
    prepare();
    load();
}

void Shader::prepare()
{
    // determining shader level and generating a part of the level string 
    std::string levelString = "_basic";
    if (shaderLevel == LEVEL_INTERMEDIATE) {
        levelString = "_intermediate";
    } else if (shaderLevel == LEVEL_ADVANCED) {
        levelString = "_advanced";
    }

    std::string vertexString;
    std::string geometryString;
    std::string fragmentString;

    // trying to use actual shader level, choosing basic as default/backup
    if (ResourceGroupManager::getSingleton()
            .resourceExistsInAnyGroup(shaderName + levelString + ".vert")) {
        vertexString = shaderName + levelString + ".vert";
    } else {
        vertexString = shaderName + "_basic.vert";
    }

    if (ResourceGroupManager::getSingleton()
            .resourceExistsInAnyGroup(shaderName + levelString + ".geom")) {
        geometryString = shaderName + levelString + ".geom";
    } else {
        geometryString = shaderName + "_basic.geom";
    }

    if (ResourceGroupManager::getSingleton()
            .resourceExistsInAnyGroup(shaderName + levelString + ".frag")) {
        fragmentString = shaderName + levelString + ".frag";
    } else {
        fragmentString = shaderName + "_basic.frag";
    }

    // creating new material
    material = MaterialManager::getSingletonPtr()->create(materialName + shaderName,
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    pass = material->getTechnique(0)->getPass(0);

    // with dynamic lighting only geometry with normals will be displayed
    pass->setLightingEnabled(true);
    material->setReceiveShadows(true);

    // loading vertex shader if there is one
    if (ResourceGroupManager::getSingleton()
            .resourceExistsInAnyGroup(vertexString)) {
        HighLevelGpuProgramPtr vertex =
                HighLevelGpuProgramManager::getSingletonPtr()->createProgram(
                shaderName + "_v",
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                "glsl", GpuProgramType::GPT_VERTEX_PROGRAM);
        vertex->setSourceFile(vertexString);
        pass->setVertexProgram(vertex->getName());
    }

    // loading geometry shader if there is one
    if (ResourceGroupManager::getSingleton()
            .resourceExistsInAnyGroup(geometryString)) {
        geometry = HighLevelGpuProgramManager::getSingletonPtr()->createProgram(
                shaderName + "_g",
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "glsl",
                GpuProgramType::GPT_GEOMETRY_PROGRAM);
        geometry->setSourceFile(geometryString);
        pass->setGeometryProgram(geometry->getName());
    }

    // loading fragment shader if there is one
    if (ResourceGroupManager::getSingleton()
            .resourceExistsInAnyGroup(fragmentString)) {
        HighLevelGpuProgramPtr fragment =
                HighLevelGpuProgramManager::
                getSingletonPtr()->createProgram(shaderName + "_f",
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                "glsl", GpuProgramType::GPT_FRAGMENT_PROGRAM);
        fragment->setSourceFile(fragmentString);
        pass->setFragmentProgram(fragment->getName());
    }
    entity->getOgreEntity()->setMaterialName(material->getName());
}