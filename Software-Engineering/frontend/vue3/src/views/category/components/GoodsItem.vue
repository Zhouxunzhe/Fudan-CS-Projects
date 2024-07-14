<script>
import { IMAGE_URL  } from '@/utils/url';
export default {
  name: "GoodsItem",
  props: {
    goods: {
      type: Object,
      default: () => ({}),
    },
  },
  setup() {

    return { IMAGE_URL };
  },
};
</script>

<template>
  <RouterLink :to="`/good/${goods.id}`" class="goods-item">
    <view v-for="(picture,index) in goods.images" :key="index">
      <view v-if="index==0">
        <img :src="IMAGE_URL+picture" :alt="goods.goodname" />
      </view>
    </view>
    <p class="name ellipsis">{{ goods.goodname }}</p>
    <p class="price">&yen;{{ goods.price }}</p>
  </RouterLink>
</template>

<style scoped lang="less">
.goods-item {
  display: block;
  width: 220px;
  min-height: 100%;
  padding: 20px 30px;
  text-align: center;
  .hoverShadow();
  img {
    width: 160px;
    height: 160px;
  }
  p {
    padding-top: 10px;
  }
  .name {
    font-size: 16px;
  }
  .desc {
    color: #999;
    height: 29px;
  }
  .price {
    color: @priceColor;
    font-size: 20px;
  }
}
</style>
