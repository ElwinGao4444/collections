#!/usr/bin/python

# 该脚本从bing的rss文件中获取背景图片的URL，并将其下载到本地
# 由于该rss的地址被墙，所以需要代理的帮助

import sys
import re
import time
import urllib2
	
bingimage = 'http://feeds.feedburner.com/bingimages'                            
proxy = {'http', '127.0.0.1:8087'}
pattern = r'(http://((?!http://)(?!cache).)+?\.jpg)'

def main():
	# set urllib proxy
	handler = urllib2.ProxyHandler({'http' : 'http://:@127.0.0.1:8087'})
	opener = urllib2.build_opener(handler, urllib2.HTTPHandler)
	urllib2.install_opener(opener)

	# read from url
	print 'getting images url...',
	sys.stdout.flush()
	response = urllib2.urlopen(bingimage)
	html = response.read()
	print '\tok~'

	# get image url from html
	match = re.findall(pattern, html)
	image_list = [e[0] for e in match]

	# downlaod all image
	for i in image_list:
		current_time = time.strftime('%Y-%m-%d', time.localtime(time.time()))
		file_name = current_time + '_'  + i[i.rfind('%')+3:]

		print 'downlaoding ' + file_name + '...',
		sys.stdout.flush()
		response = urllib2.urlopen(i)
		image = response.read()
		fp = open(file_name, "wb")
		fp.write(image)
		fp.close()
		print '\tok~'

if __name__ == '__main__':
	main()

