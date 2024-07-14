<template>
  <nav class="app-topnav">
    <div class="container">
      <ul>
        <template v-if="user.profile.token">
          <li><a href="javascript:;"><i class="iconfont icon-user"></i>{{user.profile.account}}</a></li>
          <li><a @click="logout()" href="javascript:;">退出登录</a></li>
        </template>
        <template v-else>
          <li><RouterLink :to="`/login`">请先登录</RouterLink></li>
          <li><RouterLink :to="`/signin`">免费注册</RouterLink></li>
        </template>
        <template v-if="user.profile.token">
          <li><RouterLink :to="`/`">回到主页</RouterLink></li>
        </template>
        <template v-if="user.profile.is_shop">
          <li><RouterLink :to="`/setupshop`">申请开店</RouterLink></li>
        </template>
        <template v-if="user.profile.is_admin">
          <li><RouterLink :to="`/admin`">商店申请</RouterLink></li>
        </template>
        <template v-if="user.profile.is_admin">
          <li><RouterLink :to="`/adminapprovegood`">商品申请</RouterLink></li>
        </template>
        <template v-if="user.profile.is_admin">
          <li><RouterLink :to="`/adminaccount`">管理员账户</RouterLink></li>
        </template>
        <template v-if="user.profile.token && !user.profile.is_admin">
          <li><RouterLink :to="`/userinfo`">个人信息</RouterLink></li>
        </template>
        <template v-if="user.profile.is_shop">
          <li><RouterLink :to="`/myshop`">我的商店</RouterLink></li>
        </template>
        <template v-if="user.profile.token && !user.profile.is_admin">
          <li><RouterLink :to="`/orders`">查看订单</RouterLink></li>
        </template>
        <!-- <li><a href="javascript:;">我的订单</a></li>
        <li><a href="javascript:;">会员中心</a></li>
        <li><a href="javascript:;">帮助中心</a></li>
        <li><a href="javascript:;">关于我们</a></li> -->
      </ul>
    </div>
  </nav>
</template>

<script>
import { useStore } from 'vuex'
import { useRouter } from "vue-router";
// import { computed } from 'vue'  
export default {
  name: 'AppTopNav',
  setup () {
      // const props = defineProps(['isuser', 'isadmin'])
      const store = useStore()
      const user = store.state["user"];
      // const user = computed(()=>{
      //     return store.state.user.profile
      // })
      const router = useRouter()
      const logout = () => {
        store.commit('user/setUser',{})
        router.push('/login')
      }
      return { user, logout,}
  }
}
</script>

<style scoped lang="less">
.app-topnav {
  background: #333;
  ul {
    display: flex;
    height: 53px;
    justify-content: flex-end;
    align-items: center;
    li {
      a {
        padding: 0 15px;
        color: #cdcdcd;
        line-height: 1;
        display: inline-block;
        i {
          font-size: 14px;
          margin-right: 2px;
        }
        &:hover {
          color: @xtxColor;
        }
      }
      ~ li {
        a {
          border-left: 2px solid #666;
        }
      }
    }
  }
}
</style>