#!/usr/bin/env bash
set -e
cd "$(dirname "$0")"
if ! command -v g++ >/dev/null 2>&1; then echo 'Please install g++ (C++17 compiler).'; exit 1; fi
g++ -std=c++17 -O2 -Wall -Wextra SevenLabs.cpp -o sevenlabs
./sevenlabs
