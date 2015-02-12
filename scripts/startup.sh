#!/bin/bash
(
cd /home/driver/develop/
su driver -c 'git pull'
)
echo "test"
echo 65 > /sys/class/gpio/export
(
cd /home/driver/develop/scripts/overlay
make overlay
)
(
cd /home/driver/develop/
make play
)
