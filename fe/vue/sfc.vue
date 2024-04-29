<script setup>
// 定义基本变量
import { ref, reactive, computed, watch, watchEffect } from 'vue'
const msg = 'Hello World!'              // 普通变量声明
const ref_msg = ref('Hello World Ref!')  // 响应式变量声明
const ref_obj = ref({name:"ref_obj_name", tag:"ref_obj_tag"})
const reactive_obj = reactive({name:"obj_name", tag:"obj_tag"})
const value = "Hello Value!"
const tvalue = true
const fvalue = false
const inner_html = "<p>Inner Html</p>"
const attrName = "value"
const action = "click"

// 定义ref属性处理逻辑
let ref_tag=ref()
let ref_tag_content = ref('')
function getDomRef() {
  ref_tag_content.value = ref_tag.value.innerHTML
}

// 定义ref用法的相关函数
const objectOfAttrs = {
  id: 'container',
  class: 'wrapper'
}

function doSomthing() {
  alert("do somthing")
}

function ref_obj_reset() {
  ref_obj.value = {name:"ref_obj_name", tag:"ref_obj_tag"}
}

// 定义computed相关处理函数
let refmsg_upper_ro = computed(() => {
  return ref_msg.value.toUpperCase()
})

let refmsg_upper_rw = computed({
  set(new_val) {
    ref_msg.value = new_val.toLowerCase()
  },

  get() {
      return ref_msg.value.toUpperCase()
  }
})

// 监视ref
let watch_result = ref()
let stop_watch = watch(ref_msg, (new_val, old_val) => { // watch双参数即new, old
  watch_result.value = old_val + " => " + new_val
  if (new_val.length > 20) {  // 选择停止监视的时机
    stop_watch()
  }
}, {immediate:true})

// 监视ref对象（reactive对象）
// 说明：当watch object的时候，会发现，new value和old value是一样的，因为总是来自同一引用
// 用reactive与用ref类似，并且会隐式的标注deep:true
watch(ref_obj, (val) => { // watch单参即new value
  watch_result.value = val
}, {deep:true})

// 监视getter函数
// 注意，watch会按照代码顺序，依次执行，后面覆盖前面
watch(() => ref_obj.value.name, (val) => { // 只watch一个对象中的具体元素
  watch_result.value = val
})

// 监听多个内容（数组）
watch ([ref_msg, ref_obj, () => ref_obj.value.name], (val) => {
  console.log(val)
})

// 使用WatchEffect实现自动全域监听
watchEffect(()=>{
  console.log("watchEffect:" + watch_result.value)
})
</script>

<template>
  <!-- 变量的基本用法 -->
  <h3> 变量的基本用法 </h3>
  <p>{{ msg }}</p>                        <!-- 变量作为内容的使用：双打括号 -->
  <p>{{ tvalue ? "True" : "False" }}</p>  <!-- 双括号里支持完整的JS语法 -->
  <input v-bind:value=msg /> <br/>    <!-- v-bind用法（标准）：v-bind:{name}=var -->
  <input :value="msg" /> <br/>        <!-- v-bind用法（简写）：:{name}=var -->
  <input :value /> <br/>              <!-- v-bind用法（同名）：属性与变量同名:{name} -->
  <input :[attrName]="value" /> <br/> <!-- v-bind用法（属性）：动态绑定属性名：:[var]=var -->
  <div v-bind="objectOfAttrs"></div>  <!-- v-bind用法（多值）：多值绑定 -->

  <!-- 变量的单/双项绑定 -->
  <h3> 变量的双项绑定 </h3>
  <span>单向绑定：</span> <input :value="ref_msg" /> <br/>  <!-- 响应式变量 -->
  <span>对象绑定：</span> <input :value="reactive_obj" /> <br/>  <!-- 响应式变量（reactive） -->
  <span>双向绑定：</span> <input v-model="ref_msg" />       <!-- v-model默认绑定value属性 -->

  <!-- 标签的ref属性 -->
  <!-- 注意：ref属性只能加在HTML标签上，拿到的是DOM元素，放在组件标签上，拿到的是组件对象 -->
  <!-- 在组件标签上用ref属性，就需要在子组件中通过defineExpose来暴露内部变量，父组件才能访问子组件的私有变量 -->
  <h3 ref="ref_tag">通过ref标签获取DOM树的元素</h3> <!-- 通过ref属性，相比id、class，可以有效的避免重名带来覆盖问题，保持标签的局部性 -->
  <input :value="ref_tag_content" />
  <button @click="getDomRef">点击获取ref标签指定的dom树属性</button>

  <!-- computed计算属性用法 -->
  <h3> computed计算属性用法 </h3>
  <input v-model="ref_msg" />
  <p>{{ refmsg_upper_ro }}</p>        <!-- 只读计算属性 -->
  <input v-model="refmsg_upper_rw" /> <!-- 读写计算属性 -->

  <!-- watch监视用法 -->
  <h3> watch监视用法 </h3>
  <span>watch ref: </span> <input v-model="ref_msg" /> <br/>
  <span>watch ref(obj): </span>
  <input v-model="ref_obj.name" />
  <input v-model="ref_obj.tag">
  <button @click="ref_obj_reset">重置</button>  <br/>
  <p>watch结果: {{ watch_result }}</p>

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
