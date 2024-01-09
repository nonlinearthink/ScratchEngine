import os
import shutil
import subprocess
import sys

# Only Windows is supported
if sys.platform != "win32":
    print("Only Windows is supported")
    exit(1)

# Get the build tool
if len(sys.argv) > 1:
    if sys.argv[1].lower() == "ninja":
        build_tool = "ninja"
    else:
        print("Unknown build tool")
        exit(1)
else:
    build_tool = "msvc"

# Get the directory of the current file
BASEDIR = os.path.dirname(os.path.abspath(__file__))

# Delete the existing build directory
build_dir = os.path.join(BASEDIR, "conan-build-release")
if os.path.exists(build_dir):
    shutil.rmtree(build_dir)

# Install conan dependencies
conan_install_cmd = [
    "conan",
    "install",
    ".",
    "--output-folder=conan-build-release",
    "--build=missing",
]
subprocess.run(conan_install_cmd, cwd=BASEDIR, check=True)

# Configure CMake Project
if build_tool == "ninja":
    cmake_configure_cmd = ["cmake", "--preset", "conan-release"]
else:
    cmake_configure_cmd = ["cmake", "--preset", "conan-default"]
subprocess.run(cmake_configure_cmd, check=True)

# Build CMake Project
cmake_build_cmd = ["cmake", "--build", "--preset", "conan-release"]
subprocess.run(cmake_build_cmd, check=True)

# Run the executable file
if build_tool == "ninja":
    executable_path = os.path.join(build_dir, "ScratchEngine.exe")
else:
    executable_path = os.path.join(build_dir, "Release", "ScratchEngine.exe")
subprocess.run([executable_path], check=True)
