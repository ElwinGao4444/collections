#!/usr/bin/python
#coding=utf-8

# for 1.10	from __future__ imports must occur at the beginning of the file
from __future__ import division	# 各个模块中的__future__方法都是用来确保2.1之前版本的Python可以正常运行一些新的语言特性，这里用来缺表“不会截断”

# 第一章 文本

# 1.1-每次处理一个字符
def fun1_1_1(char):
	return char,	# 不加逗号表示返回一个字符，加逗号表示返回元组

def chapter1_1():
	str = 'Hello World'
	print list(str)	# 将字符串拆分成list
	print ''.join(list(str))	# list(str)的反向操作，将list组合为字符串

	for char in str:	# 循环读取字符串的每个字符
		print char,	# print默认输出回车符，加逗号可避免输出回车符
	print ''

	result = [i for i in str]	# 利用for语句初始化list
	print result

	result = map(fun1_1_1, str)	# 每读取一个字符，调用一次函数
	print result	# 结果为每个fun1_1_1返回值的list
	result = map(lambda x : (x,), str)	# 上述方法的匿名函数实现
	print result	# 结果为每个fun1_1_1返回值的list

	pass

# 1.2-字符和字符值之间的转换
def chapter1_2():
	print ord('a')	# 将字符转换成其对应的ASCII码
	print chr(97)	# 将数组看作ASCII码，并将其转换为字符

	# \u2020也可以为中文或者其他Unicode字符
	print ord(u'\u2020')	# 将Unicode字符转换为Unicode编码
	print unichr(8824)	# 将Unicode编码转换为Unicode字符

	print repr(chr(97))	# chr将数字转换成ASCII字符
	print repr(str(97))	# str将数字直接转换成其对应的字符串

	print map(ord, 'hello')	# 使用map将字符串转换成为ASCII编码数组
	print ''.join(map(chr, range(97, 123)))	# 打印26个字母

	pass

# 1.3-测试一个对象是否是类字符串
def isStringLike(anobj):
	'''
	python的特色之一就是根据自己的预期去执行任务，在此过程中检测错误和异常。
	这是一个著名的处理方式，叫做“获取事后原谅总比事先得到许可要容易的多”
	'''

	try: anobj + ''	# 通过修改try的部分，可以检测更多的字符串特性
	except: return False
	else: return True

def chapter1_3():
	string = 'Hello World'
	print isinstance(string, basestring)	# basestring是str和unicode类似哪个的共同基类
	string = 'Hello World'
	print isinstance(string, str)	# str如果被定义为变量，就无法再作为类名使用
	string = 'Hello World'
	print isinstance(string, unicode)
	string = u'Hello World'
	print isinstance(string, unicode)

	print type(string) is type(u'')	# 虽然这种方法也可以进行类型测试，不过这种方法非常糟糕，尤其无法判断自定义类型
	print isStringLike(string)	# 这种方法比isinstance要慢的多，但是却适用于类字符串类型的实例

	pass

# 1.4-字符串对齐
def chapter1_4():
	'''
	ljust & r rjust: 左对齐或右对齐，参数表示填充范围，填充字符默认为空格
	center: 居中，参数表示填充范围和填充字符
	'''

	print '|', 'hej'.ljust(20), '|', 'hej'.rjust(20), '|', 'hej'.center(20, '+'), '|'

	pass

# 1.5-去除字符串两端的空格
def chapter1_5():
	'''
	lstrip, rstrip, strip
	分别去除左、右和两端的指定字符，默认为空格
	参数也可以指定字符列表
	'''

	print '|', '+hej+'.lstrip('+'), '|', '+hej+'.rstrip('+'), '|', '+hej+'.strip('+'), '|'
	print '|', 'xyxxyy hejyx yyx'.strip('xy'), '|'

	pass

