#!/bin/bash
# Author: Christopher McCooey
# Description: Installs OpenGL and glut for Unbuntu

echo "---- Updating packages ----"
apt update

echo "---- installing OpenGL ----"
apt-get install mesa-utils

echo "---- installing FreeGLUT ----"
apt-get install freeglut3-dev
