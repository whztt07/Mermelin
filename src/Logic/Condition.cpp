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
 * @file Condition.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Logic/Condition.h"
#include "Logic/Trigger.h"

using namespace CotopaxiEngine;

Condition::Condition()
: triggers(new vector<Trigger*>), subconditions(new std::vector<Condition*>) { }

Condition::~Condition()
{
    delete triggers;
    delete subconditions;
}

void Condition::addTrigger(Trigger* trigger)
{
    triggers->push_back(trigger);
}

void Condition::addSubCondition(Condition* condition)
{
    subconditions->push_back(condition);
}

bool Condition::check()
{
    // checking triggers
    for (vector<Trigger*>::iterator it = triggers->begin(); it != triggers->end(); it++) {
        if (conditionOperator == Condition::AND) {
            if ((*it)->check() == false) {
                return false;
            }
        }
        if (conditionOperator == Condition::OR) {
            if ((*it)->check() == true) {
                return true;
            }
        }
    }

    // checking subconditions
    for (vector<Condition*>::iterator it = subconditions->begin(); it != subconditions->end(); it++) {
        if (conditionOperator == Condition::AND) {
            if ((*it)->check() == false) {
                return false;
            }
        }
        if (conditionOperator == Condition::OR) {
            if ((*it)->check() == true) {
                return true;
            }
        }
    }
    return true;
}

void Condition::setOperator(int conditionOperator)
{
    this->conditionOperator = conditionOperator;
}

int Condition::getOperator() const
{
    return conditionOperator;
}

void Condition::receiveEvent(Event* event)
{
    if (event->getType() == Event::TRIGGER) {
        if (check()) {
            parent->receiveEvent(new Event(Event::CONDITION_FULFILLED));
        }
    }
}