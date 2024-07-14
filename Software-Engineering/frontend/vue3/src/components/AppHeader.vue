<template>
  <header class="app-header">
    <div class="container">
      <h1 class="logo"><RouterLink to="/"></RouterLink></h1>
      <AppHeaderNav />
      <div class="search">
        <i class="iconfont icon-search"></i>
        <input v-if="type === `goods`" type="search" placeholder="搜商品" v-model="keyword" @keydown.enter="searchEnterFun" />
        <input v-if="type === `shops`" type="search" placeholder="搜店铺" v-model="keyword" @keydown.enter="searchEnterFun" />
      </div>
      <AppHeaderCart />
    </div>
  </header>
</template>

<script>
import AppHeaderNav from "@/components/AppHeaderNav";
import AppHeaderCart from "@/components/AppHeaderCart";
import { ref } from "vue";
import { useRouter } from "vue-router";
export default {
  name: "AppHeader",
  props: {
    type: {
      type: String,
      default: 'goods'
    }
  },
  components: { AppHeaderCart, AppHeaderNav },
  setup(props) {
    const keyword = ref(null)
    const router = useRouter()
    
    function searchEnterFun(e){
      let keyCode = window.event ? e.keyCode : e.which;
      if(keyCode == 13){
        console.log(keyword.value)
        if(props.type === 'goods'){
          router.push({path:"/goods", query:{keyword : keyword.value}})
        }
        if(props.type === 'shops'){
          router.push({path:"/shops", query:{keyword : keyword.value}})
        }
      }
    }
    return {searchEnterFun, keyword}
  },
  
};
</script>

<style scoped lang="less">
.app-header {
  background: #fff;
  .container {
    display: flex;
    align-items: center;
  }
  .logo {
    width: 200px;
    a {
      display: block;
      height: 132px;
      width: 100%;
      text-indent: -9999px;
      background: url(../assets/images/logo.png) no-repeat center 18px / contain;
    }
  }
  .search {
    width: 170px;
    height: 32px;
    position: relative;
    border-bottom: 1px solid #e7e7e7;
    line-height: 32px;
    .icon-search {
      font-size: 18px;
      margin-left: 5px;
    }
    input {
      width: 140px;
      padding-left: 5px;
      color: #666;
    }
  }
}
</style>
