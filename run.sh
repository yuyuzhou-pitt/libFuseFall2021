#!/bin/bash
make
sudo make install
sudo umount mount
rm base/*
rm mount/*
rm log.txt
ntapfuse mount base mount

