<script setup>
import { ref, reactive } from 'vue'
import { signin } from "@/api/user";
import { useRouter } from "vue-router";
import Message from "@/components/library/Message";

const formSignin = reactive({
  name: '',
  password: '',
  confirmpassword: '',
  slect: '',
  telephone: '',
  email: '',
  residentId: ''
})

const formSigninref = ref()

const telephone_reg = /^(13[0-9]|14[01456879]|15[0-35-9]|16[2567]|17[0-8]|18[0-9]|19[0-35-9])\d{8}$/
const email_reg = /^[a-zA-Z0-9_-]+@[a-zA-Z0-9_-]+(\.[a-zA-Z0-9_-]+)+$/
const name_reg = /^[a-z_A-Z0-9]*$/
const residentId_reg =
  /^[1-9]\d{5}(18|19|([23]\d))\d{2}((0[1-9])|(10|11|12))(([0-2][1-9])|10|20|30|31)\d{3}[0-9Xx]$/
const password_reg = /^(?![\d]+$)(?![A-Za-z]+$)(?![_*@!#%?$]+$)[\dA-Za-z_*@!#%?$]{6,32}$/

const validatePass = (rule, value, callback) => {
  if (value === '') {
    callback(new Error('请再次输入密码'))
  } else if (value !== formSignin.password) {
    callback(new Error('两次输入的密码不一致'))
  } else {
    callback()
  }
}

const router = useRouter();

const submitSigninForm = (formEl) => {
  if (!formEl) return
  formEl.validate((valid) => {
    if (valid) {
      console.log('submit!')
      signin({username:formSignin.name, 
              password:formSignin.password,
              is_shop:(formSignin.slect==='商户' ? true : false),
              phonenumber:formSignin.telephone,
              email:formSignin.email,
              id_num:formSignin.residentId
              }).then(data =>{
        // console.log(data.result); //@log
        if(!data.isSuccess) {
          Message({ type: 'error', text: data.message });
          return false;
        }
        router.push('/login')
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

const ruleSignin = reactive({
  name: [
    { required: true, message: '请输入用户名', trigger: 'blur' },
    {
      pattern: name_reg,
      message: '用户名仅能出现英⽂字符、数字与下划线',
      trigger: 'blur'
    },
    { min: 3, max: 10, message: '用户名长度应在3到10之间', trigger: 'blur' }
  ],
  slect: [{ required: true, message: '请输入角色', trigger: 'blur' }],
  telephone: [
    { required: true, message: '请输入电话号码', trigger: 'blur' },
    { pattern: telephone_reg, message: '电话号码格式不正确', trigger: 'blur' }
  ],
  residentId: [
    { required: true, message: '请输入身份证号', trigger: 'blur' },
    { pattern: residentId_reg, message: '身份证号码格式不正确', trigger: 'blur' }
  ],
  email: [
    { required: true, message: '请输入邮箱', trigger: 'blur' },
    { pattern: email_reg, message: '邮箱格式不正确', trigger: 'blur' }
  ],
  password: [
    { required: true, message: '请输入密码', trigger: 'blur' },
    { pattern: password_reg, message: '密码格式不正确', trigger: 'blur' }
  ],
  confirmpassword: [{ validator: validatePass, trigger: 'blur' }]
})
</script>

<template>
  <el-form class="signin-container" :model="formSignin" status-icon :rules="ruleSignin" ref="formSigninref">
    <h3 class="signin-title">注册</h3>
    <el-form-item label="用户名" prop="name" label-width="80px">
      <el-input
        v-model="formSignin.name"
        type="input"
        auto-complete="off"
        placeholder="请输入用户名"
      ></el-input>
    </el-form-item>
    <el-form-item label="角色" prop="slect" label-width="80px">
      <el-select v-model="formSignin.slect" placeholder="请选择角色">
        <el-option lable="user" value="普通用户" />
        <el-option lable="shopper" value="商户" />
      </el-select>
    </el-form-item>
    <el-form-item label="手机号" prop="telephone" label-width="80px">
      <el-input v-model="formSignin.telephone" auto-complete="off" placeholder="请输入手机号">
      </el-input>
    </el-form-item>
    <el-form-item label="身份证号" prop="residentId" label-width="80px">
      <el-input v-model="formSignin.residentId" auto-complete="off" placeholder="请输入身份证号">
      </el-input>
    </el-form-item>
    <el-form-item label="邮箱" prop="email" label-width="80px">
      <el-input v-model="formSignin.email" auto-complete="off" placeholder="请输入邮箱"> </el-input>
    </el-form-item>
    <el-form-item label="密码" prop="password" label-width="80px">
      <el-input
        type="password"
        v-model="formSignin.password"
        auto-complete="off"
        placeholder="请输入密码"
        show-password
      ></el-input>
    </el-form-item>
    <el-form-item label="确认密码" prop="confirmpassword" label-width="80px">
      <el-input
        type="password"
        v-model="formSignin.confirmpassword"
        aria-autocomplete="off"
        placeholder="请再次输入密码"
        show-password
      >
      </el-input>
    </el-form-item>
    <el-form-item class="signin-submit">
        <el-button class="signin-submit" @click="submitSigninForm(formSigninref)">注册</el-button>
    </el-form-item>
  </el-form>
</template>

<style scoped>
.signin-container {
    border-radius: 15px;
    background-clip: padding-box;
    margin: 180px auto;
    width: 500px;
    padding: 35px 35px 15px 35px;
    background-color: #fff;
    border: 1px solid #eaeaea;
    box-shadow: 0 0 25px #cac6c6;
}
.signin-title {
    margin: 0px auto 40px auto;
    text-align: center;
    color: #505458;
}
.signin-submit {
    margin: 10px auto 0 auto;
    justify-content: center;
}
</style>
