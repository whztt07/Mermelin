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
 * @file Trigger.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Logic/Trigger.h"

using namespace CotopaxiEngine;

Trigger::Trigger(std::string name)
: name(name), targets(new vector<Condition*>) { }

void Trigger::setName(std::string name)
{
    this->name = name;
}

std::string Trigger::getName() const
{
    return name;
}

Trigger::~Trigger()
{
    delete targets;
    targets = NULL;
}

void Trigger::addTarget(Condition* condition)
{
    targets->push_back(condition);
}

bool Trigger::check()
{
    return fired;
}

void Trigger::fire()
{
    for (int i = 0; i < targets->size(); i++) {
        targets->at(i)->receiveEvent(new Event(Event::TRIGGER));
    }
    fired = true;
}

void Trigger::receiveEvent(Event* event)
{
    if (event->getType() == Event::COLLISION_ENTER) {
        fire();
    }
}