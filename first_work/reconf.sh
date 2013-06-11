#!/bin/sh -e
/data/app/php/bin/phpize
./configure --with-php-config=/data/app/php/bin/php-config --with-firstprotocol=/data/bluebird_first/first_protocol/protocol_gcc
