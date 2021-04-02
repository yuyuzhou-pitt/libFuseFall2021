#!/bin/bash
sudo make install
make clean
fusermount -u mount

rm -r base/*
rm -r mount/*
rm log.txt
rm database.csv

ntapfuse mount base mount

