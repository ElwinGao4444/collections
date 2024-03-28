<script setup>
const msg = 'Hello World!'  // 变量的声明
const value = "Hello Value"
const tvalue = true
const fvalue = false
const inner_html = "<h1>Inner Html</h1>"
</script>

<script default>
</script>

<template>
  <h1>{{ msg }}</h1>                        <!-- 变量作为内容的使用：双打括号 -->
  <h1>{{ tvalue ? "True" : "False" }}</h1>  <!-- 双括号里支持完整的JS语法 -->
  <input v-bind:value=msg />  <!-- 变量作为属性的使用：v-bind:{name}=var -->
  <input :value="msg" />      <!-- 变量作为属性的使用（简写）：:{name}=var -->
  <input :value />            <!-- 变量作为属性的使用（简写：属性与变量同名）：:{name} -->
  <input v-model="msg" />

  <p v-if="tvalue">根据属性判断是否进行显示</p> <!-- 显示 -->
  <p v-if="fvalue">根据属性判断是否进行显示</p> <!-- 隐藏 -->
  
  <div v-html="inner_html"></div> <!-- v-html可以注入任何html信息 -->
</template>
<!--  -->
