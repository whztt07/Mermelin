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
 * @file TypeChanger.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Entities/TypeChanger.h"

using namespace CotopaxiEngine;

TypeChanger::TypeChanger(std::string name, Ogre::SceneNode* parentNode)
: Entity(name, "TypeChanger", parentNode)
{
    this->addComponent(ENGINE->getGraphic()->getComponent(this));
    symbolBar = ENGINE->createEntity("TC_Bar_" + name, "TypeChangerAE", this->getNode());
    symbolBar->addComponent(ENGINE->getGraphic()->getComponent(symbolBar));
    symbolTriangle = ENGINE->createEntity("TC_Tri_" + name, "TypeChangerElement", this->getNode());
    symbolTriangle->addComponent(ENGINE->getGraphic()->getComponent(symbolTriangle));
}

TypeChanger::~TypeChanger()
{
}

void TypeChanger::setElement(CotopaxiEngine::Element element)
{
    this->element = element;

    switch (element)
    {
        case Element::AIR:
            symbolTriangle->getGraphicComponent()->setShader(new AirShader(symbolTriangle));
            symbolBar->getGraphicComponent()->setShader(new AirShader(symbolBar));
            break;
        case Element::EARTH:
            symbolTriangle->getGraphicComponent()->setShader(new EarthShader(symbolTriangle));
            symbolBar->getGraphicComponent()->setShader(new EarthShader(symbolBar));
            break;
        case Element::FIRE:
            symbolTriangle->getGraphicComponent()->setShader(new FireShader(symbolTriangle));
            break;
        case Element::WATER:
            symbolTriangle->getGraphicComponent()->setShader(new WaterShader(symbolTriangle));
            break;
    }
}

void TypeChanger::receiveEvent(Event* e)
{
    if (e->getType() == Event::COLLISION_ENTER) {
        Sphere* sphere = dynamic_cast<Sphere*> (e->entity);
        if (sphere != NULL) {
            sphere->setElement(this->element);
        }
    } else {
        Entity::receiveEvent(e);
    }
}