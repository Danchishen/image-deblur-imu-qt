# Build instructions for Windows

The recommended way to build the project is through Qt Creator.

## Requirements

- Windows 10 or Windows 11.
- Qt Creator.
- Qt 5.x or a compatible Qt version used by the project.
- C++ compiler compatible with the selected Qt Kit:
  - MinGW, or
  - MSVC.
- FFTW dependency files.

The project folder already contains an `FFTW/` directory. If the build fails because FFTW is not found, check the paths in `blur.pro` or `CMakeLists.txt`.

## Option 1: Build with qmake

1. Open Qt Creator.
2. Click **File -> Open File or Project**.
3. Select `blur.pro`.
4. Choose a suitable Kit.
5. Click **Configure Project**.
6. Run **Build -> Run qmake**.
7. Run **Build -> Build Project**.
8. Start the application.

## Option 2: Build with CMake

1. Open Qt Creator.
2. Click **File -> Open File or Project**.
3. Select `CMakeLists.txt`.
4. Choose a suitable Kit.
5. Click **Configure Project**.
6. Build and run.

## Possible problems

### Qt Creator opens the project but does not build

Check that the selected Kit contains:

- Qt version;
- compiler;
- debugger;
- CMake or qmake.

### FFTW is not found

Check that the `FFTW/` folder exists and that project configuration points to the correct include and library directories.

### Build folder is too large

Do not upload generated build folders to GitHub. They are already ignored by `.gitignore`:

```text
build/
build-*/
cmake-build-*/
Debug/
Release/
```

### `.pro.user` files appear in the folder

These files are created by Qt Creator and store local machine settings. They should not be uploaded to GitHub.