# 1.6-合并字符串
def chapter1_6():
	'''
	python的字符串无法改变，只能创建新的字符串，所以，哪种处理方式中间结果最少，速度就越快
	由此来看，'+'运算的性能最差，需要N-1个中间结果。而join方式是最快的，只需要一个中间buf
	对于字符串拼接输出的情况，推荐使用格式化输出机制，可读性强、可扩展性强（避免了很多对数字的str()转换），并且效率上也不是问题
	'''
	print 'foo'+'bar'	# '+'操作对于字符串类型即为连接操作，当然'+='也是支持的
	print 'foo','bar'	# 逗号的的默认处理为空格分割
	print '%s%s' % ('foo', 'bar')	# 类C语言的格式化输出
	import operator
	print reduce(operator.add, ['x','y','z'], 'init')	# reduce的第三个参数为初始化值，为可选参数
	print ''.join(['foo', 'bar'])	# 将字符串变成序列，然后连接序列的元素也可以达到字符串连接的效果
	print 'foo'.join('bar')	# 这种用法是错的，结果为：bfooafoor，join将'bar'作为list，以'foo'为分割符进行重组

	pass

# 1.7-将字符串逐字符活逐词反转
def chapter1_7():
	# 逐字反转
	print 'hello'[::-1]

	wlist = list('hello')
	wlist.reverse()	# list的reverse方法只反转list元素，没有返回值
	print ''.join(wlist)
	print ''.join(list('hello')[::-1])	# 上述方法的一行表示

	print reduce(lambda x,y: y+x, 'hello')

	# 逐词反转
	wlist = 'hello world'.split()
	wlist.reverse()
	print ' '.join(wlist)
	print ' '.join(('hello world').split()[::-1])	# 上述方法的一行表示

	import re	# 正则表达式方式
	wlist = re.split(r'(\s+)', 'hello world')
	wlist.reverse()
	print ''.join(wlist)	# 正则将空格也作为反转元素，故join的时候，无需在使用空格作为连接符
	print ''.join(re.split(r'(\s+)', 'hello world')[::-1])	# 上述方法的一行表示

	pass

# 1.8-检查字符串中是否包含某字符集合中的字符
def containsAny(seq, aset):	# 判断aset中是否包含seq中的字符集合
	''' 检查序列seq是否含有aset中的项 '''
	for c in seq:
		if c in aset: return True
	return False

def containsOnly(seq, aset):
	''' 检查序列seq是否只有aset中的项 '''
	for c in seq:
		if c not in aset: return False
	return True

import string
notrans = string.maketrans('', '')	# maketrans: make map of attr1 to attr2
def containsAny2(astr, strset):
	return len(strset) != len(strset.translate(notrans, astr))	# translate: delete set of attr2, and translate by map table(attr1)
def containsAll(astr, strset):
	return not strset.translate(notrans, astr)

def chapter1_8():
	print 'abc' in '123zxcabcqweasd'	# in 关键字可以进行子串检测

	print containsAny('abc', '123zxcabcqweasd')
	print set( '123zxcabcqweasd').intersection('abc')	# 使用build-in的set数据类型实现containsAny的功能
	print bool(set( '123zxcabcqweasd').intersection('abc'))	# 这种找全集的方法如果仅仅为了判断就太慢了，上面的“短路法”（找到一个就返回）相对快的多

	print containsAny('abc', 'acbac')
	print set('acbacd').difference('abc')	# set.difference()表示取差集
	print not set('acbac').difference('abc')	# 使用build-in的set数据类型实现containsOnly的功能

	# 这个方法速度不过而且灵活易用，不过通用性不如上面的方法，它要求astr和strset都是普通字符串，甚至Unicode字符串都不行
	print containsAny2('abc', 'acbac')
	print containsAll('abc', 'acbacd')

	pass

