#!/bin/bash

cc pthread_key.c -lpthread
./a.out

rm ./a.out
