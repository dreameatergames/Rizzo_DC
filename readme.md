# Rizzo Island Software Rendered Version

This is the software rendered of Rizzo Island, based on Makaqu 1.3 by Mankrip.

Currently, it only works with a modified version of KOS 1.3 which I'll put up soon.
I've only been able to get the Dreamcast version to compile, and the Windows and Linux version currently allude me on how to compile it. I've spoken with Mankrip, but even with his help, we've been unable to get it to compile on my version.

##Requirements:

- Modified KOS 1.3 Distribution from other github project located here: [Modified-KOS-1.3-Distrobution](https://github.com/rizzoislandgame/Modified-KOS-1.3-For-Makaqu)
- Windows XP Machine or Virtual Machine
- DCDEV_R1.ISO: [Download Here](https://mega.nz/#!yX52VaDJ!XKYZbdM0HbQ3TUm88BwsKSH8xqwxb1M2CBAdXaNtpdA)

##Compiling:

1. Install The DCDEV_R1.ISO Cygwin KOS 1.3 distribution in Windows XP 

2. Go into the directory "C:\cygwin\usr\local" and delete the directory "dc".

3. Put the new "dc" directory you got from the modfied Github distribution in the       
   "C:\cygwin\usr\local" directory.

4. Start Cygwin and type "cd \usr\local\dc\kos1.3" and type "chmod u+x environ.sh" and press "Enter".

5. Type "source environ.sh" and press enter.

6. Type "cd ~" and press "Enter".

7. Type "git clone https://github.com/rizzoislandgame/Rizzo_DC.git" and press "Enter".

8. Type "cd rizzo_dc\rizzo_island_dc\s_dc"

9. Type "make clean" then press enter

10. Type "make"

11. Type "sh binaries.sh" then press "Enter"

You should now have a "1st_read.bin" that can be used with the Rizzo Island game!