<script setup>
import { ref } from 'vue'
import { get_order_by_id,cancel_order,pay_order } from "@/api/user";
import { useRouter, useRoute } from 'vue-router';
import AppTopNav from "@/components/AppTopNav";
import Message from "@/components/library/Message";
import { IMAGE_URL  } from '@/utils/url';

const goodList = ref([])
const order_status = ref(0)
const total_price = ref(0)
const route= useRoute()

function getOrderReq() {
    let order_id = route.query.keyword
    console.log(order_id)
    get_order_by_id({order_id:order_id}).then(data => {
    console.log(data)
    goodList.value = data.result.goods
    order_status.value = data.result.status
    total_price.value = data.result.total
}).catch(() => {
    Message({ type: 'error', text: '请求异常'});
  })
}

getOrderReq()

const router = useRouter()

function cancelOrderReq() {
    let order_id = route.query.keyword
  cancel_order({order_id:order_id}).then(data => {
    if(!data.isSuccess) {
      Message({ type: 'error', text: data.message });
    }else{
      Message({ type: 'success', text: data.message });
      router.push({path:'/orders'})
    }
  }).catch(() => {
    Message({ type: 'error', text: '请求异常'});
  })
}

function payOrderReq() {
  let time = new Date()
  let order_id = route.query.keyword
  pay_order({order_id:order_id,bill_date:time.toLocaleDateString}).then(data => {
    if(!data.isSuccess) {
      Message({ type: 'error', text: data.message });
    }else{
      Message({ type: 'success', text: data.message });
      router.push({path:'/orders'})
    }
  }).catch(() => {
    Message({ type: 'error', text: '请求异常'});
  })
}

</script>

<template>
  <AppTopNav/>
    <div class="cart-page">
      <div class="container">
        <div class="cart">
          <table>
            <tr>
              <th>商品名称</th>
              <th>商店名称</th>
              <th>图片</th>
              <th>数量</th>
              <th>总价</th>
            </tr>   
            <tbody>
              <template v-for="req in goodList" :key="req.good_id">
                <tr>
                    <td class="cart">
                    {{ req.goodname }}
                    </td>
                    <td class="cart">
                    {{ req.shopname }}
                    </td>
                    <td class="cart">
                      <img :src="IMAGE_URL+req.image" style="width: 100px; height: 100px"/>
                    </td>
                    <td class="cart">
                    {{ req.count }}
                    </td>
                    <td class="cart">
                    {{ req.price*req.count }}
                    </td>
                </tr>
              </template>
            </tbody>
          </table>
        </div>
      </div>
    </div>
    <div class="cart-page">
      <div class="container">
        <div class="cart">
          <table>
            <tr>
              <th>订单总价</th>
            </tr>
            <tbody>
              <tr>
                <td class="cart">
                    {{ total_price }}
                </td>
              </tr>
            </tbody>
          </table>
        </div>
      </div>
    </div> 
    <el-button type="primary" class="admin-submit" @click="payOrderReq()">支付订单</el-button>
    <el-button type="warning" class="admin-submit" @click="cancelOrderReq()">撤销订单</el-button>
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