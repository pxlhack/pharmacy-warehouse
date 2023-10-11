BEGIN;
INSERT INTO pharmacies (name, address, phone_number)
VALUES ('MediCare Pharmacy', '1234 Elm Street, Suite 101, Springfield, IL 62701, USA', '+1 (555) 123-4567');
INSERT INTO pharmacies (name, address, phone_number)
VALUES ('HealthFirst Pharmacy', '5678 Oak Avenue, Apt 3B, Brooklyn, NY 11201, USA', '+1 (555) 234-5678');
INSERT INTO pharmacies (name, address, phone_number)
VALUES ('City Drugs', '9876 Maple Road, Unit A, Los Angeles, CA 90001, USA', '+1 (555) 345-6789');
INSERT INTO pharmacies (name, address, phone_number)
VALUES ('Community Health Pharmacy', '5432 Cedar Lane, Suite 205, Dallas, TX 75201, USA', '+1 (555) 456-7890');
INSERT INTO pharmacies (name, address, phone_number)
VALUES ('Vitality Pharmacy', '8765 Birch Street, Suite 302, Miami, FL 33101, USA', '+1 (555) 567-8901');
INSERT INTO pharmacies (name, address, phone_number)
VALUES ('Sunset Healthcare', '4321 Pine Avenue, Apt 4C, Chicago, IL 60601, USA', '+1 (555) 678-9012');
INSERT INTO pharmacies (name, address, phone_number)
VALUES ('Wellness Corner Pharmacy', '123 Main Street, Suite 210, San Francisco, CA 94101, USA', '+1 (555) 789-0123');
INSERT INTO pharmacies (name, address, phone_number)
VALUES ('LifeLine Medical Supplies', '654 Elm Road, Unit 15, Atlanta, GA 30301, USA', '+1 (555) 890-1234');
INSERT INTO pharmacies (name, address, phone_number)
VALUES ('Green Hills Pharmacy', '2345 Oak Drive, Apt 6D, Seattle, WA 98101, USA', '+1 (555) 901-2345');
INSERT INTO pharmacies (name, address, phone_number)
VALUES ('MediLink Health Center', '9876 Cedar Lane, Suite 102, Boston, MA 02101, USA', '+1 (555) 012-3456');

INSERT INTO countries (name)
VALUES ('Russia');
INSERT INTO countries (name)
VALUES ('Finland');
INSERT INTO countries (name)
VALUES ('USA');
INSERT INTO countries (name)
VALUES ('Germany');
INSERT INTO countries (name)
VALUES ('India');
INSERT INTO countries (name)
VALUES ('China');
INSERT INTO countries (name)
VALUES ('France');
INSERT INTO countries (name)
VALUES ('Great Britain');

INSERT INTO manufacturers (name, country_id)
VALUES ('Bayer AG', 4);
INSERT INTO manufacturers (name, country_id)
VALUES ('Johnson & Johnson', 3);
INSERT INTO manufacturers (name, country_id)
VALUES ('R-Pharm', 1);
INSERT INTO manufacturers (name, country_id)
VALUES ('Pharmstandard Group', 1);
INSERT INTO manufacturers (name, country_id)
VALUES ('Orion Corporation', 2);
INSERT INTO manufacturers (name, country_id)
VALUES ('Lupin Limited', 5);
INSERT INTO manufacturers (name, country_id)
VALUES ('Sinopharm Group Co., Ltd.', 6);
INSERT INTO manufacturers (name, country_id)
VALUES ('Servier Laboratories', 7);
INSERT INTO manufacturers (name, country_id)
VALUES ('AstraZeneca plc', 8);
INSERT INTO manufacturers (name, country_id)
VALUES ('GlaxoSmithKline plc', 8);

INSERT INTO medicines (name, manufacturer_id, price)
VALUES ('Aspirin', 2, 150);
INSERT INTO medicines (name, manufacturer_id, price)
VALUES ('Ibuprofen', 3, 200);
INSERT INTO medicines (name, manufacturer_id, price)
VALUES ('Amoxicillin', 4, 250);
INSERT INTO medicines (name, manufacturer_id, price)
VALUES ('Lisinopril', 5, 180);
INSERT INTO medicines (name, manufacturer_id, price)
VALUES ('Metformin', 6, 220);
INSERT INTO medicines (name, manufacturer_id, price)
VALUES ('Atorvastatin', 7, 280);
INSERT INTO medicines (name, manufacturer_id, price)
VALUES ('Omeprazole', 8, 190);
INSERT INTO medicines (name, manufacturer_id, price)
VALUES ('Levothyroxine', 9, 160);
INSERT INTO medicines (name, manufacturer_id, price)
VALUES ('Ciprofloxacin', 10, 270);
INSERT INTO medicines (name, manufacturer_id, price)
VALUES ('Prednisone', 1, 210);

INSERT INTO requests (creation_date, completion_date, pharmacy_id)
SELECT
    '2023-09-01'::DATE + (n - 1) AS creation_date,
        '2023-09-01'::DATE + (n - 1) AS completion_date,
        n AS pharmacy_id
FROM generate_series(1, 10) AS n;



INSERT INTO medicine_buyings (request_id, medicine_id, medicine_number)
SELECT floor(random() * 20) + 1,
       floor(random() * 10) + 1,
       floor(random() * 100) + 1
FROM generate_series(1, 30) ON CONFLICT (request_id, medicine_id) DO NOTHING;

COMMIT;