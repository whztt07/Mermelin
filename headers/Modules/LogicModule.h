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
 * @file LogicModule.h
 * @brief Contains the LogicModule
 * 
 * @author Eduardo Hahn Paredes
 */

#ifndef LOGICMODULE_H
#define LOGICMODULE_H

#include "Modules/LogicModule.h"
#include "Components/BaseComponent.h"

using namespace std;
namespace CotopaxiEngine
{
    //forward declarations
    class LogicComponent;
    class Trigger;
    class Condition;
    
    /**
     * @class LogicModule
     * @brief handles logic behaviour between Entities
     * 
     * Games are driven by rules. These rules are enforced by the LogicModule.
     */
    class LogicModule : public BaseModule
    {
    public:
        virtual ~LogicModule();
        Trigger* getTrigger(std::string name);
        Condition* getTarget(Entity* parent);
        BaseComponent* createTrigger(Entity* parent, std::string name);
        BaseComponent* createTarget(Entity* parent);
        BaseComponent* getComponent(Entity* parent);
        void unload();
    private:
        std::map<std::string, Trigger*> triggers;
        std::map<std::string, Condition*> targets;
    };
}

#endif