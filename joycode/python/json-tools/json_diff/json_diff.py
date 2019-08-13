#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################
# 
# Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
# 
########################################################################

"""
File: json_diff.py
Author: chenpeng(chenpeng08@baidu.com)
Date: 2015/12/11 12:24:09
"""


# usage:
# python json_diff.py 1.json 2.json

# output:
# <L> : only the left input file has the key
# <R> : only the right input file has the key
# <B> : both the two input file have the key, but the value not equal


import json
import sys


diff_type_map = {
    "left"  : "<L>",
    "right" : "<R>",
    "both"  : "<B>"
}


def main(argv):
    json_1_dict = None
    json_2_dict = None

    if len(argv) == 2:
        json_1_file = argv[0]
        json_2_file = argv[1]
        with open(json_1_file, "r") as f1:
            json_1_dict = json.load(f1)
        with open(json_2_file, "r") as f2:
            json_2_dict = json.load(f2)

    diff_list = []
    if json_1_dict is not None and json_2_dict is not None:
        path_list = []
        json_diff(json_1_dict, json_2_dict, path_list, diff_list)

    for diff in diff_list:
        print diff


def json_diff(dict_1, dict_2, path_list, diff_list):
    if not isinstance(dict_1, list) and not isinstance(dict_1, dict) and \
            not isinstance(dict_2, list) and not isinstance(dict_2, dict):
        if dict_1 != dict_2:
            diff_list.append(diff_item_maker("both", path_list))
        return
    for key_1 in dict_1:
        path_list.append(key_1)
        if key_1 not in dict_2:
            diff_list.append(diff_item_maker("left", path_list))
        elif type(dict_1[key_1]) == type(dict_2[key_1]):
            if isinstance(dict_1[key_1], dict):
                json_diff(dict_1[key_1], dict_2[key_1], path_list, diff_list)
            elif isinstance(dict_1[key_1], list):
                for idx, each_dict in enumerate(dict_1[key_1]):
                    path_list.append("[" + str(idx) + "]")
                    json_diff(dict_1[key_1][idx], dict_2[key_1][idx], path_list, diff_list)
                    del path_list[-1]
            elif dict_1[key_1] != dict_2[key_1]:
                diff_list.append(diff_item_maker("both", path_list))
            else:
                pass
        else:
            diff_list.append(diff_item_maker("both", path_list))
        del path_list[-1]
    for key_2 in dict_2:
        path_list.append(key_2)
        if key_2 not in dict_1:
            diff_list.append(diff_item_maker("right", path_list))
        del path_list[-1]


def diff_item_maker(diff_type, path_list):
    return diff_type_map[diff_type] + " " + "/".join(path_list)


if __name__ == "__main__":
    main(sys.argv[1:])

