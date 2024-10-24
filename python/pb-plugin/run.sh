#!/usr/bin/env bash
# Author: Elwin.Gao
# Created Time : Thu Oct 24 16:53:56 2024
# File Name: run.sh
# Description:

source ~/workspace/python/venv/default/bin/activate

rm -rf google/
rm -rf message.proto.json
chmod +x plugin.py
protoc -I./ message.proto --diy_out=. --plugin=protoc-gen-diy=plugin.py

deactivate
