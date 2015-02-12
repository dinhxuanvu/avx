#!/bin/bash
git pull
sudo sh -c 'echo 65 > /sys/class/gpio/export'
(
cd ~/develop/scripts/overlay
make overlay
)
make play
