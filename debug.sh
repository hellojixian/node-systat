#!/bin/sh

SERVER=192.168.100.37
LOCALHOST=192.168.100.24
LOCALUSER=jixianwang

rsync -av --delete ./ root@${SERVER}:/tmp/test
ssh root@${SERVER} "(cd /tmp/test; npm install --unsafe-perm; npm test)"