# 1.9-简化字符串的translate方法的使用
import string
def translator(frm='', to='', delete='', keep=None):	# 函数无法满足同时指定delete和keep的情况，因为根本没有必要这么指定
	if len(to) == 1:
		to = to * len(frm)
	trans = string.maketrans(frm, to)
	if keep is not None:
		allchars = string.maketrans('', '')	# maketrans的返回值类型为字符串
		delete = allchars.translate(allchars, keep.translate(allchars,delete))	# 将keep之外的字符设定为delete
	def translate(s):
		return s.translate(trans, delete)
	return translate

def chapter1_9():
	digits_only = translator(keep=string.digits)
	print digits_only('Chris Perkings : 224-7992')

	no_digits = translator(delete=string.digits)
	print no_digits('Chris Perkings : 224-7992')

	digits_to_hash = translator(frm=string.digits, to='#')
	print digits_to_hash('Chris Perkings : 224-7992')

	# 同时指定delete和keep没有任何意义，二者都指定时可以选择忽略其一，或者抛出异常
	trans = translator(delete='abcd', keep='cdef')
	print trans('abcdefg')

	pass

# 过滤字符串中不属于指定集合的字符
import string
# 生成所有字符的可复用的字符串，它还可以作为一个翻译表，指明“无需翻译”
allchars = string.maketrans('', '')	# 作为全局变量，避免了多次拷贝，减少内存开销，提高效率
def makefilter(keep):
	'''
	返回一个函数，此返回函数接受一个字符串作为参数 
	并返回字符串的一个部分拷贝，此拷贝只包含在keep
	中的字符，注意keep必须是一个普通字符串
	'''

	# 生成一个由所有不再keep中的字符组成的字符串：
	# keep的补集，即所有我们需要删除的字符
	delchars = allchars.translate(allchars, keep)

	# 生成并返回需要的过滤函数（作为闭包）
	def thefilter(s):
		return s.translate(allchars, delchars)
	return thefilter

	# 上述内嵌函数返回的单行形式
	return lambda s: s.translate(allchars, delchars)

import sets
class Keeper(object):
	def __init__(self, keep):
		self.keep = sets.Set(map(ord, keep))
	def __getitem__(self, n):	# __getitem__(self,key)   使用x[key]索引操作符的时候调用。
		if n not in self.keep:
			return None
		return unichr(n)
	def __call__(self, s):		# __call__(self, *args)   called when an instance is called as function.
		return unicode(s).translate(self)

makefilter2 = Keeper

def chapter1_10():
	just_vowels = makefilter('aeiouy')
	print just_vowels('four score and seven years ago')	# output: ouoeaeeyeaao
	print just_vowels('tiger, tiger burning bright')	# output: ieieuii
	# 上述方法的单行实现
	print makefilter('aeiouy')('four score and seven years ago')
	print makefilter('aeiouy')('tiger, tiger burning bright')

	# 在直接处理字符串的任务中，string库的translate总是比set要快一些
	# 但遗憾的是不够通用，例如上述方法，对Unicode字符串不适用
	instr = u"自由民主制（Liberal democracy）是民主主义在21世纪的主导形式。它采取了代议民主制，由人民选出的民意代表行使决策权力，其权力则受到法治的监督和限制"
	trans_table = dict([[ord(char), u"*"] for char in u"民主"])	# unicode字符串只能适用dict作为映射表
	print trans_table
	filtered = unicode.translate(instr, trans_table)	# translate支持适用dict作为参数，而不仅仅是maketrans的返回值
	print filtered

	# 更好的办法是，编写一个简单的大致实现了__getitem__方法的类
	just_vowels = makefilter2('aeiouy')
	print just_vowels(u'four score and seven years ago')	# output: ouoeaeeyeaao
	print just_vowels(u'tiger, tiger burning bright')	# output: ieieuii

	pass

