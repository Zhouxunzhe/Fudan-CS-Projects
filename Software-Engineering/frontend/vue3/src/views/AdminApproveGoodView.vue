<script setup>
import { ref } from 'vue'
import { admin_approve_good_request,admin_get_good_request } from "@/api/user";

import AppTopNav from "@/components/AppTopNav";
import Message from "@/components/library/Message";

const requestList = ref([])
admin_get_good_request().then(data => {
  requestList.value = data.result
})

function approveGoodReq(request_id) {
  admin_approve_good_request({request_id:request_id, approval:true}).then(data => {
    if(!data.isSuccess) {
      Message({ type: 'error', text: data.message });
    }else{
      Message({ type: 'success', text: data.message });
      admin_get_good_request().then(data => {
        requestList.value = data.result
      })
    }
  }).catch(() => {
    Message({ type: 'error', text: '请求异常'});
  })
}

function rejectGoodReq(request_id) {
  admin_approve_good_request({request_id:request_id, approval:false}).then(data => {
    if(!data.isSuccess) {
      Message({ type: 'error', text: data.message });
    }else{
      Message({ type: 'success', text: data.message });
      admin_get_good_request().then(data => {
        requestList.value = data.result
      })
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
              <th>店铺名称</th>
              <th>商品名称</th>
              <th>请求类型</th>
              <th>简介</th>
              <th>申请时间</th>
              <th>操作/状态</th>
            </tr>   
            <!-- 有效商品 -->
            <tbody>
              <tr v-for="req in requestList" :key="req.request_id">
                <td class="cart">
                  {{ req.shopname }}
                </td>
                <td class="cart"> 
                  {{ req.goodname }}
                </td>
                <td class="cart"> 
                  {{ req.request_type === 1 ? "添加" : request_type === 2 ? "删除" : "修改" }}
                </td>
                <td class="cart">
                  {{ req.info }}
                </td>
                <td class="cart">
                  {{ req.request_date }}
                </td>
                <td class="tc">
                    <div v-if="!req.is_open">
                      失效
                    </div>
                    <div v-else-if="req.is_check">
                      <div v-if="req.comment == 1">
                        通过
                      </div>
                      <div v-else>
                        拒绝
                      </div>
                    </div>
                    <div v-else>
                      <el-button type="primary" class="admin-submit" @click="approveGoodReq(req.request_id)">批准</el-button>
                      <el-button type="warning" class="admin-submit" @click="rejectGoodReq(req.request_id)">拒绝</el-button>
                    </div>
                </td>
              </tr>
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