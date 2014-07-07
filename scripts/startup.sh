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
echo BB-I2C1 > /sys/devices/bone_capemgr.9/slots
(
cd /home/driver/develop/
make play
)
