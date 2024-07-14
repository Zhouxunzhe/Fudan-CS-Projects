const user = {
  namespaced: true, //开启命名空间
  state() {
    return {
      // 用户信息
      profile: {
        account: "", //用户账号
        mobile: "", //用户手机号
        id_num:"", //用户身份证号
        email:"", //用户邮箱
        token: "", //用户登录令牌
        is_shop:"", //是否是商户(string true or false)
        is_admin:"", //是否是商户(string true or false)
      },
      // 重定向地址
      redirectURL: "",
    };
  },
  mutations: {
    setUser(state, payload) {
      state.profile = payload;
    },
    setToken(state, payload = "") {
      state.profile.token = payload;
    },
    // 设置重定向地址
    setRedirectURL(state, payload = "") {
      state.redirectURL = payload;
    },
  },
  actions: {
    test(){
      
    }
  },
};

export default user;
