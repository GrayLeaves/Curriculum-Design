<?php
### 需要安装PDO与PDO_MYSQL库
require 'model/NoteModel.php';

$model = new NoteModel();
if (!empty($_REQUEST['id'])) {
    # 根据ID删除对应笔记
    $model->delete($_REQUEST['id']);
    echo json_encode(['error' => $model->getError()]);
    return;
}
# 如果未指定ID，重定向到首页
header('index.php');
/** header失效原因概述：
1、location和“:”之间不能有空格，否则出错。
2、在用header前不能有任何的输出，包括include的页面中标记“?>”后不能有空格！！
3、header后的php程序还会被执行，exit;
*/