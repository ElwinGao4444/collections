#!/usr/bin/env python
# -*- coding=utf8 -*-
"""
# Author: Elwin.Gao
# Created Time : Tue Mar 11 18:00:21 2025
# File Name: ccls.py
# Description:
"""

import os
import sys
import json
import subprocess
import threading
from queue import Queue
from pylsp_jsonrpc.streams import JsonRpcStreamReader, JsonRpcStreamWriter

class ccls:

    # 构造函数
    def __init__(self):
        self.request_id = 0
        self.reader = None
        self.writer = None
        self.process = None
        self.indexing_done = threading.Event()
        self.message_queue = Queue()

    def start(self):
        # 启动ccls server
        project_root = os.getcwd()
        ccls_command = ['ccls', '-init={"index":{"onChange":true}}', '--log-file=./ccls.log', '-v=2']
        self.process = subprocess.Popen(ccls_command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        # 将stdin和stdout嫁接json-rpc
        self.reader = JsonRpcStreamReader(self.process.stdout)
        self.writer = JsonRpcStreamWriter(self.process.stdin)

        # 启动reader线程
        reader_thread = threading.Thread(target=self.reader.listen, args=(self.message_consumer,))
        reader_thread.daemon = True
        reader_thread.start()

        # Initialize the server
        init_params = {"rootUri": f"file://{os.path.abspath(project_root)}"}

        self.send_request("initialize", init_params)

    def stop(self):
        # Shut down the server
        self.send_request("shutdown", None)
        self.send_notification("exit", None)

        # Close the ccls process
        self.process.terminate()
        self.reader.close()
        self.writer.close()


    # 定义reader的消息处理函数
    def message_consumer(self, message):
        print(f"Received: {json.dumps(message, indent=2)}")
        self.indexing_done.set()

    # 定义writer的消息发送函数
    def send_request(self, method, params):
        self.request_id = self.request_id + 1
        request = {
            "jsonrpc": "2.0",
            "id": self.request_id,
            "method": method,
            "params": params
        }
        print("sending request:", json.dumps(request, indent=2))
        self.writer.write(request)

    def send_notification(self, method, params):
        notification = {
            "jsonrpc": "2.0",
            "method": method,
            "params": params
        }
        print("sending notification:", json.dumps(notification, indent=2))
        self.writer.write(notification)

    def info(self, file):
        index_params = {"textDocument": {"uri": f"file://{file}"}}
        self.send_request("$ccls/info", index_params)

    def symble(self, file):
        symble_params = {"textDocument": {"uri": f"file://{file}"}}
        self.send_request("textDocument/documentSymbol", symble_params)

    def did_open(self, file):
        did_open_params = {
        "textDocument": {
            "uri": f"file://{file}"},
            "languageId": "cpp",
            "version": 1,
        }
        self.send_notification("textDocument/didOpen", did_open_params)

    def did_close(self, file):
        did_close_params = {
        "textDocument": {"uri": f"file://{file}"}}
        self.send_notification("textDocument/didClose", did_close_params)

    def goto_defination(self, file, line, column):
        definition_params = {
            "textDocument": {"uri": f"file://{file}"},
            "position": {"line": int(line), "character": int(column)},
        }
        self.send_request("textDocument/definition", definition_params)

    def find_reference(self, file, line, column):
        reference_params = {
            "textDocument": {"uri": f"file://{file}"},
            "position": {"line": int(line), "character": int(column)},
            "context": {"includeDeclaration": True},
        }
        self.send_request("textDocument/references", reference_params)

if __name__ == '__main__':
    obj = ccls()
    obj.start()
    cmd_map = {
            "i": obj.info,
            "s": obj.symble,
            "o": obj.did_open,
            "c": obj.did_close,
            "d": obj.goto_defination,
            "r": obj.find_reference,
            }
    while True:
        input_list = input().strip().split(" ")
        print('--------------------------------')
        cmd_map[input_list[0]](*tuple(input_list[1:]))
        print('--------------------------------')
