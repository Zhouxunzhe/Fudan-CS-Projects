import { onBeforeRouteUpdate, useRoute } from "vue-router";
import { ref } from "vue";
import { getGoodsDetailById } from "@/api/goods";
import Message from "@/components/library/Message";
import { useStore } from "vuex";
import { IMAGE_URL  } from '@/utils/url';
import { useRouter } from "vue-router";

export default function useGoods() {
  const route = useRoute();
  const store = useStore();
  const router = useRouter();
  // 存储数据
  const result = ref(null);

  // 获取数据
  const getGoodsDetail = (id) => {
    getGoodsDetailById(id).then((res) => {
      
      for(let i=0; i<res.good.images.length; i++){
        res.good.images[i] = IMAGE_URL+res.good.images[i];
      }
      result.value = res.good;
    });
  };
  getGoodsDetail(route.params.id);

  // 路由跳转更新
  onBeforeRouteUpdate((to) => {
    getGoodsDetail(to.params.id);
  });

  // 接收数据变化
  const onSpecChange = (data) => {
    // console.log(data); //@log
    result.value.price = data.price;
    result.value.oldPrice = data.oldPrice;
    result.value.stock = data.stock; //商品库存
    result.value.currentSelectedSkuId = data.skuId; //商品skuid
    result.value.currentSelectedSkuText = data.specsText; //商品规格描述
  };

  // 存储用户选择的商品数量
  const count = ref(1);

  //#region 加入购物车
  const addCart = () => {
    // 判断用户是否存储了规格
    // if (!result.value.currentSelectedSkuId) {
    //   return Message({ type: "error", text: "请选择商品规格" });
    // }
    // 当前添加的商品详情信息
    const goods = {
      id: result.value.id, // 商品id
      // skuId: result.value.currentSelectedSkuId, // 商品 skuId
      goodname: result.value.goodname, //商品名称
      intro: result.value.intro, //商品名称
      // attrsText: result.value.currentSelectedSkuText, //商品规格属性文字
      picture: result.value.images[0], //商品图片
      // price: result.value.oldPrice, //商品原价
      nowPrice: result.value.price, //商品现价
      price: result.value.price, //商品现价
      selected: false, //是否选中
      stock: result.value.stock, //商品库存
      count: count.value, //用户选择的商品数量
      isEffective: true, //如果商品选择了规格，该商品就一定是有效商品，因为能够选择的规格都是有库存的
      shop_name: result.value.shop_name,
    };
    // console.log(goods); //@log

    // 将商品数据存到本地store
    store
      .dispatch("cart/addGoodsToCart", goods)
      .then(() => {
        // Message({ type: "success", text: "商品已经成功被添加到购物车中" });
      })
      .catch((error) => {
        const msg = error.err.response.data.message;
        Message({ type: "success", text: msg });
      });
  };
  //#endregion


  //#region 立即下单
  const gotoOrder = () => {
    // 判断用户是否存储了规格
    // if (!result.value.currentSelectedSkuId) {
    //   return Message({ type: "error", text: "请选择商品规格" });
    // }
    // 当前添加的商品详情信息
    const goods = {
      id: result.value.id, // 商品id
      // skuId: result.value.currentSelectedSkuId, // 商品 skuId
      goodname: result.value.goodname, //商品名称
      intro: result.value.intro, //商品名称
      // attrsText: result.value.currentSelectedSkuText, //商品规格属性文字
      picture: result.value.images[0], //商品图片
      // price: result.value.oldPrice, //商品原价
      nowPrice: result.value.price, //商品现价
      price: result.value.price, //商品现价
      selected: true, //是否选中
      stock: result.value.stock, //商品库存
      count: count.value, //用户选择的商品数量
      isEffective: true, //如果商品选择了规格，该商品就一定是有效商品，因为能够选择的规格都是有库存的
    };
    console.log(goods); //@log

    router.push({path: "/checkout/order", query:goods})

    // 将商品数据存到本地store
    // store
    //   .dispatch("cart/addGoodsToCart", goods)
    //   .then(() => {
    //     // Message({ type: "success", text: "商品已经成功被添加到购物车中" });
    //     router.push({
    //       path: "/checkout/order"
    //     });
    //   })
    //   .catch((error) => {
    //     const msg = error.err.response.data.message;
    //     Message({ type: "success", text: msg });
    //   });

  };
  //#endregion

  return { result, onSpecChange, count, addCart, gotoOrder };
}
