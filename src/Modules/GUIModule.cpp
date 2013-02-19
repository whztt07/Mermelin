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
 * GUIModule.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Engine.h"
#include "Modules/GUIModule.h"
#include "Entities/Camera.h"

using namespace CotopaxiEngine;

GUIModule::GUIModule()
:running(true){}

GUIModule::~GUIModule() {
    delete cursor;
    delete document;
    delete context;
}

void GUIModule::load()
{    
    int width = (int) ENGINE->getWindow()->getWidth();
    int height = (int) ENGINE->getWindow()->getHeight();

    // Rocket initialisation.
    renderSystem = Ogre::Root::getSingleton().getRenderSystem();

    // Configure the colour blending mode.
    colourBlend.blendType = Ogre::LBT_COLOUR;
    colourBlend.source1 = Ogre::LBS_DIFFUSE;
    colourBlend.source2 = Ogre::LBS_TEXTURE;
    colourBlend.operation = Ogre::LBX_MODULATE;

    // Configure the alpha blending mode.
    alphaBlend.blendType = Ogre::LBT_ALPHA;
    alphaBlend.source1 = Ogre::LBS_DIFFUSE;
    alphaBlend.source2 = Ogre::LBS_TEXTURE;
    alphaBlend.operation = Ogre::LBX_MODULATE;

    scissoring = false;

    scissorLeft = 0;
    scissorTop = 0;
    scissorRight = width;
    scissorBottom = height;

    if (!Ogre::ResourceGroupManager::getSingleton().resourceGroupExists("Rocket"))
        Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Rocket");

    Rocket::Core::SetRenderInterface(this);
    Rocket::Core::SetSystemInterface(this);
    Rocket::Core::SetFileInterface(this);

    Rocket::Core::Initialise();
    Rocket::Controls::Initialise();

    bool success = false;

    success = Rocket::Core::FontDatabase::LoadFontFace("BouwsUnc.ttf");

    context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(width, height));
    // Load the mouse cursor and release the caller's reference.

    cursor = context->LoadDocumentFromMemory(loadDocument("cursor.rml").c_str());
    if (cursor) {
        cursor->RemoveReference();
    }

    context->AddMouseCursor(cursor);

    document = context->LoadDocumentFromMemory(loadDocument("menu.rml").c_str());
    if (document) {
        document->GetElementById("start")->AddEventListener("click", this);
        document->GetElementById("credits")->AddEventListener("click", this);
        document->GetElementById("quit")->AddEventListener("click", this);
    }
    showMainMenu();

    ENGINE->getSceneManager()->addRenderQueueListener(this);
}

std::string GUIModule::loadDocument(const std::string& documentName)
{
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton()
            .openResource(documentName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    std::string line;
    line = "";

    while (!stream->eof()) {
        line += stream->getLine();
    }
    stream->close();

    return line;
}

float GUIModule::GetElapsedTime()
{
    return timer.getMilliseconds() * 0.001f;
}

bool GUIModule::LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message)
{
    Ogre::LogMessageLevel level;
    switch (type) {
        case Rocket::Core::Log::LT_ALWAYS:
        case Rocket::Core::Log::LT_ERROR:
        case Rocket::Core::Log::LT_ASSERT:
        case Rocket::Core::Log::LT_WARNING:
        {
            level = Ogre::LML_CRITICAL;
            break;
        }
        default:
        {
            level = Ogre::LML_NORMAL;
            break;
        }
    }

    Ogre::LogManager::getSingleton().logMessage(message.CString(), level);
    return false;
}

