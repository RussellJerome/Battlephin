# Battlephin
This is a experimental proof of concept runtime mod loader for Star Wars Battlefront 3 on the Dolphin Emulator

## Disclamer
I have no plans to update this program or expand on it with mods. This was written out of my own curiosity and a way to long Christmas bender.

## How to use

 1. Compile the solution with Visual Studio 2022 or another IDE
 2. Keep the Launcher.exe and Mod Loader dll in the same folder
 3. Place any DLL mods (such as example mod) into a "Mods" folder in the same directory
 4. Launch dolphin and r911 then run the exe at the main menu or some other place in game
 5. Realize this has no actual real world use and delete the entire program
 6. Drink.

## Extra Info
 - Hot reloading exists. If you just exit the Battlephin window it will safely eject all loaded mods and then itself.
- I provided a example mod fitted with a POC prop location editor, and also some simple class information printing.
- **WARNING** this was not coded with the best of practices in mind. Although honestly its not like I ever do that so whatever

## Reference

![Screen Shot](https://i.imgur.com/2AaO0RK.png)

## 3rd Party Software
* [ImGui](https://github.com/ocornut/imgui)