# 检查一个字符串是文本还是二进制
# from __future__ import division at head of file
import string
text_characters = ''.join(map(chr, range(32, 127))) + '\n\r\t\b'
_null_trans = string.maketrans('','')
def istext(s, text_characters=text_characters, threshold=0.30):
	# 若s包含了空值，它不是文本
	if '\0' in s:
		return False
	# 一个“空”字符串是“文本”（这是一个主管但又很合理的选择)
	if not s:
		return True
	# 获得s的由非文本字符构成的子串
	t = s.translate(_null_trans, text_characters)
	# 如果不超过30%的字符是非文本字符，s是字符串
	return len(t)/len(s) < threshold	# 如果没有"from __future__ import division"除法会被截断。故意截断可以适用'//'操作符

def chapter1_11():
	''' 这个问题并没有精确算法，完全是启发性的试探 '''
	print istext('')
	print istext('abcd')
	print istext('aa\0')
	print istext('aa\v\v')

	pass

# 控制大小写
def chapter1_12():
	print 'aaa'.upper()	# 对应的测试函数：isupper()
	print 'AAA'.lower()	# 对应的测试函数：islower()
	print 'one tWo thrEe'.capitalize()	# 没有对应的测试函数，可以简单的使用：s==s.capitalize()来判断，不过对于空字符串也会返回True
	print 'one tWo thrEe'.title()	# 对应的测试函数：istitle()

	pass

# 访问子字符串
import struct
def fields(baseformat, theline, lastfield=False):
	# theline超出的长度也是由这个base-format确定
	numremain = len(theline) - struct.calcsize(baseformat)
	# 用合适的s或x字段完成格式，然后unpack
	format = '%s %d%s' % (baseformat, numremain, lastfield and 's' or 'x')	#  lastfield and 's' or 'x'的效果等价于C语言中的 lastfield ? 's' : 'x'
	return struct.unpack(format, theline)

def split_by(theline, n, lastfield=False):
	# 切割所有需要的片段
	pieces = [theline[k:k+n] for k in xrange(0, len(theline), n)]
	# 若最后一段太短或不要，丢弃之
	if not lastfield and len(pieces[-1]) < n:
		pieces.pop()
	return pieces

def split_at(theline, cuts, lastfield=False):
	# 切割所有需要的片段
	pieces = [ theline[i:j] for i, j in zip([0]+cuts, cuts+[None]) ]
	# 若最后一段太短或不要，丢弃之
	if not lastfield:
		pieces.pop()
	return pieces

def chapter1_13():
	print 'hello world'[3:8]	# 切片

	theline = 'abcdefghijklmnopqrstuvwxyz'
	baseformat = '5s 3x 8s 8s'	# 得到一个5字节的字符串，跳过3字节，得到两个8字节字符串，以及其余部分：
	numremain = len(theline) - struct.calcsize(baseformat)	# theline超出的长度也是由这个base-format确定
	format = '%s %ds' % (baseformat, numremain)
	l, s1, s2, t = struct.unpack(format, theline)
	print l, s1, s2, t
	l, s1, s2 = struct.unpack(baseformat, theline[:struct.calcsize(baseformat)])	# 忽略“其余部分”
	print l, s1, s2

	# 将上面的功能封装成一个函数
	l, s1, s2, t = fields(baseformat, theline, True)
	print l, s1, s2, t
	l, s1, s2 = fields(baseformat, theline)
	print l, s1, s2

	# xrange返回生成器，每次调用返回一个值；而range返回list。所以在循环效率上xrange要高的多，而且不用开辟一个list的空间
	print xrange(0, 10, 5)
	print list(xrange(0, 10, 5))
	print range(0, 10, 5)

	print ['0123456789abc'[k:k+5] for k in xrange(0, 13, 5)]	# 切片越界也不会导致程序错误，但这样编程非常业余

	# 将上面的功能封装成一个函数
	print split_by('0123456789abc', 5)

	# zip函数返回一个list，其中每项都是形如(cuts[k], cuts[k+1])这样的数对，除了第一项和最后一项，这两项分别是(0, cuts[0])和(cuts[len(cuts)-1], None)
	cuts = [8, 14, 20, 26, 30]
	print [ '000111222333444555666777888999xyz'[i:j] for i, j in zip([0]+cuts, cuts+[None]) ]

	# 将上面的功能封装成一个函数
	print split_at('000111222333444555666777888999xyz', cuts)

	pass

