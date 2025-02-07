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
		switch t := cursor.Node().(type) {
		case *ast.FuncDecl:
			var funcIdent = t.Name
			if funcIdent.Name == "main" {
				fmt.Println("funcIdent:", funcIdent)
				funcIdent.Name = "main_out" // 修改函数名
				return false                // 终止遍历
			}
		}
		return true // 返回 false 会终止遍历
	}).(*ast.File) // 修改函数名
	return file
}

func basic_usage() {
	inputContent, _ := os.ReadFile("main.go") // 读取输入文件内容
	fset := token.NewFileSet()
	file, _ := parser.ParseFile(fset, "main.go", inputContent, parser.ParseComments) // 解析代码为 AST(第二个参数和第三个参数，任选1个都可以作为输入代码)

	printTree(fset, file)
	inspectTree(fset, file)
	file = applyTree(file)

	os.MkdirAll("out", 0755)                // 创建输出目录
	output, _ := os.Create("out/output.go") // 创建输出文件
	defer output.Close()

	printer.Fprint(output, fset, file) // 将修改后的 AST 写入输出文件
}

func marcos_replace() {
	var src = `                                                                                                                                                                                                                              
package tmp
const a_b_c = 1.0
var x = W(a_b_c)
`
	var _ = `                                                                                                                                                                                                                              
package tmp
const a_b_c = 1.0
var x = func(){return a_b_c}()
`
	fset := token.NewFileSet()
	file, _ := parser.ParseFile(fset, "", src, 0)
	ast.Print(fset, file)

	file = astutil.Apply(file, nil, func(cursor *astutil.Cursor) bool {
		switch t := cursor.Node().(type) {
		case *ast.CallExpr:
			var name = t.Fun.(*ast.Ident).Name
			var args = t.Args[0].(*ast.Ident).Name
			fmt.Println("target:", name, args)
			var callExpr = &ast.CallExpr{
				Fun: &ast.FuncLit{
					Type: &ast.FuncType{},
					Body: &ast.BlockStmt{
						List: []ast.Stmt{
							&ast.ReturnStmt{
								Results: []ast.Expr{
									&ast.Ident{
										Name: args,
									},
								},
							},
						},
					},
				},
			}
			cursor.Replace(callExpr) // Replace的参数语法，直接把目标ast打印出来，对着打印结果照葫芦画瓢即可
		}
		return true // 返回 false 会终止遍历
	}).(*ast.File) // 修改函数名

	printer.Fprint(os.Stdout, fset, file) // 将 AST 输出到控制台
}

func main() {
	basic_usage()
	marcos_replace()
}
