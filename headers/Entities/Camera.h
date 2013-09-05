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
 * @file Camera.h
 * @brief Contains the Camera Entity. An Ogre::Camera object is used.
 * 
 * @authors Eduardo Hahn Paredes
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "stdafx.h"
#include "Entity.h"
#include "Engine.h"

namespace CotopaxiEngine {

    /**
     * @class Camera
     * @brief Contains the Camera Entity. An Ogre::Camera object is used.
     */
    class Camera : public Entity
    {
    public:
        Camera(std::string name = "Camera", Ogre::SceneNode* 
                parentNode = ENGINE->getSceneManager()->getRootSceneNode());
        virtual ~Camera();
        
        /**
         * @fn attachTo
         * Attaches the Camera to an Entity. It will follow the object using a
         * specied offset.
         * @param object the Entity to which the camera will be attached.
         */
        void attachTo(Entity* object);
        
        /**
         * @fn reattach
         * Attaches the camera to a previous attached object.
         */
        void reattach();
        
        /**
         * @fn detach
         * Detaches the Camera from the Entity. It doesn't follow the object
         * anymore (unitl it's reattached).
         */
        void detach();
        
        /**
         * @fn setOffset
         * Defines the distance between the object and the camera.
         * @param offset Ogre::Vector3 with x, y, z coordinates.
         */
        void setOffset(Ogre::Vector3 offset);
        
        /**
         * @fn update
         * Updates the position and view direction of the camera. This
         * function should be called as often as possible, when the Camera 
         * instance is attached to an object.
         */
        void update();

        virtual void setParentNode(Ogre::SceneNode* newParent);
        
        /**
         * @fn translate
         * Moves the Camera along the Cartesian axes.
         * @param translation Ogre::Vector with x, y, z values representing the 
         * translation.
         */
        void translate(const Ogre::Vector3& translation);
        
        /**
         * @fn rotate
         * Rotate the node around an arbitrary axis using a Quarternion.
         * @param rotation Quaternion defining the rotation.
         */
        void rotate(const Ogre::Quaternion& rotation);
        
        static Entity* create(std::string name, Ogre::SceneNode* parentNode)
        {
            return new Camera(name, parentNode);
        };
        
        /**
         * @fn getCamera
         * Makes the Ogre::Camera accesible from outside.
         * @return pointer to a Ogre::Camera instance.
         */
        Ogre::Camera* getCamera();

        /**
         * @fn isAttached
         * Checks attachment state of the Camera.
         * @return true if Camera is attached to an object.
         */
        bool isAttached() const;

    private:
        std::string name;
        bool attached;
        Ogre::Camera* camera;
        Ogre::Vector3 offset;
        Entity* object;
        
        void prepare();
    };
}
#endif