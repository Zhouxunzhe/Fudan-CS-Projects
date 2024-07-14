import { computed } from "vue";
import { submitOrder } from "@/api/order";
import Message from "@/components/library/Message";
import { useRouter, useRoute } from "vue-router";
import { useStore } from "vuex";
import { Number } from "core-js";

export default function useCheckout() {
  const router = useRouter();
  const store = useStore();
  const route = useRoute();

  // console.log(route.query);
  // console.log(typeof (route.query.id));
  // console.log(typeof (parseInt(route.query.id)));
  // console.log(parseInt(route.query.id));
  // console.log(route.query.price);

  function notEmpty(obj) {
    return !(Object.keys(obj).length === 0);
  }

  // 创建订单并存储订单信息
  const order_goods = notEmpty(route.query)
                ? [route.query] 
                : computed(
                  () => store.getters["cart/userSelectedGoodsList"]
                );

  // const order_goods = computed(
  //                 () => store.getters["cart/userSelectedGoodsList"]
  //               );

  const pre_order_count = notEmpty(route.query)
                ? parseInt(route.query.count)
                : computed(
                  () => store.getters["cart/userSelectedGoodsCount"]
                );

  const pre_order_totalPrice = notEmpty(route.query)
                ? parseInt(route.query.count) * Number(route.query.nowPrice)
                : computed(
                  () => store.getters["cart/userSelectedGoodsPrice"]
                );
  // 收货地址组件的实例对象

  console.log("order_goods");
  console.log(computed(
    () => store.getters["cart/userSelectedGoodsList"]
  ));

  const checkoutAddressInstance = {
    id:1,
    receiver:"csl",
    contact:"12345678911",
    fullLocation:"中国 上海市",
    address:"邯郸路 220号",
  };

  // // 判断路由中是否有ID参数
  // if (route.query.id) {
  //   // 创建新的订单（根据订单ID）
  //   createOrderById(route.query.id).then((data) => {
  //     order.value = data.result;
  //   });
  // } else {
  //   // 发起订单创建申请（无ID参数 根据服务器购物车创建订单）
  //   createOrder().then((data) => {
  //     order.value = data.result;
  //   });
  // }

  //#region 批量删除用户选择的商品、清空失效商品
  const deletePrePayedGoodsOfCart = () => {
    console.log("delete goods in cart")
    let flag = "userSelectedGoodsList";
    store.dispatch("cart/deleteGoodsOfCartByUserSelectedOrInvalid", flag);
  };
  //#endregion

  let time = new Date();

  //#region 创建提交订单信息
  const onSubmitOrder = () => {
    // 判断是否选择了地址
    // 收集订单信息
    const orderParams = {
      // 商品集合
      goods: order_goods.value 
            ? order_goods.value.map((item) => ({
              good_id: item.id,
              count: item.count
            }))
            : order_goods.map((item) => ({
              good_id: item.id,
              count: item.count
            })),
      total_price: pre_order_totalPrice.value ? pre_order_totalPrice.value : pre_order_totalPrice,
      request_date: time.toLocaleDateString(),
      addressId: 123, // 地址id，随便写一个
      deliveryTimeType: 1, // 配送时间类型:，1为不限，2为工作日，3为双休或假日
      payType: 1, // 支付方式:1为在线支付，2为货到付款
      payChannel: 1, // 支付渠道:1支付宝、2微信
      buyerMessage: "", // 买家留言
    };

    // 提交订单
    submitOrder(orderParams)
      .then((data) => {
        console.log("here")
        console.log(data);
        // 从购物车中删除商品
        console.log("here22");
        if(order_goods.value){
          deletePrePayedGoodsOfCart();
        }
        // 1.跳转到支付页面，将订单id作为路由参数
        console.log("result order_id")
        console.log(data.result.order_id)
        router.push({
          path: "/checkout/pay",
          query: {
            orderId: data.result.order_id,
          },
        });
        
        // 2.将服务端购物车中的商品同步到客户端
        if(order_goods.value){
          store.dispatch("cart/updateGoodsBySkuId");
        }
      })
      .catch(() => {
        Message({ type: "error", text: "订单提交失败" });
      });
  };

  //#endregion

  return { order_goods, checkoutAddressInstance, onSubmitOrder, pre_order_count, pre_order_totalPrice };
}
