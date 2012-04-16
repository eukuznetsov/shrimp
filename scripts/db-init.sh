#!/bin/bash

MYSQL=`which mysql`
USERNAME="shrimp"
DATABASE="shrimp"
HOST="localhost"
PASSWORD="test"

echo "Enter MySQL root passwd"
$MYSQL -uroot -p -e "CREATE DATABASE IF NOT EXISTS ${DATABASE}; GRANT ALL ON *.* TO '${DATABASE}'@'${HOST}' IDENTIFIED BY '${PASSWORD}';FLUSH PRIVILEGES;"
$MYSQL -u ${USERNAME} -p${PASSWORD} -e "USE ${DATABASE}; CREATE TABLE squid_access_log (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, time DOUBLE, duration INT, client_address VARCHAR(20), result_codes VARCHAR(100), bytes BIGINT, request_method VARCHAR(20), url VARCHAR(500), rfc931 VARCHAR(50), hierarchy_code VARCHAR(50), type VARCHAR(50)) ENGINE = MYISAM CHARACTER SET utf8 COLLATE utf8_general_ci;"
