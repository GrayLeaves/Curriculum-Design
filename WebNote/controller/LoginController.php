<?php
### 用户权限系统
### 需要安装PDO与PDO_MYSQL库

require_once 'model/UserModel.php';

/**
 * 控制器
 * 处理逻辑并响应用户请求
 */
class LoginController {
    /**
     * 分发用户请求
     * @return
     */
    public function act() {
        $action = $_REQUEST['action'] ?? 'index';
        $action = 'act' . ucfirst($action); //首字母大写-actIndex, actLogin, actLogout
        if (!method_exists($this, $action)) {
            $action = 'actIndex';
        }
        return $this->$action();
    }

    /**
     * 首页,判断去向
     */
    public function actIndex() {
        session_start();
        # 如果用户未登录，跳转到登录页
        if (empty($_SESSION['loginUser'])) {
            header('Location: index.php?action=login');
            return;
        }
        $user = $_SESSION['loginUser'];
        include 'view/head.php';
    }

    /**
     * 登录,判断是否登陆成功
     */
    public function actLogin() {
        session_start();
        if (!empty($_SESSION['loginUser'])) {
            # 如果用户已经登录，跳转到首页
            header('Location: index.php');
            return;
        }

        $user = new UserModel();
        $users = $user->get_users();
        if (!empty($_POST['username']) && !empty($_POST['password'])) {
            # 根据提交的用户名和密码登录
            if ($user->login($_POST['username'], $_POST['password'])) {
                # 用户登录成功,记录用户
                $_SESSION['loginUser'] = $user;
                if(!empty($_POST['record'])){
                    if($_POST['record'] == 'true'){ //设置寿命为1h的cookies 注意传递过来的是字符串,不是布尔值
                        setcookie("username", $_POST['username'], time()+3600*1*1);
                        setcookie("password", $_POST['password'], time()+3600*1*1);
                        setcookie("record", $_POST['record'], time()+3600*1*1);
                    }else{  //删除cookies
                        setcookie("username", $_POST['username'], time()+3600*1*1); //保留用户名
                        setcookie("password", '', time()-3600*1*1);
                        setcookie("record", '', time()-3600*1*1);
                    }
                }
            }
            echo json_encode(['error' => $user->getError()]);
            return;
        }
        include 'view/login.php';
    }

    /**
     * 登出,取消登录session
     */
    public function actLogout() {
        session_start();
        unset($_SESSION['loginUser']);
        header('Location: index.php?action=login');
    }
    
    /**
     *注册,看是否已被注册
     */
    public function actRegister() {
        session_start();
        $user = new UserModel();
        if (!empty($_POST['username']) && !empty($_POST['password'])) {
            # 根据提交的用户名注册
            if(!$user->notExist($_POST['username'])){
                echo json_encode(['error' => $user->getError()]);
                return;
            }
            if(!$user->create($_POST['username'],$_POST['password'])){
                echo json_encode(['error' => $user->getError()]);
            }
            else{
                echo json_encode(['error' => '创建成功:<<账户: '.$_POST['username'].', 密码: '.$_POST['password'].'>>']);
            }
        }
    }
    public function actUpdatepwd() {
        session_start();
        $user = new UserModel();
        if (!empty($_POST['username']) && !empty($_POST['password'])) {
            # 根据提交的用户名注册
            if(!$user->notExist($_POST['username'], true)){
                echo json_encode(['error' => $user->getError()]);
                return;
            }
            if(!$user->create($_POST['username'], $_POST['password'], true)){
                echo json_encode(['error' => $user->getError()]);
            }
            else{
                echo json_encode(['error' => '重置成功:<<账户: '.$_POST['username'].', 密码: '.$_POST['password'].'>>']);
            }
        }
    }
}