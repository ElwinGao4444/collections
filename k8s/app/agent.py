#!/usr/bin/env python
# -*- coding=utf8 -*-
"""
# Author: Elwin.Gao
# Created Time : Sat Jun 14 13:35:08 2025
# File Name: agent.py
# Description:
"""

import time
import logging
from http.server import HTTPServer, BaseHTTPRequestHandler

class MyHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

        logging.info(f"recv: {self.path}")
        self.wfile.write(f'agent recv: {self.path}'.encode())

if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG, format='%(levelname)s:%(asctime)s:%(message)s', datefmt='%Y-%d-%m %H:%M:%S')
    server = HTTPServer(('', 8080), MyHandler)
    server.serve_forever()
