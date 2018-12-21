<?php

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