# 改变多行文本字符串的缩进
def reindent(s, numSpaces):
	leading_space = numSpaces * ' '
	lines = [ leading_space + line.lstrip() for line in s.splitlines() ]
	return '\n'.join(lines)

# more functions about reindent
def addSpaces(s, numAdd):
	white = ' ' * numAdd
	return white + white.join(s.splitlines(True))
def numSpaces(s):
	return [len(line)-len(line.lstrip()) for line in s.splitlines()]
def delSpaces(s, numDel):
	if numDel > min(numSpaces(s)):
		raise ValueError, 'removing more spaces than there are!'
	return '\n'.join([line[numDel:] for line in s.splitlines()])
def unIndentBlock(s):
	return delSpaces(s, min(numSpaces(s)))

def chapter1_14():
	x = '  line one line\n  two and\n  line three'
	print x
	print reindent(x, 4)

	print addSpaces(x,1)
	print numSpaces(x)
	print delSpaces(x, 1)
	print unIndentBlock(x)

	pass

# 扩展和压缩制表符
def unexpand(astring, tablen=8):
	import re
	# 切分成空格和非空格的序列
	pieces = re.split(r'( +)', astring.expandtabs(tablen))
	# 记录目前字符串的总长度
	lensofar = 0
	for i, piece in enumerate(pieces):
		thislen = len(piece)
		lensofar += thislen
		if piece.isspace():
			# 将各个空格序列改成tabs+spaces
			numblanks = thislen % tablen
			numtabs = (thislen-numblanks)//tablen
			pieces[i] = '\t'*numtabs + ' '*numblanks
	return ''.join(pieces)

def chapter1_15():
	# 将制表符替换为空格
	print len('\tabc'.expandtabs())
	print unexpand('        abc d        e')
	print len('        abc d        e')
	print len(unexpand('        abc d        e'))

	pass

# 替换字符串中的子串
def expand(format, d, marker="'", safe=False):
	if safe:
		def loopup(w): return d.get(w, w.join(marker*2))	# 不存在返回默认值
	else:
		def lookup(w): return d[w]	# 不存在抛出异常
	parts = format.split(marker)
	parts[1::2] = map(lookup, parts[1::2])	# parts[1::2]绑定字符串的奇数项索引，当前语句只修改list的奇数项
	return ''.join(parts)

def chapter1_16():
	print expand("just 'a' test", {'a': 'one'})
	
	pass

# 替换字符串中的字串——Python 2.4
import string
def chapter1_17():
	# 从字符串生成模板，其中标识符被$标记
	new_style = string.Template('this is $thing')
	# 给模板的substitute方法传入一个字典参数并调用之
	print new_style.substitute({'thing':5})
	print new_style.substitute({'thing':'test'})
	# 另外，也可以给substitute方法传递关键字参数
	print new_style.substitute(thing=5)
	print new_style.substitute(thing='test')

	# 使用两个$来对$自身进行转意，即两个$表示打印一个$
	from_letter = '''Dear $customer,
	i hope you are having a great time.
	if you do not find Room $room to your satisaction,
	let us know. Please accept this $$5 coupon.
			Sincerely,
			$manager
			${name}Inn'''
	letter_template = string.Template(from_letter)
	print letter_template.substitute({'name':'Sleepy', 'customer':'Fred Smith','manager':'Barney Mills', 'room':307,})

	# locals()函数返回将本地变量及其值所组成的字典
	msg = string.Template('the square of $number is $square')
	for number in range(10):
		square = number * number
		print msg.substitute(locals())
	# 另一个方法是使用关键字而非字典
	msg = string.Template('the square of $number is $square')
	for i in range(10):
		print msg.substitute(number=i, square=i*i)
	# 甚至可以同时传递字典和关键字参数
	msg = string.Template('the square of $number is $square')
	for number in range(10):
		print msg.substitute(locals(), square=number*number)
	# 为了防止字典的条目与关键字参数显式传递的值发生冲突，关键字参数优先
	msg = string.Template('an $adj $msg')
	adj = 'interesting'
	print msg.substitute(locals(), msg='message')


	pass

