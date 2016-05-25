#pragma mark create(创建表)
// 格式
create table 表名 (字段名1 字段类型1, 字段名2 字段类型2, …) ;
create table if not exists 表名 (字段名1 字段类型1, 字段名2 字段类型2, …) ;
// 示例
create table t_student (id integer, name text, age inetger, score real) ;
CREATE TABLE IF NOT EXISTS t_student(id INTEGER , name TEXT, age , score REAL);


#pragma mark create+（简单约束）
// 建表时可以给特定的字段设置一些约束条件，常见的约束有
not null ：规定字段的值不能为null
unique ：规定字段的值必须唯一
default ：指定字段的默认值
（建议：尽量给字段设定严格的约束，以保证数据的规范性）

// 示例
create table t_student (id integer, name text not null unique, age integer not null default 1) ;
// name字段不能为null，并且唯一
// age字段不能为null，并且默认为1

#pragma mark create + id(添加主键)
create table t_student (id integer primary key autoincrement, name text, age integer) ;


#pragma mark create (新建一个外键)
create table t_student (id integer primary key autoincrement, name text, age integer, class_id integer, constraint fk_t_student_class_id_t_class_id foreign key (class_id) references t_class (id)) ; 


#pragma mark - drop（删除表）
// 格式
drop table 表名 ;
drop table if exists 表名 ;
// 示例
drop table t_student ;
DROP TABLE IF EXISTS t_student;


#pragma mark - insert(插入数据)
// 格式
insert into 表名 (字段1, 字段2, …) values (字段1的值, 字段2的值, …) ;
// 示例
insert into t_student (name, age) values (‘lnj’, 10) ; //数据库中的字符串内容应该用单引号 ’ 括住
INSERT INTO t_student(age, score, name) VALUES ('28', 100, 'jonathan');
INSERT INTO t_student(name, age) VALUES ('lee', '28');
INSERT INTO t_student(score) VALUES (100);

#pragma mark - update（更新数据）
// 格式
update 表名 set 字段1 = 字段1的值, 字段2 = 字段2的值, … ; 
// 示例
update t_student set name = ‘jack’, age = 20 ;//会将t_student表中所有记录的name都改为jack，age都改为20
UPDATE t_student SET name = 'MM' WHERE age = 10;
UPDATE t_student SET name = 'WW' WHERE age is 7;
UPDATE t_student SET name = 'XXOO' WHERE age < 20;
UPDATE t_student SET name = 'NNMM' WHERE age < 50 and score > 10;

#pragma mark - delete（删除数据）
// 格式
delete from 表名 ;
// 示例
delete from t_student ;//将t_student表中所有记录都删掉


#pragma mark - where（条件语句）
// 如果只想更新或者删除某些固定的记录，那就必须在DML语句后加上一些条件

// 条件语句的常见格式
where 字段 = 某个值 ;   // 不能用两个 =
where 字段 is 某个值 ;   // is 相当于 = 
where 字段 != 某个值 ; 
where 字段 is not 某个值 ;   // is not 相当于 != 
where 字段 > 某个值 ; 
where 字段1 = 某个值 and 字段2 > 某个值 ;  // and相当于C语言中的 &&
where 字段1 = 某个值 or 字段2 = 某个值 ;  //  or 相当于C语言中的 ||
// 示例
// 将t_student表中年龄大于10 并且 姓名不等于jack的记录，年龄都改为 5
update t_student set age = 5 where age > 10 and name != ‘jack’ ;

// 删除t_student表中年龄小于等于10 或者 年龄大于30的记录
delete from t_student where age <= 10 or age > 30 ;

// 将t_student表中名字等于jack的记录，score字段的值 都改为 age字段的值
update t_student set score = age where name = ‘jack’ ;



#pragma mark - select(DQL语句)
#pragma mark select(查询)
// 格式
select 字段1, 字段2, … from 表名 ;
select * from 表名;   //  查询所有的字段
// 示例
select name, age from t_student ;
select * from t_student ;
select * from t_student where age > 10 ;  //  条件查询


#pragma mark select(别名)
// 格式(字段和表都可以起别名)
select 字段1 别名 , 字段2 别名 , … from 表名 别名 ; 
select 字段1 别名, 字段2 as 别名, … from 表名 as 别名 ;
select 别名.字段1, 别名.字段2, … from 表名 别名 ;
// 示例
select name myname, age myage from t_student ;//给name起个叫做myname的别名，给age起个叫做myage的别名
select s.name, s.age from t_student s ;//给t_student表起个别名叫做s，利用s来引用表中的字段
SELECT s.name myName, s.age myAge, s.score myScore FROM t_student s WHERE s.age > 50;


#pragma mark select（计算记录的数量）
// 格式
select count (字段) from 表名 ;
select count ( * ) from 表名 ;
// 示例
select count (age) from t_student ;
select count ( * ) from t_student where score >= 60;


#pragma mark select（排序）
// 格式
// 查询出来的结果可以用order by进行排序
select * from t_student order by 字段 ;
select * from t_student order by age ;
// 示例
// 默认是按照升序排序（由小到大），也可以变为降序（由大到小）
select * from t_student order by age desc ;  //降序
select * from t_student order by age asc ;   // 升序（默认）

// 多字段进行排序
select * from t_student order by age asc, height desc ;
先按照年龄排序（升序），年龄相等就按照身高排序（降序）


#pragma mark select+limit(分页)
// 使用limit可以精确地控制查询结果的数量，比如每次只查询10条数据
// 格式
select * from 表名 limit 数值1, 数值2 ;
// 示例
select * from t_student limit 4, 8 ;//可以理解为：跳过最前面4条语句，然后取8条记录

// limit常用来做分页查询，比如每页固定显示5条数据，那么应该这样取数据
第1页：limit 0, 5
第2页：limit 5, 5
第3页：limit 10, 5
…
第n页：limit 5*(n-1), 5


select * from t_student limit 7 ;
// 相当于select * from t_student limit 0, 7 ;
// 表示取最前面的7条记录


#pragma mark - select (表连接查询)
// 表连接的类型
// 内连接：inner join 或者 join  （显示的是左右表都有完整字段值的记录）
// 左外连接：left outer join （保证左表数据的完整性）

// 示例
// 查询0316iOS班的所有学生
select s.name,s.age from t_student s, t_class c where s.class_id = c.id and c.name = ‘0316iOS’;

