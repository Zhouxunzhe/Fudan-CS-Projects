<template>
  <AppLayout type="shops">
    <div class="sub-category">
      <div class="container">
        <!-- 商品区块-->
        <div class="goods-list">
          <!-- 商品排序 -->
          <ShopsSubSort @onSortParamsChanged="onFilterSortChanged" />
          <!-- 商品列表 -->
          <ShopsList v-if="shops" :shops="shops" />
          <!-- 无限列表加载组件 -->
          <XtxInfiniteLoading
            :loading="loading"
            :finished="finished"
            @infinite="loadMore"
          />
        </div>
      </div>
    </div>
  </AppLayout>
</template>

<script>
import AppLayout from "@/components/AppLayout";
import ShopsSubSort from "@/views/shops/components/ShopsSubSort";
import ShopsList from "@/views/shops/components/ShopsList";
import useShops from "@/hooks/shops/useShops";
export default {
  name: "ShopAllPage",
  components: { ShopsList, ShopsSubSort, AppLayout },
  created() {
    // watch 路由的参数，以便再次获取数据
    this.$watch(
      () => this.$route.query,
      () => {
        console.log("create watch")
        this.modifyKeywords(this.$route.query.keyword)
      },
      // 组件创建完后获取数据，
      // 此时 data 已经被 observed 了
      { immediate: true }
    )
  },
  setup() {
    // 获取用户选择的筛选条件
    // const onFilterChanged = (target) => {
    //   console.log("fliterChange:", target);
    // };
    // 获取排序条件查询
    // const onSortParamsChanged = (target) => {
    //   console.log("sortParams:", target);
    // };

    // 获取商品数据
    const {
      result: shops,
      onFilterSortChanged,
      loading,
      finished,
      loadMore,
      modifyKeywords,
    } = useShops();

    onFilterSortChanged();

    return {
      shops,
      onFilterSortChanged,
      loading,
      finished,
      loadMore,
      modifyKeywords,
    };
  },
};
</script>

<style scoped lang="less">
.goods-list {
  background: #fff;
  padding: 0 25px;
  margin-top: 25px;
}
</style>
