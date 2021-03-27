#!/bin/bash

: '
Database records:
user_id,             byte_total,          byte_quota,          file_total,          file_quota
00000000000000001000,00000000000000000005,00000000000001048576,00000000000000000000,00000000000000000100
'

: '
cas380@cas380-VirtualBox:~/Documents/libFuseFall2021$ sudo -H -u cas380 bash -c 'echo "I am $USER, with uid $UID"' 
I am cas380, with uid 1000
cas380@cas380-VirtualBox:~/Documents/libFuseFall2021$ sudo -H -u root bash -c 'echo "I am $USER, with uid $UID"' 
I am root, with uid 0
'

echo "Unimplemented"