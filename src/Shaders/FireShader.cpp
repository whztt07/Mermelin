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
 * @file FireShader.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Shaders/FireShader.h"

using namespace CotopaxiEngine;

FireShader::FireShader(Entity* entity)
: Shader(entity, "fire")
{
    load();
}

void FireShader::load()
{
    if (Shader::getShaderLevel() == Shader::LEVEL_BASIC) {
        this->getFragment()->setNamedConstantFromTime("time", Ogre::Real(1));
        this->getFragment()->setNamedConstant("startColor", Ogre::ColourValue(1, 1, 0, 1));
        this->getFragment()->setNamedConstant("endColor", Ogre::ColourValue(1, 0, 0, 1));
    } else if (Shader::getShaderLevel() == Shader::ShaderLevel::LEVEL_INTERMEDIATE) {
		this->getFragment()->setNamedConstantFromTime("time", Ogre::Real(1));
		this->getFragment()->setNamedConstant("modifier", 0.1f);

		this->setTexture("lava_alpha.png");
		this->getFragment()->setNamedConstant("Texture1", 0);
		this->setTexture("lava_color.jpg");
		this->getFragment()->setNamedConstant("Texture2", 1);
    }
}