import { onBeforeRouteUpdate } from "vue-router";
import { getRelevantGoods } from "@/api/goods";
import { ref } from "vue";

export default function useGoodsRelevant(goodsid) {
  // 轮播图数据 [[{},{}],[{},{}],[{},{}]]
  const carousels = ref(null);

  // 获取数据
  const getGoodsRelevant = (id) => {
    console.log('getGoodsRelevant')
    console.log(id)
    carousels.value = [];
    getRelevantGoods(id).then((res) => {
      const size = 4;
      const total = Math.ceil(res.goods.length / size);
      // 存储数据
      for (let i = 0; i < total; i++) {
        carousels.value.push(res.goods.slice(i * size, i * size + size));
      }
    });
  };
  getGoodsRelevant(goodsid);

  // 路由跳转更新
  onBeforeRouteUpdate((to) => {
    getGoodsRelevant(to.params.id);
  });

  return { carousels };
}
