# GitHub upload instruction

This instruction describes how to upload the project to GitHub from Windows PowerShell.

## 1. Prepare the project folder

Open the project folder that contains files such as:

```text
blur.pro
CMakeLists.txt
main.cpp
MainWindow.cpp
MainWindow.h
DeconvolutionTool.cpp
DeconvolutionTool.h
```

Copy the files from this package into the root of your project folder:

```text
README.md
.gitignore
.gitattributes
.editorconfig
LICENSE
CHANGELOG.md
docs/
data/
screenshots/
scripts/
```

Do not copy the whole wrapper folder itself. Copy only its contents.

## 2. Remove user-specific project files from Git tracking

The project folder may contain local IDE files such as:

```text
blur.pro.user
blur.pro.user.*
*.aps
```

They are already ignored in `.gitignore`, but if they were previously added to Git, remove them from Git tracking:

```powershell
git rm --cached blur.pro.user
```

If the command says the file is not tracked, it is fine.

## 3. Create a new GitHub repository

Recommended repository name:

```text
image-deblur-imu-qt
```

Recommended description:

```text
Qt/C++ application for restoring motion-blurred images using smartphone inertial sensor data and deconvolution methods.
```

When creating the repository on GitHub, do not add README, .gitignore, or license there, because these files already exist locally.

## 4. Configure Git identity

Run once on your computer:

```powershell
git config --global user.name "Danchishen Daniil"
git config --global user.email "dani-danchishen@mail.ru"
```

## 5. Initialize and push the repository

Open PowerShell in the project folder and run:

```powershell
git init
git add .
git commit -m "Initial Qt image deblurring project"
git branch -M main
git remote add origin https://github.com/Danchishen/image-deblur-imu-qt.git
git push -u origin main
```

If Git says that `origin` already exists, use this instead of `git remote add`:

```powershell
git remote set-url origin https://github.com/Danchishen/image-deblur-imu-qt.git
git push -u origin main
```

## 6. Check the result

After a successful push, open:

```text
https://github.com/Danchishen/image-deblur-imu-qt
```

Check that GitHub shows:

- `README.md` on the main page;
- source files;
- `docs/` folder;
- `data/` folder;
- `screenshots/` folder.

## 7. Add screenshots

Create screenshots of the application and place them in the `screenshots/` folder:

```text
screenshots/main-window.png
screenshots/settings.png
screenshots/result.png
```

Then update the screenshot section in `README.md` and push again:

```powershell
git add README.md screenshots/
git commit -m "Add application screenshots"
git push
```
