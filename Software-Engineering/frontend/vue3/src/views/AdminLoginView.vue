<script setup>
import { ref, reactive } from 'vue'
import { adminLoginByAccountAndPassword } from "@/api/user";
import { useRouter } from "vue-router";
import { useStore } from "vuex";
import Message from "@/components/library/Message";

const formAdmin = reactive({
  name: 'admin',
  password: 'abc123'
})

const formAdminRef = ref()

const router = useRouter();
const store = useStore();

const submitAdminForm = (formEl) => {
  if (!formEl) return
  formEl.validate((valid) => {  
    if (valid) {
      console.log('submit!')
      adminLoginByAccountAndPassword({username:formAdmin.name, password:formAdmin.password}).then(data =>{
        // console.log(data.result); //@log
        if(!data.isSuccess) {
          Message({ type: 'error', text: data.message });
          return false;
        }
        store.commit('user/setUser', {
          account: 'admin', //用户账号
          mobile: '', //用户手机号
          id_num: '', //用户身份证号
          email: '', //用户邮箱
          token: data.token, //用户登录令牌
          is_shop: false, //用户登录令牌
          is_admin: true, //用户登录令牌
        })
        // 跳转到目前地址 or 首页
        router.push("/admin").then(() => {
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

const ruleAdmin = reactive({
  name: [{ required: true, message: '请输入用户名', trigger: 'blur' }],
  password: [{ required: true, message: '请输入密码', trigger: 'change' }]
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
  <el-form class="admin-container" :model="formAdmin" status-icon :rules="ruleAdmin" ref="formAdminRef">
    <h3 class="admin-title">管理员登录</h3>
    <el-form-item label="用户名" prop="name" label-width="80px">
      <el-input
        v-model="formAdmin.name"
        type="input"
        auto-complete="off"
        placeholder="请输入用户名"
      ></el-input>
    </el-form-item>
    <el-form-item label="密码" prop="password" label-width="80px">
      <el-input
        type="password"
        v-model="formAdmin.password"
        auto-complete="off"
        placeholder="请输入密码"
        show-password
      ></el-input>
    </el-form-item>
    <el-form-item class="admin-submit">
        <el-button class="admin-submit" @click="submitAdminForm(formAdminRef)">登录</el-button>
    </el-form-item>
    <br />
    <br />
    <RouterLink to="/login">普通用户登录</RouterLink>
  </el-form>
</template>

<style scoped lang="less">
.admin-container {
    border-radius: 15px;
    background-clip: padding-box;
    margin: 180px auto;
    width: 350px;
    padding: 35px 35px 15px 35px;
    background-color: #fff;
    border: 1px solid #eaeaea;
    box-shadow: 0 0 25px #cac6c6;
}
.admin-title {
    margin: 0px auto 40px auto;
    text-align: center;
    color: #505458;
}
.admin-submit {
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
