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
 * @file Airhader.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Shaders/AirShader.h"

using namespace CotopaxiEngine;
using namespace Ogre;

AirShader::AirShader(CotopaxiEngine::Entity* entity)
: Shader(entity, "air")
{
    load();
}

AirShader::~AirShader()
{
    ENGINE->getSceneManager()->destroyCamera(cubeCam);
    MeshManager::getSingleton().remove("floor");
    TextureManager::getSingleton().remove("cubeMap");
}

void AirShader::createCubeMap()
{
    // creating the camera used to render to our cubemap
    Ogre::String name = entity->getName() + Ogre::String("_cubeCam");

    if (ENGINE->getSceneManager()->hasCamera(name)) {
        cubeCam = ENGINE->getSceneManager()->getCamera(name);
    } else {
        cubeCam = ENGINE->getSceneManager()->createCamera(name);
        cubeCam->setFOVy(Degree(30));
        cubeCam->setNearClipDistance(0.1);
        entity->getNode()->attachObject(cubeCam);

        // creating the dynamic cube map texture
        TexturePtr tex = TextureManager::getSingleton().createManual("cubeMap",
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                TEX_TYPE_CUBE_MAP, 128, 128, 0, PF_R8G8B8, TU_RENDERTARGET);

        // assigning the camera to all 6 render targets of the texture
        // (1 for each direction)
        for (unsigned int i = 0; i < 6; i++) {
            targets[i] = tex->getBuffer(i)->getRenderTarget();
            targets[i]->addViewport(cubeCam)->setOverlaysEnabled(false);
            targets[i]->addListener(this);
        }
    }
}

void AirShader::preRenderTargetUpdate(const RenderTargetEvent& evt)
{
    entity->getNode()->setVisible(false);
    cubeCam->setOrientation(Quaternion::IDENTITY);
    if (evt.source == targets[0]) cubeCam->yaw(Degree(-90));
    else if (evt.source == targets[1]) cubeCam->yaw(Degree(90));
    else if (evt.source == targets[2]) cubeCam->pitch(Degree(90));
    else if (evt.source == targets[3]) cubeCam->pitch(Degree(-90));
    else if (evt.source == targets[5]) cubeCam->yaw(Degree(180));
}

void AirShader::postRenderTargetUpdate(const RenderTargetEvent& evt)
{
    entity->getNode()->setVisible(true);
}

void AirShader::load()
{
    if (Shader::getShaderLevel() == Shader::LEVEL_BASIC) {
        this->getVertex()->setNamedConstantFromTime("time", Real(1));
        this->setTexture("caustics_color.png");
    } else if (Shader::getShaderLevel() == Shader::ShaderLevel::LEVEL_INTERMEDIATE) {
        createCubeMap();
        this->getPass()->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        this->getPass()->setDepthWriteEnabled(false);

        this->getVertex()->setNamedAutoConstant("camera",
                GpuProgramParameters::ACT_CAMERA_POSITION);
        this->getVertex()->setNamedAutoConstant("world",
                GpuProgramParameters::ACT_WORLDVIEW_MATRIX);

        this->setTexture("clouds_color.png");
        this->setTexture("cubeMap");
        this->getFragment()->setNamedConstantFromTime("time", Ogre::Real(1));
        this->getFragment()->setNamedConstant("colorMap", 0);
        this->getFragment()->setNamedConstant("cubeMap", 1);
    }
}