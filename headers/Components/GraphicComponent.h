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
 * @file GraphicComponent.h
 * @author Eduardo Hahn Paredes
 * @brief Contains the GraphicComponent.
 */

#ifndef GRAPHICCOMPONENT_H
#define	GRAPHICCOMPONENT_H

#include "Components/BaseComponent.h"
#include "Shaders/Shader.h"

namespace CotopaxiEngine
{

    /**
     * @class GraphicComponent
     * @brief This component is responisble for the graphic representation of 
     * an Entity.
     * 
     * The GraphicComponent handles everything a game Entity needs for its 
     * visual representation. Over this component it's possible to set a 
     * variety of visual stuff for an Entity, such as shaders, animations and 
     * meshes.
     */
    class GraphicComponent : public BaseComponent
    {
    public:
        GraphicComponent();
        virtual ~GraphicComponent();

        /**
         * @fn setParent
         * Attaches the GraphicComponent to a new parent. Also, it registers it 
         * for Shader events
         * @param parent The new parent for the GraphicComponent
         */
        void setParent(Entity* parent);

        /**
         * @fn setAnimation
         * Sets the current animation in the GraphicComponent
         * @param name The name of the animation
         * @param loop If the animation should loop
         */
        void setAnimation(std::string name, bool loop = false);

        /**
         * @fn setShader
         * Sets the current shader of the GraphicComponent
         * @param name Name of the Shader
         */
        void setShader(Shader* shader);

        /**
         * @fn reloadShader		
         * Reloads the current shader
         */
        void reloadShader();

        /**
         * @fn update
         * Updates the animation of the GraphicComponent
         * @param time How much time has passed since the last update
         */
        void update(Ogre::Real time);

        /**
         * @fn getShader
         * Returns the current shader
         * @return The current shader
         */
        Shader* getShader() const {
            return shader;
        }

        /**
         * @fn isAnitmated
         * Tells if the component has an animation
         * @return true if it's animated
         */
        bool isAnimated() const {
            return animated;
        }

        /**
         * @fn getAnimation
         * Enables access to the current state of the animation
         * @return a pointer to the actual AnimationState
         */
        Ogre::AnimationState* getAnimation() const {
            return animation;
        }

        /**
         * @fn hasShader
         * Indicates if a Shader is used
         * @return true if there is an active shader program
         */
        bool hasShader() const {
            return shaderUsage;
        }

    private:
        bool animated;
        bool shaderUsage;
        Ogre::AnimationState* animation;
        Shader* shader;
    };
}

#endif