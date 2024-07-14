<script setup>
import { ref, reactive } from 'vue'
import { add_goods,upload_image } from "@/api/user";
import { useRouter } from "vue-router";
import Message from "@/components/library/Message";
import {
  Plus
} from '@element-plus/icons-vue'
import AppTopNav from "@/components/AppTopNav";

const formAddGood= reactive({
  goodname: '',
  intro: '',
  price: '0.00',
  images: [],
  goodamount: 0
})

const formAddGoodref = ref()

const uploadUrl = ref('/api/common/upload')
const limitCountImg = ref(9)
const showBtnDealImg = ref(true)
const noneBtnImg = ref(false)

function handleImgChange(file, fileList){
  noneBtnImg.value = fileList.length >= limitCountImg.value;
}

function beforeImageUpload(rawFile){
  if(rawFile.size / 1024 / 1024 > 10){
    Message({type:'error',text:"单张图片大小不能超过10MB!"});
    return false;
  }
  return true;
}

function FileSuccess(response, file, fileList) {
  console.log(fileList)
  if(response.isSuccess){
    let obj = response.data.url;
    formAddGood.images.push(obj);
  }else{
    Message({type:'error',text:response.message});
  }            
}

function FileRemove(file, fileList) {
  formAddGood.images.forEach((item, index) => {
    if (file.name == item) {
      formAddGood.images.splice(index, 1);
    }
  });
  noneBtnImg.value = fileList.length >= limitCountImg.value;
}

function handleExceedCover(files,fileList){
  console.log(files)
  console.log(fileList)
  Message({type:'error',text:'上传图片数量超出限制！'});
}

const amount_reg =
  /^\d+.\d{2}$/


const validateGoodAmount = (rule, value, callback) => {
  if (value === '') {
    callback(new Error('请输入商品数量'))
  } else if (value <= 0) {
    callback(new Error('商品数量应大于0'))
  } else {
    callback()
  }
}

function uploadImage(file){
  let formData = new FormData()
  console.log(file.file)
  formData.append('image', file.file)
  console.log(formData)
  upload_image(formData).then(data => {
    if(data.isSuccess){
      let obj = data.result.url;
      formAddGood.images.push(obj);
    }else{
      Message({type:'error',text:data.message});
    }      
  }).catch(() => {
    Message({ type: 'error', text:'请求异常' })
    return false;
  })
}

const router = useRouter();

const submitAddGoodForm = (formEl) => {
  if (!formEl) return
  formEl.validate((valid) => {
    if (valid) {
      console.log('submit!')
      let time = new Date()
      console.log(formAddGood.images)
       add_goods({goodname: formAddGood.goodname,
                intro: formAddGood.intro,
                price: formAddGood.price,
                images: formAddGood.images,
                request_date: time.toLocaleDateString(),
                goodamount: formAddGood.goodamount
              }).then(data =>{
        // console.log(data.result); //@log
        if(!data.isSuccess) {Message({ type: 'error', text:'请求失败' })}
        else {
        router.push('/myshop')
        Message({ type: 'success', text: '请求成功' });}
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

const ruleAddGood = reactive({
  goodname: [
    { required: true, message: '请输入商品名', trigger: 'blur' },
    { max: 12, message: '商品名最大长度为12个字符', trigger: 'blur' }
  ],
  image: {required: true, message: '请上传图片', trigger: 'blur'},
  intro: [
    { required: true, message: '请输入商品描述', trigger: 'blur' },
    { max: 128, message: '商品描述最大长度为128个字符', trigger: 'blur' }
  ],
  price: [
    { required: true, message: '请输入商品价格', trigger: 'blur' },
    { pattern: amount_reg, message: '商品价格格式不正确，应大于0且有两位小数', trigger: 'blur' }
  ],
  goodamount: [
    { validator: validateGoodAmount, trigger: 'blur' }
  ],
})
</script>

<template>
  <AppTopNav/>
  <el-form class="addgood-container" :model="formAddGood" status-icon :rules="ruleAddGood" ref="formAddGoodref">
    <h3 class="addgood-title">添加商品</h3>
    <el-form-item label="商品名" prop="goodname" label-width="80px">
      <el-input
        v-model="formAddGood.goodname"
        type="input"
        auto-complete="off"
        placeholder="请输入商品名"
      ></el-input>
    </el-form-item>
    <el-form-item label="上传图片" :prop="image" v-loading="loading">
      <el-upload 
            class="dl-avatar-uploader-min square" 
            :class="{uoloadBtn:showBtnDealImg,disUoloadBtn:noneBtnImg}"
            :action="uploadUrl"
            :limit="limitCountImg"
            :on-success="FileSuccess" 
            :on-remove="FileRemove"
            :on-exceed="handleExceedCover"
            :before-upload="beforeImageUpload"
            :on-change="handleImgChange"
            :file-list="formAddGood.images"
            :http-request="uploadImage"
            list-type="picture-card"
            accept="image/*"
            multiple
            >
            <div class="uploadIcon">
              <el-icon>
                <Plus />
              </el-icon>
            </div>
            <template #tip>
                <div class="el-upload__tip">最多上传9张图片,且单张图片大小不能超过10MB</div>
            </template>
        </el-upload>
    </el-form-item>

    <el-form-item label="商品描述" prop="intro" label-width="80px">
      <el-input
        v-model="formAddGood.intro"
        type="input"
        auto-complete="off"
        placeholder="请输入商品描述"
      ></el-input>
    </el-form-item>
    <el-form-item label="商品价格" prop="price" label-width="80px">
      <el-input
        v-model="formAddGood.price"
        aria-autocomplete="off"
        placeholder="请输入商品价格"
      >
      </el-input>
    </el-form-item>
    <el-form-item label="商品数量" prop="goodamount" label-width="80px">
      <el-input
        v-model="formAddGood.goodamount"
        aria-autocomplete="off"
        placeholder="请输入商品数量"
      >
      </el-input>
    </el-form-item>
    <el-form-item class="addgood-submit">
        <el-button class="addgood-submit" @click="submitAddGoodForm(formAddGoodref)">提交</el-button>
    </el-form-item>
  </el-form>
</template>

<style scoped>
.addgood-container {
    border-radius: 15px;
    background-clip: padding-box;
    margin: 180px auto;
    width: 800px;
    padding: 35px 35px 15px 35px;
    background-color: #fff;
    border: 1px solid #eaeaea;
    box-shadow: 0 0 25px #cac6c6;
}
.addgood-title {
    margin: 0px auto 40px auto;
    text-align: center;
    color: #505458;
}
.addgood-submit {
    margin: 10px auto 0 auto;
    justify-content: center;
}
.disUoloadBtn .el-upload--picture-card{
    display:none;   /* 上传按钮隐藏 */
}

</style>
