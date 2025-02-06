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

func printTree(fset *token.FileSet, node *ast.File) {
	ast.Print(fset, node) // 打印语法树
	fmt.Println("----------------")
}

func inspectTree(fset *token.FileSet, node *ast.File) {
	ast.Inspect(node, func(n ast.Node) bool {
		var s string
		switch x := n.(type) {
		case *ast.BasicLit:
			s = x.Value
		case *ast.Ident: // 获取属性
			s = x.Name // 此处获取
		}
		if s != "" {
			fmt.Printf("%s:\t%s\n", fset.Position(n.Pos()), s)
		}
		return true
	})
}

func applyTree(file *ast.File) *ast.File {
	file.Name = ast.NewIdent("out") // 修改包名
	file = astutil.Apply(file, nil, func(cursor *astutil.Cursor) bool {
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
	}).(*ast.File) // 修改函数名
	return file
}

func main() {
	inputContent, _ := os.ReadFile("./main.go") // 读取输入文件内容
	fset := token.NewFileSet()
	file, _ := parser.ParseFile(fset, "", inputContent, parser.ParseComments) // 解析代码为 AST

	printTree(fset, file)
	inspectTree(fset, file)
	file = applyTree(file)

	os.MkdirAll("out", 0755)                // 创建输出目录
	output, _ := os.Create("out/output.go") // 创建输出文件
	defer output.Close()

	printer.Fprint(output, fset, file) // 将修改后的 AST 写入输出文件
}
