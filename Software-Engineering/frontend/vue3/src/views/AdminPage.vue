<template>
  <AppTopNav/>
    <div class="cart-page">
      <el-dialog
      title="修改意见"
      v-model="dialogVisible"
      width="30%"
      >
        <el-form label-width="120px">

          <el-input v-model="comment" placeholder="Please input" />
          <el-form-item></el-form-item>
          <el-form-item>
          </el-form-item>
          <el-button type="primary" class="admin-submit" @click="rejectOpenShopReq(form_request_id, comment)">提交意见</el-button>
          
          
        </el-form>  
      </el-dialog>
      <div class="container">
        <div class="cart">
          <table>
            <tr>
              <th>店铺名称</th>
              <th>审批类别</th>
              <th>申请简介</th>
              <th>操作/状态</th>
            </tr>   
            <!-- 有效商品 -->
            <tbody>
              <tr v-for="req in reqList" :key="req.request_id">
                <td class="cart">
                  {{ req.shopname }}
                </td>
                <td class="cart"> 
                  <div v-if="req.request_type === 1">
                  开店
                  </div>
                  <div v-if="req.request_type === 2">
                  关店
                  </div>
                </td>
                <td class="cart">
                  {{ req.info }}
                </td>
                <td class="tc">
                    <div v-if="req.is_check">
                      <div v-if="req.comment == 1">
                        通过
                      </div>
                      <div v-else>
                        拒绝
                      </div>
                    </div>
                    <div v-else>
                      <el-button type="primary" class="admin-submit" @click="approveOpenShopReq(req.request_id)">批准</el-button>
                      <el-button type="warning" class="admin-submit" @click="showModal(req.request_id)">拒绝</el-button>
                    </div>
                </td>
              </tr>
            </tbody>
          </table>
        </div>
      </div>
    </div>
</template>
<script>
import { ref } from 'vue'
import { getOpenRequest } from '@/api/home'
import { approveOpenShop } from '@/api/home'
import AppTopNav from "@/components/AppTopNav";
import Message from "@/components/library/Message";
export default {
  name: "AdminPage",
  components: { AppTopNav, },
  setup() {
    const reqList = ref([])
    const comment = ref('')

    const dialogVisible = ref(false)
    const form_request_id = ref(0);

    const showModal = (request_id) => {
      console.log('show')
      dialogVisible.value = true;
      form_request_id.value = request_id;
    };

    getOpenRequest().then(data => {
      reqList.value = data.result
    })
    const approveOpenShopReq = (request_id) => {
      // console.log(shop_id);
      approveOpenShop({request_id, approval:true, comment:""}).then(data => {
        if(!data.isSuccess) {
          Message({ type: 'error', text: data.message });
          return false;
        }else{
          Message({ type: 'success', text: data.message });
          getOpenRequest().then(data => {
            reqList.value = data.result
          })
        }
      }).catch(() => {
        Message({ type: 'error', text: '请求异常'});
      })
    }
    const rejectOpenShopReq = (request_id, reqcomment) => {
      
      console.log(reqcomment);
      approveOpenShop({request_id, approval:false, comment:reqcomment}).then(data => {
        if(!data.isSuccess) {
          Message({ type: 'error', text: data.message });
          return false;
        }else{
          Message({ type: 'success', text: data.message });
          getOpenRequest().then(data => {
            reqList.value = data.result
          })
        }
      }).catch(() => {
        Message({ type: 'error', text: '请求异常'});
      })
      dialogVisible.value = false;
      comment.value = '';
    }
    return { reqList, approveOpenShopReq, rejectOpenShopReq, comment, form_request_id, showModal, dialogVisible }
  },
};
</script>
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
