#!/usr/bin/env python
"""Small wrapper that locates the newest SuikaEmulatorHeadless binary (same way
test.py does) and forwards all command line arguments to it.

Usage:
    python3 headless.py [OPTIONS] [FILES...]
"""

import glob
import os
import subprocess
import sys

SuikaEmulator_EXECUTABLES = [
    # Windows
    "Windows\\Debug\\SuikaEmulatorHeadless.exe",
    "Windows\\Release\\SuikaEmulatorHeadless.exe",
    "Windows\\x64\\Debug\\SuikaEmulatorHeadless.exe",
    "Windows\\x64\\Release\\SuikaEmulatorHeadless.exe",
    "build*/SuikaEmulatorHeadless.exe",
    "./SuikaEmulatorHeadless.exe",
    # Mac
    "build*/Debug/SuikaEmulatorHeadless",
    "build*/Release/SuikaEmulatorHeadless",
    "build*/RelWithDebInfo/SuikaEmulatorHeadless",
    "build*/MinSizeRel/SuikaEmulatorHeadless",
    # Linux
    "build*/SuikaEmulatorHeadless",
    "./SuikaEmulatorHeadless",
]


def find_headless():
    env_path = os.environ.get("SuikaEmulator_HEADLESS")
    if env_path and os.path.exists(env_path):
        return env_path
    possible_exes = [glob.glob(f) for f in SuikaEmulator_EXECUTABLES]
    possible_exes = [x for sublist in possible_exes for x in sublist]
    existing = [f for f in possible_exes if os.path.exists(f)]
    if not existing:
        return None
    return max(existing, key=os.path.getmtime)


def main():
    headless = find_headless()
    if headless is None:
        print("ERROR: SuikaEmulatorHeadless binary not found. Set SuikaEmulator_HEADLESS or run from the repo root.", file=sys.stderr)
        return 2
    return subprocess.call([headless] + sys.argv[1:])


if __name__ == "__main__":
    sys.exit(main())