# 一次完成多个替换
import re
def multiple_replace(text, adict):
	rx = re.compile('|'.join(map(re.escape, adict)))	# map函数传dict参数时，只遍历dict的key，而不管value
	def one_xlat(match):
		return adict[match.group(0)]
	return rx.sub(one_xlat, text)	# sub的第一个参数可以是字符串，也可以是函数

def make_xlat(*args, **kwds):
	''' 当对多个文本使用统一个规则时，使用基于闭包的方式只需做一次准备工作，用于提高效率 '''
	adict = dict(*args, **kwds)	# 带*和**使用参数变量，表示原样传递，这与不加*或**含义完全不同（比较复杂，不赘述）
	rx = re.compile('|'.join(map(re.escape, adict)))
	def one_xlat(match):
		return adict[match.group(0)]
	def xlat(text):
		return rx.sub(one_xlat, text)
	return xlat
class make_xlat_class:
	''' 当正则表达式经常修改时，我们可以利用类的重载来避免大量的代码复制
	    虽然函数更加简单快速，但是类可以让代码更加清晰简洁 '''
	def __init__(self, *args, **kwds):
		self.adict = dict(*args, **kwds)
		self.rx = self.make_rx()
	def make_rx(self):
		return re.compile('|'.join(map(re.escape, self.adict)))
	def one_xlat(self, match):
		return self.adict[match.group(0)]
	def __call__(self, text):
		return self.rx.sub(self.one_xlat, text)
class make_xlat_by_whole_words(make_xlat_class):
	def make_rx(self):
		return re.compile(r'\b%s\b' % r'\b|\b'.join(map(re.escape, self.adict)))
def chapter1_18():
	text = 'Larry Wall is the creator of Perl'
	adict = {
			'Larry Wall' : 'Guido van Rossum',
			'creator' : 'Benevolent Dictator for Life',
			'Perl' : 'Python'
			}

	print multiple_replace(text, adict)
	translate = make_xlat(adict)
	print translate(text)

	translate = make_xlat_class(adict)
	print translate(text)
	translate = make_xlat_by_whole_words(adict)
	print translate(text)

	pass

# 检查字符串中结束标记
import itertools	# itertools的高效性需要对该模块进行深入学习
def anyTrue(predicate, sequence):	# itertools.imap给出了一种快速方便的解决办法
	return True in itertools.imap(predicate, sequence)
def endsWith(s, *endings):
	return anyTrue(s.endswith, endings)
def chapter1_19():
	print 'hello world'.endswith('ld')	# 这是一种最直接，但是最笨重的方法
	print endsWith('hello world', 'ld')

	pass

# 使用Unicode来处理国际化文本
def chapter1_20():
	german_ae = unicode('\xc3\xa4', 'utf8')
	print german_ae

	# 可以像处理普通str字符串那样操纵unicode字符串
	s = 'this is a ' + german_ae
	s2 = 'easy!'
	para = '. '.join([s, s2])
	print para

	pass

#	bytestring = '\xc3\xa4'	# 这是个非ASCII字符串，这种操作将导致抛出异常
#	german_ae += bytestring

