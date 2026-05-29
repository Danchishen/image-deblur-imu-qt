# Run this script from the project root folder.
# Before running, create an empty GitHub repository named image-deblur-imu-qt.

$ErrorActionPreference = "Stop"

Write-Host "Configuring Git identity..."
git config --global user.name "Danchishen Daniil"
git config --global user.email "dani-danchishen@mail.ru"

Write-Host "Initializing Git repository..."
git init

git add .
git commit -m "Initial Qt image deblurring project"
git branch -M main

$remoteUrl = "https://github.com/Danchishen/image-deblur-imu-qt.git"

$originExists = git remote | Select-String -Pattern "^origin$"
if ($originExists) {
    git remote set-url origin $remoteUrl
} else {
    git remote add origin $remoteUrl
}

git push -u origin main

Write-Host "Done. Open: https://github.com/Danchishen/image-deblur-imu-qt"
