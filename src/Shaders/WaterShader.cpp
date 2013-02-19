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
 * @file WaterShader.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Shaders/WaterShader.h"

using namespace CotopaxiEngine;

WaterShader::WaterShader(Entity* entity)
: Shader(entity, "water")
{
    load();
}

void WaterShader::load()
{
    if (Shader::getShaderLevel() == Shader::LEVEL_BASIC) {
        this->setTexture("ocean_floor_color.png");
        this->setTexture("caustics_color.png");
        this->getVertex()->setNamedConstantFromTime("time", Ogre::Real(1));
    } else if (Shader::getShaderLevel() == Shader::ShaderLevel::LEVEL_INTERMEDIATE) {
        this->setTexture("sun_color.png");
        this->setTexture("ocean_floor_color.png");
        this->getFragment()->setNamedConstant("LightMap", 0);
        this->getFragment()->setNamedConstant("GroundMap", 1);
        this->getFragment()->setNamedConstantFromTime("time", Ogre::Real(1));
    }
}