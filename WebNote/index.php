<?php

require 'controller/LoginController.php';

# 生成控制器并响应请求
$controller = new LoginController();
$controller->act();

?>