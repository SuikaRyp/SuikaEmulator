#!/bin/bash

echo "SDL macbundle.sh"

SuikaEmulator="${1}"
SuikaEmulator_SHORTNAME="${2}"
SuikaEmulatorSDL="${SuikaEmulator}/Contents/MacOS/${SuikaEmulator_SHORTNAME}"
MOLTENVK="${SuikaEmulator}/Contents/Frameworks/libMoltenVK.dylib"

ls ${SuikaEmulatorSDL}

if [ ! -f "${SuikaEmulatorSDL}" ]; then
  echo "No such file: ${SuikaEmulatorSDL}!"
  exit 0
fi

echo pwd=`pwd`
echo SuikaEmulator=$SuikaEmulator
echo SuikaEmulatorSDL=$SuikaEmulatorSDL

cd "$(dirname "$0")"
RPATH="$(pwd)/macOS"
cd -
echo RPATH=$RPATH
SDL="${RPATH}/SDL2.framework"
if [ ! -d "${SDL}" ]; then
  echo "Cannot locate SDL.framework: ${SDL}!"
  exit 0
fi

rm -rf "${SuikaEmulator}/Contents/Frameworks/SDL2.framework" || exit 0
mkdir -p "${SuikaEmulator}/Contents/Frameworks" || exit 0
cp -a "$SDL" "${SuikaEmulator}/Contents/Frameworks" || exit 0
echo install_name_tool -rpath "${RPATH}" "@executable_path/../Frameworks" "${SuikaEmulatorSDL}" || echo "Already patched."
install_name_tool -rpath "${RPATH}" "@executable_path/../Frameworks" "${SuikaEmulatorSDL}" || echo "Already patched."

echo "Done."

GIT_VERSION_LINE=$(grep "SuikaEmulator_GIT_VERSION = " "$(dirname "${0}")/../build/git-version.cpp")

echo "Setting version to '${GIT_VERSION_LINE}'..."
SHORT_VERSION_MATCH='.*"v([0-9\.]+(-[0-9]+)?).*";'
LONG_VERSION_MATCH='.*"v(.*)";'
if [[ "${GIT_VERSION_LINE}" =~ ^${SHORT_VERSION_MATCH}$ ]]; then
	plutil -replace CFBundleShortVersionString -string $(echo ${GIT_VERSION_LINE} | perl -pe "s/${SHORT_VERSION_MATCH}/\$1/g") ${SuikaEmulator}/Contents/Info.plist
	plutil -replace CFBundleVersion            -string $(echo ${GIT_VERSION_LINE} | perl -pe "s/${LONG_VERSION_MATCH}/\$1/g")  ${SuikaEmulator}/Contents/Info.plist
else
	plutil -replace CFBundleShortVersionString -string "" ${SuikaEmulator}/Contents/Info.plist
	plutil -replace CFBundleVersion            -string "" ${SuikaEmulator}/Contents/Info.plist
fi

# AdHoc codesign is required for Apple Silicon.
echo "Signing..."

codesign -fs - --entitlements ../macOS/Entitlements.plist --timestamp "${MOLTENVK}" || echo "Failed signing MoltenVK"
codesign -fs - --entitlements ../macOS/Entitlements.plist --timestamp "${SuikaEmulatorSDL}" || echo "Failed signing SuikaEmulatorSDL"
codesign -fs - --entitlements ../macOS/Entitlements.plist --timestamp "${SuikaEmulator}" || echo "Failed signing SuikaEmulator"
