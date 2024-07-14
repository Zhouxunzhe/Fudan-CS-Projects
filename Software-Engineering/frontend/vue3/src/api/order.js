/**
 * @userAPI 订单API
 */
import request from "@/utils/request";

/**
 * 创建订单
 * @returns {*}
 */
export function createOrder() {
  return request.get("/member/order/pre", {}, { withToken: true });
}

/**
 * 添加收货地址
 * @param address 收货地址信息对象
 * @returns {*}
 */
export function addAddress(address) {
  return request.post("/member/address", address, { withToken: true });
}

/**
 * 获取收货地址
 * @param address 收获地址信息
 * @returns {*}
 */
export function getAddress(address) {
  return request.get("/member/address", address, { withToken: true });
}

/**
 * 修改收货地址（根据ID）
 * @param address 收获地址信息
 * @returns {*}
 */
export function updateAddress(address) {
  return request.put(`/member/address/${address.id}`, address, {
    withToken: true,
  });
}

/**
 * 提交订单
 * @param order 订单对象
 * @returns {*}
 */
export function submitOrder(order) {
  return request.post(`/user/submit_order`, order, { withToken: true, });
}

/**
 * 根据订单id获取订单详情
 * @param id 订单id
 * @returns {*}
 */
export function getOrderInfoById(order_id) {
  return request.post(`/user/get_order_by_id`, {order_id}, { withToken: true });
}

/**
 * 根据订单id支付订单
 * @param id 订单id
 * @param bill_data 订单日期
 * @returns {*}
 */
export function payOrderById(order_id, bill_data) {
  console.log("pay_order:")
  console.log({order_id, bill_data})
  return request.post(`/user/pay_order`, {order_id, bill_data}, { withToken: true });
}