Rocket::Core::CompiledGeometryHandle GUIModule::CompileGeometry(Rocket::Core::Vertex* vertices, 
        int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture)
{
    GUIGeometry* geometry = new GUIGeometry();
    geometry->texture = (GUITexture*) texture;

    geometry->render_operation.vertexData = new Ogre::VertexData();
    geometry->render_operation.vertexData->vertexStart = 0;
    geometry->render_operation.vertexData->vertexCount = num_vertices;

    geometry->render_operation.indexData = new Ogre::IndexData();
    geometry->render_operation.indexData->indexStart = 0;
    geometry->render_operation.indexData->indexCount = num_indices;
    geometry->render_operation.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

    // setting up the vertex declaration.
    Ogre::VertexDeclaration* vertex_declaration = geometry->render_operation.vertexData->vertexDeclaration;
    size_t element_offset = 0;
    vertex_declaration->addElement(0, element_offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    element_offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
    vertex_declaration->addElement(0, element_offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
    element_offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);
    vertex_declaration->addElement(0, element_offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);

    // creating the vertex buffer.
    Ogre::HardwareVertexBufferSharedPtr vertex_buffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(vertex_declaration->getVertexSize(0), num_vertices, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
    geometry->render_operation.vertexData->vertexBufferBinding->setBinding(0, vertex_buffer);

    // filling the vertex buffer.
    GUIVertex* ogre_vertices = (GUIVertex*) vertex_buffer->lock(0, vertex_buffer->getSizeInBytes(), Ogre::HardwareBuffer::HBL_NORMAL);
    for (int i = 0; i < num_vertices; ++i) {
        ogre_vertices[i].x = vertices[i].position.x;
        ogre_vertices[i].y = vertices[i].position.y;
        ogre_vertices[i].z = 0;

        Ogre::ColourValue diffuse(vertices[i].colour.red / 255.0f, vertices[i].colour.green / 255.0f, vertices[i].colour.blue / 255.0f, vertices[i].colour.alpha / 255.0f);
        renderSystem->convertColourValue(diffuse, &ogre_vertices[i].diffuse);

        ogre_vertices[i].u = vertices[i].tex_coord[0];
        ogre_vertices[i].v = vertices[i].tex_coord[1];
    }
    
    vertex_buffer->unlock();


    // creating the index buffer.
    Ogre::HardwareIndexBufferSharedPtr index_buffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_32BIT, num_indices, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
    geometry->render_operation.indexData->indexBuffer = index_buffer;
    geometry->render_operation.useIndexes = true;

    // filling the index buffer.
    void* ogre_indices = index_buffer->lock(0, index_buffer->getSizeInBytes(), Ogre::HardwareBuffer::HBL_NORMAL);
    memcpy(ogre_indices, indices, sizeof (unsigned int) * num_indices);
    index_buffer->unlock();

    return reinterpret_cast<Rocket::Core::CompiledGeometryHandle> (geometry);
}

void GUIModule::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation)
{
    Ogre::Matrix4 transform;
    transform.makeTrans(translation.x, translation.y, 0);
    renderSystem->_setWorldMatrix(transform);

    renderSystem = Ogre::Root::getSingleton().getRenderSystem();
    GUIGeometry* ogre3d_geometry = (GUIGeometry*) geometry;

    if (ogre3d_geometry->texture != NULL) {
        renderSystem->_setTexture(0, true, ogre3d_geometry->texture->texture);

        // Ogre can change the blending modes when textures are disabled - so in case the last 
        // render had no texture, re-specifying them.
        renderSystem->_setTextureBlendMode(0, colourBlend);
        renderSystem->_setTextureBlendMode(0, alphaBlend);
    } else
        renderSystem->_disableTextureUnit(0);

    renderSystem->_render(ogre3d_geometry->render_operation);
}

void GUIModule::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry)
{
    GUIGeometry* ogre3d_geometry = reinterpret_cast<GUIGeometry*> (geometry);
    delete ogre3d_geometry->render_operation.vertexData;
    delete ogre3d_geometry->render_operation.indexData;
    delete ogre3d_geometry;
}

void GUIModule::EnableScissorRegion(bool enable)
{
    scissoring = enable;
    if (!scissoring)
        renderSystem->setScissorTest(false);
    else
        renderSystem->setScissorTest(true, scissorLeft, scissorTop, scissorRight, scissorBottom);
}

void GUIModule::SetScissorRegion(int x, int y, int width, int height)
{
    scissorLeft = x;
    scissorTop = y;
    scissorRight = x + width;
    scissorBottom = y + height;

    if (scissoring) {
        renderSystem->setScissorTest(true, scissorLeft, scissorTop, scissorRight, scissorBottom);
    }
}

