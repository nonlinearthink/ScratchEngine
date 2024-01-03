import os
from conan import ConanFile
from conan.tools.files import copy
from conan.tools.cmake import CMake


class ScratchEngine(ConanFile):
    name = "ScratchEngine"
    version = "0.1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        # Runtime requirements
        self.requires("imgui/1.90")

    def build_requirements(self):
        # Build requirements
        self.requires("cmake/[>=3.20]")

    def generate(self):
        # Copy the ImGUI bindings
        print("ImGUI Library Folder: ", self.dependencies["imgui"].package_folder)
        copy(
            self,
            "*dx12*",
            os.path.join(self.dependencies["imgui"].package_folder, "res", "bindings"),
            os.path.join(self.source_folder, "bindings"),
        )
        copy(
            self,
            "*win32*",
            os.path.join(self.dependencies["imgui"].package_folder, "res", "bindings"),
            os.path.join(self.source_folder, "bindings"),
        )

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.definitions["CMAKE_EXPORT_COMPILE_COMMANDS"] = True
        cmake.build()
