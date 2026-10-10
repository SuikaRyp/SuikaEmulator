Updating with a self-built MoltenVK
===================================

Probably won't be needed again.

cp -r ../dev/build-molten/MoltenVK/Package/Release/MoltenVK/static/MoltenVK.xcframework ios/MoltenVK

The Old iOS Build Instructions
==============================

Important Notes
---------------

These instructions have not been updated for a long time and may be outdated.


Prerequisites:
--------------

* Xcode (from the Mac App Store) with command line tools installed
* MacPorts (from macports.org); easiest to install with their package installers
* cmake build system (from MacPorts); run "sudo port install cmake" from the command line

If you need to build ffmpeg yourself too, then you'll also need:

* gas-preprocessor; download the zip from https://github.com/mansr/gas-preprocessor, unzip and from the command line run:

        sudo cp gas-preprocessor.pl /usr/bin/
        sudo chmod +rw /usr/bin/gas-preprocessor.pl

* you may need pkg-config (from MacPorts); run "sudo port install pkgconfig" from the command line

Most of this is done from the command line:
-------------------------------------------

Change directory to wherever you want to install suikaemulator (eg. "cd ~"), and then clone the main suikaemulator repository:

    git clone https://github.com/hrydgard/suikaemulator.git

Change directory to the newly created suikaemulator directory and run:

    git submodule update --init

The above command will pull in the submodules required by SuikaEmulator, including the native, ffmpeg, and lang directories.  Included in the ffmpeg directory should be the necessary libs and includes for ffmpeg, so most people can skip the next command.  However, if you need to recompile ffmpeg for some reason, change directory into ffmpeg and run (this will take a while):

    ./ios-build.sh

Change directory back up to the main suikaemulator directory and do the following:

    mkdir build-ios
    cd build-ios
    cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchains/ios.cmake -GXcode ..

You now should have an Xcode project file in the build-ios directory named SuikaEmulator.xcodeproj.  Open it up in Xcode and do Product->Build to build the debug version, or Product->Build For->Archiving to build the release version (which is much faster).  If your iOS device is plugged in, you may be able to just Run in Xcode to install and test it.  Otherwise, copy the SuikaEmulator app from build-ios/Debug-iphoneos/SuikaEmulator.app or build-ios/Release-iphoneos/SuikaEmulator.app to the /Applications directory on your device and from ssh or MobileTerminal do a "chmod +x SuikaEmulator" inside the SuikaEmulator.app directory.  If this is the first time you've installed the SuikaEmulator app, you'll have to respring or restart your device for the icon to show up.
