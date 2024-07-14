<template>
  <AppTopNav />
  <AppHeader :type="type"/>
  <AppHeaderSticky />
  <slot />
  <AppFooter />
</template>
<script>
import AppTopNav from "@/components/AppTopNav";
import AppFooter from "@/components/AppFooter";
import AppHeader from "@/components/AppHeader";
import AppHeaderSticky from "@/components/AppHeaderSticky";

export default {
  name: "AppLayout",
  props: {
    type: {
      type: String,
      default: 'goods'
    }
  },
  components: { AppHeaderSticky, AppHeader, AppFooter, AppTopNav },
  setup() {
    // const store = useStore();
    // store.dispatch('category/setCategories');
  },
};
</script>
