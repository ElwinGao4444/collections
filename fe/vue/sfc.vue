<script setup>
const msg = 'Hello World!'  // 变量的声明
const value = "Hello Value!"
const tvalue = true
const fvalue = false
const inner_html = "<h1>Inner Html</h1>"
const attrName = "value"
const action = "click"

const objectOfAttrs = {
  id: 'container',
  class: 'wrapper'
}

function doSomthing() {
  alert("do somthing")
}
</script>

<template>
  <!-- 变量的基本用法 -->
  <h1>{{ msg }}</h1>                        <!-- 变量作为内容的使用：双打括号 -->
  <h1>{{ tvalue ? "True" : "False" }}</h1>  <!-- 双括号里支持完整的JS语法 -->
  <input v-bind:value=msg />    <!-- 单向数据绑定：v-bind:{name}=var -->
  <input :value="msg" />        <!-- 单向数据绑定（简写）：:{name}=var -->
  <input :value />              <!-- 单向数据绑定（简写：属性与变量同名）：:{name} -->
  <input :[attrName]="value" /> <!-- 动态绑定属性名：:[var]=var -->

  <!-- v-if 用法 -->
  <p v-if="tvalue">根据属性判断是否进行显示</p> <!-- 显示 -->
  <p v-if="fvalue">根据属性判断是否进行显示</p> <!-- 隐藏 -->
  
  <!-- v-on 用法 -->
  <p><a href="about:blank" v-on:click="doSomthing"> 点击弹出警告框（标准） </a></p>   <!-- 事件响应：标准用法 -->
  <p><a href="about:blank" @click="doSomthing"> 点击弹出警告框（简写） </a></p>       <!-- 事件响应：简化用法 -->
  <p><a href="about:blank" @[action]="doSomthing"> 点击弹出警告框（动态） </a></p>    <!-- 事件响应：动态用法 -->
  <p><a href="about:blank" @click.prevent="doSomthing">点击弹出警告框（修饰）</a></p> <!-- 事件响应：修饰用法 -->

  <div v-html="inner_html"></div> <!-- v-html可以注入任何html信息 -->
</template>
<!--  -->
