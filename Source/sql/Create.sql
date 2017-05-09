/*version - 1.7.4*/

create table groups(
    id int NOT NULL AUTO_INCREMENT, 
    name varchar(100) UNIQUE,
    receipt_date date not null,
    year int not NULL default 0,
    expiration_date date ,
    PRIMARY KEY(id)
)ENGINE=InnoDB CHARACTER SET=UTF8;

DELIMITER |

CREATE PROCEDURE update_Groups_year ()
    NOT DETERMINISTIC
    SQL SECURITY INVOKER
    COMMENT 'Обновит поля групп (год обучения)'
BEGIN
	update groups set year=YEAR(CURDATE())-YEAR(receipt_date)+1 where expiration_date IS NULL AND expiration_date>CURDATE();
END|

DELIMITER ;


CREATE EVENT UPDATE_DATE_P
  ON SCHEDULE EVERY 1 WEEK STARTS CURRENT_TIMESTAMP
  ON COMPLETION NOT PRESERVE
  ENABLE
  COMMENT 'обновит даты посещаемости'  DO
	INSERT INTO dates(date_group,point) VALUE(1,CURDATE());

CREATE EVENT UPDATE_DATE_A
  ON SCHEDULE EVERY 1 MONTH STARTS CURRENT_TIMESTAMP
  ON COMPLETION NOT PRESERVE
  ENABLE
  COMMENT 'обновит даты успеваемости'  DO
	INSERT INTO dates(date_group,point) VALUE(2,CURDATE());

create table work_groups(
    id int NOT NULL AUTO_INCREMENT, 
    name varchar(100) UNIQUE,
    about text,
    PRIMARY KEY(id)
)ENGINE=InnoDB CHARACTER SET=UTF8;

create table date_groups(
    id int NOT NULL AUTO_INCREMENT, 
    name varchar(100) UNIQUE,
    about text,
    PRIMARY KEY(id)
)ENGINE=InnoDB CHARACTER SET=UTF8;

INSERT INTO date_groups(name) VALUES
('attendance'),
('performance');

create table subjects(
    id int NOT NULL AUTO_INCREMENT, 
    name varchar(100) UNIQUE,
    about text,
    PRIMARY KEY(id)
)ENGINE=InnoDB CHARACTER SET=UTF8;

create table subjects_groups(
    id int NOT NULL AUTO_INCREMENT, 
    _group int NOT NULL,
    subject int NOT NULL,
    PRIMARY KEY(id),
    FOREIGN KEY(_group) REFERENCES groups(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
    FOREIGN KEY(subject) REFERENCES subjects(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE
)ENGINE=InnoDB CHARACTER SET=UTF8;
ALTER TABLE subjects_groups ADD UNIQUE INDEX (_group,subject);

create table students(
    id int NOT NULL AUTO_INCREMENT, 
    name varchar(100) UNIQUE,
    _group int NOT NULL,
    PRIMARY KEY(id),
    FOREIGN KEY(_group) REFERENCES groups(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE
)ENGINE=InnoDB CHARACTER SET=UTF8;

create table dates(
    id int NOT NULL AUTO_INCREMENT,
    date_group int NOT NULL,
    point date NOT NULL,
    PRIMARY KEY(id),
    FOREIGN KEY(date_group) REFERENCES date_groups(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE
)ENGINE=InnoDB CHARACTER SET=UTF8;
ALTER TABLE dates ADD UNIQUE INDEX (date_group,point);

create table attendance(
    id int NOT NULL AUTO_INCREMENT,
    student int NOT NULL,
    _date int NOT NULL,
    _value int NOT NULL DEFAULT 0,
    PRIMARY KEY(id),
    FOREIGN KEY(_date) REFERENCES dates(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
    FOREIGN KEY(student) REFERENCES students(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE
)ENGINE=InnoDB CHARACTER SET=UTF8;
ALTER TABLE attendance ADD UNIQUE INDEX (student,_date);

create table performance(
    id int NOT NULL AUTO_INCREMENT,
    student int NOT NULL,
    _date int NOT NULL,
    work_group int NOT NULL,
    subject int NOT NULL,
    _value int NOT NULL DEFAULT 0,
    PRIMARY KEY(id),
    FOREIGN KEY(_date) REFERENCES dates(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
    FOREIGN KEY(student) REFERENCES students(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
    FOREIGN KEY(subject) REFERENCES subjects(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
    FOREIGN KEY(work_group) REFERENCES work_groups(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE
)ENGINE=InnoDB CHARACTER SET=UTF8;
ALTER TABLE performance ADD UNIQUE INDEX (student,_date,subject,work_group);

create table limits(
    _value int DEFAULT 0,
    _group int NOT NULL,
    subject int NOT NULL,
    work_group int NOT NULL,
FOREIGN KEY(_group) REFERENCES groups(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
FOREIGN KEY(subject) REFERENCES subjects(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
FOREIGN KEY(work_group) REFERENCES work_groups(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE
)ENGINE=InnoDB CHARACTER SET=UTF8;

ALTER TABLE limits ADD UNIQUE INDEX (_group,subject,work_group);

create table limitsGroup(
    _value int DEFAULT 0,
    _group int NOT NULL UNIQUE,
FOREIGN KEY(_group) REFERENCES groups(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE
)ENGINE=InnoDB CHARACTER SET=UTF8;

