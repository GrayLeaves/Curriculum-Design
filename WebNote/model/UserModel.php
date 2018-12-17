<?php

### 需要安装PDO与PDO_MYSQL库
require_once 'model/SQLModel.php';

/**
 * 用户数据模型
 * 实现用户的登录和角色查询
 */
class UserModel extends SQLModel {
    /** 表名 */
    protected $table = 'owner';
    /** 字段列表 */
    protected $fields = ['id', 'name', 'password', 'rank', 'date_created', 'date_modified'];

    /**
     * 用户登录
     * @param string $username 用户名
     * @param string $password 密码
     * @return bool
     */
    public function login(string $username, string $password): bool
    {
        # 验证数据
        if (empty($username)) {
            $this->error = '账号为空,请输入账号';
            return false;
        }
        if (empty($password)) {
            $this->error = '密码为空,请输入密码';
            return false;
        }
        # 生成用户验证语句
        $sql = "SELECT * FROM {$this->table} where name='$username' and password='$password'"; //注意{}和''的区别
        $result = $this->query($sql, ['name' => $username, 'password' => $password]);
        if (empty($this->error) && count($result) > 0) {
            $this->setup($result[0]);   //已填充数据,便于查询
            return true;
        }
        $this->error = '请核对账号或密码是否有误<username: '.$username.', password: '.$password.'>';
        return false;
    }
    
    public function get_users():array{
        # 取得所有用户
        $sql = "select * from {$this->table}";
        $users = $this->query($sql);
        return $users;
    }
    
    public function notExist(string $username, $update=false): bool{
        # 验证数据
        if (empty($username)) {
            $this->error = '账号为空,请输入账号';
            return false;
        }
        # 生成用户验证语句
        $sql = "SELECT * FROM {$this->table} where name='$username'"; //注意{}和''的区别
        $result = $this->query($sql);
        if (count($result) == 0 && empty($this->error)) {
            if(!$update) return true;
            else{
                $this->error = $username.'不存在,请注册或更改用户名';
                return false;
            }
        }
        else{
            if($update) return true;
            else{
                $this->error = $username.'已存在,请登陆或更改用户名';
                return false;
            }
        }
    }
    
    public function create(string $username, string $password, $update=false): bool{
        # 验证数据
        if (empty($username)) {
            $this->error = '账号为空,请输入账号';
            return false;
        }
        if (empty($password)) {
            $this->error = '密码为空,请输入密码';
            return false;
        }
        else{
            if(!preg_match('/(?=.*[a-zA-Z])(?=.*\d)(?!.*\s).{6,16}/', $password)){
                $this->error = '密码应由6-16个字母和数字组成，不能是纯数字或纯英文';
                return false;
            }
        }
        
        # 创建用户
        if(!$update){
            $sql = "insert into {$this->table} (name, password) values('$username', '$password')"; //注意{}和''的区别
            $result = $this->query($sql);
            if (!empty($this->error)) {
                $this->error = '很遗憾,创建用户失败';
                return false;
            }
            return true;
        }
        else{
            $sql = "update {$this->table} set password='$password' where name='$username'"; //注意{}和''的区别
            $result = $this->query($sql);
            if (!empty($this->error)) {
                $this->error = '很遗憾,重置密码失败';
                return false;
            }
            return true;
        }
    }
    
    public function clean(string $id): bool{
        # 验证数据
        if (empty($id)) {
            $this->error = '未指定用户';
            return false;
        }
        # 删除用户
        $sql = "delete from {$this->table} where id = {$id}"; //注意{}和''的区别
        $result = $this->query($sql);
        if (!empty($this->error)) {
            $this->error = '很遗憾,删除用户失败';
            return false;
        }
        return true;
    }
}
?>