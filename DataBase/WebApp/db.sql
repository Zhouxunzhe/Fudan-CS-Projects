CREATE TABLE student (
    sid character varying(255) PRIMARY KEY,
    name character varying(255) NOT NULL,
    password character varying(255) NOT NULL,
    is_superuser boolean NOT NULL,
    major character varying(255) NOT NULL,
    phone character varying(255) NOT NULL,
    situation character varying(255) NOT NULL,
    is_active boolean NOT NULL
);

INSERT INTO student
VALUES ('123123123','admin','123',true,'CS','18012341234','negative',true);
INSERT INTO building(sid,bid)
VALUES ('123123123','001');

CREATE TABLE building(
    ubid serial PRIMARY KEY,
    sid character varying(255) NOT NULL,
    bid character varying(255) NOT NULL,
    FOREIGN KEY(sid)REFERENCES student(sid)
);

CREATE TABLE inspection(
    uiid serial PRIMARY KEY,
    sid character varying(255) NOT NULL,
    iid character varying(255) NOT NULL,
    inspect_date character varying(255) NOT NULL,
    is_inspected character varying(255) NOT NULL,
    result character varying(255) NOT NULL,
    FOREIGN KEY(sid)REFERENCES student(sid)
);