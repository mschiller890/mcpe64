# mcpe64

Source code for **Minecraft Pocket Edition 0.6.1 alpha** with various fixes and improvements.

This project builds upon the original repository by Kolyah35 and aims to preserve and improve this early version of Minecraft PE.

I've done some bad and pointless changes to the code so if someone could improve my horrible debugging skills I'd be very happy.

## Roadmap
- [x] Fix fog
- [x] Fix sound
- [x] Add sprinting
- [x] Chat (semi working) and commands
- [ ] Controller support
- [ ] Minecraft server hosting
- [ ] Android build support
- [ ] Better F3
- [ ] Performance optimalizations

## How to Build
1. Open **Visual Studio 2022**.
2. Select **File → Open → Folder** and choose the extracted repository.  
   > Make sure you open it **as a folder, not as a project**.
3. Visual Studio will automatically detect the `CMakeLists.txt` file and start generating the CMake configuration.
4. Wait until the **Output** window shows:
   ```
   1> CMake generation finished.
   ```
5. Set **`MinecraftPE.exe`** as the **debug target**.
6. Build or run the project from Visual Studio.   

## Credits
Original repository:  
https://gitea.sffempire.ru/Kolyah35/minecraft-pe-0.6.1
