<template>
  <div class="home">

<b-jumbotron header="WELCOME!" lead="Dashboard page to monitor your company parking lot">
<form action="/dashboard" method="get" class="form-login">
        <div class="form-login">
          <label for="name">Username</label>
          <input type="text" name="name" id="name" />
        </div>
        <div class="form-login">
          <label for="password">Password</label>
          <input type="text" name="pass" id="pass" />
        </div>
        <!-- <div class="form-login">
          <input type="submit" value="Login" />
        </div> -->
      </form>
      <a href="/dashboard" class="btn btn-primary">Login</a>
 </b-jumbotron>
  </div>
</template>

<script>
// @ is an alias to /src
// [was] import HelloWorld from '@/components/HelloWorld.vue'

export default {
  name: "Home",
  components: {
    // [was] HelloWorld
  }
};
</script>

<style lang="scss" scoped>

.form-login {
  padding: 10px 0 0 0;
  text-align: center;
}
</style>