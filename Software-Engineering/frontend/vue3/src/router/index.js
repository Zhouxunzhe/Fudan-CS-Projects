import { createRouter, createWebHashHistory } from 'vue-router'
import authGuard from "./authGuard";

const Home = () => import('@/views/home/index')
const LoginView = () => import('@/views/LoginView')
const SigninView = () => import('@/views/SigninView')
const AdminLoginView = () => import('@/views/AdminLoginView')
const SetupShopApplyView = () => import('@/views/SetupShopApplyView')
const AdminPage = () => import('@/views/AdminPage')
const ShopsPage = () => import('@/views/ShopAllPage')
const ShopPage = () => import('@/views/ShopPage')
const GoodsPage = () => import('@/views/category/SubCategoryPage')
const GoodPage = () => import('@/views/goods/GoodsDetailPage')
const CartPage = () => import('@/views/cart/CartPage')
const ModifyUserInfoView = () => import('@/views/modifyUserInfoView')
const UserInfoView = () => import('@/views/UserInfoView')
const MyShopView = () => import('@/views/MyShopView')
const ShopGoodRequestView = () => import('@/views/ShopGoodRequestView')
const AdminAccountView = () => import('@/views/AdminAccountView')
const AdminApproveGoodView = () => import('@/views/AdminApproveGoodView')
const AddGood = () => import('@/components/AddGoodForm')
const ModifyGood = () => import('@/components/ModifyGoodForm')
const CheckoutPage = () => import('@/views/pay/CheckoutPage')
const PayPage = () => import("@/views/pay/PayPage");
const PayResultPage = () => import("@/views/pay/PayResultPage");
const OrderView = () => import('@/views/OrderView')
const SingleOrderView = () => import('@/views/SingleOrderView')


const routes = [
  { path: '/', component: Home },
  { path: '/shops', component: ShopsPage },
  { path: '/shop/:id', component: ShopPage },
  { path: '/admin', component: AdminPage },
  { path: '/goods', component: GoodsPage },
  { path: '/good/:id', component: GoodPage },
  { path: '/login', component: LoginView },
  { path: '/signin', component: SigninView },
  { path: '/adminlogin', component: AdminLoginView },
  { path: '/setupshop', component: SetupShopApplyView },
  { path: '/cart', component: CartPage },
  { path: '/modifyinfo', component: ModifyUserInfoView },
  { path: '/userinfo', component: UserInfoView },
  { path: '/myshop', component: MyShopView },
  { path: '/shopgoodrequest', component: ShopGoodRequestView },
  { path: '/adminaccount', component: AdminAccountView },
  { path: '/adminapprovegood', component: AdminApproveGoodView },
  { path: '/addgood', component: AddGood},
  { path: '/modifygood', component: ModifyGood},
  { path: '/checkout/order', component: CheckoutPage},
  { path: "/checkout/pay", component: PayPage },
  { path: "/pay/callback", component: PayResultPage },
  { path: '/orders', component: OrderView},
  { path: '/order', component: SingleOrderView}
]

const router = createRouter({
  history: createWebHashHistory(),
  routes
})

// 路由守卫——登录拦截
router.beforeEach(authGuard);

export default router
