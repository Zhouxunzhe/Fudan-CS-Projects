<template>
  <RouterLink :to="`/shop/${shops.shop_id}`" class="goods-item">
    <img :src="shops.logo" :alt="shops.shopname" />
    <p class="name ellipsis">{{ shops.shopname }}</p>
  </RouterLink>
</template>

<script>
export default {
  name: "ShopsItem",
  props: {
    shops: {
      type: Object,
      default: () => ({}),
    },
  },
};
</script>

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