# 在Unicode和普通字符串之间转换
def chapter1_21():
	unicodestring = u'Hello World'
	# 将Unidoce转化为普通字符串: "encode"
	utf8string = unicodestring.encode('utf-8')
	asciistring = unicodestring.encode('ascii')
	isostring = unicodestring.encode('ISO-8859-1')
	utf16string = unicodestring.encode('utf-16')
	# 将普通字符串转化为unicode: "decode"
	plainstring1 = unicode(utf8string, 'utf-8')
	plainstring2 = unicode(asciistring, 'ascii')
	plainstring3 = unicode(isostring, 'ISO-8859-1')
	plainstring4 = unicode(utf16string, 'utf-16')
	assert plainstring1 == plainstring2 == plainstring3 == plainstring4

	pass

# 在标准输出中打印Unicode字符
import codecs, sys
def chapter1_22():
	# 将sys.stdout流用转换器包装起来
	old = sys.stdout
	sys.stdout = codecs.lookup('iso8859-1')[-1](sys.stdout)
	sys.stdout = old

	# 输出一个unicode字符串
	# 如果抛出异常（由于Python没有自动识别出正确的编码），则需要使用上述方法对stdout进行包装
	char = u'\N{LATIN SMALL LETTER A WITH DIAERESIS}'
	print char

	pass

# 对Unicode数据编码并用于XML和HTML
def encode_for_xml(unicode_data, encoding = 'ascii'):
	''' python提供了一种编码错误处理工具，叫做xmlcharrefreplace，
	他会将所有不属于所选编码的字符用XML的数字字符引用来代替 '''
	return unicode_data.encode(encoding, 'xmlcharrefreplace')

import codecs
from htmlentitydefs import codepoint2name
def html_replace(exc):
	''' 注册一个自定义的编码错误处理函数，用于处理HTML输出 '''
	if isinstance(exc, (UnicodeEncodeError, UnicodeTranslateError)):
		s = [u'&%s;' % codepoint2name[ord(c)]
				for c in exc.object[exc.start:exc.end]]
		return ''.join(s), exc.end
	else:
		raise TypeError("can't handle %s" % exc.__name__)

def encode_for_html(unicode_data, encoding = 'ascii'):
	return unicode_data.encode(encoding, 'html_replace')
		
def chapter1_23():
	data = u'''\
<html>
	<head>
		<title>Encoding Test</title>
	</head>
	<body>
		<p>accented characters:
		<ul>
			<li>\xe0 (a + grave)
			<li>\xe7 (c + cedilla)
			<li>\xe9 (e + acute)
		</ul> 
		<p>symbols:
		<ul>
			<li>\xa3 (British pound)
			<li>\u20ac (Euro)
			<li>\u221e (infinity)
		</ul>
	</body>
</html>''' 
	print encode_for_xml(data)
	codecs.register_error('html_replace', html_replace)
	print encode_for_html(data)

	pass

# 让某些字符串大小写不敏感
class iStr(str):
	'''
	大小写不敏感的字符串类
	行为方式类似于str，只是所有的比较和查询
	都是大小写不敏感的
	'''
	def __init__(self, *args):
		self._lowered = str.lower(self)
	def __repr__(self):
		return '%s(%s)' % (type(self).__name__, str.__repr__(self))
	def __hash__(self):
		return hash(self._lowered)
	def lower(self):
		return self._lowered

def _make_case_insensitive(name):
	''' 将str的方法封装成iStr的方法，大小写不敏感 '''
	str_meth = getattr(str, name)
	def x(self, other, *args):
		'''
		先尝试将other小写化，通常这应该是一个字符串，
		但必须要做好准备应付这个过程中出现的错误，
		因为字符串是可以和非字符串正确比较的
		'''
		try : other = other.lower()
		except (TypeError, AttributeError, ValueError):pass
		return str_meth(self._lowered, other, *args)
	# 仅python 2.4，增加一条语句：x.func_name = name
	setattr(iStr, name, x)

def _make_return_iStr(name):
	''' 将所有的返回字符串的iStr方法封装
	使得所有的返回结果都是iStr类型，而不是默认的str类型'''
	str_meth = getattr(str, name)
	def x(*args):
		return iStr(str_meth(*args))
	setattr(iStr, name, x)

