#!/bin/bash

echo "You must be root"
echo "Make folder for logging [/var/log/shrimp] .. is ok"
sudo mkdir /var/log/shrimp
echo "Using chmod [/var/log/shrimp] .. is ok"
sudo chmod 777 /var/log/shrimp
