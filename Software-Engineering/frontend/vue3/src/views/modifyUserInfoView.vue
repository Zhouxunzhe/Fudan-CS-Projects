<script setup>
import { ref, reactive } from 'vue'
import { modify_userinfo } from "@/api/user";
import { useRouter } from "vue-router";
import Message from "@/components/library/Message";
import AppTopNav from "@/components/AppTopNav";

const formChange = reactive({
  name: '',
  password: '',
  confirmpassword: '',
  telephone: '',
  email: '',
})

const formChangeref = ref()

const telephone_reg = /^(13[0-9]|14[01456879]|15[0-35-9]|16[2567]|17[0-8]|18[0-9]|19[0-35-9])\d{8}$/
const email_reg = /^[a-zA-Z0-9_-]+@[a-zA-Z0-9_-]+(\.[a-zA-Z0-9_-]+)+$/
const name_reg = /^[a-z_A-Z0-9]*$/
const password_reg = /^(?![\d]+$)(?![A-Za-z]+$)(?![_*@!#%?$]+$)[\dA-Za-z_*@!#%?$]{6,32}$/

const validatePass = (rule, value, callback) => {
  if (value === '' && formChange.password !== '') {
    callback(new Error('请再次输入密码'))
  } else if (value !== formChange.password) {
    callback(new Error('两次输入的密码不一致'))
  } else {
    callback()
  }
}

const router = useRouter();

const submitChangeForm = (formEl) => {
  if (!formEl) return
  formEl.validate((valid) => {
    if (valid) {
      console.log('submit!')
      modify_userinfo({username:formChange.name, 
              email:formChange.email,
              phonenumber:formChange.telephone,
              password:formChange.password,
              }).then(data =>{
        // console.log(data.result); //@log
        if(!data.isSuccess) {
          Message({ type: 'error', text: data.message });
        }
        router.push('/')
        Message({ type: 'success', text: data.message });
        return true;
      }).catch(() => {
        Message({ type: 'error', text: '请求异常' })
        return false;
      })
    } else {
      console.log('error submit!')
      return false
    }
  })
}

const ruleChange = reactive({
  name: [
    {
      pattern: name_reg,
      message: '用户名仅能出现英⽂字符、数字与下划线',
      trigger: 'blur'
    },
    { min: 3, max: 10, message: '用户名长度应在3到10之间', trigger: 'blur' }
  ],
  telephone: [
    { pattern: telephone_reg, message: '电话号码格式不正确', trigger: 'blur' }
  ],
  email: [
    { pattern: email_reg, message: '邮箱格式不正确', trigger: 'blur' }
  ],
  password: [
    { pattern: password_reg, message: '密码格式不正确', trigger: 'blur' }
  ],
  confirmpassword: [{ validator: validatePass, trigger: 'blur' }]
})
</script>

<template>
  <AppTopNav/>
  <el-form class="change-container" :model="formChange" status-icon :rules="ruleChange" ref="formChangeref">
    <h3 class="change-title">修改个人信息</h3>
    <el-form-item label="用户名" prop="name" label-width="80px">
      <el-input
        v-model="formChange.name"
        type="input"
        auto-complete="off"
        placeholder="请输入用户名，为空代表不修改用户名"
      ></el-input>
    </el-form-item>
    <el-form-item label="手机号" prop="telephone" label-width="80px">
      <el-input v-model="formChange.telephone" auto-complete="off" placeholder="请输入手机号，为空代表不修改手机号">
      </el-input>
    </el-form-item>
    <el-form-item label="邮箱" prop="email" label-width="80px">
      <el-input v-model="formChange.email" auto-complete="off" placeholder="请输入邮箱，为空代表不修改邮箱"> </el-input>
    </el-form-item>
    <el-form-item label="密码" prop="password" label-width="80px">
      <el-input
        type="password"
        v-model="formChange.password"
        auto-complete="off"
        placeholder="请输入密码，为空代表不修改密码"
        show-password
      ></el-input>
    </el-form-item>
    <el-form-item label="确认密码" prop="confirmpassword" label-width="80px">
      <el-input
        type="password"
        v-model="formChange.confirmpassword"
        aria-autocomplete="off"
        placeholder="请再次输入密码"
        show-password
      >
      </el-input>
    </el-form-item>
    <el-form-item class="change-submit">
        <el-button class="change-submit" @click="submitChangeForm(formChangeref)">提交</el-button>
    </el-form-item>
  </el-form>
</template>

<style scoped>
.change-container {
    border-radius: 15px;
    background-clip: padding-box;
    margin: 180px auto;
    width: 500px;
    padding: 35px 35px 15px 35px;
    background-color: #fff;
    border: 1px solid #eaeaea;
    box-shadow: 0 0 25px #cac6c6;
}
.change-title {
    margin: 0px auto 40px auto;
    text-align: center;
    color: #505458;
}
.change-submit {
    margin: 10px auto 0 auto;
    justify-content: center;
}
</style>