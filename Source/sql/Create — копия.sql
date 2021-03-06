/*version - 1.6.0*/


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
id int NOT NULL DEFAULT 0,
Даты date NOT NULL UNIQUE
)ENGINE=InnoDB CHARACTER SET=UTF8;
create table времяУроки(
id int NOT NULL DEFAULT 0,
Даты date NOT NULL UNIQUE
)ENGINE=InnoDB CHARACTER SET=UTF8;


create table группы(
num int NOT NULL DEFAULT 0,
lim int NOT NULL DEFAULT 48,
Имя VARCHAR(100) NOT NULL,
PRIMARY KEY(Имя)
)ENGINE=InnoDB CHARACTER SET=UTF8;
create table предметы(
num int NOT NULL DEFAULT 0,
Наименование VARCHAR(100) NOT NULL,
PRIMARY KEY(Наименование)
)ENGINE=InnoDB CHARACTER SET=UTF8;

create table limits(
lim INT DEFAULT 0,
группа VARCHAR(100),
предмет VARCHAR(100),
prefix_ VARCHAR(100),
FOREIGN KEY(группа) REFERENCES группы(Имя)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
FOREIGN KEY(предмет) REFERENCES предметы(Наименование)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
FOREIGN KEY(prefix_) REFERENCES prefix(word)
        ON UPDATE CASCADE
        ON DELETE CASCADE
)ENGINE=InnoDB CHARACTER SET=UTF8;

ALTER TABLE limits ADD UNIQUE INDEX (группа,предмет,prefix_);

DELIMITER |
CREATE PROCEDURE update_table (IN name VARCHAR(100) CHARACTER SET utf8,
IN tim VARCHAR(100) CHARACTER SET utf8)
    NOT DETERMINISTIC
    SQL SECURITY INVOKER
    COMMENT 'Обновит таблицу в ссотвецтвии
    с текущими датами'
BEGIN
                SET @i=(SELECT COUNT(*) FROM information_schema.COLUMNS WHERE TABLE_NAME=name)-1;
                SET @len=0;
                SET @sql= CONCAT('select count(Даты) INTO @len from ',
                tim,' where Даты<=NOW();');
                PREPARE getCountrySql FROM @sql;
                EXECUTE getCountrySql;
                DEALLOCATE PREPARE getCountrySql;
        WHILE @i<=@len DO
                SET @temp=0;
                SET @sql= CONCAT('select DATE_FORMAT((select Даты from ',
                tim,' WHERE id=@i),"%Y_%m_%d") INTO @temp;');
                PREPARE getCountrySql FROM @sql;
                EXECUTE getCountrySql;
                DEALLOCATE PREPARE getCountrySql;
                SET @sql= CONCAT('ALTER TABLE ',name,' ADD ',@temp,' int NOT NULL DEFAULT 0;');
                PREPARE getCountrySql FROM @sql;
                EXECUTE getCountrySql;
                DEALLOCATE PREPARE getCountrySql;
                SET @i=@i+1;
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
		SUM INT DEFAULT 0,
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
        num INT,
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
 IN Gr VARCHAR(100) CHARACTER SET utf8, IN NUM_UPDATE INT)
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
        IF(NUM_UPDATE>0) THEN
                SET @sql= CONCAT('update P_',Gr,' set  num=(
                SELECT @number_:= @number_ + 1 FROM
                (SELECT @number_:= 0) as tbl);');
                PREPARE getCountrySql FROM @sql;
                EXECUTE getCountrySql;
                DEALLOCATE PREPARE getCountrySql;
        END IF;

        SET @i=1;
        SET @len=(select count(word) from prefix);
        WHILE @i<=@len DO
                SET @sql= CONCAT('CREATE TABLE ',(select word from
                prefix where id=@i),Name,Gr,'(
				SUM INT NOT NULL DEFAULT 0,
                ФИО VARCHAR(100) NOT NULL UNIQUE,
                FOREIGN KEY(ФИО) REFERENCES ',Gr,'(ФИО)
                ON UPDATE CASCADE
                ON DELETE CASCADE
                )ENGINE=InnoDB CHARACTER SET=UTF8;');

                PREPARE getCountrySql FROM @sql;
                EXECUTE getCountrySql;
                DEALLOCATE PREPARE getCountrySql;

                SET @sql= CONCAT('INSERT INTO ',(select word from
                prefix where id=@i),Name,Gr,'(ФИО) (select ФИО from ',Gr,')');
                PREPARE getCountrySql FROM @sql;
                EXECUTE getCountrySql;
                DEALLOCATE PREPARE getCountrySql;


                SET @i=@i+1;
        END WHILE;
		INSERT INTO  limits(группа,предмет,prefix_) VALUES 
		(Gr,name,'LC_'),
		(Gr,name,'PC_'),
		(Gr,name,'KRC_'),
		(Gr,name,'RGRC_');
