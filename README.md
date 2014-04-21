libnpa
======

*And the award for worst archive file format design goes to...* **Nitroplus!**

libnpa is a free library for manipulating .npa (NitroPlus Archive) files.
It can also parse and repack .nsb and .map files found in .npa archives.

There are 4 known .npa file formats, three of which can be read by [nipa][1] tool.
This library (currently) implements fourth which is used by Steins;Gate.

Building
--------

It is the choice of the Steins Gate if you can build from master branch today:
[![Travis Build Status](https://travis-ci.org/FGRE/libnpa.svg?branch=master)][2]

    $ git clone https://github.com/FGRE/libnpa.git
    $ cd libnpa

optionally, check out latest tag, if master branch status is red:

    $ git checkout `git describe --tags --abbrev=0`

now compile:

    $ cmake .
    $ make

[1]: https://github.com/Wilhansen/nipa
[2]: https://travis-ci.org/FGRE/libnpa
