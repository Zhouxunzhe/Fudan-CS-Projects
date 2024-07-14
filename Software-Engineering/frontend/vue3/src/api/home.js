/**
 * @homeAPI 首页API
 */
import request from "@/utils/request";

/**
 * 获取热门店铺信息
 * @param limit 店铺数量
 * @returns {*}
 */
export function getShops(limit = 4) {
  return request.get("/home/shops", { limit }, { withToken: true });
}

/**
 * 获所有取店铺信息
 * @returns {*}
 */
export function getallShops(params) {
  return request.get("/home/allshops", params);
}

/**
 * 获取查询店铺信息
 * @param params 分类id、筛选条件、排序条件、分页信息
 * @param keywords 搜索关键词
 * @returns {*}
 */
export function reqallShops(categoryId, page, pageSize, keywords) {
  console.log({categoryId, page, pageSize, keywords})
  return request.post("/home/allshops/search", {categoryId, page, pageSize, keywords});
}

/**
 * 获取开店申请
 * @param limit 店铺数量
 * @returns {*}
 */
export function getOpenRequest(limit = 4) {
  return request.get("/admin/get_shop_request", { limit }, { withToken: true });
}

/**
 * 批准开店申请
 * @returns {*}
 */
export function approveOpenShop({request_id, approval, comment}) {
  return request.post("/admin/approve_shop_request", { request_id, approval, comment }, { withToken: true });
}

/**
 * 获取商店详细信息
 * @returns {*}
 */
export function getShopDetails(shop_id) {
  return request.get("/home/shophome", { shop_id }, { withToken: true });
}

/**
 * 获取新鲜好物
 * @param limit 商品数量
 * @returns {*}
 */
export function getNewGoods(limit = 4) {
  return request.get("/home/get_hotgoods", { limit });
}

// 获取首页分类数据
export function getCategoriesAPI() {
  return request.get("/home/category/head");
}

