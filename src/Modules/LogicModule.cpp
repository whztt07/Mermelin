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
 * LogicModule.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Modules/LogicModule.h"
#include "Logic/Trigger.h"
#include "Logic/Condition.h"

using namespace CotopaxiEngine;

LogicModule::~LogicModule()
{
    unload();
}

BaseComponent* LogicModule::createTrigger(Entity* parent, std::string name)
{
    Trigger* trigger = new Trigger(name);
    trigger->setParent(parent);
    triggers[name] = trigger;
    return (BaseComponent*) trigger;
}

BaseComponent* LogicModule::createTarget(Entity* parent)
{
    Condition* target = new Condition();
    target->setParent(parent);
    targets[parent->getName()] = target;
    return (BaseComponent*) target;
}

Trigger* LogicModule::getTrigger(std::string name)
{
    return triggers[name];
}

Condition* LogicModule::getTarget(Entity* parent)
{
    return targets[parent->getName()];
}

BaseComponent* LogicModule::getComponent(Entity* parent)
{
    return (BaseComponent*) targets[parent->getName()];
}

void LogicModule::unload()
{
    for (std::map<std::string, Trigger*>::iterator i = triggers.begin(); i != triggers.end(); i++) {
        triggers.erase(i);
    }
    triggers.clear();

    for (std::map<std::string, Condition*>::iterator i = targets.begin(); i != targets.end(); i++) {
        targets.erase(i);
    }
    targets.clear();
}