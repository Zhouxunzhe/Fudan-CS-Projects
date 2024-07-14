<script setup>
import { ref, reactive } from 'vue'
import { myshop,shop_account,ban_good,close_shop } from "@/api/user";
import { useRouter } from "vue-router";
import Message from "@/components/library/Message";
import AppTopNav from "@/components/AppTopNav";
import { IMAGE_URL  } from '@/utils/url';

const requestList = reactive({
    is_null:true,
    goods:[],
    shopname:'',
    total:0
})
const shopAccount = ref('')
const hasShop = ref(true)
myshop().then((data) => {
  console.log(data)
  if(data.isSuccess){
    requestList.is_null = data.result.is_null
    requestList.goods = data.result.goods
    requestList.shopname = data.result.shopname
    requestList.total = data.result.total
  }
  else{
    hasShop.value = false
    Message({ type: 'error', text: "你还没有开店！" });
  }
})

shop_account().then((data) => {
  shopAccount.value = data.result
})

const router = useRouter()

function modifyGood(good_id) {
  router.push({path:'/modifygood',query:{keyword : good_id}})
}

function deleteGood(good_id) {
  let time = new Date()
  ban_good({good_id:good_id, request_date:time.toLocaleDateString()}).then(data => {
    if(!data.isSuccess) {
      Message({ type: 'error', text: data.message });
    }else{
      Message({ type: 'success', text: data.message });
      myshop().then((data) => {
      console.log(data)
      if(data.isSuccess){
        requestList.is_null = data.result.is_null
        requestList.goods = data.result.goods
        requestList.shopname = data.result.shopname
        requestList.total = data.result.total
      }
      else{
        hasShop.value = false
        Message({ type: 'error', text: "你还没有开店！" });
      }
      })
      shop_account().then((data) => {
        shopAccount.value = data.result
      })
    }
  }).catch(() => {
    Message({ type: 'error', text: '请求异常'});
  })
}

function closeShopBtn(){
  let time = new Date()
  close_shop({request_date:time.toLocaleDateString()}).then(data => {
    if(!data.isSuccess) {
      Message({ type: 'error', text: data.message });
    }else{
      Message({ type: 'success', text: data.message });
    }
  }).catch(() => {
    Message({ type: 'error', text: '请求异常'});
  })
}

</script>

<template>
    <app-top-nav/>
    <br/>
    <div v-if="hasShop">
    <el-descriptions
        class="margin-top"
        title="商店信息"
        :column="2"
        size="large"
        border
    > 
        <template #extra>
          <el-button @click="closeShopBtn" type="danger">关闭商店</el-button>
        </template>
        <el-descriptions-item label="商店名称">
          {{ requestList.shopname }}
        </el-descriptions-item>
        <el-descriptions-item label="商店账户余额">
          {{ shopAccount.amount }}
        </el-descriptions-item>
    </el-descriptions>
    <br/>
    <el-button>
      <router-link :to="`/addgood`">添加商品</router-link>
    </el-button>
    <el-button>
      <router-link :to="`/shopgoodrequest`">商品申请记录</router-link>
    </el-button>
    <div class="cart-page">
      <div class="container">
        <div class="cart">
          <table>
            <tr>
              <th>商品名称</th>
              <th>图片</th>
              <th>简介</th>
              <th>价格</th>
              <th>数量</th>
              <th>操作</th>
            </tr>   
            <!-- 有效商品 -->
            <tbody>
              <tr v-for="good in requestList.goods" :key="good.goodname">
                <td class="cart">
                  {{ good.goodname }}
                </td>
                <td class="cart"> 
                  <img :src="IMAGE_URL+good.images[0]" style="width: 100px; height: 100px"/>
                </td>
                <td class="cart"> 
                  {{ good.intro }}
                </td>
                <td class="cart">
                  {{ good.price }}
                </td>
                <td class="cart">
                  {{ good.goodamount }}
                </td>
                <td class="tc">
                    <div>
                        <el-button type="primary" class="admin-submit" @click="modifyGood(good.good_id)">修改</el-button>
                        <el-button type="warning" class="admin-submit" @click="deleteGood(good.good_id)">删除</el-button>
                    </div>
                </td>
              </tr>
            </tbody>
          </table>
        </div>
      </div>
    </div>
    </div>
    <div v-else>
      <h1>请先申请开店</h1>
    </div>
</template>

<style scoped lang="less">
.tc {
  text-align: center;
  .xtx-number-box {
    margin: 0 auto;
    width: 120px;
  }
}
.red {
  color: @priceColor;
}
.green {
  color: @xtxColor;
}
.f16 {
  font-size: 16px;
}
.shops {
  display: flex;
  align-items: center;
  img {
    width: 100px;
    height: 100px;
  }
  > div {
    // width: 120px;
    text-overflow:ellipsis; 
		overflow:hidden; 
		white-space:nowrap; 
    font-size: 16px;
    text-align: center;
    // padding-left: 10px;
    .attr {
      font-size: 14px;
      color: #999;
    }
  }
}
.admin-submit {
    margin-right: 3px;
    justify-content: center;
}
.action {
  display: flex;
  background: #fff;
  margin-top: 20px;
  height: 80px;
  align-items: center;
  font-size: 16px;
  justify-content: space-between;
  padding: 0 30px;
  .xtx-checkbox {
    color: #999;
  }
  .batch {
    a {
      margin-left: 20px;
    }
  }
  .red {
    font-size: 18px;
    margin-right: 20px;
    font-weight: bold;
  }
}
.tit {
  color: #666;
  font-size: 16px;
  font-weight: normal;
  line-height: 50px;
}
.cart-page {
  .cart {
    background: #fff;
    color: #666;
    table {
      border-spacing: 0;
      border-collapse: collapse;
      line-height: 24px;
      width: 100%;
      th,
      td {
        padding: 10px;
        border-bottom: 1px solid #f5f5f5;
        text-align:center;
      }
      th {
        font-size: 16px;
        font-weight: normal;
        line-height: 50px;
      }
    }
  }
}
</style>