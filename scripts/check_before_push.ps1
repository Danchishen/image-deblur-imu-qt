# Quick repository check before pushing to GitHub.
# Run from the project root folder.

Write-Host "Git status:"
git status

Write-Host "\nFiles that should NOT be committed:"
Get-ChildItem -Recurse -Force -Include *.pro.user,*.aps,CMakeCache.txt -ErrorAction SilentlyContinue | Select-Object FullName

Write-Host "\nBuild folders that should NOT be committed:"
Get-ChildItem -Directory -Force -ErrorAction SilentlyContinue | Where-Object {
    $_.Name -match '^(build|build-|cmake-build-|Debug|Release)$'
} | Select-Object FullName
