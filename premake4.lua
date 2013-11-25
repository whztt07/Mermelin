-- This is the premake4 script for the CotopaxiEngine.
-- Eduardo Hahn Paredes, 2013 (Linux and Mac part)
-- Patrick Joos, 2013 (Windows part)

function default(osName, actionName)
    if osName ~= nil and os.is(osName) == false then
        return
    end

    if _ACTION == nil then
        _ACTION = actionName
    end
end

default("windows", "vs2010")
default("linux", "gmake")
default("macosx", "xcode3")

solution "Cotopaxi"
project "Mermelin"
kind "WindowedApp"
language "C++"

files { "headers/**.h", "src/**.cpp" }
includedirs { "headers" }

configurations { "Debug", "Release" }
if os.is("windows") then
    configuration { "windows" }

    pchheader "stdafx.h"
    pchsource "src/stdafx.cpp"

    ogre_home = os.getenv("OGRE_HOME")
    gamedev_libs = os.getenv("GAMEDEV_LIBS")
    boost_include = os.getenv("BOOST_INCLUDE")
    boost_libs = os.getenv("BOOST_LIBS")

    includedirs{
        ogre_home .. "/include/OGRE",
        ogre_home .. "/include/OIS",
        gamedev_libs .. "/bullet-2.80-rev2531/src",
        gamedev_libs .. "/SFML-1.6/include",
        ogre_home .. "/boost",
        gamedev_libs .. "/libRocket/Include"
    }

    libdirs{ gamedev_libs .. "/SFML-1.6/extlibs/bin" }

    os.remove("Thesis.sdf")
    buildoptions { "/Zm200" }
end

if os.is("linux") then
    configuration { "linux" }

    pchheader "headers/stdafx.h"
    pchsource "src/stdafx.cpp"

    libdirs {
        "/usr/lib/OGRE/",
        "/usr/local/lib",
        "/usr/lib/x86_64-linux-gnu/",
        "/usr/lib/"
    }

    includedirs {
        "/usr/include/OGRE/",
        "/usr/include/ois/",
        "/usr/local/include/",
        "/usr/local/include/bullet/",
        "/usr/local/include/bullet/ConvexDecomposition",          
    }

    links {
        "OgreMain",
        "OgreTerrain",
        "OIS",
        "BulletDynamics",
        "BulletCollision",
        "LinearMath",
        "sfml-system",
        "sfml-audio",
        "pthread",
        "RocketCore",
        "RocketControls",
        "boost_system"
    }

    buildoptions { "-std=c++0x" }
end

configuration "Debug"
    targetdir ("bin/debug")
    defines { "DEBUG" }
    flags { "Symbols" }

    if os.is("windows") then
        libdirs {
            ogre_home .. "/lib/debug",
            gamedev_libs .. "/bullet-2.80-rev2531/lib/Debug",
            gamedev_libs .. "/cppunit/lib",
            gamedev_libs .. "/SFML-1.6/lib",
            ogre_home .. "/boost/lib/",
            gamedev_libs .. "/libRocket/bin"
        }
		
        links {
            "OgreMain_d",
            "OIS_d",
            "OgreTerrain_d",
            "OgrePaging_d",
            "OgreProperty_d",
            "OgreRTShaderSystem_d",
            "BulletCollision_Debug",
            "BulletDynamics_Debug",
            "LinearMath_Debug",
            "sfml-system-s-d",
            "sfml-audio-s-d",
            "RocketCore_d",
            "RocketControls_d",
            "RocketDebugger_d"
        }
	
        debugdir(ogre_home .. "/bin/debug")
    end

    if os.is("linux") then
        buildoptions { "-O2 -g" }
    end

configuration "Release"
targetdir ("bin/release")
defines { "NDEBUG" }
flags { "Optimize" }

if os.is("windows") then
    libdirs {
        ogre_home .. "/lib/release",
        gamedev_libs .. "/bullet-2.80-rev2531/lib/Release",
        gamedev_libs .. "/SFML-1.6/lib",
        ogre_home .. "/boost/lib/",
        gamedev_libs .. "/libRocket/bin"
    }

    links {
        "OgreMain",
        "OIS",
        "OgreTerrain",
        "OgrePaging",
        "OgreProperty",
        "OgreRTShaderSystem",
        "BulletCollision",
        "BulletDynamics",
        "LinearMath",
        "sfml-system-s",
        "sfml-audio-s",
        "RocketCore",
        "RocketControls"            
    }

    debugdir(ogre_home .. "/bin/release")
end

if os.is("linux") then
    buildoptions { "-O3" }
end