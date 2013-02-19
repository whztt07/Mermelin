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
@file IntroState.h
@brief Contains the IntroState

@author Eduardo Hahn Paredes
 */
#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "stdafx.h"
#include "GameStates/GameState.h"
//#include "GameStates/PlayState.h"

namespace CotopaxiEngine
{
    class PlayState;

    /**
     * @class IntroState
     * @brief Shows intro videos and images
     *
     * This state shows a splashscreen and intro videos. It changes into menustate, when 
     * finished or the user cancles the intro
     * @todo implement
     */
    class IntroState : public GameState
    {
    private:
        Ogre::Rectangle2D* rect;
        Ogre::Timer timer;
        PlayState* playState;
    public:
        IntroState();
        ~IntroState();

        virtual void load();
        virtual void unload();

        bool frameRenderingQueued(const Ogre::FrameEvent& evt);

        virtual void pause() { }
    };
}

#endif