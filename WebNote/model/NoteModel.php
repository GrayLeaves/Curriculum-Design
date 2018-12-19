<?php
### 需要安装PDO与PDO_MYSQL库

/**
 * 笔记数据模型
 * 实现note表的增删改查
 */
class NoteModel {
    /** 数据库连接串 */
    private $dsn = 'mysql:host=localhost; port=3306; dbname=php';
    /** 用户名 */
    private $user = 'root';
    /** 密码 */
    private $password = '';
    /** 表名 */
    private $table = 'note';
    /** 错误信息 */
    private $error = '';

    /**
     * 读取所有笔记
     * @param who :0表示共享笔记(默认),其他则对应用户的专属笔记
     * @return array
     */
    public function read(string $who): array
    {
        # 生成查询语句
        if($who=='0'){
            $sql = "SELECT * FROM {$this->table} where who=0";
            $result = $this->query($sql);
            return $result === false ? [] : $result;
        }
        else{
            $sql = "SELECT * FROM {$this->table} where who={$who} or who=0";
            $result = $this->query($sql);
            return $result === false ? [] : $result;
        }
    }
    
    /**
     * 根据title查询笔记
     * @param value: 关键字
     * @return array
     */
    public function search(string $value, string $who): array
    {
        # 生成查询语句
        $value = '%'.$value.'%';
        $sql = "SELECT * FROM {$this->table} where title like '$value' and who={$who} or title like '$value' and who=0";
        $result = $this->query($sql);
        return $result === false ? [] : $result;
    }
    
    /**
     * 读取一条笔记
     * @param string $id 笔记ID
     * @return array
     */
    public function find(string $id): array
    {
        # 验证数据
        if (empty($id)) {
            $this->error = 'ID未指定';
            return [];
        }
        # 生成查询语句
        $sql = "SELECT * FROM {$this->table} WHERE id=:id";
        $result = $this->query($sql, ['id' => $id]);
        if ($result === false) {
            return [];
        }
        return count($result) > 0 ? $result[0] : $result;
    }

    /**
     * 添加一条笔记
     * @param array $params 笔记内容
     * @param who 默认是共用笔记,否则为私有笔记
     * @return bool
     */
    public function create(array $params): bool
    {
        # 验证数据
        if (empty($params['who'])) {
            //$this->error = '提示：点击加号创建的是共用笔记共用笔记';
            $params['who'] = 0;
        }
        if (empty($params['title'])) {
            $this->error = '标题不能为空';
            return false;
        }
        if (empty($params['content'])) {
            $this->error = '正文不能为空';
            return false;
        }
        # 生成插入语句
        $sql = "INSERT INTO {$this->table} (who, title, content, date_created, date_modified) "
               . " VALUES (:who, :title, :content, current_timestamp, current_timestamp)";
        $result = $this->query($sql, ['who' => $params['who'], 'title' => $params['title'], 'content' => $params['content']]);
        return $result > 0;
    }

    /**
     * 更新一条笔记
     * @param array $params 笔记内容
     * @return boolean
     */
    public function update(array $params): bool
    {
        # 验证数据
        if (empty($params['id'])) {
            $this->error = 'ID未指定';
            return false;
        }
        $fields = [];
        if (!empty($params['title'])) {
            $fields['title'] = $params['title'];
        }
        if (!empty($params['content'])) {
            $fields['content'] = $params['content'];
        }
        if (empty($fields)) {
            $this->error = '请输入需要更新的标题或内容';
            return false;
        }
        # 生成更新语句
        $sql = "UPDATE {$this->table} set ";
        foreach ($fields as $key => $value) {
            $sql .= " {$key}=:{$key}, ";
        }
        $sql .= " date_modified=current_timestamp ";
        $sql .= " WHERE id=:id";
        $fields['id'] = $params['id'];
        $result = $this->query($sql, $fields);
        return $result > 0;
    }

    /**
     * 删除一条笔记
     * @param string $id 笔记ID
     * @return bool
     */
    public function delete(string $id)
    {
        # 验证数据
        if (empty($id)) {
            $this->error = 'ID未指定';
            return false;
        }
        # 生成删除语句
        $sql = "DELETE FROM {$this->table} WHERE id=:id";
        $result = $this->query($sql, ['id' => $id]);
        return $result > 0;
    }

    /**
     * 删除用户全部笔记
     * @param string $who 用户代码
     * @return bool
     */
    public function truncate(string $who)
    {
        # 验证数据
        if (empty($who)) {
            $this->error = '用户未指定';
            return false;
        }
        # 生成删除语句
        $sql = "DELETE FROM {$this->table} WHERE who=:who";
        $result = $this->query($sql, ['who' => $who]);
        if(!empty($this->error)){
            $this->error = '删除用户的笔记失败';
            return false;
        }
        return true;
    }
    
    /**
     * 执行SQL语句
     * @param string $sql SQL语句
     * @param array $params SQL参数
     * @return bool
     */
    protected function query(string $sql, array $params = [])
    {
        # 连接数据库
        $pdo = null;
        try {
            $pdo = new PDO($this->dsn, $this->user, $this->password);
        } catch (PDOException $e) {
            $this->error = '数据库连接错误：' . $e->getMessage();
            return false;
        }

        # 执行SQL语句
        $stm = $pdo->prepare($sql, [
            PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY
        ]);
        if (!$stm) {
            $this->error = 'SQL语句或参数有错';
        }
        if (!$stm->execute($params)) {
            $this->error = 'SQL执行出错：' . $stm->errorInfo();
            return false;
        }

        # 获取返回结果
        $column = $stm->columnCount();
        if ($column > 0) {
            # 获取结果集
            $rows = $stm->fetchAll(PDO::FETCH_ASSOC);
            foreach ($rows as &$row) {
                $row = array_change_key_case($row, CASE_LOWER);
            }
            return $rows;
        }
        return $stm->rowCount();
    }

    /**
     * 获取错误信息
     * @return string
     */
    public function getError(): string
    {
        return $this->error;
    }
}