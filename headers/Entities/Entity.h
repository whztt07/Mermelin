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
 * @file Entity.h
 * @brief Contains the Entity object
 *
 * @author Eduardo Hahn Paredes
 */

#ifndef ENTITY_H
#define ENTITY_H

#include "Event.h"
#include "Modules/BaseModule.h"
#include "Components/BaseComponent.h"

namespace CotopaxiEngine
{

    // forward declaration
    class Entity;
    class GraphicComponent;

    enum Element
    {
        AIR,
        DEFAULT,
        EARTH,
        FIRE,
        WATER
    };

    typedef Entity* (*ProduceEntity)(std::string, Ogre::SceneNode*);

    /**
     * @class Entity
     * @brief Container class for handeling components that belong together.
     * 
     * The Entity is the main game object in the CotopaxiEngine. It can be 
     * thought of as a container for all components, providing the ability for 
     * them to communicate with each other.
     */
    class Entity: public EventListener
    {
        friend class Engine;
    public:

        Entity();
        Entity(std::string name, std::string meshName, Ogre::SceneNode* parentNode);
		virtual ~Entity();

        /**
         * @fn addComponent
         * Adds a Component to an Entity
         * @param component Pointer to a component that is to be added
         */
        void addComponent(BaseComponent* component);

        /**
         * @fn registerListener
         * Registers a listener to this entity for a specific type of event. 
         * This can be any component
         * @param type The event type the component want to listen to
         * @param listener The BaseComponent that wants to register itself
         */
        void registerListener(const Event::EventType& type, EventListener* listener);

        /**
         * @fn receiveEvent
         * Function to send an event to an Entity. 
         * @attention This method is called immediatly, no caching of events 
         * is done.
         * @param event An event for the entity to process
         */
        virtual void receiveEvent(Event* event);

        /**
         * @fn setParentNode
         * Changes the parent node of an Entity. This will automaticaly detach 
         * the Entity from the previous parent node and attach it to the new one.
         * @params newParent The new parent node for an Entity
         */
        virtual void setParentNode(Ogre::SceneNode* newParent);

        /**
         * @fn getNode
         * Returns the current node for an entity
         */
        Ogre::SceneNode* getNode() const;

        /**
         * @fn setOgreEntity
         * Sets a new Ogre::Entity to a entity
         * @param ogreEntity The new Ogre::Entity to add
         */
        void setOgreEntity(Ogre::Entity* ogreEntity);

        /**
         * @fn getOgreEntity
         * Returns the current Ogre::Entity for this entity. This exists only
         * for convenience.
         */
        Ogre::Entity* getOgreEntity() const;

        /**
         * @fn setGraphicComponent
         * Sets a graphic component
         * This exists only for convenience.
         */
        void setGraphicComponent(GraphicComponent* graphicComponent);
        
        /**
         * @fn getGraphicComponent
         * Returns the current GraphicComponent for an entity
         * @return Returns a GraphicComponent
         */
        GraphicComponent* getGraphicComponent() const;

        /**
         * @fn setName
         * Sets the name for the entity
         * @attention This only sets the name for the entity and not for the 
         * corresponding Ogre::Node. Ogre::Nodes can not be renamed
         * @param name The new name of the entity
         */
        void setName(std::string name);

        /**
         * @fn getName
         * @return Returns the name of the entity
         */
        std::string getName() const;

        /**
         * @fn create
         * Static method to create an entity. This is used by the function 
         * @see produceEntity to create new entities
         * @param name The name for the entity
         * @param parentNode A parent node
         * @return Returns a new entity
         */
        static Entity* create(std::string name, Ogre::SceneNode* parentNode) {
            return new Entity();
        }

        void setNode(Ogre::SceneNode* node) {
            this->node = node;
        }

		void removeComponent(BaseComponent* component);
    private:
        GraphicComponent* graphicComponent;
        Ogre::SceneNode* node;
        Ogre::Entity* ogreEntity;
        typedef std::vector<BaseComponent*> ComponentVector;
		typedef std::vector<EventListener*> EventListenerVector;
        ComponentVector components;
		std::map<Event::EventType, EventListenerVector> eventListeners;
        std::string name;
    };
}

#endif