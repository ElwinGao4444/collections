#!/usr/bin/env python
# -*- coding=utf8 -*-
"""
# Author: Elwin.Gao
# Created Time : Thu Oct 24 14:39:45 2024
# File Name: plugin.py
# Description: chmod +x plugin.py && protoc -I./ message.proto --diy_out=. --plugin=protoc-gen-diy=plugin.py
"""

import sys
import json
import logging

from google.protobuf.compiler.plugin_pb2 import CodeGeneratorResponse, CodeGeneratorRequest
from google.protobuf.descriptor_pb2 import FileDescriptorProto

logger = logging.getLogger(__name__)

def simple_demo():
    request = CodeGeneratorRequest.FromString(sys.stdin.buffer.read())  #0 读取结构化的pb信息
    logger.info(f"debug: {request}")
    response = CodeGeneratorResponse()                    #1 构建一个空response
    generated_file = response.file.add()                  #2 创建一个输出文件
    generated_file.name = "hello_world.txt"               #3 配置文件名
    generated_file.content = "Greetings, world!"          #4 写入文件内容
    sys.stdout.buffer.write(response.SerializeToString()) #5 通过标准输出返回生成内容

def process_file(file: FileDescriptorProto, response: CodeGeneratorResponse) -> None:
    logger.info(f"Processing proto_file: {file.name}")

    # Create dict of options
    options = str(file.options).strip().replace("\n", ", ").replace('"', "")
    options_dict = dict(item.split(": ") for item in options.split(", ") if options)

    # Create list of dependencies
    dependencies_list = list(file.dependency)

    data = {
        "package": f"{file.package}",
        "filename": f"{file.name}",
        "dependencies": dependencies_list,
        "options": options_dict,
    }

    out_file = response.file.add()
    out_file.name = file.name + ".json"
    logger.info(f"Creating new file: {out_file.name}")
    out_file.content = json.dumps(data, indent=2) + "\n"

def process(request: CodeGeneratorRequest, response: CodeGeneratorResponse) -> None:
    for file in request.proto_file:
        process_file(file, response)

if __name__ == "__main__":
    logging.basicConfig(stream=sys.stderr, level=logging.INFO)  # 由于protoc是要读取plugin的stdout，所以必须提前将logging的输出重定向到stderr
    request = CodeGeneratorRequest.FromString(sys.stdin.buffer.read())  # plugin从日志中
    response = CodeGeneratorResponse()
    process(request, response)
    sys.stdout.buffer.write(response.SerializeToString())
