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
 * InputModule.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Modules/InputModule.h"
#include "Modules/AudioModule.h"
#include "Modules/GraphicModule.h"
#include "Modules/GUIModule.h"
#include "Components/PhysicsComponent.h"
#include "Components/InputComponent.h"
#include "Shaders/Shader.h"
#include "Entities/Camera.h"

using namespace OIS;
using namespace CotopaxiEngine;

InputModule::InputModule()
{
    ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    ENGINE->getWindow()->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    // needed in Linux to make debugging possible
#if defined OIS_LINUX_PLATFORM
    pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

    input = InputManager::createInputSystem(pl);
    key = static_cast<Keyboard*> (
            input->createInputObject(OISKeyboard, true));
    mouse = static_cast<Mouse*> (input->createInputObject(OISMouse, true));
    mouse->setEventCallback(this);
    key->setEventCallback(this);

    windowResized(ENGINE->getWindow());
    Ogre::WindowEventUtilities::addWindowEventListener(ENGINE->getWindow(), this);

    buildKeyMap();
    menuMode = false;
}

InputModule::~InputModule()
{
    Ogre::WindowEventUtilities::removeWindowEventListener(ENGINE->getWindow(), this);
    windowClosed(ENGINE->getWindow());
}

bool InputModule::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    mouse->capture();
    key->capture();

    Ogre::Real time = evt.timeSinceLastFrame;
    Ogre::Real speed = 20.0f;

    if (ENGINE->getState() != ENGINE->STATE_STOPPED) {
        if (key->isKeyDown(OIS::KC_NUMPAD4)) {
            ENGINE->getCamera()->translate(Ogre::Vector3(speed, 0.0, 0.0) * time);
        }
        if (key->isKeyDown(OIS::KC_NUMPAD6)) {
            ENGINE->getCamera()->translate(Ogre::Vector3(-speed, 0.0, 0.0) * time);
        }
        if (key->isKeyDown(OIS::KC_NUMPAD8)) {
            ENGINE->getCamera()->translate(Ogre::Vector3(0.0, 0.0, speed) * time);
        }
        if (key->isKeyDown(OIS::KC_NUMPAD2)) {
            ENGINE->getCamera()->translate(Ogre::Vector3(0.0, 0.0, -speed) * time);
        }
        if (key->isKeyDown(OIS::KC_PGDOWN)) {
            ENGINE->getCamera()->translate(Ogre::Vector3(0.0, -speed, 0.0) * time);
        }
        if (key->isKeyDown(OIS::KC_PGUP)) {
            ENGINE->getCamera()->translate(Ogre::Vector3(0.0, speed, 0.0) * time);
        }
    }

    if (key->isKeyDown(OIS::KC_UP)) {
        moveSphere(Ogre::Vector3(0, -1, -1));
    }
    if (key->isKeyDown(OIS::KC_DOWN)) {
        moveSphere(Ogre::Vector3(0, -1, 1));
    }
    if (key->isKeyDown(OIS::KC_LEFT)) {
        moveSphere(Ogre::Vector3(-1, -1, 0));
    }
    if (key->isKeyDown(OIS::KC_RIGHT)) {
        moveSphere(Ogre::Vector3(1, -1, 0));
    }

    // quitting the game
    if (key->isKeyDown(OIS::KC_Q) ||
            (key->isKeyDown(OIS::KC_LMENU) && key->isKeyDown(OIS::KC_F4))) {
        return false; //quitting
    } else {
        return true;
    }
}

