<script setup>
import { ref, reactive } from 'vue'
import { loginByAccountAndPassword } from "@/api/user";
import { useRouter } from "vue-router";
import { useStore } from "vuex";
import Message from "@/components/library/Message";

const formLogin = reactive({
  name: '',
  password: ''
})

const formLoginRef = ref()

const router = useRouter();
const store = useStore();

const submitLoginForm = (formEl) => {
  if (!formEl) return
  formEl.validate((valid) => {
    if (valid) {
      console.log('submit!')
      
      loginByAccountAndPassword({username:formLogin.name, password:formLogin.password}).then(data =>{
        console.log(data.isSuccess); //@log
        if(!data.isSuccess) {
          Message({ type: 'error', text: data.message });
          return false;
        }

        console.log(data.result)
        store.commit('user/setUser', {
          account: data.result.username, //用户账号
          mobile: data.result.phonenumber, //用户手机号
          id_num: data.result.id_num, //用户身份证号
          email: data.result.email, //用户邮箱
          token: data.result.token, //用户登录令牌
          is_shop: data.result.is_shop, //用户登录令牌
          is_admin: false, //用户登录令牌
        })

        // 跳转到目前地址 or 首页
        router.push("/").then(() => {
          // 登录成功之后的提示信息
          Message({ type: 'success', text: data.message });
        });
        
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

const name_reg = /^[a-z_A-Z0-9]*$/
const password_reg = /^(?![\d]+$)(?![A-Za-z]+$)(?![_*@!#%?$]+$)[\dA-Za-z_*@!#%?$]{6,32}$/

const ruleLogin = reactive({
  name: [
    { required: true, message: '请输入用户名', trigger: 'blur' },
    {
      pattern: name_reg,
      message: '用户名仅能出现英⽂字符、数字与下划线',
      trigger: 'blur'
    },
    { min: 3, max: 10, message: '用户名长度应在3到10之间', trigger: 'blur' }
  ],
  password: [
    { required: true, message: '请输入密码', trigger: 'blur' },
    { pattern: password_reg, message: '密码格式不正确', trigger: 'blur' }
  ]
})
</script>

<template>
  <header class="login-header">
    <div class="container">
      <RouterLink class="entry" to="/">
        进入网站首页
        <i class="iconfont icon-angle-right"></i>
        <i class="iconfont icon-angle-right"></i>
      </RouterLink>
    </div>
  </header>
  <el-form class="login-container" :model="formLogin" status-icon :rules="ruleLogin" ref="formLoginRef">
    <h3 class="login-title">登录</h3>
    <el-form-item label="用户名" prop="name" label-width="80px">
      <el-input
        v-model="formLogin.name"
        type="input"
        auto-complete="off"
        placeholder="请输入用户名"
      ></el-input>
    </el-form-item>
    <el-form-item label="密码" prop="password" label-width="80px">
      <el-input
        type="password"
        v-model="formLogin.password"
        auto-complete="off"
        placeholder="请输入密码"
        show-password
      ></el-input>
    </el-form-item>
    <el-form-item class="login-submit">
        <el-button class="login-submit" @click="submitLoginForm(formLoginRef)">登录</el-button>
    </el-form-item>
    <br />
    <br />
    <RouterLink to="/adminlogin">管理员登录</RouterLink>
  </el-form>
</template>

<style scoped lang="less">
.login-container {
    border-radius: 15px;
    background-clip: padding-box;
    margin: 180px auto;
    width: 350px;
    padding: 35px 35px 15px 35px;
    background-color: #fff;
    border: 1px solid #eaeaea;
    box-shadow: 0 0 25px #cac6c6;
}
.login-title {
    margin: 0px auto 40px auto;
    text-align: center;
    color: #505458;
}
.login-submit {
    margin: 10px auto 0 auto;
    justify-content: center;
}
.login-header {
  background: #fff;
  border-bottom: 1px solid #e4e4e4;
  .container {
    display: flex;
    align-items: flex-end;
    justify-content: space-between;
  }
  .logo {
    width: 200px;
    a {
      display: block;
      height: 132px;
      width: 100%;
      text-indent: -9999px;
    }
  }
  .sub {
    flex: 1;
    font-size: 24px;
    font-weight: normal;
    margin-bottom: 38px;
    margin-left: 20px;
    color: #666;
  }
  .entry {
    width: 120px;
    margin-bottom: 38px;
    font-size: 16px;
    i {
      font-size: 14px;
      color: @xtxColor;
      letter-spacing: -5px;
    }
  }
}
</style>
