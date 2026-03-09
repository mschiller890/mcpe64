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
1. **Open the Repository**
   In Visual Studio 2022, open the extracted repository **as a folder, not as a project**.

2. **CMake Configuration** 
   Visual Studio should automatically detect the `CMakeLists.txt` file and generate a configuration.

3. **Check Output**  
   In the **Output** tab at the bottom, you should see:
   ```
   1> CMake generation finished.
   ```
   once the configuration is complete.
   
4. **Set Debug Target**  
    After CMake finishes, set `MinecraftPE.exe` as the **debug target** to start debugging.

## Credits
Original repository:  
https://gitea.sffempire.ru/Kolyah35/minecraft-pe-0.6.1
