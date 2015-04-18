#!/bin/bash
(
cd /home/ubuntu/develop/
make kill-python-daemon
make python-daemon
su ubuntu -c 'git pull'
)
ntpdate pool.ntp.org
echo "Update ntp" >> /home/ubuntu/develop/PythonDaemon/lcd-log
echo 65 > /sys/class/gpio/export
(
cd /home/ubuntu/develop/scripts/overlay
make overlay
)
echo BB-I2C1 > /sys/devices/bone_capemgr.9/slots
echo "Set up overlays" >> /home/ubuntu/develop/PythonDaemon/lcd-log
(
cd /home/ubuntu/develop/
echo "Starting AVX..." >> /home/ubuntu/develop/PythonDaemon/lcd-log
make play
)
