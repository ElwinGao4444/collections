#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################
# 
# Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
# 
########################################################################
 
"""
File: modify_spec.py
Author: work(work@baidu.com)
Date: 2015/12/10 18:39:17
"""


import json
import sys


def main(argv):
    spec_str = sys.stdin.readlines()
    spec_str = "".join(spec_str)

    spec_dict = json.loads(spec_str)

    filter_keys_file = None
    filter_keys = []
    if len(argv) == 1:
        filter_keys_file = argv[0]
        with open(filter_keys_file, "r") as f:
            for line in f.readlines():
                line = line.strip()
                if line != "" and line[0] != "#":
                    filter_keys.append(line)
    
    for keys in filter_keys:
        key_list = keys.split("/")
        del_dict(spec_dict, key_list)
    
    spec_out_str = json.dumps(spec_dict, sort_keys=True, indent=4)
    print spec_out_str


def del_dict(spec_dict, key_list):
    if isinstance(spec_dict, list):
        for item_dict in spec_dict:
            del_dict(item_dict, key_list)
    if isinstance(spec_dict, dict):
        if key_list[0] not in spec_dict:
            return
        if 1 == len(key_list):
            if key_list[-1] in spec_dict:
                del spec_dict[key_list[-1]]
                return
        del_dict(spec_dict[key_list[0]], key_list[1:])


if __name__ == "__main__":
    main(sys.argv[1:])

