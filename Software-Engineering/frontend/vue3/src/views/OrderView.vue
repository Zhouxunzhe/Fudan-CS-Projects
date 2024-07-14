<script setup>
import { ref } from 'vue'
import { get_order } from "@/api/user";

import AppTopNav from "@/components/AppTopNav";
import { useRouter } from "vue-router";

const orderList = ref([])
const order_status = ref(1)
get_order().then(data => {
  console.log(data)
  orderList.value = data.result
})
const options = [
  {
    value: 1,
    label: '待支付',
  },
  {
    value: 2,
    label: '已支付',
  },
  {
    value: 3,
    label: '已撤销',
  }
]

const router = useRouter()

function toOrder(order_id) {
  router.push({path:'/order',query:{keyword : order_id}})
}
</script>

<template>
  <AppTopNav/>
  <el-select v-model="order_status" placeholder="请选择订单状态" size="large">
    <el-option
      v-for="item in options"
      :key="item.value"
      :label="item.label"
      :value="item.value"
    />
  </el-select>
    <div class="cart-page">
      <div class="container">
        <div class="cart">
          <table>
            <tr>
              <th>订单编号</th>
              <th>总价</th>
              <th>下单时间</th>
              <th>状态/操作</th>
            </tr>   
            <tbody>
              <template v-for="req in orderList" :key="req.order_id">
                <tr v-if="req.status==order_status">
                    <td class="cart">
                    {{ req.order_id }}
                    </td>
                    <td class="cart">
                    {{ req.total }}
                    </td>
                    <td class="cart">
                    {{ req.date }}
                    </td>
                    <td class="tc">
                      <el-button type="primary" class="admin-submit" @click="toOrder(req.order_id)">查看订单</el-button>
                    </td>
                </tr>
              </template>
            </tbody>
          </table>
        </div>
      </div>
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