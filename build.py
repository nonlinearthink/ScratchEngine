import os
import subprocess
import sys

# Get Build Mode
if "--release" in sys.argv:
    release = True
else:
    release = False

# Get Base Directory
BASEDIR = os.path.dirname(os.path.abspath(__file__))
build_dir = os.path.join(BASEDIR, "build", "Release" if release else "Debug")

# Configure CMake Project
cmake_configure_cmd = [
    "cmake",
    "--preset",
    "conan-release" if release else "conan-debug",
]
subprocess.run(cmake_configure_cmd, check=True)

# Build CMake Project
cmake_build_cmd = [
    "cmake",
    "--build",
    "--preset",
    "conan-release" if release else "conan-debug",
]
subprocess.run(cmake_build_cmd, check=True)

# Run the executable file
executable_path = os.path.join(build_dir, "ScratchEngine.exe")
subprocess.run([executable_path], check=True)
