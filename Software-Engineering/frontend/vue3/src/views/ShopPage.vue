<template>
  <AppLayout type="shops">
    <head>
      <meta name="referrer" content="no-referrer">
    </head>
    
    <div class="xtx-goods-page">
      <div class="container" v-if="shop">
        <!-- 商品信息 -->
        <div class="goods-info">
          <!-- 左侧 -->
          <div class="media">
            <img src="https://wqxuetang.oss-cn-beijing.aliyuncs.com/cover/3/237/3237558/3237558.jpg!wqb"/>
          </div>
          <!-- 右侧 -->
          <div class="spec">
            <p class="name">店铺名称：{{ shop.shopname }}</p>
            <p class="kind">商品种类：{{ shop.kind }}</p>
            <p class="intro">店铺简介：{{ shop.intro }}</p>
            <p class="addr">店铺地址：{{ shop.address }}</p>
            <p class="addr">注册时间：{{ shop.register_date }}</p>
          </div>
        </div>
      </div>
    </div>
  </AppLayout>
</template>

<script>
import { ref } from 'vue'
import { getShopDetails } from '@/api/home'
import { useRoute } from 'vue-router'
import AppLayout from "@/components/AppLayout";
export default {
  name: "ShopPage",
  components: { AppLayout },
  setup() {
    const shop = ref([])
    const route = useRoute()

    getShopDetails(route.params.id).then(data => {
      shop.value = data.result
    })

    return { shop };
  },
};
</script>

<style scoped lang="less">

.name {
  font-size: 30px;
}
.kind {
  font-size: 20px;
}
.intro {
  font-size: 18px;
}
.addr {
  font-size: 16px;
}
.goods-info {
  min-height: 600px;
  background: #fff;
  display: flex;
  .media {
    width: 580px;
    height: 600px;
    padding: 30px 50px;
  }
  .spec {
    flex: 1;
    padding: 30px 30px 30px 0;
  }
}
.goods-footer {
  display: flex;
  margin-top: 20px;
  .goods-article {
    width: 940px;
    margin-right: 20px;
  }
  .goods-aside {
    width: 280px;
    min-height: 1000px;
  }
}
.goods-tabs {
  min-height: 600px;
  background: #fff;
}
.goods-warn {
  min-height: 600px;
  background: #fff;
  margin-top: 20px;
}
</style>
