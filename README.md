### Prototype OS

### Build instructions

## WINDOWS:<br>

  <b>Requirements:</b><br>
    + WSL2 [Link](https://learn.microsoft.com/it-it/windows/wsl/install)<br>
    + Open Watcom 2 with 16 bit support. [link](https://github.com/open-watcom/open-watcom-v2)<br>
    + nasm [Link](https://www.nasm.us/)<br>
    + qemu [Link](https://qemu.weilnetz.de/w64/)<br>
  
  <b>Instructions:</b><br>
      <b>NB:</b> To build with the bootloader please replace stage2 with stage2_old folder<br>
      Once installed the dependencies run the make comand<br>
      to run the os execute: <b>run.bat</b><br>
      to install toolchain execute <b>make toolchain</b>