#!/bin/bash

gcc -pthread -o bai1 bai1.c
echo "start"
echo "1000000">freq.txt
timeout 60s ./bai1
echo "100000">freq.txt
timeout 60s ./bai1
echo "10000">freq.txt
timeout 60s ./bai1
echo "1000">freq.txt
timeout 60s ./bai1
echo "100">freq.txt
timeout 60s ./bai1
echo "end"

