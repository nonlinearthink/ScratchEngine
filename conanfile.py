import os
from conan import ConanFile
from conan.tools.files import copy
from conan.tools.cmake import cmake_layout


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
        self.tool_requires("cmake/[>=3.23]")
        self.tool_requires("ninja/1.11.1")

    def generate(self):
        # Copy the ImGUI bindings
        print("ImGUI Library Folder: ", self.dependencies["imgui"].package_folder)
        copy(
            self,
            "*impl_dx12*",
            os.path.join(self.dependencies["imgui"].package_folder, "res", "bindings"),
            os.path.join(self.source_folder, "bindings"),
        )
        copy(
            self,
            "*impl_win32*",
            os.path.join(self.dependencies["imgui"].package_folder, "res", "bindings"),
            os.path.join(self.source_folder, "bindings"),
        )

    def layout(self):
        cmake_layout(self)
