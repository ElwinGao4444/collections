package main

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/printer"
	"go/token"
	"os"

	"golang.org/x/tools/go/ast/astutil"
)

func main() {
	inputContent, _ := os.ReadFile("./main.go") // 读取输入文件内容
	fset := token.NewFileSet()
	node, _ := parser.ParseFile(fset, "", inputContent, parser.ParseComments) // 解析代码为 AST
	// node.Name = ast.NewIdent(packageName) // 修改包名

	// 定义 visitor 函数
	visitor := func(cursor *astutil.Cursor) bool {
		n := cursor.Node()
		fmt.Println("visitor:", n)
		return true
	}

	node = astutil.Apply(node, nil, visitor).(*ast.File) // 应用 visitor 到 AST

	output, _ := os.Create("output.go") // 创建输出文件
	defer output.Close()

	printer.Fprint(output, fset, node) // 将修改后的 AST 写入输出文件
}
