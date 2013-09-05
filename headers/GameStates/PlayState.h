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
@file PlayState.h
@brief Contains the PlayState

@author Eduardo Hahn Paredes
 */
#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include "stdafx.h"
#include "GameStates/GameState.h"
#include "Shaders/Shader.h"

namespace CotopaxiEngine
{
    /**
     * @class PlayState
     * @brief The main state, when the user is playing the game.
     *
     * The level is loaded and playable.
     */
    class PlayState : public GameState
    {
    public:
        PlayState();
        void load();
        void loadNext();

        void setLevel(int level) {
            this->level = level;
        }

        int getLevel() const {
            return level;
        }

		virtual void receiveEvent(Event* e);
    private:
        int level;		
    };
}

#endif