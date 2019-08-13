#!/bin/bash

tab_string="a\tb"
echo $tab_string

# sed法，sed法的好处在于可以指定tab的空格数量
echo $tab_string | sed 's/\t/    /g'

# tr法，tr法的问题在于tab只能替换成1个空格
echo $tab_string | tr  "\t" " "

# col法，col法的好处在于，系统会自动根据tab与空格的对应关系进行补齐
echo $tab_string | col -x

