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
@file GraphicModule.h
@brief Contains the GraphicModule.

@author Eduardo Hahn Paredes
 */

#ifndef GRAPHICMODULE_H
#define GRAPHICMODULE_H
#include "Modules/BaseModule.h"
#include "Components/BaseComponent.h"
#include "Components/GraphicComponent.h"

using namespace std;
namespace CotopaxiEngine
{
    class Entity;
    class GraphicComponent;

    /**
     * @class GraphicModule
     * @brief Will handle updating animations and such things.
     */
    class GraphicModule : public BaseModule
    {
    public:
        virtual ~GraphicModule();
        virtual bool frameRenderingQueued(const Ogre::FrameEvent& event);
        BaseComponent* getComponent(Entity* parent);
        void refreshAllShaders();
        void receiveEvent(Event* e);
        void removeComponent(std::string component);
        void unload();
    private:
        std::map<std::string, GraphicComponent*> components;
    };
}
#endif