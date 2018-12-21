<?php

require 'model/NoteModel.php';

$model = new NoteModel();
if (!empty($_POST['note'])) {
    if (!empty($_POST['note']['id'])) {
        # 如果POST请求包含笔记及ID，更新ID对应的笔记
        $model->update($_POST['note']);
        echo json_encode(['error' => $model->getError()]);
        return;
    }
    else{
        $model->create($_POST['note']);
        echo json_encode(['error' => $model->getError()]);
        return;
    }
}

$record = [];
$error = '';
if (!empty($_REQUEST['id'])) {
    # 如果包含ID，获取ID对应的笔记用于更新
    $record = $model->find($_REQUEST['id']);
    $error = $model->getError();
}
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
      <div class="row">
        <div class="col-xs-12">
          <h1 class="text-center"><img src="logo.png" style="width: 60px;"><font face="Courier New" color="gray" size="6">WebNote</font></h1>
          <hr/>
        </div>
      </div>
      <div class="row">
        <div class="col-sm-12 col-md-10 col-md-offset-1 col-lg-8 col-lg-offset-2">
          <?php if (!empty($error)) { ?>
          <h3 class="text-center text-danger"><?= $error ?></h3>
          <?php } else { ?>
          <input type="hidden" name="id" value="<?= $record['id'] ?? '' ?>" />
          <input type="hidden" name="who" value="<?= $_REQUEST['who'] ?? '' ?>" />  <!--get from url-->
          <div class="form-group">
            <label for="title">标题</label>
            <input type="text" name="title" value="<?= $record['title'] ?? '' ?>" class="form-control" id="title" placeholder="标题">
          </div>
          <div class="form-group">
            <label for="content">正文</label>
            <textarea id="editor" name="content" class="form-control" cols="80" placeholder="正文"><?= $record['content'] ?? '' ?></textarea>
          </div>
          <br>
          <div class="form-group text-center">
            <button class="btn btn-primary btn-lg" id="submit" type="button" style="min-width: 160px;">保存提交</button>
            <button class="btn btn-primary btn-lg" id="return" type="button" style="min-width: 160px;">取消返回</button>
            <!--onClick="location.href='index.php'"-->
          </div>
          <?php } ?>
          <br>
        </div>
      </div>
    </div>

    <script src="js/jquery-3.2.1.js"></script>
    <script src="ckeditor5/ckeditor.js"></script>
    <script type="text/javascript">
    $(function() {
        var editorText = null;
        ClassicEditor.create(document.querySelector("#editor")).then(editor => {
            editorText = editor;
        }).catch(error => {console.error(error.stack);});
        
        $('#submit').on('click', function(e) {
            var title = $('#title').val();
            if ($.trim(title) == '') {
                $('#title').focus();
                alert('标题为空,请输入标题');
                return;
            }
            var passage = editorText.getData();
            //alert(passage); <p>&nbsp;  </p>|| /<p>.*?<\/p>/i,''(single_line)
            if ($.trim(passage.replace('<p>&nbsp;</p>','')) == '') {
                editor.focus();
                alert('正文为空,请输入内容');
                return;
            }
            //var who = getQueryVariable("who");   //通过js传递who来指定专有笔记权
            //alert($('input[name=who]').val());    //当who=0时,说明是更新笔记,此时url没传递who,只传递id
            $.post('edit.php', {
                note: {
                    id: $('input[name=id]').val(),
                    who: $('input[name=who]').val(),
                    title: title,
                    content: passage
                }
            }, function(result) {
                if (result && result.error) {
                    alert(result.error);
                    return;
                }
                location.href = 'index.php';
            }, 'json');
        });
        $('#return').on('click', function(e) {
            if (!confirm('是否放弃所有的改动呢？( ╯□╰ )')) {
                return;
            }
            location.href = 'index.php';
        });
    });
    </script>
  </body>
</html>
