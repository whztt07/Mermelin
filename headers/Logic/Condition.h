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
 * @file Condition.h
 * @brief Is used to check logic conditions
 *
 * @author Eduardo Hahn Paredes
 */

#ifndef CONDITION_H
#define	CONDITION_H

#include "Components/LogicComponent.h"

namespace CotopaxiEngine {

    // forward declaration
    class Trigger;

    /**
     * @class Condition
     * 
     * @brief Can be used to build a logical tree.
     *
     * It can be fulfilled or not. The @link check @endlink function indicates it.
     */
    class Condition : public LogicComponent
    {
    public:
        Condition();
        virtual ~Condition();

        /** @brief logical & operator */
        static const int AND = 0;
        /** @brief logical | operator */
        static const int OR = 1;
        
        /**
         * @fn addTrigger
         * Adds a reference to a triger to the Condition
         * @param trigger This can be a Button for example
         */
        void addTrigger(Trigger* trigger);
        
        /**
         * @fn addSubCondition
         * Adds a reference to another Condition as subcondition to the Condition
         * @param condition It can also be composed of other conditions
         */
        void addSubCondition(Condition* condition);

        /**
         * @fn check
         * Shows if the Condition has been fulfilled or not. It calls the @link check @endlink
         * function recursvily on the subconditions and on the triggers.
         * @return true if it has been fulfilled
         */
        bool check();
        
        /**
         * @fn setOperator
         * Sets the operator that will be used by the @link check @endlink function.
         * @param conditionOperator The operators are AND and OR
         */
        void setOperator(int conditionOperator);
        
        /**
         * @fn getOperator
         * Shows the operator used in this Condition.
         * @return AND or OR
         */
        int getOperator() const;
        
        /**
         * @fn receiveEvent
         * Sends a Event::CONDITION_FULFILLED to the Entity if @link check @endlink returns true.
         * @param event Has to be an event of type Event::TRIGGER
         */
        void receiveEvent(Event* event);
    private:
        int conditionOperator;
        std::vector<Trigger*>* triggers;
        std::vector<Condition*>* subconditions;
    };
}

#endif

