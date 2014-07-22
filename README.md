libnpa
======

libnpa is a free library for manipulating .npa (NitroPlus Archive) files.
It can also parse and recompile .nsb and .map scripts from nss.npa archive.

Building
--------

It is the choice of the Steins Gate if you can build from master branch today:
[![Travis Build Status](https://travis-ci.org/FGRE/libnpa.svg?branch=master)][1]

    $ git clone https://github.com/FGRE/libnpa.git
    $ cd libnpa

optionally, check out latest tag, if master branch status is red:

    $ git checkout `git describe --tags --abbrev=0`

now compile:

    $ cmake .
    $ make

[1]: https://travis-ci.org/FGRE/libnpa
