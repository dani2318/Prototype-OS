### Prototype OS

### Build instructions

## WINDOWS:<br>

  <b>Requirements:</b><br>
    + WSL2 [Link](https://learn.microsoft.com/it-it/windows/wsl/install)<br>
    + nasm [Link](https://www.nasm.us/)<br>
    + qemu [Link](https://qemu.weilnetz.de/w64/)<br>

  <b>Instructions:</b><br>
      Once installed the dependencies run the make comand <b>make toolchain</b> then <b>make</b> <br>
      to run the os execute: <b>run.bat</b><br>


## Linux:<br>

  <b>Requirements:</b><br>
    + nasm [Link](https://www.nasm.us/)<br>
    + qemu [Link](https://qemu.weilnetz.de/w64/)<br>

  <b>Instructions:</b><br>
      Once installed the dependencies run the make comand <b>make toolchain</b> then <b>make</b> <br>
      to run the os execute: <b>qemu-system-i686 -fda build/main_floppy.img</b><br>
