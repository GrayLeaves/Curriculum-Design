<?php
### 需要安装PDO与PDO_MYSQL库
require 'model/NoteModel.php';
require 'model/UserModel.php';

$note = new NoteModel();
$user = new UserModel();

if (!empty($_REQUEST['id'])) {
    # 删除用户
    if(!$user->clean($_REQUEST['id'])){
        echo json_encode(['error' => $note->getError()]);
    }
    # 根据用户ID删除对应笔记
    if(!$note->truncate($_REQUEST['id'])){
        echo json_encode(['error' => $note->getError()]);
    }
}
# 如果未指定ID，重定向到首页
header('location: index.php?action=login');
