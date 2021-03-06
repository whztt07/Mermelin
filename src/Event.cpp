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
 * Event.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Event.h"

using namespace CotopaxiEngine;

Event::Event(EventType e)
: type(e) 
{ 
	component = NULL;
	entity = NULL;
}

Event::EventType Event::getType() const
{
    return type;
}

void Event::setEntity(Entity* entity)
{
    this->entity = entity;
}

Entity* Event::getEntity() const
{
    return entity;
}

void Event::setComponent(BaseComponent* component)
{
    this->component = component;
}

BaseComponent* Event::getComponent() const
{
    return component;
}