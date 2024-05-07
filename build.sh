#!/bin/sh

set -xe

mkdir -p ./build/

CFLAGS="-O3 -Wall -Wextra"

gcc $CFLAGS -o ./build/catsay ./src/catsay.c
