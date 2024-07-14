<script>
import { inject } from "vue";
import { IMAGE_URL  } from '@/utils/url';
export default {
  setup() {
    const goodsDetail = inject("goodsDetail");
    console.log('goodsdetail', goodsDetail)
    return { goodsDetail, IMAGE_URL };
  },
};
</script>

<!-- 静态布局 -->
<template>
  <div class="goods-detail">
    <!-- 属性 -->
    <ul class="attrs">
      <li :key="goodsDetail.goodname">
        <span class="dt">{{ goodsDetail.goodname }}</span>
        <span class="dd">{{ goodsDetail.price }}</span>
      </li>
    </ul>
    <!-- 图片 -->
    <view v-for="(picture,index) in goodsDetail.images" :key="index">
      <view v-if="index==0">
        <img :src="IMAGE_URL+picture" alt="goodsDetail.goodname" />
      </view>
    </view>
  </div>
</template>


<style scoped lang="less">
.goods-detail {
  padding: 40px;
  .attrs {
    display: flex;
    flex-wrap: wrap;
    margin-bottom: 30px;
    li {
      display: flex;
      margin-bottom: 10px;
      width: 50%;
      .dt {
        width: 100px;
        color: #999;
      }
      .dd {
        flex: 1;
        color: #666;
      }
    }
  }
  > img {
    width: 100%;
  }
}
</style>
