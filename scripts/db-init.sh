#!/bin/bash

MYSQL=`which mysql`
USERNAME="shrimp"
DATABASE="shrimp"
HOST="localhost"
PASSWORD="test"

echo "Enter MySQL root passwd"
$MYSQL -u root -p -e "CREATE DATABASE IF NOT EXISTS ${DATABASE}; GRANT ALL ON *.* TO '${DATABASE}'@'${HOST}' IDENTIFIED BY '${PASSWORD}';FLUSH PRIVILEGES;"
