<?php
### 需要安装PDO与PDO_MYSQL库
require 'model/NoteModel.php';

$model = new NoteModel();
$records = $model->read($user->id);
$error = $model->getError();
?>

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
      <div id="header" class="neighbort-header new shift-with-hiw pc">
        <div class="regular-header regular-header--new clearfix" id="new-header">
          <div class="comp pull-left" style="padding: 0px 0px;">
            <span style="float: left;">
              <img src="logo.png" style="width: 60px;">
            </span>
            <span style="float: left;">
              <h3 style="margin: 0; font-size: 30px; padding: 15px 0px 0px 0px"><font face="Courier New" color="#696969">WebNote</font></h3> <!--top right bottom left-->
            </span>
          </div>
          <div class="comp pull-right" style="padding: 0px 0px;">
            <span style="float: left;">
              <h4 class="text-muted" style="margin: 0; font-size: 25px; padding: 15px 10px 0px 5px"><font face="Courier New" color="#696969"><?= $user->name ?></font>
              </h4>
            </span>
            <span style="float: left;">
              <a class="pull-right text-primary glyphicon glyphicon-plus" style="text-decoration:none; font-size:30px; padding: 15px 0px 0px 10px" href="edit.php?who=<?= 0 ?>"></a>
              <a class="pull-right text-primary glyphicon glyphicon-log-out" style="text-decoration:none; font-size:30px; padding: 15px 0px 0px 0px" href="index.php?action=logout"></a>
            </span>
          </div>
          <div class="clearfix"></div>
        </div>
      </div>
      <hr/>
      <div class="row">
        <div class="col-sm-12 col-md-10 col-md-offset-1 col-lg-8 col-lg-offset-2">
          <div class="form-group input-group" action="search.php" method="get" style="min-width:500px; padding: 0px 150px 0px 150px">
	        <input type="search" name="find" class="form-control" placeholder="笔记查询" />
            <span class="input-group-addon" style="border:1px #bfc9ca solid;background-color:white;">
            <a class="glyphicon glyphicon-search" style="text-decoration:none" href=""></a></span>
		  </div>
        </div>
      </div>
      <div class="row">
        <div class="col-sm-12 col-md-10 col-md-offset-1 col-lg-8 col-lg-offset-2">
          <!--note-list-->
          <?php if (!empty($error)) { ?>
          <h4 class="text-center text-danger"><?= $error ?></h4>
          <?php } else if (count($records) == 0) { ?>
          <h4 class="text-center">〒_〒没找到笔记, 请点击按钮创建在线笔记吧( ●'◡'● )</h4>
          <?php } else { $rank = 0; ?>
            <?php foreach ($records as $record) {  $rank += 1; ?>
            <h4><a class="pull-left text-primary glyphicon glyphicon-grain" style="text-decoration:none; padding: 0px 2px 0px 0px" ></a>
            <font face="Courier New" color="#696969"> <?= $rank ?>
              <?= $record['title'] ?></font>
              <span class="pull-right">
                <a id="bookmark-<?= $record['id'] ?>" class="glyphicon glyphicon glyphicon-bookmark" style="text-decoration:none;" href="javascript:show()" data-id="<?= $record['id'] ?>"></a>
                &nbsp;
                <a class="glyphicon glyphicon glyphicon-pencil" style="text-decoration:none;" href="edit.php?id=<?= $record['id'] ?>"></a>
                &nbsp;
                <a class="pull-right glyphicon glyphicon-trash" style="text-decoration:none;" href="#" data-id="<?= $record['id'] ?>"></a>
              </span>
            </h4>
            <h5><span class="text-muted" style="font-size: 85%; padding: 0px 10px;">Created At <?= $record['date_created'] ?> / Modified At <?= $record['date_modified'] ?></span></h5>
            <div id="content-<?= $record['id'] ?>" style="display:none; margin: 0 8px; padding: 8px 16px; box-shadow: 0 4px 4px #eee; border:1px #dcdcdc solid; background-color:white;"> 
            <!--多行文本缩略:style+="overflow : hidden;
                        text-overflow: ellipsis; display: -webkit-box; -webkit-line-clamp: 2; -webkit-box-orient: vertical;"-->
                <?= $record['content'] ?>
            </div>
            <br/>
            <?php } ?>
          <?php } ?>
        </div>
      </div>
      <div class="row">
        <div class="col-xs-12 text-center">
          <hr/>
          <a class="btn btn-primary btn-lg" style="min-width: 300px;" href="edit.php?who=<?= $user->id ?>">创建私人笔记</a>
          <br/>
          <br/>
          <br/>
        </div>
      </div>
    </div>

    <script src="js/jquery-3.2.1.js"></script>
    <script type="text/javascript">
    var obj, which;
    $(function() {
        $('.glyphicon-bookmark').on('click', function(e) {
            obj = $(this), which = obj.data('id');
            //alert('你点击了第'+which+'个bookmark.');
        });
        
        $('.glyphicon-trash').on('click', function(e) {
            var el = $(this),
                id = el.data('id');
            if (!confirm('是否删除这条笔记？删除后可没法恢复哦(T_T)')) {
                return;
            }
            $.post('delete.php', {
                id: $(this).data('id')
            }, function(result) {
                if (result && result.error) {
                    alert(result.error);
                    return;
                }
                location.reload();
            }, 'json');
        });
    });
    function show(){
        document.getElementById('content-'+which).style.display='block';
        document.getElementById('bookmark-'+which).href='javascript:hide()';
    }
    function hide(){
        document.getElementById('content-'+which).style.display='none';
        document.getElementById('bookmark-'+which).href='javascript:show()';
    }
    </script>
  </body>
</html>
