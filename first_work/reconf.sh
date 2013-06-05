#!/bin/sh -e
#cd ../yile_net_client/
#./compile.sh
#cd ../yile_new/
/data/app/php/bin/phpize
./configure --with-php-config=/data/app/php/bin/php-config --with-yilenet=/data/bluebird_studio/yile_net_client/ --with-yilefight=/data/bluebird_studio/fight/yile_fight --with-yilefightformula=/data/bluebird_studio/fight/yile_fight --with-yileutil=/data/bluebird_studio/utils/ --with-yileprotocol=/data/bluebird_studio/yile_protocol/protocol_so --with-yile_net_lib=/data/bluebird_studio/lib_yile_net/
