<script setup>
import { ref, reactive } from 'vue'
import { user_account,recharge } from "@/api/user";
import Message from "@/components/library/Message";
import AppTopNav from "@/components/AppTopNav";


const userInfo = reactive({
  amount: 0,
  username: '',
  phonenumber: '',
  id_num: '',
  email: '',
  is_shop: false
})

user_account().then(data => {
    userInfo.amount = data.result.amount
    userInfo.username = data.result.username
    userInfo.phonenumber = data.result.phonenumber
    userInfo.id_num = data.result.id_num
    userInfo.email = data.result.email
    userInfo.is_shop = data.result.is_shop
})

const isShowRecharge = ref(false)



const formRecharge = reactive(
  { amount:'0.00' }
)
const formRechargeRef = ref()

const amount_reg =
  /^\d+.\d{2}$/

const ruleRecharge = reactive({
  amount: [
    { required: true, message: '请输入充值金额', trigger: 'blur' },
    { pattern: amount_reg, message: '充值金额格式不正确，应大于0且有两位小数', trigger: 'blur' }
  ]
})

const submitRechargeForm = (formEl) => {
  if (!formEl) return
  formEl.validate((valid) => {
    if (valid) {
      console.log('submit!')
      let time = new Date()
      recharge({amount:formRecharge.amount,bill_date:time.toLocaleDateString()}).then(data =>{
        // console.log(data.result); //@log
        if(!data.isSuccess) {
          Message({ type: 'error', text: data.message });
          return false;
        }
        // todo
        user_account().then(data => {
            userInfo.amount = data.result.amount
            userInfo.username = data.result.username
            userInfo.phonenumber = data.result.phonenumber
            userInfo.id_num = data.result.id_num
            userInfo.email = data.result.email
            userInfo.is_shop = data.result.is_shop
        })
        isShowRecharge.value = !isShowRecharge.value
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

</script>

<template>
  <app-top-nav/>
  <br/>
  <el-descriptions
    class="margin-top"
    title="账号信息"
    :column="3"
    size="large"
    border
  > 
    <template #extra>
      <el-button>
        <router-link :to="`modifyinfo`">修改个人信息</router-link>
      </el-button>
    </template>
    <el-descriptions-item label="用户名">
      {{ userInfo.username }}
    </el-descriptions-item>
    <el-descriptions-item label="电话号码">
      {{ userInfo.phonenumber }}
    </el-descriptions-item>
    <el-descriptions-item label="邮箱">
      {{ userInfo.email }}
    </el-descriptions-item>
    <el-descriptions-item label="身份证号">
      {{ userInfo.id_num }}
    </el-descriptions-item>
    <el-descriptions-item label="用户类型">
      <el-tag size="small">{{ userInfo.is_shop?'商户':'普通用户' }}</el-tag>
    </el-descriptions-item>
    <el-descriptions-item label="个人账户余额">
      {{ userInfo.amount }}
    </el-descriptions-item>
  </el-descriptions>
  <br/>
  <el-button @click="isShowRecharge=!isShowRecharge">
    {{ !isShowRecharge ? "点击充值" : "关闭充值"}}
  </el-button>
  <el-form v-if="isShowRecharge" :model="formRecharge" status-icon :rules="ruleRecharge" ref="formRechargeRef" class="change-container">
    <h3 class="change-title">充值</h3>
    <el-form-item label="充值金额" prop="amount" label-width="80px">
      <el-input
        v-model="formRecharge.amount"
        type="input"
        auto-complete="off"
        placeholder="请输入充值金额"
      ></el-input>
    </el-form-item>
    <el-form-item class="change-submit">
      <el-button class="change-submit" @click="submitRechargeForm(formRechargeRef)">充值</el-button>
    </el-form-item>
  </el-form>


</template>

<style scoped>
.el-descriptions {
  margin-top: 20px;
}
.cell-item {
  display: flex;
  align-items: center;
}
.margin-top {
  margin-top: 20px;
}
.change-container {
    border-radius: 15px;
    background-clip: padding-box;
    margin: 18px auto;
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