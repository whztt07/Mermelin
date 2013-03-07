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
 * @file GUIModule.h
 * @brief This class enables the use of Rocket.
 *
 * @author Peter Curry, Eduardo Hahn Paredes
 */
#ifndef GUIMODULE_H
#define GUIMODULE_H

#include "stdafx.h"
#include "Modules/BaseModule.h"

namespace CotopaxiEngine
{

    /**
     * @class GUIModule
     * @brief Any menu or HUD can be created with this module.
     * 
     * It makes use of the Rocket library. All the functions that begin with uppercase come
     * from that library. Also its implementation is taken from their samples.
     */
    class GUIModule : public BaseModule, Ogre::RenderQueueListener, Rocket::Core::RenderInterface,
    Rocket::Core::SystemInterface, Rocket::Core::FileInterface, Rocket::Core::EventListener
    {
    public:

        /** @brief A vertex loaded by Rocket for Ogre. */
        struct GUIVertex
        {
            float x, y, z;
            Ogre::uint32 diffuse;
            float u, v;
        };

        /** @brief A texture loaded by Rocket for Ogre. */
        struct GUITexture
        {

            GUITexture(Ogre::TexturePtr texture) : texture(texture) { }

            Ogre::TexturePtr texture;
        };

        /** @brief A set of geometry that Rocket compiles. */
        struct GUIGeometry
        {
            Ogre::RenderOperation render_operation;
            GUITexture* texture;
        };

        GUIModule();
        virtual ~GUIModule();

        /// Gets the number of seconds elapsed since the start of the application.
        virtual float GetElapsedTime();

        /// Logs the specified message.
        virtual bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message);

        /// Called by Rocket when it compiles geometry it believes will be static soon.
        virtual Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex* vertices,
                int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture);

        /// Called by Rocket when it wants to render application-compiled geometry.
        virtual void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry,
                const Rocket::Core::Vector2f& translation);

        /// Called by Rocket when it wants to release application-compiled geometry.
        virtual void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry);

        /// Called by Rocket when it wants to enable or disable scissoring to clip content.
        virtual void EnableScissorRegion(bool enable);

        /// Called by Rocket when it wants to change the scissor region.
        virtual void SetScissorRegion(int x, int y, int width, int height);

        /// Called by Rocket when a texture is required by the library.
        virtual bool LoadTexture(Rocket::Core::TextureHandle& texture_handle,
                Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source);

        /// Called by Rocket when a texture has to be built from an internal sequence of pixels.
        virtual bool GenerateTexture(Rocket::Core::TextureHandle& texture_handle,
                const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions);

        /// Called by Rocket when a loaded texture is no longer required.
        virtual void ReleaseTexture(Rocket::Core::TextureHandle texture);

        /// Returns the native horizontal texel offset for the renderer.
        float GetHorizontalTexelOffset();

        /// Returns the native vertical texel offset for the renderer.
        float GetVerticalTexelOffset();

        // Configures Ogre's rendering system for rendering Rocket.
        void ConfigureRenderSystem();

        // Builds an OpenGL-style orthographic projection matrix.
        void BuildProjectionMatrix(Ogre::Matrix4& matrix);

        /// Opens a file.
        virtual Rocket::Core::FileHandle Open(const Rocket::Core::String& path);

        /// Closes a previously opened file.
        virtual void Close(Rocket::Core::FileHandle file);

        /// Reads data from a previously opened file.
        virtual size_t Read(void* buffer, size_t size, Rocket::Core::FileHandle file);

        /// Seeks to a point in a previously opened file.
        virtual bool Seek(Rocket::Core::FileHandle file, long offset, int origin);

        /// Returns the current position of the file pointer.
        virtual size_t Tell(Rocket::Core::FileHandle file);

        /// Processes an Event
        virtual void ProcessEvent(Rocket::Core::Event& e);
        
        /// Not implemented
        void RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices,
                int num_indices, Rocket::Core::TextureHandle texture,
                const Rocket::Core::Vector2f& translation) {}

        /** Event raised before a queue group is rendered */
        virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation,
                bool& skipThisInvocation);

        void load();
        void unload();
        void hideMainMenu();
        void showMainMenu();

        BaseComponent* getComponent(Entity* entity);

        void setContext(Rocket::Core::Context* context) {
            this->context = context;
        }

        Rocket::Core::Context* getContext() const {
            return context;
        }

        bool frameRenderingQueued(const Ogre::FrameEvent& e);

        bool isVisible() const {
            return visible;
        }

    private:
        Ogre::Timer timer;
        Ogre::RenderSystem* renderSystem;

        Ogre::LayerBlendModeEx colourBlend;
        Ogre::LayerBlendModeEx alphaBlend;

        Rocket::Core::Context* context;
        Rocket::Core::ElementDocument* document;
        Rocket::Core::ElementDocument* cursor;

        bool visible;
        bool running;
        bool scissoring;
        int scissorLeft;
        int scissorTop;
        int scissorRight;
        int scissorBottom;

        std::string loadDocument(const std::string& documentName);
    };
}

#endif