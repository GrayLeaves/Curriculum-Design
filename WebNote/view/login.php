<!DOCTYPE html>
<html lang="zh-CN">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>WebNote</title>

    <link href="css/bootstrap.css" rel="stylesheet">

    <!--[if lt IE 9]>
      <script src="https://cdn.bootcss.com/html5shiv/3.7.3/html5shiv.min.js"></script>
      <script src="https://cdn.bootcss.com/respond.js/1.4.2/respond.min.js"></script>
    <![endif]-->
  </head>
  <body>
    <div class="container">
      <div class="row">
        <div class="col-xs-12">
          <h1 class="text-center"><img src="logo.png" style="width: 100px;"><font face="Courier New" color="gray" size="10">WebNote</font></h1>
          <hr/>
        </div>
      </div>
      <div class="row">
        <div class="col-xs-12">
          <div style="max-width: 320px; margin: 20px auto;">
            <div class="form-group input-group">
              <!--<label for="username">UserName</label>-->
              <span class="input-group-addon" style="border:1px #bfc9ca solid; background-color:white;">
                <a class="glyphicon glyphicon-user" style="text-decoration:none"></a></span>
              <input type="text" name="username" value="<?= $loginUser->name ?? '' ?>" class="form-control" id="username" placeholder="账号">
            </div>
            <div class="form-group input-group">
              <!--<label for="password">PassWord</label>-->
              <span class="input-group-addon" style="border:1px #bfc9ca solid;background-color:white;">
                <a class="glyphicon glyphicon-lock" style="text-decoration:none"></a></span>
                <input type="password" name="password" value="" class="form-control" id="password" placeholder="密码">
            </div>
            <div class="comp pull-left" style="padding: 0px 5px;">
              <input type="checkbox" name="remember[]" value="" style="font-size:12px;"> 记住密码?
            </div>
            <div class="comp pull-right" style="padding: 0px 5px;">
              <a class="pull-right text-primary " id="update" style="text-decoration:none; font-size:13px; " href="#">重置密码?</a>
            </div>
            <br>
            <br>
            <div class="form-group comp pull-left">
              <button class="btn btn-primary btn-lg" id="login" type="button" style="min-width: 150px;">Login</button>
            </div>
            <div class="form-group comp pull-right">
              <button class="btn btn-primary btn-lg" id="register" type="button" style="min-width: 150px;">Register</button>
            </div>
          </div>
        </div>
      </div>
        
        <div class="row">
          <div class="col-xs-12 text-center">
            <hr/>
            <h6>
            <a id="hide" class="text-primary glyphicon glyphicon-eye-close" style="text-decoration:none; padding: 0px 0px 0px 10px" href="javascript:show()"></a> <a id="show" class="text-primary glyphicon glyphicon-eye-open" style="display:none; text-decoration:none; padding: 0px 0px 0px 10px" href="javascript:hide()"></a></h6>
            <h6><font face="Courier New" color="#696969" size="2">注:若刚注册账户或重置密码,需刷新查看</font></h6>
            <div class="col-xs-12 text-muted" id="users" style="display:none">
              <?php 
                //$users = array(array("1", "wzg", "2745351", "1"));
                if(!empty($users)){
                    echo "<table width='400' border='1' align='center'><tr align='center'>";
                    echo "<td>序号</td><td>账户</td><td>密码</td><td>等级</td><td>操作</td></tr>";
                    foreach ($users as $user) {
                        //print_r($user);
                        echo "<tr><td>".$user['id']."</td><td>".$user['name']."</td><td>".$user['password']."</td><td>".$user['rank']."</td><td><a class=\"text-primary glyphicon glyphicon-remove-circle\" style=\"text-decoration:none;\" href=\"clean.php?id=".$user['id']."\"></a></td></tr>";
                    }
                }
                echo "</table>";
              ?>
            </div>
            
          </div>
        </div>
    </div>

    <script src="js/jquery-3.2.1.js"></script>
    <script type="text/javascript">
    function show(){
        document.getElementById('users').style.display='block';
        document.getElementById('hide').style.display='none';
        document.getElementById('show').style.display='block';
    }
    function hide(){
        document.getElementById('users').style.display='none';
        document.getElementById('hide').style.display='block';
        document.getElementById('show').style.display='none';
    }
    $(function() {
        $('#update').on('click', function(e) {
            var username = $('#username').val();
            if ($.trim(username) == '') {
                $('#username').focus();
                alert('请输入要更改的账户');
                return;
            }
            var password = $('#password').val();
            if ($.trim(password) == '') {
                $('#password').focus();
                alert('请输入更新后的密码');
                return;
            }
            if (!confirm('是否确定重置该账户的密码？( ╯□╰ )')) {
                return;
            }
            $.post('index.php', {
                action: 'updatepwd',
                username: username,
                password: password
            }, function(result) {
                if (result && result.error) {
                    alert(result.error);
                    return;
                }
                location.href = 'index.php';
            }, 'json');
        });
        
        $('#login').on('click', function(e) {
            var username = $('#username').val();
            if ($.trim(username) == '') {
                $('#username').focus();
                alert('请输入用户名');
                return;
            }
            var password = $('#password').val();
            if ($.trim(password) == '') {
                $('#password').focus();
                alert('请输入密码');
                return;
            }
            $.post('index.php', {
                action: 'login',
                username: username,
                password: password
            }, function(result) {
                if (result && result.error) {
                    alert(result.error);
                    return;
                }
                location.href = 'index.php';
            }, 'json');
        });
        
        $('#register').on('click', function(e){
            if (!confirm('是否要注册新的账户呢？( ╯□╰ )')) {
                return;
            }
            var username = $('#username').val();
            if ($.trim(username) == '') {
                $('#username').focus();
                alert('请输入要注册的帐户名字');
                return;
            }
            var password = $('#password').val();
            if ($.trim(password) == '') {
                $('#password').focus();
                alert('请输入要注册账户的密码');
                return;
            }
            $.post('index.php', {
                action: 'register',
                username: username,
                password: password
            }, function(result) {
                if (result && result.error) {
                    alert(result.error);
                    return;
                }
                //location.reload();
                location.href = 'index.php';
            }, 'json');
        });
        
        $('.glyphicon-remove-circle').on('click', function(e) {
            if (!confirm('是否删除该账户及其私有笔记？删除后可没法恢复哦(T_T)')) {
                return;
            }
            $.post('clean.php', {}, function(result) {
                if (result && result.error) {
                    alert(result.error);
                    return;
                }
                location.go(0);
                //location.href = 'index.php';
            }, 'json');
        });
    });
    </script>
  </body>
</html>