bool InputModule::keyPressed(const KeyEvent& event)
{
    if (menuMode) {
        Rocket::Core::Input::KeyIdentifier key = keys[event.key];

        // toggling the debugger on a shift-~ press.
        if (key == Rocket::Core::Input::KI_OEM_3 &&
                (getModifier() & Rocket::Core::Input::KM_SHIFT)) {
            return true;
        }

        if (key != Rocket::Core::Input::KI_UNKNOWN) {
            getContext()->ProcessKeyDown(key, getModifier());
        }

        // sending through the ASCII value as text input if it is printable.
        if (event.text >= 32)
            getContext()->ProcessTextInput((Rocket::Core::word) event.text);
        else if (key == Rocket::Core::Input::KI_RETURN)
            getContext()->ProcessTextInput((Rocket::Core::word) '\n');
    } else {
        switch (event.key)
        {
            case OIS::KC_F3:
            {
                if (ENGINE->getState() != Engine::STATE_DEBUGING) {
                    ENGINE->setState(Engine::STATE_DEBUGING);
                } else {
                    ENGINE->setState(Engine::STATE_RUNNING);
                }
                break;
            }
            case OIS::KC_F4:
            {
                if (ENGINE->getCamera()->isAttached()) {
                    ENGINE->getCamera()->detach();
                } else {
                    ENGINE->getCamera()->reattach();
                }
                break;
            }
            case OIS::KC_F10:
            {
                Shader::setShaderLevel(Shader::LEVEL_BASIC);
                ENGINE->getGraphic()->refreshAllShaders();
                break;
            }
            case OIS::KC_F11:
            {
                Shader::setShaderLevel(Shader::LEVEL_INTERMEDIATE);
                ENGINE->getGraphic()->refreshAllShaders();
                break;
            }
            case OIS::KC_F12:
            {
                Shader::setShaderLevel(Shader::LEVEL_ADVANCED);
                ENGINE->getGraphic()->refreshAllShaders();
                break;
            }
            case OIS::KC_ESCAPE:
            {
                if (ENGINE->getGUI()->isVisible()) {
                    ENGINE->setState(ENGINE->STATE_RUNNING);
                    ENGINE->getGUI()->hideMainMenu();
                } else {
                    ENGINE->setState(ENGINE->STATE_STOPPED);
                    ENGINE->getGUI()->showMainMenu();
                }
            }
        }
    }
    return true;
}

bool InputModule::keyReleased(const KeyEvent& arg)
{
    switch (arg.key)
    {
        case OIS::KC_UP:
        {
            moveSphere(Ogre::Vector3(0, -1, 0));
            break;
        }
        case OIS::KC_DOWN:
        {
            moveSphere(Ogre::Vector3(0, -1, 0));
            break;
        }
        case OIS::KC_LEFT:
        {
            moveSphere(Ogre::Vector3(0, -1, 0));
            break;
        }
        case OIS::KC_RIGHT:
        {
            moveSphere(Ogre::Vector3(0, -1, 0));
            break;
        }
    }
    return true;
}

bool InputModule::mouseMoved(const MouseEvent& event)
{
    int key_modifier_state = getModifier();
    getContext()->ProcessMouseMove(event.state.X.abs, event.state.Y.abs, key_modifier_state);
    if (event.state.Z.rel != 0) {
        getContext()->ProcessMouseWheel(event.state.Z.rel / -120, key_modifier_state);
    }
    return true;
}

bool InputModule::mousePressed(const MouseEvent& arg, MouseButtonID id)
{
    getContext()->ProcessMouseButtonDown((int) id, getModifier());
    return true;
}

bool InputModule::mouseReleased(const MouseEvent& arg, MouseButtonID id)
{
    getContext()->ProcessMouseButtonUp((int) id, getModifier());
    return true;
}

