#!/bin/bash

# 方法一：时间戳法（date）

## 在某些低速情况下，时间戳可以直接作为随机数使用，精度为秒
date +%s
## 在高速情况下，可以使用时间戳的纳秒部分，这种情况下，随机效果更好
date +%N
## 也可以混合使用上述两种方法，拼接为完整时间戳
date +%s%N
## 时间戳法配合md5sum命令，也可用于生成随机字符串
date +%s%N | md5sum | cut -d ' ' -f 1

echo ' '


# 方法二：环境变量法（$RANDOM）

# 使用环境变量的前提是，有系统的支持，比较奇怪的是，该命令可以直接使用，但是在shell脚本中，却无法使用
echo $RANDOM

echo ' '


# 方法三：系统随机设备法（/dev/urandom, /dev/random）

# /dev/random依赖系统中断，随机数产生速度慢，但效果好
# /dev/urandom不依赖系统中断，随机数产生速度快，但随机效果不好
tr -cd 0-9 < /dev/urandom | head -c 8 && echo ''
# 设备法的好处在于，不仅可以生成随机数，还能生成随机字符串，只需修改过滤条件即可
tr -cd [:alnum:] < /dev/urandom | head -c 8 && echo ''

echo ' '


# 方法四：uuid法

# 使用系统提供的uuid生成唯一ID（格式为：8-4-4-4-12 的32个字符）
cat /proc/sys/kernel/random/uuid
# 由于uuid为字符串，需要cksum配合使用，利用CRC校验生成随机数
cat /proc/sys/kernel/random/uuid | cksum | cut -d ' ' -f 1

