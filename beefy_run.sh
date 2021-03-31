#!/bin/bash

autoreconf --install
./configure
make
sudo make install
sudo umount mount
rm -rf base
rm -rf mount
rm log.txt
mkdir base
mkdir mount
ntapfuse mount base mount