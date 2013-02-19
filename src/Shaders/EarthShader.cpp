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
 * @file EarthShader.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Shaders/EarthShader.h"

using namespace CotopaxiEngine;

EarthShader::EarthShader(Entity* entity)
: Shader(entity, "earth")
{
    load();
}

void EarthShader::load()
{
    if (Shader::getShaderLevel() == Shader::LEVEL_BASIC) {
        this->setTexture("dirt_color.png");
    } else if (Shader::getShaderLevel() == Shader::ShaderLevel::LEVEL_INTERMEDIATE) {
        this->setTexture("dirt_color.png");
        this->setTexture("dirt_normal.png");
        this->getVertex()->setNamedAutoConstant("light", 
                Ogre::GpuProgramParameters::ACT_LIGHT_POSITION, 0);
        this->getVertex()->setNamedAutoConstant("camera", 
                Ogre::GpuProgramParameters::ACT_CAMERA_POSITION);
        this->getFragment()->setNamedConstant("dirt_color", 0);
        this->getFragment()->setNamedConstant("dirt_normal", 1);
    }
}