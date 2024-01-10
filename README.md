# ScratchEngine

My first Game Engine based on `Dear ImGUI` and `DirectX 12`.

## Getting Started

**Could run on `Windows` only.**

### Prepare

```shell
pip install conan
conan profile detect --name cmake_msvc_ninja
python prebuild.py --release # or --debug
```

Add settings to your profile `cmake_msvc_ninja`.

```
[conf]
tools.cmake.cmaketoolchain:generator=Ninja
```

### Activate Environment

```shell
build\Debug\generators\conanbuild.bat
# or build\Release\generators\conanbuild.bat
```

Open your `VS Code` with recommended extensions:

```shell
code .
```

### Run

```shell
python build.py --release # or --debug
```
