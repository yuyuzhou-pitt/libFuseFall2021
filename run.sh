#!/bin/bash
sudo make install
make clean
fusermount -u mount
rm base/*
rm log.txt
ntapfuse mount base mount

