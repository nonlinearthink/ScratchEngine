import os
import shutil
import subprocess
import sys

# Get Build Mode
if "--release" in sys.argv:
    release = True
else:
    release = False

# Get Base Directory
BASEDIR = os.path.dirname(os.path.abspath(__file__))

# Delete the existing build directory
relative_build_dir = os.path.join("build", "Release" if release else "Debug")
build_dir = os.path.join(BASEDIR, relative_build_dir)
if os.path.exists(build_dir):
    shutil.rmtree(build_dir)

# Install conan dependencies
conan_install_cmd = [
    "conan",
    "install",
    ".",
    "--profile:host=cmake_msvc_ninja",
    "--profile:build=cmake_msvc_ninja",
    "--build=missing",
    f"--settings=build_type={'Release' if release else 'Debug'}",
]
subprocess.run(conan_install_cmd, cwd=BASEDIR, check=True)
