#!/bin/bash
# Installation on Ubuntu Linux 12.10 64 Bit
sudo apt-get update

# C++, Ogre, libraries and other dependencies
sudo apt-get reinstall libogre-dev ogre-tools gcc build-essential autoconf libtool libdevil-dev libfreeimage-dev libfreetype6-dev libglew-dev libxaw7-dev libxrandr-dev libxt-dev libois-dev libzzip-dev freeglut3-dev blender gimp-plugin-registry cmake premake4 doxygen libsfml-dev libboost-python-dev

# Download .scene Export-Plugin (import it manually later!)
if [ -f ~/.blender/2.63/scripts/addons/blender2Ogre* ]; then echo "Blender2Ogre already installed"
else echo "Downloading Blender2Ogre plugin to your home folder. Decompress it and load it form within blender."
    cd
    wget http://blender2ogre.googlecode.com/files/blender2ogre-0.5.8.zip
    cd -
fi

# Download and install Netbeans 7.2.1
if [ -d /usr/local/netbeans-7.2.1 ]; then echo "Netbeans already installed"
	else echo "Un-Installing Netbeans 7.2. Follow the instructions of the graphic installer."
    if [ -f /usr/local/netbeans-7.2/uninstall.sh ]; then
        sudo /usr/local/netbeans-7.2/uninstall.sh    
    fi
    echo "Installing Netbeans 7.2.1 Follow the instructions of the graphic installer."
    mkdir /tmp/installNetbeans
    cd /tmp/installNetbeans
	wget http://download.netbeans.org/netbeans/7.2.1/final/bundles/netbeans-7.2.1-ml-linux.sh
    chmod +x netbeans-7.2.1-ml-linux.sh
    sudo ./netbeans-7.2.1-ml-linux.sh    
    cd -
    rm -r /tmp/installNetbeans    
fi

# Download and install Bullet 2.81-rev2613
if [ -d /usr/local/include/bullet ]; then echo "Bullet already installed"
else echo "Installing Bullet 2.81-rev2613"
    mkdir /tmp/installBullet
    cd /tmp/installBullet
    wget http://bullet.googlecode.com/files/bullet-2.81-rev2613.tgz
    tar -xf bullet-2.81-rev2613.tgz
    cd bullet-2.81-rev2613
    cmake . -G "Unix Makefiles" -DBUILD_DEMOS=FALSE -DINSTALL_EXTRA_LIBS=TRUE
    make -j4
    sudo make install
    cd -
    rm -r /tmp/installBullet    
fi

# Check libRocket
if [ -d /usr/local/include/Rocket/ ]; then echo "Rocket already installed"
else 
echo "libRocket not found on this system. Please install it manually:"
echo "http://librocket.com/download"
echo "http://librocket.com/wiki/documentation/BuildingWithCMake"   
fi