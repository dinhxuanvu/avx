#!/bin/bash
sudo sh -c 'echo 65 > /sys/class/gpio/export'
(
cd ~/develop/scripts/overlay
make overlay
)
