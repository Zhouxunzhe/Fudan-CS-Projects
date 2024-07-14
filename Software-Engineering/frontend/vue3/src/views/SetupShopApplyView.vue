<script setup>
import { ref, reactive } from 'vue'
import { open_shop } from "@/api/user";
import { useRouter } from "vue-router";
import Message from "@/components/library/Message";
import XtxCity from "@/components/library/XtxCity";

const formSetupShop = reactive({
  name: '',
  types: '',
  residentId: '',
  introduction: '',
  addr: '',
  capital: '0.00',
  reg_date: ''
})

const formSetupShopref = ref()

const capital_reg =
  /^[1-9]\d*\d{3}.\d{2}$/


const router = useRouter();

const submitSetupShopForm = (formEl) => {
  if (!formEl) return
  formEl.validate((valid) => {
    if (valid) {
      console.log('submit!')
      for(let i = 0; i < formSetupShop.addr.length; i++){
        if (formSetupShop.addr[i] == ' ') {
          formSetupShop.addr = formSetupShop.addr.replace(' ',','); // 注意替换之后就变成新数组了
        }
      }
      console.log(formSetupShop.addr)
      open_shop({shopname:formSetupShop.name, 
              kind:formSetupShop.types,
              intro:formSetupShop.introduction,
              address:formSetupShop.addr,
              is_shop:true,
              register_capital:formSetupShop.capital,
              register_date:formSetupShop.reg_date
              }).then(data =>{
        // console.log(data.result); //@log
        if(!data.isSuccess){
          Message({ type: 'error', text: data.message })
          return false;
        }
        router.push('/')
        Message({ type: 'success', text: '请求成功' })
        return true;
      }).catch(() => {
        Message({ type: 'error', text:'请求异常' })
        return false;
      })
    } else {
      console.log('error submit!')
      return false
    }
  })
}

const ruleSetupShop = reactive({
  name: [
    { required: true, message: '请输入店名', trigger: 'blur' },
    { max: 12, message: '店面最大长度为12个字符', trigger: 'blur' }
  ],
  types: [
    { required: true, message: '请输入商品类型', trigger: 'blur' },
    { max: 128, message: '商品类型最大长度为128个字符', trigger: 'blur' }
  ],
  introduction: [
    { required: true, message: '请输入商店简介', trigger: 'blur' },
    { max: 128, message: '商店简介应在128字符以内', trigger: 'blur' }
  ],
  addr: [
    { required: true, message: '请输入备案地址', trigger: 'blur' },
  ],
  capital: [
    { required: true, message: '请输入注册资金', trigger: 'blur' },
    { pattern: capital_reg, message: '注册资金格式不正确，应大于1000且有两位小数', trigger: 'blur' }
  ],
  reg_date: [
    { required: true, message: '请选择注册时间', trigger: 'blur'}
  ]
})

const cityChanged = (data) => {
  formSetupShop.addr = data.location;
};
</script>

<template>
  <el-form class="setupshop-container" :model="formSetupShop" status-icon :rules="ruleSetupShop" ref="formSetupShopref">
    <h3 class="setupshop-title">注册</h3>
    <el-form-item label="店名" prop="name" label-width="80px">
      <el-input
        v-model="formSetupShop.name"
        type="input"
        auto-complete="off"
        placeholder="请输入店名"
      ></el-input>
    </el-form-item>
    <el-form-item label="商品类型" prop="types" label-width="80px">
      <el-input
        v-model="formSetupShop.types"
        type="input"
        auto-complete="off"
        placeholder="请输入商品类型（类型间用空格隔开）"
      ></el-input>
    </el-form-item>
    <el-form-item label="商店简介" prop="introduction" label-width="80px">
      <el-input type="textarea" v-model="formSetupShop.introduction" auto-complete="off" placeholder="请输入商店简介，长度在128个字符内"> </el-input>
    </el-form-item>
    <el-form-item label="备案地址" prop="addr" label-width="80px">
      <XtxCity :location="formSetupShop.addr" @onCityChanged="cityChanged" />
    </el-form-item>
    <el-form-item label="注册资金" prop="capital" label-width="80px">
      <el-input
        v-model="formSetupShop.capital"
        aria-autocomplete="off"
        placeholder="请输入注册资金"
      >
      </el-input>
    </el-form-item>
    <el-form-item label="注册时间" prop="reg_date" label-width="80px">
      <el-date-picker v-model="formSetupShop.reg_date" type="date" placeholder="请选择注册时间"/>
    </el-form-item>
    <el-form-item class="setupshop-submit">
        <el-button class="setupshop-submit" @click="submitSetupShopForm(formSetupShopref)">申请</el-button>
    </el-form-item>
  </el-form>
  {{ formSetupShop.reg_date }}
</template>

<style scoped>
.setupshop-container {
    border-radius: 15px;
    background-clip: padding-box;
    margin: 180px auto;
    width: 800px;
    padding: 35px 35px 15px 35px;
    background-color: #fff;
    border: 1px solid #eaeaea;
    box-shadow: 0 0 25px #cac6c6;
}
.setupshop-title {
    margin: 0px auto 40px auto;
    text-align: center;
    color: #505458;
}
.setupshop-submit {
    margin: 10px auto 0 auto;
    justify-content: center;
}
</style>
