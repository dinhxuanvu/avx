#!/bin/bash
(
cd /home/ubuntu/develop/
su ubuntu -c 'git pull'
)
echo "test"
echo 65 > /sys/class/gpio/export
(
cd /home/ubuntu/develop/scripts/overlay
make overlay
)
echo BB-I2C1 > /sys/devices/bone_capemgr.9/slots
(
cd /home/ubuntu/develop/
make play
)
