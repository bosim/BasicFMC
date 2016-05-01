BasicFMC
========

What is BasicFMC?
-----------------

BasicFMC is a very very simple FMC for X-Plane. Since the only
Open-Source FMC available for X-Plane used to be vasFMC there was
no real starting point if one wanted to create their own FMC, the
reasons for that is:

* vasFMC was never written for X-Plane but written to be a standalone
  FMC for particular MSFS. This means a lot of code is redudant for X-Plane
  users, and it is hard to understand the complete structure
* vasFMC is no longer maintained which means no changes from upstream.

The reason why I created BasicFMC was to get a minimal FMC and provide
a starting point for aircraft designers. I decided to publish it was 
GPLv3 to ensure these authors would also publish the source code.

BasicFMC is unfortunately not maintained anymore by me, since I have
sort of lost my interest in the project, but the code will remain
in this repository.

How to compile?
---------------

You need the X-Plane SDK. Download from 
[here](http://www.xsquawkbox.net/xpsdk/mediawiki/Main_Page) and place 
it within the repository.

You will need the GCC compiler (with C++ support to compile) and also 
various development packages. Here is what you need for openSUSE

    # zypper in gcc gcc-c++ gcc-32bit gcc-c++-32bit 
    # zypper in glibc-devel glibc-devel-32bit
    # zypper in Mesa-devel freeglut-devel 

Then just compile 

    $ make

The X-Plane plugins are placed in `build/BasicFMC/32/lin.xpl` and 
`build/BasicFMC/64/lin.xpl`.

How does it look?
-----------------

![screenshot](screenshot.png)

Author
------

* Bo Simonsen <bo@geekworld.dk> 
