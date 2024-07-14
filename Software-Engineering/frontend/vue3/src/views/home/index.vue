<template>
  <AppLayout type="goods">
  <div class='xtx-home-page'>
    <!-- 店铺展示 -->
    <HomeShops />
    <!-- 新鲜好物 -->
    <HomeNew />
  </div>
  </AppLayout>
</template>

<script>
import HomeShops from './components/HomeShops.vue';
import HomeNew from './components/HomeNew.vue';
import AppLayout from "@/components/AppLayout";

export default {
    name: "xtx-home-page",
    components: { HomeShops, HomeNew, AppLayout }
}
</script>

<style scoped lang='less'></style>