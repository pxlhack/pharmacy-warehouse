-- 38) БД «Аптечный склад»
-- В БД должна храниться информация:
-- Лекарства (код лекарства, название лекарства, производитель, цена (руб.));
-- Аптеки (номер аптеки, название аптеки, адрес аптеки, номер телефона);
-- Заявки (номер заявки, дата составления заявки, номер аптеки, дата выполнения заявки);
-- Закупки лекарственных препаратов (номер заявки, код лекарства, количество (шт.)).
--
CREATE USER pharmacy_administrator WITH PASSWORD '1234';

CREATE DATABASE pharmacy_warehouse WITH OWNER pharmacy_administrator;
GRANT CREATE ON DATABASE pharmacy_warehouse TO pharmacy_administrator;
-- psql -U pharmacy_administrator -d pharmacy_warehouse -h 127.0.0.1 -W;

CREATE TABLE IF NOT EXISTS countries
(
    id   SERIAL PRIMARY KEY,
    name VARCHAR
);

CREATE TABLE IF NOT EXISTS manufacturers
(
    id         SERIAL PRIMARY KEY,
    name       VARCHAR,
    country_id INTEGER,

    FOREIGN KEY (country_id) REFERENCES countries (id)
);


CREATE TABLE IF NOT EXISTS medicines
(
    id              SERIAL PRIMARY KEY,
    name            VARCHAR,
    manufacturer_id INTEGER,
    price           INTEGER,

    FOREIGN KEY (manufacturer_id) REFERENCES manufacturers (id)
);

CREATE TABLE IF NOT EXISTS pharmacies
(
    id           SERIAL PRIMARY KEY,
    name         VARCHAR,
    address      VARCHAR,
    phone_number VARCHAR
);

CREATE TABLE IF NOT EXISTS requests
(
    id              SERIAL PRIMARY KEY,
    creation_date   DATE,
    completion_date DATE,
    pharmacy_id     INTEGER,

    FOREIGN KEY (pharmacy_id) REFERENCES pharmacies (id)
);

CREATE TABLE IF NOT EXISTS medicine_buyings
(
    request_id      INTEGER,
    medicine_id     INTEGER,
    medicine_number INTEGER,

    PRIMARY KEY (request_id, medicine_id),
    FOREIGN KEY (request_id) REFERENCES requests (id),
    FOREIGN KEY (medicine_id) REFERENCES medicines (id)
);

