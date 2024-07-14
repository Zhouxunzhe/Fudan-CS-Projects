const { defineConfig } = require('@vue/cli-service')
const path = require('path')
module.exports = defineConfig({
  // devServer: {
  //   host: '0.0.0.0',
  //   port: 6103,
  //   client: {
  //     webSocketURL: 'ws://0.0.0.0:6103/ws',
  //   },
  //   headers: {
  //     'Access-Control-Allow-Origin': '*',
  //   }
  // },

  transpileDependencies: true,

  pluginOptions: {
    'style-resources-loader': {
      preProcessor: 'less',
      patterns: [
        // 自动引入 less
        path.join(__dirname, './src/assets/styles/variables.less'),
        path.join(__dirname, './src/assets/styles/mixins.less')
      ]
    }
  },
})
