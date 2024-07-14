<template>
  <AppLayout>
    <div class="xtx-pay-page">
      <div class="container">
        <XtxBread>
          <XtxBreadItem path="/">首页</XtxBreadItem>
          <XtxBreadItem path="/cart">购物车</XtxBreadItem>
          <XtxBreadItem>支付订单</XtxBreadItem>
        </XtxBread>
        <!-- 付款信息 -->
        <div class="pay-info">
          <span class="icon iconfont icon-queren2"></span>
          <div class="tip">
            <p>订单提交成功！请尽快完成支付。</p>
            <p v-if="count > 0">
              支付还剩 <span>{{ dayjs.unix(count).format("mm分ss秒") }}</span
              >, 超时后将取消订单
            </p>
            <p v-else>订单已超时</p>
          </div>
          <div class="amount">
            <span>应付总额：</span>
            <span>¥{{ info?.total.toFixed(2) }}</span>
          </div>
        </div>
        <!-- 付款方式 -->
        <div class="pay-type" v-if="count > 0">
          <div class="submit">
            <XtxButton type="primary" @click="PayOrder(info.id)"
              >确认支付</XtxButton
            >
          </div>

        </div>
      </div>
    </div>
  </AppLayout>
  <XtxDialog title="正在支付..." v-model:visible="isPaying">
    <div class="pay-wait">
      <img src="@/assets/images/load.gif" alt="" />
      <div v-if="info">
        <p>如果支付成功：</p>
        <RouterLink :to="`/user/get_order_by_id/${info.id}`">查看订单详情></RouterLink>
        <p>如果支付失败：</p>
        <RouterLink to="/">查看相关疑问></RouterLink>
      </div>
    </div>
  </XtxDialog>
</template>
<script>
import AppLayout from "@/components/AppLayout";
import usePay from "@/hooks/order/usePay";
import XtxBreadItem from "@/components/library/XtxBreadItem";
import XtxBread from "@/components/library/XtxBread";
import XtxButton from "@/components/library/XtxButton";
import XtxDialog from "@/components/library/XtxDialog";
export default {
  name: "PayPage",
  components: { AppLayout, XtxBread, XtxBreadItem, XtxButton, XtxDialog },
  setup() {
    const { info, count, dayjs, payUrl, isPaying, PayOrder } = usePay();

    return { info, count, dayjs, payUrl, isPaying, PayOrder };
  },
};
</script>
<style scoped lang="less">
.pay-info {
  background: #fff;
  display: flex;
  align-items: center;
  height: 240px;
  padding: 0 80px;
  .icon {
    font-size: 80px;
    color: #1dc779;
  }
  .tip {
    padding-left: 10px;
    flex: 1;
    p {
      &:first-child {
        font-size: 20px;
        margin-bottom: 5px;
      }
      &:last-child {
        color: #999;
        font-size: 16px;
      }
    }
  }
  .amount {
    span {
      &:first-child {
        font-size: 16px;
        color: #999;
      }
      &:last-child {
        color: @priceColor;
        font-size: 20px;
      }
    }
  }
}
.pay-type {
  margin-top: 20px;
  background-color: #fff;
  padding-bottom: 70px;
  p {
    line-height: 70px;
    height: 70px;
    padding-left: 30px;
    font-size: 16px;
    &.head {
      border-bottom: 1px solid #f5f5f5;
    }
  }
  .btn {
    width: 150px;
    height: 50px;
    border: 1px solid #e4e4e4;
    text-align: center;
    line-height: 48px;
    margin-left: 30px;
    color: #666666;
    display: inline-block;
    &.active,
    &:hover {
      border-color: @xtxColor;
    }
    &.alipay {
      background: url(https://cdn.cnbj1.fds.api.mi-img.com/mi-mall/7b6b02396368c9314528c0bbd85a2e06.png)
        no-repeat center / contain;
    }
    &.wx {
      background: url(https://cdn.cnbj1.fds.api.mi-img.com/mi-mall/c66f98cff8649bd5ba722c2e8067c6ca.jpg)
        no-repeat center / contain;
    }
  }
}
.pay-wait {
  display: flex;
  justify-content: space-around;
  p {
    margin-top: 30px;
    font-size: 14px;
  }
  a {
    color: @xtxColor;
  }
}

.submit {
  text-align: right;
  padding: 60px;
  border-top: 1px solid #f5f5f5;
}
</style>
