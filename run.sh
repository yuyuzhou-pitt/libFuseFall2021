#!/bin/bash
sudo make install
make clean
fusermount -u mount
rm base/*
rmdir base/*
rm mount/*
rmdir mount/*
rm log.txt
ntapfuse mount base mount