bool GUIModule::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
{
    Ogre::TextureManager* texture_manager = Ogre::TextureManager::getSingletonPtr();
    Ogre::TexturePtr ogre_texture = texture_manager->getByName(Ogre::String(source.CString()));
    if (ogre_texture.isNull()) {
        ogre_texture = texture_manager->load(Ogre::String(source.CString()),
                "Rocket",
                Ogre::TEX_TYPE_2D,
                0);
    }

    if (ogre_texture.isNull()) {
        return false;
    }

    texture_dimensions.x = ogre_texture->getWidth();
    texture_dimensions.y = ogre_texture->getHeight();

    texture_handle = reinterpret_cast<Rocket::Core::TextureHandle> (new GUITexture(ogre_texture));
    return true;
}

bool GUIModule::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, 
        const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
    static int texture_id = 1;

    Ogre::DataStreamPtr stream(OGRE_NEW Ogre::MemoryDataStream((void*) source, source_dimensions.x * source_dimensions.y * sizeof (unsigned int)));
    Ogre::TexturePtr ogre_texture = Ogre::TextureManager::getSingleton().loadRawData(
            Rocket::Core::String(16, "%d", texture_id++).CString(),
            "Rocket",
            stream,
            (Ogre::ushort)source_dimensions.x,
            (Ogre::ushort)source_dimensions.y,
            Ogre::PF_A8B8G8R8,
            Ogre::TEX_TYPE_2D,
            0);

    if (ogre_texture.isNull()) {
        return false;
    }

    texture_handle = reinterpret_cast<Rocket::Core::TextureHandle> (new GUITexture(ogre_texture));
    return true;
}

void GUIModule::ReleaseTexture(Rocket::Core::TextureHandle texture)
{
    delete ((GUITexture*) texture);
}

float GUIModule::GetHorizontalTexelOffset()
{
    return -renderSystem->getHorizontalTexelOffset();
}

float GUIModule::GetVerticalTexelOffset()
{
    return -renderSystem->getVerticalTexelOffset();
}

BaseComponent* GUIModule::getComponent(Entity* parent)
{
    return NULL;
}

void GUIModule::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, 
        bool& ROCKET_UNUSED(skipThisInvocation))
{
    if (queueGroupId == Ogre::RENDER_QUEUE_OVERLAY && Ogre::Root::getSingleton()
            .getRenderSystem()->_getViewport()->getOverlaysEnabled()) {
        context->Update();

        ConfigureRenderSystem();
        context->Render();
    }
}

void GUIModule::ConfigureRenderSystem()
{
    Ogre::RenderSystem* render_system = Ogre::Root::getSingleton().getRenderSystem();

    // Set up the projection and view matrices.
    Ogre::Matrix4 projection_matrix;
    BuildProjectionMatrix(projection_matrix);
    render_system->_setProjectionMatrix(projection_matrix);
    render_system->_setViewMatrix(Ogre::Matrix4::IDENTITY);

    // Disable lighting, as all of Rocket's geometry is unlit.
    render_system->setLightingEnabled(false);
    // Disable depth-buffering; all of the geometry is already depth-sorted.
    render_system->_setDepthBufferParams(false, false);
    // Rocket generates anti-clockwise geometry, so enable clockwise-culling.
    render_system->_setCullingMode(Ogre::CULL_CLOCKWISE);
    // Disable fogging.
    render_system->_setFog(Ogre::FOG_NONE);
    // Enable writing to all four channels.
    render_system->_setColourBufferWriteEnabled(true, true, true, true);
    // Unbind any vertex or fragment programs bound previously by the application.
    render_system->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
    render_system->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);

    // Set texture settings to clamp along both axes.
    Ogre::TextureUnitState::UVWAddressingMode addressing_mode;
    addressing_mode.u = Ogre::TextureUnitState::TAM_CLAMP;
    addressing_mode.v = Ogre::TextureUnitState::TAM_CLAMP;
    addressing_mode.w = Ogre::TextureUnitState::TAM_CLAMP;
    render_system->_setTextureAddressingMode(0, addressing_mode);

    // Set the texture coordinates for unit 0 to be read from unit 0.
    render_system->_setTextureCoordSet(0, 0);
    // Disable texture coordinate calculation.
    render_system->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
    // Enable linear filtering; images should be rendering 1 texel == 1 pixel, so point filtering could be used
    // except in the case of scaling tiled decorators.
    render_system->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);
    // Disable texture coordinate transforms.
    render_system->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
    // Reject pixels with an alpha of 0.
    render_system->_setAlphaRejectSettings(Ogre::CMPF_GREATER, 0, false);
    // Disable all texture units but the first.
    render_system->_disableTextureUnitsFrom(1);

    // Enable simple alpha blending.
    render_system->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    // Disable depth bias.
    render_system->_setDepthBias(0, 0);
}

