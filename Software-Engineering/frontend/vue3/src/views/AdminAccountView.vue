<script setup>
import { ref, reactive } from 'vue'
import { admin_account,admin_recharge } from "@/api/user";
import Message from "@/components/library/Message";
import AppTopNav from "@/components/AppTopNav";


const adminInfo = reactive({
    profit_amount:0,
    median_amount:0,
    admin_amount: 0
})

admin_account().then(data => {
    adminInfo.profit_amount = data.result.profit_amount
    adminInfo.median_amount = data.result.median_amount
    adminInfo.admin_amount = data.result.admin_amount
})

const isShowRecharge = ref(false)
const amount_reg =
  /^\d+.\d{2}$/


const formRecharge = reactive(
  { amount:'0.00' }
)
const formRechargeRef = ref()

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
      
      admin_recharge({amount:formRecharge.amount}).then(data =>{
        // console.log(data.result); //@log
        if(!data.isSuccess) {
          Message({ type: 'error', text: data.message });
        }
        // todo
        admin_account().then(data => {
            adminInfo.profit_amount = data.profit_amount
            adminInfo.median_amount = data.median_amount
            adminInfo.admin_amount = data.admin_amount
        })
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
  <AppTopNav/>
  <el-descriptions
    class="margin-top"
    title="账号信息"
    :column="3"
    size="large"
    border
  > 
    <el-descriptions-item label="利润账户余额">
      {{ adminInfo.profit_amount}}
    </el-descriptions-item>
    <el-descriptions-item label="中间账户余额">
      {{ adminInfo.median_amount}}
    </el-descriptions-item>
    <el-descriptions-item label="个人账户余额">
      {{ adminInfo.admin_amount}}
    </el-descriptions-item>
  </el-descriptions>
  <el-button @click="isShowRecharge=!isShowRecharge">
    点击充值
  </el-button>
  <el-form v-if="isShowRecharge" :model="formRecharge" status-icon :rules="ruleRecharge" ref="formRechargeRef">
    <h3>充值</h3>
    <el-form-item label="充值金额" prop="amount" label-width="80px">
      <el-input
        v-model="formRecharge.amount"
        type="input"
        auto-complete="off"
        placeholder="请输入充值金额"
      ></el-input>
    </el-form-item>
    <el-form-item>
      <el-button @click="submitRechargeForm(formRechargeRef)">充值</el-button>
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
</style>