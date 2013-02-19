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
 * @mainpage CotopaxiEngine
 *
 * The CotopaxiEngine is a 3D game engine, based on OGRE, Bullet, SFML and libRocket.
 * It begun as a student project intended to teach game engine design as well as being a learning 
 * experience. It was a colaboration between Patrick Joos and Eduardo Hahn Paredes at the
 * Bern University of Applied Science in Berne from 2012 to 2013.
 * 
 * It is written completly in C++, using several features of the new C++11 standard, as supported 
 * by various compilers.
 * 
 * The most important library in this project is OGRE.
 * @see http://www.ogre3d.org/
 *
 * @file Main.cpp
 * @brief This is the entry point. It loads everything.
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Engine.h"

/** 
 * @namespace CotopaxiEngine
 * @brief The namespace used for the CotopaxiEngine. Contains the game and its engine.
 * 
 * Every class (except external source code) should use this namespace.
 * 
 * @namespace BtOgre
 * @brief The namespace of the BtOgre part. It's the link between OGRE and Bullet.
 * 
 * Those files where developed by Nikhilesh (nikki).
 * More information at: https://github.com/nikki93/btogre
 */
using namespace CotopaxiEngine;

int main(int argc, char* argv[])
{
    try {
        ENGINE->load("Mermelin");
        ENGINE->start();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}