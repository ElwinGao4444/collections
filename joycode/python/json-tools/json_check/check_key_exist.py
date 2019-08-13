#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################
# 
# Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
# 
########################################################################
 
"""
File: check_key_exist.py
Author: work(work@baidu.com)
Date: 2015/12/11 12:24:09
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

    ret_code = 0
    err_list = []

    for keys in filter_keys:
        key_list = keys.split("/")
        if not key_exist(spec_dict, key_list):
            ret_code = 1
            err_list.append(keys)

    for err in err_list:
        print err

    sys.exit(ret_code)


def key_exist(spec_dict, key_list):
    if isinstance(spec_dict, list):
        flag = True
        for item_dict in spec_dict:
            if not key_exist(item_dict, key_list):
                flag = False
        return flag     
    if isinstance(spec_dict, dict):
        if key_list[0] not in spec_dict:
            return False
        if 1 == len(key_list):
            if key_list[-1] in spec_dict:
                return True
            else:
                return False
        return key_exist(spec_dict[key_list[0]], key_list[1:])


if __name__ == "__main__":
    main(sys.argv[1:])


