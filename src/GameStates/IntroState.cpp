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
 * @file IntroState.cpp
 * @author Patrick Joos, Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Engine.h"
#include "GameStates/IntroState.h"
#include "GameStates/PlayState.h"

using namespace CotopaxiEngine;
using namespace Ogre;

IntroState::IntroState() { }

IntroState::~IntroState()
{
    delete rect;
    rect = NULL;
}

void IntroState::load()
{

    // Create background material
    MaterialPtr material = MaterialManager::getSingleton().create("Background", "General");
    material->getTechnique(0)->getPass(0)->createTextureUnitState("background_color.jpg");
    material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

    // Create background rectangle covering the whole screen
    rect = new Rectangle2D(true);
    rect->setCorners(-1.0, 1.0, 1.0, -1.0);
    rect->setMaterial("Background");

    // Render the background before everything else
    rect->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);

    // Use infinite AAB to always stay visible
    AxisAlignedBox aabInf;
    aabInf.setInfinite();
    rect->setBoundingBox(aabInf);

    // Attach background to the scene
    SceneNode* node = ENGINE->getSceneManager()->getRootSceneNode()->createChildSceneNode("Intro");
    node->attachObject(rect);

    ENGINE->pushState(new PlayState());

    timer.reset();
    loaded = true;
}

void IntroState::unload()
{
    ENGINE->getSceneManager()->getRootSceneNode()->removeChild("Intro");
}