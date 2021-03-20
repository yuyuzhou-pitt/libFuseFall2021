#!/bin/bash
make
sudo make install
sudo umount mount
rm base/*
rm log.txt
ntapfuse mount base mount

