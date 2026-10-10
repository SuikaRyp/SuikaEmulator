#!/bin/bash

SuikaEmulator="${1}"
SuikaEmulatoriOS="${SuikaEmulator}/SuikaEmulator"

if [ ! -f "${SuikaEmulatoriOS}" ]; then
  echo "macbundle.sh: No such file: ${SuikaEmulatoriOS}!"
  exit 0
fi

GIT_VERSION_LINE=$(grep "SuikaEmulator_GIT_VERSION = " "$(dirname "${0}")/../git-version.cpp")
SHORT_VERSION_MATCH='.*"v([0-9\.]+(-[0-9]+)?).*";'
LONG_VERSION_MATCH='.*"v(.*)";'
if [[ "${GIT_VERSION_LINE}" =~ ^${SHORT_VERSION_MATCH}$ ]]; then
	plutil -replace CFBundleShortVersionString -string $(echo ${GIT_VERSION_LINE} | perl -pe "s/${SHORT_VERSION_MATCH}/\$1/g") ${SuikaEmulator}/Info.plist
	plutil -replace CFBundleVersion            -string $(echo ${GIT_VERSION_LINE} | perl -pe "s/${LONG_VERSION_MATCH}/\$1/g")  ${SuikaEmulator}/Info.plist
else
	plutil -replace CFBundleShortVersionString -string "" ${SuikaEmulator}/Info.plist
	plutil -replace CFBundleVersion            -string "" ${SuikaEmulator}/Info.plist
fi

echo "macbundle.sh: Updated ${SuikaEmulator}/Info.plist"