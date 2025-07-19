#!/bin/bash

set -e

echo "Building fileclip..."
make
sleep 1

echo "Installing fileclip to /usr/local/bin..."
sudo make install
make clean

echo "✅ Done. Run 'fileclip -h' for usage."
