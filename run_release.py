import os
import shutil
import subprocess

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

# cd to the build directory
os.chdir(os.path.join(BASEDIR, "conan-build-release"))

# Configure CMake Project
cmake_configure_cmd = [
    "cmake",
    "..",
    "-G",
    "Visual Studio 17 2022",
    "-DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake",
]
subprocess.run(cmake_configure_cmd, check=True)

# Build CMake Project
cmake_build_cmd = ["cmake", "--build", ".", "--config", "Release"]
subprocess.run(cmake_build_cmd, check=True)

# Run the executable file
executable_path = os.path.join(
    BASEDIR, "conan-build-release", "Release", "ScratchEngine.exe"
)
subprocess.run([executable_path], check=True)
