/**
 * @userAPI 用户API
 */
import request from "@/utils/request";

/**
 * 用户登录
 * @param username 账号
 * @param password 密码
 * @returns {*}
 */
export function loginByAccountAndPassword({ username, password }) {
  return request.post("/login", { username, password }, { withToken: true });
}

/**
 * 管理员登录
 * @param username 账号
 * @param password 密码
 * @returns {*}
 */
export function adminLoginByAccountAndPassword({ username, password }) {
  return request.post("/admin_login", { username, password }, { withToken: true });
}

/**
 * 注册
 */
export function signin({ username, password, is_shop, phonenumber, email, id_num }) {
  // console.log({ username, password, is_shop, phonenumber, email, id_num });
  return request.post("/register", { username, password, is_shop, phonenumber, email, id_num }, { withToken: true });
}

/**
 * 开店申请
 */
export function open_shop({ shopname, kind, intro, address, is_shop, register_capital, register_date }) {
  // console.log({ username, password, is_shop, phonenumber, email, id_num });
  return request.post("/open_shop", { shopname, kind, intro, address, is_shop, register_capital, register_date }, { withToken: true });
}

/**
 * 个人信息修改
 */
export function modify_userinfo({ username, email, phonenumber, password}) {
  return request.post("/modify_userinfo", { username, email, phonenumber, password}, { withToken: true });
}

/**
 * 账户信息
 */
export function user_account() {
  return request.get("/user/user_account", {}, { withToken: true });
}

/**
 * 用户充值
 */
export function recharge({ amount,bill_date }) {
  return request.post("/recharge", { amount,bill_date }, { withToken: true });
}

/**
 * 管理员账户信息
 */
export function admin_account() {
  return request.post("/admin_account", {}, { withToken: true });
}

/**
 * 管理员充值
 */
export function admin_recharge({ amount }) {
  return request.post("/admin_recharge", { amount }, { withToken: true });
}

/**
 * 管理员获取商品相关申请
 */
export function admin_get_good_request() {
  return request.get("/admin/get_good_request", {}, { withToken: true });
}

/**
 * 管理员审批商品相关请求
 */
export function admin_approve_good_request({ request_id,approval }) {
  return request.post("/admin/approve_good_request", { request_id,approval }, { withToken: true });
}

/**
 * 我的商店
 */
export function myshop() {
  return request.get("/shop/myshop", {}, { withToken: true });
}

/**
 * 新增商品
 */
export function add_goods({ goodname,intro,price,images,request_date,goodamount }) {
  return request.post("/add_goods", { goodname,intro,price,images,request_date,goodamount }, { withToken: true });
}

/**
 * 用户获取商品相关申请
 */
export function good_application_record() {
  return request.get("/shop/good_application_record", { }, { withToken: true });
}

/**
 * 修改商品信息
 */
export function modify_goods({ goodname,intro,price,images,good_id,request_date }) {
  return request.post("/modify_goods", { goodname,intro,price,images,good_id,request_date }, { withToken: true });
}

/**
 * 下架商品
 */
export function ban_good({ good_id,request_date }) {
  return request.post("/ban_good", { good_id,request_date }, { withToken: true });
}

export function shop_account() {
  return request.get("/shop/shop_account", {}, { withToken: true });
}

export function upload_image(formData) {
  return request.post("/upload_image", formData, { withToken: true,isFormData: true });
}

export function close_shop({request_date}) {
  return request.post("/close_shop", {request_date}, { withToken: true });
}

/**
 * 创建新的订单(根据订单ID)
 * @param id 订单ID
 * @returns {*}
 */
export function createOrderById(id) {
  return request.get(`/member/order/repurchase/${id}`, {}, { withToken: true });
}

export function get_order() {
  return request.get("/user/get_order", {}, { withToken: true });
}

export function cancel_order({order_id}) {
  return request.post("/user/cancel_order", {order_id}, { withToken: true });
}

export function get_order_by_id({order_id}) {
  return request.post("/user/get_order_by_id", {order_id}, { withToken: true });
}

export function pay_order({order_id,bill_date}) {
  return request.post("/user/pay_order", {order_id,bill_date}, { withToken: true });
}