def chapter1_24():
	global _make_case_insensitive	# 不声明global将del时将报错
	for name in 'eq lt le gt gt ne contains'.split():
		_make_case_insensitive('__%s__' % name)
	for name in 'count endswith find index rfind rindex startswith'.split():
		_make_case_insensitive(name)
	# 注意，我们并不修改replace、split、strip等方法
	# 当然，如果有需要，也可以对它们进行修改
	del _make_case_insensitive	# 删除帮数函数，已经不再需要了

	s1 = 'hello'
	s2 = 'HELLO'
	print s1 == s2

	s1 = iStr('hello')
	s2 = iStr('HELLO')
	print s1 == s2

	print type(s1.strip())
	for name in 'center ljust rjust strip lstrip rstrip'.split():
		_make_return_iStr(name)
	print type(s2.strip())

	# 书中P51页，还有一个基于iStr实现大小写无关的iList

	pass

# 将HTML文档转化为文本显示到UNIX终端上
import sys, os, htmllib, formatter
# 使用UNIX的tput来获得粗体、下划线和重设的转义序列
set_bold = os.popen('tput bold').read()
set_underline = os.popen('tput smul').read()
perform_reset = os.popen('tput sgr0').read()
class TtyFormatter(formatter.AbstractFormatter):
	''' 一个保留粗体和斜体状态的格式化对象
	并输出相应的终端控制序列 '''
	def __init__(self, writer):
		# 首先，像往常一样，初始化超类
		formatter.AbstractFormatter.__init__(self, writer)
		# 一开始既没有粗体也没有斜体状态，未保留任何信息
		self.fontState = False, False
		self.fontStack = []
	def push_font(self, font):
		# font元组有4项， 我们只看与粗体和斜体的状态有关的两个标志
		size, is_italic, is_bold, is_tt = font
		self.fontStack.append((is_italic, is_bold))
		self._updateFontState()
	def pop_font(self, *args):
		# 回到前一个font状态
		try:
			self.fontStack.pop()
		except IndexError:
			pass
		self._updateFontState
	def updateFontState(self):
		# 输出正确的终端控制序列，如果粗体和/或斜体(==underline)
		# 的状态刚刚被改变的话
		try:
			newState = self.fontStack[-1]
		except IndexError:
			newState = False, False
		if self.fontState != newState:
			# 相关的状态改变：重置终端
			print perform_reset,
			# 如果需要的话，设置下划线与/或粗体状态
			if newState[0]:
				print set_underline,
			if newState[1]:
				print set_bold,
			# 记住当前的两个状态
			self.fontState = newState
def chapter1_25():
	# 生成写入、格式化、解析对象，根据需要将他们连接起来
	myWriter = formatter.DumbWriter()
	if sys.stdout.isatty():
		myFormatter = TtyFormatter(myWriter)
	else:
		myFormatter = formatter.AbstractFormatter(myWriter)
	myParser = htmllib.HTMLParser(myFormatter)
	# 将标准输入和终端操作提供给解析器
	myParser.feed(sys.stdin.read())
	myParser.close()

	pass

def main():
#	chapter1_1()
#	chapter1_2()
#	chapter1_3()
#	chapter1_4()
#	chapter1_5()
#	chapter1_6()
#	chapter1_7()
#	chapter1_8()
#	chapter1_9()
#	chapter1_10()
#	chapter1_11()
#	chapter1_12()
#	chapter1_13()
#	chapter1_14()
#	chapter1_15()
#	chapter1_16()
#	chapter1_17()
#	chapter1_18()
#	chapter1_19()
#	chapter1_20()
#	chapter1_21()
#	chapter1_22()
#	chapter1_23()
#	chapter1_24()
	chapter1_25()	# 这个实验没看懂

	pass

if __name__ == '__main__':
	main()
