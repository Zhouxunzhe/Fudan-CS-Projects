<template>
  <ul>
    <li v-for="item in shops" :key="item.shop_id">
      <ShopsItem :shops="item" />
    </li>
  </ul>
</template>
<script>
import ShopsItem from "@/views/shops/components/ShopsItem";
export default {
  name: "ShopsList",
  components: { ShopsItem },
  props: {
    shops: {
      type: Array,
      default: () => [],
    },
  },
};
</script>

<style scoped lang="less">
ul {
  display: flex;
  flex-wrap: wrap;
  padding: 0 5px;
  li {
    margin-right: 20px;
    margin-bottom: 20px;
    &:nth-child(5n) {
      margin-right: 0;
    }
  }
}
</style>
