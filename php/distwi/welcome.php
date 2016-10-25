<div id="welcomebox">
  <div id="registerbox">
    <h2>Register!</h2>
    <b>Want to try Distwi? Create an account!</b>
    <form method="POST" action="register.php">
      <table>
        <tr>
          <td>Username</td><td><input type="text" name="username"></td>
        </tr>
        <tr>
          <td>Password</td><td><input type="password" name="password"></td>
        </tr>
        <tr>
          <td>Password (again)</td><td><input type="password" name="password2"></td>
        </tr>
        <tr>
          <td colspan="2" align="right"><input type="submit" name="doit" value="Create an account"></td>
        </tr>
      </table>
    </form>
    <h2>Already registered? Login here</h2>
    <form method="POST" action="login.php">
      <table>
        <tr>
          <td>Username</td><td><input type="text" name="username"></td>
        </tr>
        <tr>
          <td>Password</td><td><input type="password" name="password"></td>
        </tr>
        <tr>
          <td colspan="2" align="right"><input type="submit" name="doit" value="Login"></td>
        </tr>
      </table>
    </form>
  </div>
  Distwi is a very simple PHP application.
</div>
