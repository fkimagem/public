@echo off
git status
pause
git add .\boards.json
pause
git commit -m "update boards"
pause
git push origin
pause