// Builds an OpenGL-style orthographic projection matrix.

void GUIModule::BuildProjectionMatrix(Ogre::Matrix4& projection_matrix)
{
    float z_near = -1;
    float z_far = 1;

    projection_matrix = Ogre::Matrix4::ZERO;



    // Set up matrices.
    projection_matrix[0][0] = 2.0f / ENGINE->getWindow()->getWidth();
    projection_matrix[0][3] = -1.0000000f;
    projection_matrix[1][1] = -2.0f / ENGINE->getWindow()->getHeight();
    projection_matrix[1][3] = 1.0000000f;
    projection_matrix[2][2] = -2.0f / (z_far - z_near);
    projection_matrix[3][3] = 1.0000000f;
}

bool GUIModule::frameRenderingQueued(const Ogre::FrameEvent& e)
{
    return running;
}

Rocket::Core::FileHandle GUIModule::Open(const Rocket::Core::String& path)
{
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(path.CString());
    if (stream.isNull())
        return 0;

    return (Rocket::Core::FileHandle)(new Ogre::DataStreamPtr(stream));
}


void GUIModule::Close(Rocket::Core::FileHandle file)
{
    if (!file) return;

    Ogre::DataStreamPtr* pstream = (Ogre::DataStreamPtr*)(file);
    delete pstream;
}

size_t GUIModule::Read(void* buffer, size_t size, Rocket::Core::FileHandle file)
{
    if (!file) return 0;

    Ogre::DataStreamPtr stream = *(Ogre::DataStreamPtr*)(file);
    return stream->read(buffer, size);
}

bool GUIModule::Seek(Rocket::Core::FileHandle file, long offset, int origin)
{
    if (!file) return false;

    Ogre::DataStreamPtr stream = *(Ogre::DataStreamPtr*)(file);
    long pos = 0;
    size_t size = stream->size();
    if (origin == SEEK_CUR)
        pos = stream->tell() + offset;
    else if (origin == SEEK_END)
        pos = size + offset;
    else
        pos = offset;

    if (pos < 0 || pos > (long) size)
        return false;

    stream->seek((size_t) pos);
    return true;
}

size_t GUIModule::Tell(Rocket::Core::FileHandle file)
{
    if (!file) return 0;

    Ogre::DataStreamPtr stream = *(Ogre::DataStreamPtr*)(file);

    return stream->tell();
}

void GUIModule::ProcessEvent(Rocket::Core::Event& e)
{
    if (e.GetType() == "click") {
        if (e.GetCurrentElement()->GetId() == "start") {
            ENGINE->getGUI()->hideMainMenu();
            ENGINE->setState(ENGINE->STATE_RUNNING);
        } else if (e.GetCurrentElement()->GetId() == "credits") {

        } else if (e.GetCurrentElement()->GetId() == "quit") {
            running = false;
        }
    }
}

void GUIModule::hideMainMenu()
{
    if(document) {
        document->GetElementById("start")->SetInnerRML("return");
        document->Hide();
        document->AddReference();
        context->UnloadMouseCursor(cursor->GetId());
        cursor->Hide();
        visible = false;        
    }
}

void GUIModule::showMainMenu()
{
    if(document) {
        cursor->Show();
        context->AddMouseCursor(cursor);        
        document->Show();
        document->RemoveReference();
        visible = true;
    }
}