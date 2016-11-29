create table config_flags(
id int NOT NULL AUTO_INCREMENT,
Описание VARCHAR(100),
flag int NOT NULL DEFAULT 0,
PRIMARY KEY(id) 
)ENGINE=InnoDB CHARACTER SET=UTF8;
insert into config_flags(id,Описание,flag) values 
(1,'состояние базы данных 	(0 - Ни разу не запускалась )
							(1 - Запущена
							(2 - Остановленна',0);
(2,'день отчетности успеваемости (1 - 28)',1),
(3,'день недели отчётности пропусков (0-6)',6);

create table prefix(
id int NOT NULL AUTO_INCREMENT,
word VARCHAR(100) NOT NULL UNIQUE,
Описание VARCHAR(100) NOT NULL,
PRIMARY KEY(id)
)ENGINE=InnoDB CHARACTER SET=UTF8;
 
INSERT INTO prefix(word,Описание) VALUES
('LC_','успеваемость выполненно лаб'),
('PC_','успеваемость выполненно практических работ'),
('KRC_','успеваемость выполненно контрольных работ'),
('RGRC_','успеваемасть выполненно РГР'),
('LD_','успеваемость зашищенно лаб'),
('KRD_','успеваемость зашищенно практических работ'),
('RGRD_','успеваемость зашищенно РГР');


create table времяПропуски(
id int NOT NULL AUTO_INCREMENT,
Даты date NOT NULL UNIQUE,
PRIMARY KEY(id)
)ENGINE=InnoDB CHARACTER SET=UTF8;  
create table времяУроки(
id int NOT NULL AUTO_INCREMENT,
Даты date NOT NULL UNIQUE,
PRIMARY KEY(id)
)ENGINE=InnoDB CHARACTER SET=UTF8;
create table группы(
num int NOT NULL DEFAULT 0,
Имя VARCHAR(100) NOT NULL,
PRIMARY KEY(Имя)
)ENGINE=InnoDB CHARACTER SET=UTF8;
create table предметы(
num int NOT NULL DEFAULT 0,
Наименование VARCHAR(100) NOT NULL,
PRIMARY KEY(Наименование)
)ENGINE=InnoDB CHARACTER SET=UTF8;

DELIMITER |
CREATE PROCEDURE update_table (IN name VARCHAR(100),
IN time VARCHAR(100))
    NOT DETERMINISTIC
    SQL SECURITY INVOKER
    COMMENT 'Обновит таблицу в ссотвецтвии 
    с текущими датами'
BEGIN
	DECLARE i INT DEFAULT (SELECT COUNT(*) FROM
	information_schema.COLUMNS WHERE TABLE_NAME=name);
		SET @sql= CONCAT('select count(Даты) from ',
		time, ');');
		PREPARE getCountrySql FROM @sql;
		EXECUTE getCountrySql;
		DEALLOCATE PREPARE getCountrySql;
	WHILE i<=len DO
		SET @temp=(select DATE_FORMAT((select Даты from
		времяПропуски WHERE времяПропуски.id=i),
		'%d_%m_%Y'));
		SET @sql= CONCAT('ALTER TABLE ',name,' ADD
		',@temp,' int NOT NULL DEFAULT 0;');
		PREPARE getCountrySql FROM @sql;
		EXECUTE getCountrySql;
		DEALLOCATE PREPARE getCountrySql;
		SET i=i+1;
	END WHILE;
END|
DELIMITER ;

DELIMITER |
CREATE PROCEDURE createGroup(IN name VARCHAR(100) CHARACTER SET utf8)
	NOT DETERMINISTIC
    SQL SECURITY INVOKER
    COMMENT 'создаст пустую группу'
BEGIN
	SET @sql= CONCAT('CREATE TABLE ',name,'( 
	ФИО VARCHAR(100) NOT NULL, 
	PRIMARY KEY(ФИО)
	)ENGINE=InnoDB CHARACTER SET=UTF8;');
	PREPARE getCountrySql FROM @sql;
	EXECUTE getCountrySql;
	DEALLOCATE PREPARE getCountrySql;
	SET @sql= CONCAT('INSERT INTO группы (Имя) VALUES ("',name,'");');
	PREPARE getCountrySql FROM @sql;
	EXECUTE getCountrySql;
	DEALLOCATE PREPARE getCountrySql;
	update группы set  num=(
	SELECT @number_:= @number_ + 1 FROM
	(SELECT @number_:= 0) as tbl);	
	SET @sql= CONCAT('CREATE TABLE P_',name,'( 
	предмет VARCHAR(100) NOT NULL UNIQUE,
	FOREIGN KEY(предмет) REFERENCES предметы(Наименование)
	ON UPDATE CASCADE
	ON DELETE CASCADE,
	LC_ 	VARCHAR(100) NOT NULL UNIQUE,
	PC_ 	VARCHAR(100) NOT NULL UNIQUE,
	KRC_ 	VARCHAR(100) NOT NULL UNIQUE,
	RGRC_ 	VARCHAR(100) NOT NULL UNIQUE,
	LD_ 	VARCHAR(100) NOT NULL UNIQUE,
	KRD_ 	VARCHAR(100) NOT NULL UNIQUE,
	RGRD_ 	VARCHAR(100) NOT NULL UNIQUE	
	)ENGINE=InnoDB CHARACTER SET=UTF8;');
	PREPARE getCountrySql FROM @sql;
	EXECUTE getCountrySql;
	DEALLOCATE PREPARE getCountrySql;
END|
DELIMITER ;


DELIMITER |
CREATE PROCEDURE createPredmet(IN Name VARCHAR(100) CHARACTER SET utf8)
	NOT DETERMINISTIC
    SQL SECURITY INVOKER
    COMMENT 'Добавит новый предмет'
BEGIN
	SET @sql= CONCAT('INSERT INTO предметы(Наименование)
	VALUES ("',Name,'");');
	PREPARE getCountrySql FROM @sql;
	EXECUTE getCountrySql;
	DEALLOCATE PREPARE getCountrySql;
	update предметы set  num=(
	SELECT @number_:= @number_ + 1 FROM
	(SELECT @number_:= 0) as tbl);		 
END|
DELIMITER ;


DELIMITER |
CREATE PROCEDURE addPredmet(IN Name VARCHAR(100)CHARACTER SET utf8,
 IN Gr VARCHAR(100) CHARACTER SET utf8)
	NOT DETERMINISTIC
    SQL SECURITY INVOKER
    COMMENT 'Добавит предмет группе'
BEGIN
	SET @sql= CONCAT('INSERT INTO
	P_',Gr,'(предмет,LC_,PC_,KRC_,RGRC_,LD_,KRD_,RGRD_) VALUES
	("',Name,'",
	"LC_',Name,Gr,'",
	"PC_',Name,Gr,'",
	"KRC_',Name,Gr,'",
	"RGRC_',Name,Gr,'",
	"LD_',Name,Gr,'",
	"KRD_',Name,Gr,'",
	"RGRD_',Name,Gr,'");
	');
	PREPARE getCountrySql FROM @sql;
	EXECUTE getCountrySql;
	DEALLOCATE PREPARE getCountrySql;
	SET @i=1;
	SET @len=(select count(word) from prefix); 
	WHILE @i<=@len DO
		SET @sql= CONCAT('CREATE TABLE ',(select word from
		prefix where id=@i),Name,Gr,'( 
		ФИО VARCHAR(100) NOT NULL, 
		FOREIGN KEY(ФИО) REFERENCES ',Gr,'(ФИО)
		ON UPDATE CASCADE
		ON DELETE CASCADE
		)ENGINE=InnoDB CHARACTER SET=UTF8;');
		
		PREPARE getCountrySql FROM @sql;
		EXECUTE getCountrySql;
		DEALLOCATE PREPARE getCountrySql;
		SET @i=@i+1;
	END WHILE;	
END|
DELIMITER ;


DELIMITER |
CREATE PROCEDURE deleteGlobalPredmet(IN Name VARCHAR(100) CHARACTER SET utf8)
	NOT DETERMINISTIC
    SQL SECURITY INVOKER
    COMMENT 'Удалит предмет'
BEGIN
	SET @i=1;
	SET @len=(select max(id) from группы);
	WHILE @i<=@len DO
		SET @sql= CONCAT('CALL deletePredmet(',
		(select Имя from группы where num=@i),');');
		PREPARE getCountrySql FROM @sql;
		EXECUTE getCountrySql;
		DEALLOCATE PREPARE getCountrySql;
		SET @i=@i+1;
	END WHILE;
	SET @sql= CONCAT('DELETE FROM предметы 
	where Наименование=',Name,');');
	PREPARE getCountrySql FROM @sql;
	EXECUTE getCountrySql;
	DEALLOCATE PREPARE getCountrySql;
	update предметы set  num=(
	SELECT @number_:= @number_ + 1 FROM
	(SELECT @number_:= 0) as tbl);			
END |
DELIMITER ;


DELIMITER |

CREATE PROCEDURE deletePredmet(IN Name VARCHAR(100) CHARACTER SET utf8,
IN Gr VARCHAR(100) CHARACTER SET utf8)
	NOT DETERMINISTIC
    SQL SECURITY INVOKER
    COMMENT 'Удалит предмет группе'
BEGIN
	SET @i=1;
	SET @len=(select count(word) from prefix);
	WHILE @i<=@len DO
		SET @sql= CONCAT('DROP TABLE ',(select word from
		prefix where id=@i),Name,Gr,';');
		PREPARE getCountrySql FROM @sql;
		EXECUTE getCountrySql;
		DEALLOCATE PREPARE getCountrySql;
		SET @i=@i+1;
	END WHILE;
	SET @sql= CONCAT('DELETE FROM P_',Gr,
	' where предмет=',name);
		PREPARE getCountrySql FROM @sql;
		EXECUTE getCountrySql;
		DEALLOCATE PREPARE getCountrySql;
END|
DELIMITER ;

DELIMITER |
create PROCEDURE addStudent(IN Name VARCHAR(100) CHARACTER SET utf8,
IN group_ VARCHAR(100) CHARACTER SET utf8)
	NOT DETERMINISTIC
    SQL SECURITY INVOKER
    COMMENT 'Добавить нового ученика в группу'
BEGIN
	SET @sql= CONCAT('INSERT INTO ',group_,' (ФИО)
	VALUES ("',Name,'");');
	PREPARE getCountrySql FROM @sql;
	EXECUTE getCountrySql;
	DEALLOCATE PREPARE getCountrySql;
END|
DELIMITER ;


DELIMITER |
create PROCEDURE deleteStudent(IN Name VARCHAR(100) CHARACTER SET utf8,
IN group_ VARCHAR(100) CHARACTER SET utf8)
	NOT DETERMINISTIC
    SQL SECURITY INVOKER
    COMMENT 'удалить ученика из группы'
BEGIN
	SET @sql= CONCAT('DELETE  FROM ',group_,
	' WHERE ФИО="',Name,'";');
	PREPARE getCountrySql FROM @sql;
	EXECUTE getCountrySql;
	DEALLOCATE PREPARE getCountrySql;
END|
DELIMITER ;



DELIMITER |
CREATE PROCEDURE deleteGroup(IN Name VARCHAR(100) CHARACTER SET utf8)
	NOT DETERMINISTIC
	SQL SECURITY INVOKER
	COMMENT 'удалит группу'
BEGIN 
	SET @sql= CONCAT('DROP TABLE ',Name,';');
	PREPARE getCountrySql FROM @sql;
	EXECUTE getCountrySql;
	DEALLOCATE PREPARE getCountrySql;
	
	SET @sql= CONCAT('SELECT deletePredmet(предмет) from P_',Name,';');
	PREPARE getCountrySql FROM @sql;
	EXECUTE getCountrySql;
	DEALLOCATE PREPARE getCountrySql;
	
	SET @sql= CONCAT('DROP TABLE P_',Name,';');
	PREPARE getCountrySql FROM @sql;
	EXECUTE getCountrySql;
	DEALLOCATE PREPARE getCountrySql;
	
	SET @sql= CONCAT('DELETE FROM группы WHERE Имя="',Name,'";');
	PREPARE getCountrySql FROM @sql;
	EXECUTE getCountrySql;
	DEALLOCATE PREPARE getCountrySql;
	update группы set  num=(
	SELECT @number_:= @number_ + 1 FROM
	(SELECT @number_:= 0) as tbl);
END|
DELIMITER ;



DELIMITER |
create procedure upgrade_date_sync()
	NOT DETERMINISTIC
	SQL SECURITY INVOKER
	COMMENT 'синхронизирует время '
BEGIN

	IF((CURDATE()<>(select Даты from времяПропуски where
	id=(select MAX(id) from времяПропуски ))) AND 
	(WEEKDAY(NOW())=(select flag from config_flags where id=3))) THEN
		INSERT INTO времяПропуски (Даты) VALUES (NOW());
	END IF;
	
	IF((CURDATE()<>(select Даты from времяПропуски where
	id=(select MAX(id) from времяПропуски ))) AND
	(DAYOFMONTH(NOW())=(select flag from config_flags where id=2))) THEN
		INSERT INTO времяУроки(Даты) VALUES (NOW());	
	END IF;
END|
DELIMITER ;

DELIMITER |
create procedure upgrade_date()
	NOT DETERMINISTIC
	SQL SECURITY INVOKER
	COMMENT 'обновит даты базы данных'
BEGIN
	IF((select flag from config_flags where id=1)=1) THEN
		call upgrade_date_sync();
	END IF;
END|
DELIMITER ;


DELIMITER |
create procedure stopBd()
	NOT DETERMINISTIC
	SQL SECURITY INVOKER
	COMMENT 'остоновить базу данных'
BEGIN
	IF((select flag from config_flags where id=1)=1) THEN
		
		update config_flags set flag=1 
		where id=2;				
	END IF;
END|
DELIMITER ;

DELIMITER |
create procedure clear()
	NOT DETERMINISTIC
	SQL SECURITY INVOKER
	COMMENT 'отчистить данные если бд остановлина '
BEGIN
	IF((select flag from config_flags where id=1)=2) THEN
		DROP TABLE времяПропуски;
		DROP TABLE времяПропуски;
		
		create table времяПропуски(
		id int NOT NULL AUTO_INCREMENT,
		Даты date NOT NULL UNIQUE,
		PRIMARY KEY(id)
		)ENGINE=InnoDB CHARACTER SET=UTF8;  
		
		create table времяУроки(
		id int NOT NULL AUTO_INCREMENT,
		Даты date NOT NULL UNIQUE,
		PRIMARY KEY(id)
		)ENGINE=InnoDB CHARACTER SET=UTF8;
	END IF;
END|
DELIMITER ;

DELIMITER |
create procedure startBd()
	NOT DETERMINISTIC
	SQL SECURITY INVOKER
	COMMENT 'запустит базу данных'
BEGIN
	SET @d=CURDATE();
        IF(DAYOFMONTH(@d)>28) THEN
		SET @d=@d-3;
	END IF;
	IF((select flag from config_flags where id=1)<>1) THEN
		INSERT INTO времяПропуски (Даты) VALUES (@d);
		INSERT INTO времяУроки(Даты) VALUES (@d);
		update config_flags set flag=1 where id=1;	
	END IF;
END|
DELIMITER ;

DELIMITER |
create procedure showGroup(IN name VARCHAR(100) CHARACTER SET UTF8)
	NOT DETERMINISTIC
	SQL SECURITY INVOKER
	COMMENT 'просмотр пропусков'
BEGIN
	SET @sql= CONCAT('SELECT * FROM ',name,';');
	PREPARE getCountrySql FROM @sql;
	EXECUTE getCountrySql;
	DEALLOCATE PREPARE getCountrySql;
END|
DELIMITER ;


DELIMITER |
create procedure showPredmet(IN name VARCHAR(100) CHARACTER SET UTF8,
IN Gr VARCHAR(100) CHARACTER SET UTF8, IN date_ DATE )
	NOT DETERMINISTIC
	SQL SECURITY INVOKER
	COMMENT 'просмотр пропусков'
BEGIN
	SET @sql= CONCAT('SELECT ',CAST(date_ AS CHAR),' FROM LC_',name,Gr,'PC_',name,Gr,'KRC_',name,Gr,
	'RGRC_',name,Gr,'LD_',name,Gr,'KRD_',name,Gr,
	'RGRD_',name,Gr);
	PREPARE getCountrySql FROM @sql;
	EXECUTE getCountrySql;
	DEALLOCATE PREPARE getCountrySql;
END|
DELIMITER ;


DELIMITER |
create procedure showPredmets()
	NOT DETERMINISTIC
	SQL SECURITY INVOKER
	COMMENT 'просмотр предметов'
BEGIN
	select * from  предметы;
END|
DELIMITER ;


/********************************************** тригеры ***********************************************/


/* дамп базы данных (добовление в архив)
mysqldump -uroot -h82.82.82.82 -p database > database.sql
-u или -–user=... - имя пользователя
-h или --host=... - удаленный хост (для локального хоста можно опустить этот параметр)
-p или --password - запросить пароль
database - имя базы данных
database.sql - файл для дампа



mysql -uroot -h82.82.82.82 -p database < database.sql
mysql> use database;
mysql> source database.sql

****/


