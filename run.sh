#!/bin/bash
make
sudo make install
sudo umount mount
ntapfuse mount base mount
rm log.txt