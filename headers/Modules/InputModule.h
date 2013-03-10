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
 * @file InputModule.h
 * @brief Contains the InputModule
 * 
 * @author Eduardo Hahn Paredes
 */

#ifndef INPUTMODULE_H
#define	INPUTMODULE_H

#include "stdafx.h"
#include "Modules/BaseModule.h"
#include "Engine.h"
#include "Event.h"
#include "Modules/GUIModule.h"

using namespace OIS;

namespace CotopaxiEngine {
    // forward declarations
    class InputComponent;
    class Event;

    /**
     * @class InputModule
     * @brief Handels polling the user input and informing the required components
     * 
     * This module serves as a gateway to the input devices, such as Keyboard, Mouse, 
     * Wii-controller and others. It serves the connected Components with the keypresses they are 
     * interested in.
     * 
     * It's realised with the OIS (Object Oriented Input System) library
     * @see http://sourceforge.net/projects/wgois/
     */
    class InputModule : public BaseModule, public KeyListener,
    public MouseListener, public Ogre::WindowEventListener
    {
    public:

        InputModule(void);
        virtual ~InputModule();

        virtual bool update(const Ogre::FrameEvent &evt);
        virtual void windowResized(Ogre::RenderWindow *rw);
        virtual void windowClosed(Ogre::RenderWindow *rw);

        // keyboard event
        virtual bool keyPressed(const KeyEvent &event);
        virtual bool keyReleased(const KeyEvent &event);

        // mouse events
        virtual bool mouseMoved(const MouseEvent &event);
        virtual bool mousePressed(const MouseEvent &event, MouseButtonID id);
        virtual bool mouseReleased(const MouseEvent &event, MouseButtonID id);

        virtual BaseComponent* getComponent(Entity *parent);
        void unload();

    private:
        bool menuMode;
        InputManager* input;
        Mouse* mouse;
        Keyboard* key;
        std::vector<InputComponent*> components;

        Ogre::Radian calculateAngle(float* time);
        void moveSphere(Ogre::Vector3 rotation);

        // Rocket
        typedef std::map< OIS::KeyCode, Rocket::Core::Input::KeyIdentifier > KeyIdentifierMap;
        KeyIdentifierMap keys;
        Rocket::Core::Context* getContext()
        {
            return ENGINE->getGUI()->getContext();
        }

        int getModifier();
        void buildKeyMap();
    };
}

#endif