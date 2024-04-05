<script setup>
import { ref } from 'vue'
const msg = 'Hello World!'              // 普通变量声明
const refmsg = ref('Hello World Ref!')  // 响应式变量声明
const value = "Hello Value!"
const tvalue = true
const fvalue = false
const inner_html = "<p>Inner Html</p>"
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
  <!-- 变量的单项绑定 -->
  <h3> 变量的单项绑定 </h3>
  <p>{{ msg }}</p>                        <!-- 变量作为内容的使用：双打括号 -->
  <p>{{ tvalue ? "True" : "False" }}</p>  <!-- 双括号里支持完整的JS语法 -->
  <input v-bind:value=msg />    <!-- 单向数据绑定：v-bind:{name}=var -->
  <input :value="msg" />        <!-- 单向数据绑定（简写）：:{name}=var -->
  <input :value />              <!-- 单向数据绑定（简写：属性与变量同名）：:{name} -->
  <input :[attrName]="value" /> <!-- 动态绑定属性名：:[var]=var -->
  <div v-bind="objectOfAttrs"></div> <!-- 多值绑定 -->

  <!-- 变量的双项绑定 -->
  <h3> 变量的双项绑定 </h3>
  <input :value="refmsg" />   <!-- 单向数据绑定：使用响应式变量 -->
  <input v-model="refmsg" />  <!-- 双向向数据绑定：v-model默认绑定value属性 -->

  <!-- v-if 用法 -->
  <h3> v-if 用法 </h3>
  <p v-if="tvalue">根据属性判断是否进行显示</p> <!-- 显示 -->
  <p v-if="fvalue">根据属性判断是否进行显示</p> <!-- 隐藏 -->
  
  <!-- v-on 用法 -->
  <h3> v-on 用法 </h3>
  <p><a href="about:blank" v-on:click="doSomthing"> 点击弹出警告框（标准） </a></p>   <!-- 事件响应：标准用法 -->
  <p><a href="about:blank" @click="doSomthing"> 点击弹出警告框（简写） </a></p>       <!-- 事件响应：简化用法 -->
  <p><a href="about:blank" @[action]="doSomthing"> 点击弹出警告框（动态） </a></p>    <!-- 事件响应：动态用法 -->
  <p><a href="about:blank" @click.prevent="doSomthing">点击弹出警告框（修饰）</a></p> <!-- 事件响应：修饰用法 -->

  <!-- v-html 用法 -->
  <h3> v-html 用法 </h3>
  <div v-html="inner_html"></div> <!-- v-html可以注入任何html信息 -->
</template>

<style scoped>
  #container {
    border: solid;
    height: 100px;
  }
</style>
<!--  -->
