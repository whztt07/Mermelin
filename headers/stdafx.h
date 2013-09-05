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

#ifndef PRECOMPILED_HEADERS_H
#define	PRECOMPILED_HEADERS_H

// OGRE
#include <Ogre.h>
#include <RenderSystems/GL/OgreGLSLProgram.h>

// OIS
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

// SFML
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Thread.hpp>

// Bullet2Ogre
#include "External/BtOgre/BtOgreExtras.h"
#include "External/BtOgre/BtOgreGP.h"
#include "External/BtOgre/BtOgrePG.h"

// Boost
#include "boost/algorithm/string.hpp"

// Rocket
#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Core/String.h>
#include <Rocket/Core/Context.h>
#include <Rocket/Core/RenderInterface.h>
#include <Rocket/Core/SystemInterface.h>

#if DEBUG
#include <Rocket/Debugger.h>
#endif

// Common
#include <string>
#include <map>

// Other
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#endif