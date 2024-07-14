drop table if exists order_goods;
drop table if exists orders;
drop table if exists shoppingcarts;
drop table if exists bills;
drop table if exists accounts;
drop table if exists good_requests;
drop table if exists temp_goods;
drop table if exists goods;
drop table if exists shop_requests;
drop table if exists shops;
drop table if exists superusers;
drop table if exists users;
drop table if exists images;

create table users (
    id_num      char(18) primary key not null,
    username    varchar(10) unique not null,
    phonenumber char(11) unique not null,
    email       varchar(256) unique not null,
    password    varchar(256) not null,
    is_shop     boolean not null
);

create table superusers (
    username     varchar(10) primary key not null,
    password     varchar(256) not null
);

create table shops (
    shop_id          integer primary key not null auto_increment,
    kind             nvarchar(256) not null,
    shopname         nvarchar(12) unique not null,
    id_num           char(18) not null,
    intro            nvarchar(128),
    address          nvarchar(32) not null,
    logo             text,
    register_capital decimal(10, 2) not null,
    register_date    date not null,
    is_open          boolean not null default false,
    foreign key (id_num) references users(id_num)
);

create table shop_requests (
    request_id   integer primary key not null auto_increment,
    request_type enum("open", "close", "modify") not null,
    shop_id      integer not null,
    is_check     boolean not null default false,
    comment      enum("approve", "refuse"),
    request_date date not null,
    info         nvarchar(256),
    foreign key (shop_id) references shops(shop_id) 
);

create table goods (
    good_id    integer primary key not null auto_increment,
    shop_id    integer not null,
    goodname   nvarchar(256) not null,
    price      decimal(10, 2) not null,
    goodamount integer not null default 0,
    intro      nvarchar(128) not null,
    images     text,
    is_legal   boolean not null default false,
    foreign key (shop_id) references shops(shop_id)
);

create table temp_goods (
    temp_good_id integer primary key not null auto_increment,
    good_id    integer not null,
    shop_id    integer not null,
    goodname   nvarchar(256) not null,
    price      decimal(10, 2) not null,
    goodamount integer not null default 0,
    intro      nvarchar(128) not null,
    images     text,
    is_legal   boolean not null default false,
    is_updated boolean not null default false,
    foreign key (shop_id) references shops(shop_id),
    foreign key (good_id) references goods(good_id)
);

create table good_requests (
    request_id   integer primary key not null auto_increment,
    request_type enum("open", "close", "modify") not null,
    good_id      integer not null,
    is_check     boolean not null default false,
    comment      enum("approve", "refuse"),
    request_date date not null,
    info         nvarchar(256),
    foreign key (good_id) references goods(good_id) 
);

create table accounts (
    account_id   integer primary key not null auto_increment,
    id_num       char(18),
    shop_id      integer unique,
    account_type enum("user", "shop", "middle", "mall") not null,
    amount       decimal(10, 2) not null default 0,
    foreign key (id_num) references users(id_num),
    foreign key (shop_id) references shops(shop_id)
);

create table bills (
    bill_id     integer primary key not null auto_increment,
    amount      decimal(10, 2) not null,
    sender_id   integer not null,
    receiver_id integer not null,
    bill_date   date not null,
    bill_type   enum("buy", "open", "recharge", "open_success", "open_fail") not null,
    foreign key (sender_id) references accounts(account_id),
    foreign key (receiver_id) references accounts(account_id)
);

create table shoppingcarts (
    id_num  char(18) not null,
    good_id integer not null,
    count   integer not null,
    primary key (id_num, good_id),
    foreign key (id_num) references users(id_num),
    foreign key (good_id) references goods(good_id)
);

create table images (
    image_id integer primary key not null auto_increment
);

create table orders (
    order_id integer primary key not null auto_increment,
    id_num  char(18) not null,
    total decimal(10, 2) not null,
    order_date date not null,
    note text,
    order_type enum("ready", "already", "cancel") not null,
    foreign key (id_num) references users(id_num)
);

create table order_goods (
    good_id integer not null,
    order_id integer not null,
    count integer not null,
    primary key (good_id, order_id),
    foreign key (good_id) references goods(good_id),
    foreign key (order_id) references orders(order_id)
);
