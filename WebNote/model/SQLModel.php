<?php
### 需要安装PDO与PDO_MYSQL库

/**
 * 数据模型
 * 实现数据模型的基础功能
 */
class SQLModel {
    /** 数据库连接串 */
    private $dsn = 'mysql:host=localhost; port=3306; dbname=php';
    /** 用户名 */
    private $user = 'root';
    /** 密码 */
    private $password = '';

    /** 错误信息 */
    protected $error = '';
    /** 字段列表 */
    protected $fields = [];

    /**
     * 执行SQL语句
     * @param string $sql SQL语句
     * @param array $params SQL参数
     * @return mixed
     */
    public function query(string $sql, array $params = [])
    {
        # 连接数据库
        $pdo = null;
        try {
            $pdo = new PDO($this->dsn, $this->user, $this->password);
            $pdo->query('set character set utf8');
        } catch (PDOException $e) {
            $this->error = '数据库连接错误：' . $e->getMessage();
            return false;
        }

        # 执行SQL语句
        $stm = $pdo->prepare($sql, [PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY]);
        if (!$stm) {
            $this->error = 'SQL语句或参数有错';
            return false;
        }
        if (!$stm->execute($params)) {
            $this->error = 'SQL执行出错：' . $stm->errorInfo()[2];
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
     * 填充模型数据
     * @param array $record
     */
    public function setup(array $record)
    {
        foreach ($this->fields as $field) {
            if (isset($record[$field])) {
                $this->$field = $record[$field];
            }
        }
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
