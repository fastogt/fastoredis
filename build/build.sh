#!/bin/sh
set -e

createPackage() {
    dir_path="$1"
    cpack_generator="$2"
	make_program="$3"
    if [ -d "$dir_path" ]; then
        rm -rf "$dir_path"
    fi
    mkdir "$dir_path"
    cd "$dir_path"
    cmake ../../ -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE -DOS_ARCH=64 -DCPACK_GENERATOR="$cpack_generator"
    $make_program install
    cpack -G "$cpack_generator"
    if [ "$cpack_generator" = 'DEB' ]; then
        sh ./fixup_deb.sh
    fi
    cd ../
}

unamestr=`uname`
echo uname: $unamestr

if [ "$unamestr" = 'MINGW32_NT-6.1' ]; then
	echo Windows
    createPackage build_nsis NSIS mingw32-make
    createPackage build_zip ZIP mingw32-make
elif [ "$unamestr" = 'Linux' ]; then
	echo Linux
    createPackage build_deb DEB make
    createPackage build_rpm RPM make
    createPackage build_tar TGZ make
elif [ "$unamestr" = 'Darwin' ]; then
	echo Darwin
    createPackage build_dmg DragNDrop make
    createPackage build_zip ZIP make
fi

