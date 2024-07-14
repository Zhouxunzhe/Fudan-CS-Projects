<script>
import HomePanel from "@/views/home/components/HomePanel";
// import { ref } from 'vue'
// import getNewGoods from "@/hooks/home/getNewGoods";
import { IMAGE_URL  } from '@/utils/url';
import useLazyData from "@/hooks/useLazyData";
import { getNewGoods } from "@/api/home";
import HomeSkeleton from "@/views/home/components/HomeSkeleton";
import XtxMore from "@/components/library/XtxMore"
export default {
  name: "HomeNew",
  components: { HomeSkeleton, HomePanel, XtxMore },
  setup() {
    // getNewGoods().then(data => {
    //   goods.value = data.goods
    // })
    // console.log(goods.value.images);
    const { target, result: goods } = useLazyData(getNewGoods);

    return { goods, target , IMAGE_URL};
  },
};
</script>


<template>
  <HomePanel title="新鲜好物" subTitle="新鲜出炉 品质靠谱" ref="target">
    <template v-slot:right>
      <XtxMore path="/goods" />
    </template>
    <template v-slot:default>
      <Transition name="fade">
        <ul class="goods-list" v-if="goods">
          <li v-for="item in goods" :key="item.id">
            <RouterLink :to="`/good/${item.id}`">
              <view v-for="(picture,index) in item.images" :key="index">
                <view v-if="index==0">
                  <img :src="IMAGE_URL+picture" alt="item.goodname" />
                </view>
              </view>
              <p class="name ellipsis">{{ item.goodname }}</p>
              <p class="price">&yen;{{ item.price }}</p>
            </RouterLink>
          </li>
        </ul>
        <HomeSkeleton v-else />
      </Transition>
    </template>
  </HomePanel>
</template>

<style scoped lang="less">
.goods-list {
  display: flex;
  justify-content: space-between;
  height: 406px;
  li {
    width: 306px;
    height: 406px;
    background: #f0f9f4;
    //less mixin
    .hoverShadow();
    img {
      width: 306px;
      height: 306px;
    }
    p {
      font-size: 22px;
      padding: 12px 30px 0 30px;
      text-align: center;
    }
    .price {
      color: @priceColor;
    }
  }
}
</style>