END|
DELIMITER ;


DELIMITER |
CREATE PROCEDURE deleteGlobalPredmet(IN Name VARCHAR(100) CHARACTER SET utf8)
        NOT DETERMINISTIC
    SQL SECURITY INVOKER
    COMMENT 'Удалит предмет'
BEGIN
        SET @i=1;
        SET @len=(select max(num) from группы);
        WHILE @i<=@len DO
                SET @sq= CONCAT('CALL deletePredmet("',Name,'","',
                (select Имя from группы where num=@i),'",1);');
                PREPARE getCountrySq FROM @sq;
                EXECUTE getCountrySq;
                DEALLOCATE PREPARE getCountrySq;
                SET @i=@i+1;
        END WHILE;
        SET @sq= CONCAT('DELETE FROM предметы
        where Наименование="',Name,'";');
        PREPARE getCountrySq FROM @sq;
        EXECUTE getCountrySq;
        DEALLOCATE PREPARE getCountrySq;
        update предметы set  num=(
        SELECT @number_:= @number_ + 1 FROM
        (SELECT @number_:= 0) as tbl);
END |
DELIMITER ;

DELIMITER |
CREATE PROCEDURE deletePredmet(IN Name VARCHAR(100) CHARACTER SET utf8,
IN Gr VARCHAR(100) CHARACTER SET utf8,IN NUM_UPDATE INT)
        NOT DETERMINISTIC
    SQL SECURITY INVOKER
    COMMENT 'Удалит предмет группе'

BEGIN
        SET @temp=0;
        SET @sql= CONCAT('select count(предмет) INTO @temp from P_',Gr,';');
        PREPARE getCountryS FROM @sql;
        EXECUTE getCountryS;
        DEALLOCATE PREPARE getCountryS;

        IF(@temp>=1) THEN
        SET @j=1;
        SET @le=(select count(word) from prefix);
        WHILE @j<=@le DO
                SET @sql= CONCAT('DROP TABLE ',(select word from
                prefix where id=@j),Name,Gr,';');
                PREPARE getCountryS FROM @sql;
                EXECUTE getCountryS;
                DEALLOCATE PREPARE getCountryS;
                SET @j=@j+1;
        END WHILE;
        SET @sql= CONCAT('DELETE FROM P_',Gr,
        ' where предмет="',name,'";');
                PREPARE getCountryS FROM @sql;
                EXECUTE getCountryS;
                DEALLOCATE PREPARE getCountryS;
        IF(NUM_UPDATE>0) THEN
                SET @sql= CONCAT('update P_',Gr,' set  num=(
                        SELECT @number_:= @number_ + 1 FROM
                        (SELECT @number_:= 0) as tbl);');
                        PREPARE getCountryS FROM @sql;
                        EXECUTE getCountryS;
                        DEALLOCATE PREPARE getCountryS;
        END IF;
END IF;

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

        SET @i=1;
    SET @len=0;
    SET @sql= CONCAT('select count(предмет) INTO @len from  P_',Name,';');
        PREPARE getCountrySql FROM @sql;
        EXECUTE getCountrySql;
        DEALLOCATE PREPARE getCountrySql;

    WHILE @i<=@len DO
        SET @temp="";
        SET @sql= CONCAT('select предмет INTO @temp from P_',Name,' where num=@i;');
                PREPARE getCountrySql FROM @sql;
                EXECUTE getCountrySql;
                DEALLOCATE PREPARE getCountrySql;

                SET @sql= CONCAT('call deletePredmet("',@temp,'","',Name,'",0);');
                PREPARE getCountrySql FROM @sql;
                EXECUTE getCountrySql;
                DEALLOCATE PREPARE getCountrySql;
                SET @i=@i+1;
        END WHILE;

        SET @sql= CONCAT('DROP TABLE P_',Name,';');
        PREPARE getCountrySql FROM @sql;
        EXECUTE getCountrySql;
        DEALLOCATE PREPARE getCountrySql;

        SET @sql= CONCAT('DROP TABLE ',Name,';');
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
create procedure clear()
        NOT DETERMINISTIC
        SQL SECURITY INVOKER
        COMMENT 'отчистить данные если бд остановлина '
BEGIN
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


