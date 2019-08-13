#---------------- 编译路径  ----------------
BIN_DIR		=.
SRC_DIR		=.
THIRD_LIB	=../third_party_lib

# 说明：如果在编译时，源文件不在当前路径，则在使用gcov时需要使用-o参数来指定源文件路径
#       另外，gcov只能在当前路径下生成.gcov文件，而无法指定生成文件的路径
#       所以在开发时，为了方便调试和测试，应该将makefile文件复制于src路径下，并相应修改路径

#---------------- 编译链接参数  ----------------
CC = cc

RELEASE	= $(CC)	# release版本基本编译命令
R_OPT	= -O2	# release版本优化参数
R_WARN	= -Wall	# release版本警告等级
R_DEF	= # release版本编译宏
R_FLAG	= # release版本附加编译参数
R_PARAM	= $(R_OPT) $(R_WARN) $(R_DEF) $(R_FLAG)
R_LINK	= $(CC)	-o	# release版本链接参数
R_SYS	= -lexpat	# release版本系统动态库，例如：-lm, -rt, -ldl, -lpthread等等

DEBUG	= $(CC)	# debug版本基本编译命令
D_OPT	= -O0 -g3	# debug版本优化参数
D_WARN	= -ansi -pedantic -Wall -Wextra -Wc++-compat -Wconversion	# debug版本警告等级
D_DEF	= -DPROJECT_DEBUG -DPROJECT_TEST	# debug版本编译宏
D_FLAG	= -fno-strict-aliasing -fprofile-arcs -ftest-coverage 	# debug版本附加编译参数
D_PARAM	= $(D_OPT) $(D_WARN) $(D_DEF) $(D_FLAG) -std=gnu89
D_LINK	= $(CC) -o	# release版本链接参数
D_SYS	= -lexpat -lgcov	# release版本系统动态库，例如：-lm, -rt, -ldl, -lpthread等等

INCLUDE = -I $(THIRD_LIB)/cunit-2.1-2/include -I $(THIRD_LIB)/log4c-1.2.2/include	# 头文件路径
STATIC_LIB = $(THIRD_LIB)/cunit-2.1-2/lib/libcunit.a $(THIRD_LIB)/log4c-1.2.2/lib/liblog4c.a
OBJECT = $(SRC_DIR)/test_driver.o $(SRC_DIR)/demo_test.o $(SRC_DIR)/demo.o

#---------------- make标签及依赖规则  ----------------
all : debug

$(SRC_DIR)/%.o : $(SRC_DIR)/%.c
	$(RELEASE) $(R_PARAM) $(INCLUDE) -c $< -o $@

$(SRC_DIR)/%.debug.o : $(SRC_DIR)/%.c
	$(DEBUG) $(D_PARAM) $(INCLUDE) -c $< -o $@

release : $(OBJECT) $(STATIC_LIB)
	$(R_LINK) $(BIN_DIR)/release $^ $(R_SYS)
	@echo ''

debug : $(OBJECT:.o=.debug.o) $(STATIC_LIB)
	$(D_LINK) $(BIN_DIR)/debug $^ $(D_SYS)
	@echo ''

clean:
	rm -f $(BIN_DIR)/release
	rm -f $(BIN_DIR)/debug 
	rm -f $(OBJECT)
	rm -f $(OBJECT:.o=.debug.o)
	rm -f core

cleanall : clean
	rm -f $(SRC_DIR)/*.gcda 
	rm -f $(SRC_DIR)/*.gcno 
	rm -f $(SRC_DIR)/*.gcov