void InputModule::windowResized(Ogre::RenderWindow *rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

void InputModule::windowClosed(Ogre::RenderWindow *rw)
{
    if (rw == ENGINE->getWindow()) {
        if (input) {
            input->destroyInputObject(mouse);
            input->destroyInputObject(key);

            OIS::InputManager::destroyInputSystem(input);
            input = 0;
        }
    }
}

BaseComponent* InputModule::getComponent(Entity* parent)
{
    InputComponent* component = new InputComponent();
    component->setParent(parent);
    components.push_back(component);
    return components.back();
}

void InputModule::moveSphere(Ogre::Vector3 direction)
{
    PhysicsComponent* component = dynamic_cast<PhysicsComponent*> (ENGINE->getPhysics()
            ->getComponent(ENGINE->getEntity("playerSphere")));
    component->setComponentGravity((direction) * 10);
}

void InputModule::buildKeyMap()
{
    keys[OIS::KC_UNASSIGNED] = Rocket::Core::Input::KI_UNKNOWN;
    keys[OIS::KC_ESCAPE] = Rocket::Core::Input::KI_ESCAPE;
    keys[OIS::KC_1] = Rocket::Core::Input::KI_1;
    keys[OIS::KC_2] = Rocket::Core::Input::KI_2;
    keys[OIS::KC_3] = Rocket::Core::Input::KI_3;
    keys[OIS::KC_4] = Rocket::Core::Input::KI_4;
    keys[OIS::KC_5] = Rocket::Core::Input::KI_5;
    keys[OIS::KC_6] = Rocket::Core::Input::KI_6;
    keys[OIS::KC_7] = Rocket::Core::Input::KI_7;
    keys[OIS::KC_8] = Rocket::Core::Input::KI_8;
    keys[OIS::KC_9] = Rocket::Core::Input::KI_9;
    keys[OIS::KC_0] = Rocket::Core::Input::KI_0;
    keys[OIS::KC_MINUS] = Rocket::Core::Input::KI_OEM_MINUS;
    keys[OIS::KC_EQUALS] = Rocket::Core::Input::KI_OEM_PLUS;
    keys[OIS::KC_BACK] = Rocket::Core::Input::KI_BACK;
    keys[OIS::KC_TAB] = Rocket::Core::Input::KI_TAB;
    keys[OIS::KC_Q] = Rocket::Core::Input::KI_Q;
    keys[OIS::KC_W] = Rocket::Core::Input::KI_W;
    keys[OIS::KC_E] = Rocket::Core::Input::KI_E;
    keys[OIS::KC_R] = Rocket::Core::Input::KI_R;
    keys[OIS::KC_T] = Rocket::Core::Input::KI_T;
    keys[OIS::KC_Y] = Rocket::Core::Input::KI_Y;
    keys[OIS::KC_U] = Rocket::Core::Input::KI_U;
    keys[OIS::KC_I] = Rocket::Core::Input::KI_I;
    keys[OIS::KC_O] = Rocket::Core::Input::KI_O;
    keys[OIS::KC_P] = Rocket::Core::Input::KI_P;
    keys[OIS::KC_LBRACKET] = Rocket::Core::Input::KI_OEM_4;
    keys[OIS::KC_RBRACKET] = Rocket::Core::Input::KI_OEM_6;
    keys[OIS::KC_RETURN] = Rocket::Core::Input::KI_RETURN;
    keys[OIS::KC_LCONTROL] = Rocket::Core::Input::KI_LCONTROL;
    keys[OIS::KC_A] = Rocket::Core::Input::KI_A;
    keys[OIS::KC_S] = Rocket::Core::Input::KI_S;
    keys[OIS::KC_D] = Rocket::Core::Input::KI_D;
    keys[OIS::KC_F] = Rocket::Core::Input::KI_F;
    keys[OIS::KC_G] = Rocket::Core::Input::KI_G;
    keys[OIS::KC_H] = Rocket::Core::Input::KI_H;
    keys[OIS::KC_J] = Rocket::Core::Input::KI_J;
    keys[OIS::KC_K] = Rocket::Core::Input::KI_K;
    keys[OIS::KC_L] = Rocket::Core::Input::KI_L;
    keys[OIS::KC_SEMICOLON] = Rocket::Core::Input::KI_OEM_1;
    keys[OIS::KC_APOSTROPHE] = Rocket::Core::Input::KI_OEM_7;
    keys[OIS::KC_GRAVE] = Rocket::Core::Input::KI_OEM_3;
    keys[OIS::KC_LSHIFT] = Rocket::Core::Input::KI_LSHIFT;
    keys[OIS::KC_BACKSLASH] = Rocket::Core::Input::KI_OEM_5;
    keys[OIS::KC_Z] = Rocket::Core::Input::KI_Z;
    keys[OIS::KC_X] = Rocket::Core::Input::KI_X;
    keys[OIS::KC_C] = Rocket::Core::Input::KI_C;
    keys[OIS::KC_V] = Rocket::Core::Input::KI_V;
    keys[OIS::KC_B] = Rocket::Core::Input::KI_B;
    keys[OIS::KC_N] = Rocket::Core::Input::KI_N;
    keys[OIS::KC_M] = Rocket::Core::Input::KI_M;
    keys[OIS::KC_COMMA] = Rocket::Core::Input::KI_OEM_COMMA;
    keys[OIS::KC_PERIOD] = Rocket::Core::Input::KI_OEM_PERIOD;
    keys[OIS::KC_SLASH] = Rocket::Core::Input::KI_OEM_2;
    keys[OIS::KC_RSHIFT] = Rocket::Core::Input::KI_RSHIFT;
    keys[OIS::KC_MULTIPLY] = Rocket::Core::Input::KI_MULTIPLY;
    keys[OIS::KC_LMENU] = Rocket::Core::Input::KI_LMENU;
    keys[OIS::KC_SPACE] = Rocket::Core::Input::KI_SPACE;
    keys[OIS::KC_CAPITAL] = Rocket::Core::Input::KI_CAPITAL;
    keys[OIS::KC_F1] = Rocket::Core::Input::KI_F1;
    keys[OIS::KC_F2] = Rocket::Core::Input::KI_F2;
    keys[OIS::KC_F3] = Rocket::Core::Input::KI_F3;
    keys[OIS::KC_F4] = Rocket::Core::Input::KI_F4;
    keys[OIS::KC_F5] = Rocket::Core::Input::KI_F5;
    keys[OIS::KC_F6] = Rocket::Core::Input::KI_F6;
    keys[OIS::KC_F7] = Rocket::Core::Input::KI_F7;
    keys[OIS::KC_F8] = Rocket::Core::Input::KI_F8;
    keys[OIS::KC_F9] = Rocket::Core::Input::KI_F9;
    keys[OIS::KC_F10] = Rocket::Core::Input::KI_F10;
    keys[OIS::KC_NUMLOCK] = Rocket::Core::Input::KI_NUMLOCK;
    keys[OIS::KC_SCROLL] = Rocket::Core::Input::KI_SCROLL;
    keys[OIS::KC_NUMPAD7] = Rocket::Core::Input::KI_7;
    keys[OIS::KC_NUMPAD8] = Rocket::Core::Input::KI_8;
    keys[OIS::KC_NUMPAD9] = Rocket::Core::Input::KI_9;
    keys[OIS::KC_SUBTRACT] = Rocket::Core::Input::KI_SUBTRACT;
    keys[OIS::KC_NUMPAD4] = Rocket::Core::Input::KI_4;
    keys[OIS::KC_NUMPAD5] = Rocket::Core::Input::KI_5;
    keys[OIS::KC_NUMPAD6] = Rocket::Core::Input::KI_6;
    keys[OIS::KC_ADD] = Rocket::Core::Input::KI_ADD;
    keys[OIS::KC_NUMPAD1] = Rocket::Core::Input::KI_1;
    keys[OIS::KC_NUMPAD2] = Rocket::Core::Input::KI_2;
    keys[OIS::KC_NUMPAD3] = Rocket::Core::Input::KI_3;
    keys[OIS::KC_NUMPAD0] = Rocket::Core::Input::KI_0;
    keys[OIS::KC_DECIMAL] = Rocket::Core::Input::KI_DECIMAL;
    keys[OIS::KC_OEM_102] = Rocket::Core::Input::KI_OEM_102;
    keys[OIS::KC_F11] = Rocket::Core::Input::KI_F11;
    keys[OIS::KC_F12] = Rocket::Core::Input::KI_F12;
    keys[OIS::KC_F13] = Rocket::Core::Input::KI_F13;
    keys[OIS::KC_F14] = Rocket::Core::Input::KI_F14;
    keys[OIS::KC_F15] = Rocket::Core::Input::KI_F15;
    keys[OIS::KC_KANA] = Rocket::Core::Input::KI_KANA;
    keys[OIS::KC_ABNT_C1] = Rocket::Core::Input::KI_UNKNOWN;
    keys[OIS::KC_CONVERT] = Rocket::Core::Input::KI_CONVERT;
    keys[OIS::KC_NOCONVERT] = Rocket::Core::Input::KI_NONCONVERT;
    keys[OIS::KC_YEN] = Rocket::Core::Input::KI_UNKNOWN;
    keys[OIS::KC_ABNT_C2] = Rocket::Core::Input::KI_UNKNOWN;
    keys[OIS::KC_NUMPADEQUALS] = Rocket::Core::Input::KI_OEM_NEC_EQUAL;
    keys[OIS::KC_PREVTRACK] = Rocket::Core::Input::KI_MEDIA_PREV_TRACK;
    keys[OIS::KC_AT] = Rocket::Core::Input::KI_UNKNOWN;
    keys[OIS::KC_COLON] = Rocket::Core::Input::KI_OEM_1;
    keys[OIS::KC_UNDERLINE] = Rocket::Core::Input::KI_OEM_MINUS;
    keys[OIS::KC_KANJI] = Rocket::Core::Input::KI_KANJI;
    keys[OIS::KC_STOP] = Rocket::Core::Input::KI_UNKNOWN;
    keys[OIS::KC_AX] = Rocket::Core::Input::KI_OEM_AX;
    keys[OIS::KC_UNLABELED] = Rocket::Core::Input::KI_UNKNOWN;
    keys[OIS::KC_NEXTTRACK] = Rocket::Core::Input::KI_MEDIA_NEXT_TRACK;
    keys[OIS::KC_NUMPADENTER] = Rocket::Core::Input::KI_NUMPADENTER;
    keys[OIS::KC_RCONTROL] = Rocket::Core::Input::KI_RCONTROL;
    keys[OIS::KC_MUTE] = Rocket::Core::Input::KI_VOLUME_MUTE;
    keys[OIS::KC_CALCULATOR] = Rocket::Core::Input::KI_UNKNOWN;
    keys[OIS::KC_PLAYPAUSE] = Rocket::Core::Input::KI_MEDIA_PLAY_PAUSE;
    keys[OIS::KC_MEDIASTOP] = Rocket::Core::Input::KI_MEDIA_STOP;
    keys[OIS::KC_VOLUMEDOWN] = Rocket::Core::Input::KI_VOLUME_DOWN;
    keys[OIS::KC_VOLUMEUP] = Rocket::Core::Input::KI_VOLUME_UP;
    keys[OIS::KC_WEBHOME] = Rocket::Core::Input::KI_BROWSER_HOME;
    keys[OIS::KC_NUMPADCOMMA] = Rocket::Core::Input::KI_SEPARATOR;
    keys[OIS::KC_DIVIDE] = Rocket::Core::Input::KI_DIVIDE;
    keys[OIS::KC_SYSRQ] = Rocket::Core::Input::KI_SNAPSHOT;
    keys[OIS::KC_RMENU] = Rocket::Core::Input::KI_RMENU;
    keys[OIS::KC_PAUSE] = Rocket::Core::Input::KI_PAUSE;
    keys[OIS::KC_HOME] = Rocket::Core::Input::KI_HOME;
    keys[OIS::KC_UP] = Rocket::Core::Input::KI_UP;
    keys[OIS::KC_PGUP] = Rocket::Core::Input::KI_PRIOR;
    keys[OIS::KC_LEFT] = Rocket::Core::Input::KI_LEFT;
    keys[OIS::KC_RIGHT] = Rocket::Core::Input::KI_RIGHT;
    keys[OIS::KC_END] = Rocket::Core::Input::KI_END;
    keys[OIS::KC_DOWN] = Rocket::Core::Input::KI_DOWN;
    keys[OIS::KC_PGDOWN] = Rocket::Core::Input::KI_NEXT;
    keys[OIS::KC_INSERT] = Rocket::Core::Input::KI_INSERT;
    keys[OIS::KC_DELETE] = Rocket::Core::Input::KI_DELETE;
    keys[OIS::KC_LWIN] = Rocket::Core::Input::KI_LWIN;
    keys[OIS::KC_RWIN] = Rocket::Core::Input::KI_RWIN;
    keys[OIS::KC_APPS] = Rocket::Core::Input::KI_APPS;
    keys[OIS::KC_POWER] = Rocket::Core::Input::KI_POWER;
    keys[OIS::KC_SLEEP] = Rocket::Core::Input::KI_SLEEP;
    keys[OIS::KC_WAKE] = Rocket::Core::Input::KI_WAKE;
    keys[OIS::KC_WEBSEARCH] = Rocket::Core::Input::KI_BROWSER_SEARCH;
    keys[OIS::KC_WEBFAVORITES] = Rocket::Core::Input::KI_BROWSER_FAVORITES;
    keys[OIS::KC_WEBREFRESH] = Rocket::Core::Input::KI_BROWSER_REFRESH;
    keys[OIS::KC_WEBSTOP] = Rocket::Core::Input::KI_BROWSER_STOP;
    keys[OIS::KC_WEBFORWARD] = Rocket::Core::Input::KI_BROWSER_FORWARD;
    keys[OIS::KC_WEBBACK] = Rocket::Core::Input::KI_BROWSER_BACK;
    keys[OIS::KC_MYCOMPUTER] = Rocket::Core::Input::KI_UNKNOWN;
    keys[OIS::KC_MAIL] = Rocket::Core::Input::KI_LAUNCH_MAIL;
    keys[OIS::KC_MEDIASELECT] = Rocket::Core::Input::KI_LAUNCH_MEDIA_SELECT;
}

int InputModule::getModifier()
{
    int modifier = 0;

    if (key->isModifierDown(OIS::Keyboard::Ctrl)) {
        modifier |= Rocket::Core::Input::KM_CTRL;
    }
    if (key->isModifierDown(OIS::Keyboard::Shift)) {
        modifier |= Rocket::Core::Input::KM_SHIFT;
    }
    if (key->isModifierDown(OIS::Keyboard::Alt)) {
        modifier |= Rocket::Core::Input::KM_ALT;
    }
    return modifier;
}
