package main

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/printer"
	"go/token"
	"os"
	"reflect"

	"golang.org/x/tools/go/ast/astutil"
)

func printNode(cursor *astutil.Cursor) bool {
	n := cursor.Node()
	fmt.Println("debug:", reflect.TypeOf(n))
	return true // 返回 false 会终止遍历
}

func changeName(cursor *astutil.Cursor) bool {
	n := cursor.Node()
	funcDecl, ok := n.(*ast.FuncDecl)
	if ok {
		fundIdent := funcDecl.Name
		if fundIdent.Name == "main" {
			fmt.Println("visitor fundIdent:", fundIdent)
			fundIdent.Name = "main_out" // 修改函数名
			return false                // 终止遍历
		}
	}
	return true // 返回 false 会终止遍历
}

func main() {
	inputContent, _ := os.ReadFile("./main.go") // 读取输入文件内容
	fset := token.NewFileSet()
	node, _ := parser.ParseFile(fset, "", inputContent, parser.ParseComments) // 解析代码为 AST
	ast.Print(fset, node)                                                     // 打印语法树
	fmt.Println("----------------")
	node.Name = ast.NewIdent("out") // 修改包名

	node = astutil.Apply(node, nil, printNode).(*ast.File)  // 打印语法树
	node = astutil.Apply(node, nil, changeName).(*ast.File) // 修改函数名

	os.MkdirAll("out", 0755)                // 创建输出目录
	output, _ := os.Create("out/output.go") // 创建输出文件
	defer output.Close()

	printer.Fprint(output, fset, node) // 将修改后的 AST 写入输出文件
}
