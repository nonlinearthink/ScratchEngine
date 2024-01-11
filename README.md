# ScratchEngine

My first Game Engine based on `Dear ImGUI` and `DirectX 12`.

## Getting Started

**Could run on `Windows` only.**

### Setup Conan Environment

1. Install Conan and create your profile:

    ```shell
    pip install conan
    conan profile detect --name cmake_ninja_msvc_profile
    ```

2. Add settings to your `cmake_ninja_msvc_profile` profile:

    ```text
    [conf]
    tools.cmake.cmaketoolchain:generator=Ninja
    ```

3. Run `prebuild.py`:

    ```shell
    python prebuild.py --debug # or --release
    ```

### Setup IDE

1. Run `conanbuild.bat` to activate required environments:

    ```shell
    build\Release\generators\conanbuild.bat
    # or build\Debug\generators\conanbuild.bat
    ```

2. Open your `VS Code` for example:

    ```shell
    code .
    ```

### Run Code

```shell
python build.py --debug # or --release
```
