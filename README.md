# ScratchEngine

My first Game Engine based on `Dear ImGUI` and `DirectX 12`.

## Run

**Runs on `Windows` only.**

And make sure you have `python` installed.

```shell
pip install conan
conan profile default --detect
python run_release.py
```

### Run with Ninja

Add settings to your conan profile.

```
[conf]
tools.cmake.cmaketoolchain:generator=Ninja
```

It is possible that you need to run:

```bat
conan-build-release\conanbuild.bat
```

It will activate the `MSVC` environment. And it seems only work when you use `Command Prompt`.

Finally, you can run:

```
python run_release.py ninja
```
