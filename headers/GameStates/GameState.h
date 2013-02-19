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
 * @file GameState.h
 * @brief Contains the basic game state
 *
 * @author Eduardo Hahn Paredes
 */
#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "stdafx.h"
#include "Engine.h"

namespace CotopaxiEngine
{

    /**
     * @class GameState
     * @brief A base class implemented by other GameStates
     *
     * In a game engine, various different task have to be done, such as navigating a menu, 
     * playing intro videos, cutscenes, actual gameplay and many more. It's not feasible to 
     * build all that functionality staticaly somewhere in your code.
     * To do this, we implement GameStates, based on the State pattern.
     *
     * The gamestates will be managed in a stack, so switching between a previous or a following state is as easy as pushing it onto the stack.
     */
	class GameState: public EventListener
    {
    protected:
        bool loaded;
    public:
        GameState();
        ~GameState();

        bool isLoaded() {
            return loaded;
        };

        /**
         * @fn load
         * Loads the gamestate, initialize desired modules here
         */
        virtual void load() = 0;

        /**
         * @fn update
         * Updates the current gamestate
         */
        virtual bool frameStarted(const Ogre::FrameEvent& evt) {
            return true;
        }

		virtual void receiveEvent(Event* e){}

        void pushState(GameState* state);
        void popState();
    };
}


#endif

