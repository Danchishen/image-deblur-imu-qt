# Project structure

This document describes the main source files and folders.

## Root files

| File | Purpose |
|---|---|
| `main.cpp` | Application entry point. |
| `blur.pro` | Qt qmake project file. |
| `CMakeLists.txt` | CMake project configuration. |
| `MainResources.qrc` | Qt resource collection file. |
| `blur.rc` | Windows resource script. |

## GUI files

| File | Purpose |
|---|---|
| `MainWindow.cpp`, `MainWindow.h`, `MainWindow.ui` | Main application window and UI logic. |
| `HelpDialog.cpp`, `HelpDialog.h`, `HelpDialog.ui` | Help dialog window. |
| `Icons/` | Icons used by the interface. |

## Processing modules

| File | Purpose |
|---|---|
| `DeconvolutionTool.cpp`, `DeconvolutionTool.h` | Image deconvolution and restoration algorithms. |
| `ImageUtils.cpp`, `ImageUtils.h` | Image loading, conversion, and helper operations. |
| `MathUtils.cpp`, `MathUtils.h` | Mathematical helper functions. |
| `WorkerThread.cpp`, `WorkerThread.h` | Background processing to keep the UI responsive. |
| `CheckUpdatesThread.cpp`, `CheckUpdatesThread.h` | Update-checking logic, if used. |

## Dependencies and resources

| Folder | Purpose |
|---|---|
| `FFTW/` | FFTW dependency files used for Fourier transforms. |
| `CMakeModules/` | Custom or helper CMake modules. |
| `Models/` | Additional models or project resources. |

## Documentation folders

| Folder | Purpose |
|---|---|
| `docs/` | Build, usage, and GitHub documentation. |
| `data/` | Example input data. |
| `screenshots/` | Screenshots for the GitHub README. |
