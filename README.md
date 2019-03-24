This is a simple project I made to be able to use gears of war pc 2007 without startup.exe (easier to debug the wargame executable directly, since the game is designed to be run directly from startup.exe). It is also meant to be used directly with https://github.com/KILLER-CHIEF/xlivelessness when its completed for possible future multiplayer stuff (lan only for now). Probably use my fork of xlivelessness till KC finishes his version.

This works on legit copies of gears of war 2007, I've not tested on any cracked copies, or any garbage like that.

Still a WIP, can load menus/cut scenes, and launch campaign. Can sign in with my latest PR in the xlivelessness repo.

To use:

1) Get a copy of https://github.com/KILLER-CHIEF/xlivelessness dll
2) Put the xlivelessness dll into gears of war "Binaries" folder
3) Create the following folder path in "Binaries", "xlln\modules"
4) Put the dll from this project in "xlln\modules"
5) Run WarGame-G4WLive.exe
