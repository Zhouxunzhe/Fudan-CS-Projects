<template>
  <div class="home-shops">
    <head>
      <meta name="referrer" content="no-referrer">
    </head>
    <HomePanel title="热门店铺" sub-title="大众点评 品质保证">
      <template #right><XtxMore path="/shops" /></template>
      <!-- 面板内容 -->
      <ul class="shops-list">
        <li v-for="item in shops" :key="item.shop_id">
          <!-- 店铺链接 -->
          <RouterLink :to="`/shop/${item.shop_id}`">
            <img src="https://wqxuetang.oss-cn-beijing.aliyuncs.com/cover/3/237/3237558/3237558.jpg!wqb" alt="item.name">
            <p class="name">{{item.shopname}}</p>
            <p class="kind">商品种类: {{item.kind}}</p>
          </RouterLink>
        </li>
      </ul>
    </HomePanel>
  </div>
</template>
<script>
import { ref } from 'vue'
import HomePanel from './HomePanel.vue'
import { getShops } from '@/api/home'
import XtxMore from "@/components/library/XtxMore"
export default {
  name: 'HomeShops',
  components: { XtxMore, HomePanel },
  setup () {
    const shops = ref([])

    // 这里是异步调用(promise.then 语法)，如果 getShops 直接返回数据的话
    // getShops 是同步的(不是 promise)，这里会报错
    getShops().then(data => {
      shops.value = data.result
    })

    // 下面的代码用于调试前端效果，getShops() 直接返回数据
    // 直接进行调用
    // shops.value = getShops().result
    
    return { shops }
  }
}
</script>
<style scoped lang="less">
.shops-list {
  display: flex;
  justify-content: space-between;
  height: 406px;
  li {
    width: 306px;
    height: 406px;
    background: #f0f9f4;
    .hoverShadow();
    img {
      width: 306px;
      height: 306px;
    }
    p {
      font-size: 22px;
      padding: 12px 30px 0 30px;
      text-align: center;
    }
    .kind {
      font-size: 14px;
      color: @xtxColor;
    }
  }
}
</style>