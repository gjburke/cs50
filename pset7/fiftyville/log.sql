-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Used .schema to get a general understanding of the database

-- Want to know the report/what happened, maybe I can get some information
SELECT description
FROM crime_scene_reports;

-- Theres too many results to find the one with the duck, so I modify thsi statement to include the word "duck"
SELECT description
FROM crime_scene_reports
WHERE description LIKE "%duck%";

-- Now I know it took place at 10:15 at the Humphrey Street Bakery, three witnesses were inteviewed. I want to look at the interviews, which mention the bakery
SELECT transcript
FROM interviews
WHERE transcript LIKE "%bakery%";

-- One says the theif got into the car sometime within the minutes of the theft. This would make the time table 10:15-10:25. Could look for security footage from then.
    -- Theif withdrawling from the ATM on Legget Street earlier that morning
    -- Someone called the theif for less than a minute as the theif was leaving the bakery
    -- Planning on taking the earliest flight out of fiftyville tomorrow, purchased it today

-- I'm going to look at the securty footage first, with the time constraint on my query
-- First I need the day, month, year of the theft.

SELECT year, month, day
FROM crime_scene_reports
WHERE description LIKE "%duck%";

-- It is 7/28/2021 from 10:15-10:25 is where I will be looking at the security logs. A little before too because maybe he parked there earlier then robbed it, ya know?

SELECT license_plate
FROM bakery_security_logs
WHERE year = 2021
    AND month = 7
    AND day = 28
    AND minute <= 25
    AND minute >= 15
    AND activity = "exit"
ORDER BY minute;
/*
+---------------+
| license_plate |
+---------------+
| D965M59       |
| HW0488P       |
| 13FNH73       |
| 5P2BI95       |
| 94MV71O       |
| V47T75I       |
| 94KL13X       |
| 6P58WS2       |
| NAW9653       |
| R3G7486       |
| 4328GD8       |
| G412CB7       |
| L93JTIZ       |
| 322W7JE       |
| 0NTHK55       |
| HOD8639       |
+---------------+
*/

-- So many cars exited then

-- Going to look at the withdrawls from leggett street
SELECT account_number
FROM atm_transactions
WHERE atm_location = "Leggett Street"
    AND year = 2021
    AND month = 7
    AND day = 28;
/*
+----------------+
| account_number |
+----------------+
| 28500762       |
| 28296815       |
| 76054385       |
| 49610011       |
| 16153065       |
| 86363979       |
| 25506511       |
| 81061156       |
| 26013199       |
+----------------+
*/

-- Now going to look at the phone calls with duration less than a minute on that day, listing the caller and reciever
SELECT caller, receiver
FROM phone_calls
WHERE duration < 60
    AND year = 2021
    AND month = 7
    AND day = 28;

/*
+----------------+----------------+
|     caller     |    receiver    |
+----------------+----------------+
| (130) 555-0289 | (996) 555-8899 |
| (499) 555-9472 | (892) 555-8872 |
| (367) 555-5533 | (375) 555-8161 |
| (499) 555-9472 | (717) 555-1342 |
| (286) 555-6063 | (676) 555-6554 |
| (770) 555-1861 | (725) 555-3243 |
| (031) 555-6622 | (910) 555-3251 |
| (826) 555-1652 | (066) 555-9701 |
| (338) 555-6650 | (704) 555-2131 |
+----------------+----------------+
*/

--Theres just too many to be honest. Maybe I can link them all together to the person and check and see if any one person is connected

SELECT name
FROM people
JOIN phone_calls
ON people.phone_number = phone_calls.caller
JOIN bakery_security_logs
ON people.license_plate = bakery_security_logs.license_plate
JOIN bank_accounts
ON people.id = bank_accounts.person_id
JOIN atm_transactions
ON bank_accounts.account_number = atm_transactions.account_number
WHERE phone_calls.caller IN (
    SELECT caller
    FROM phone_calls
    WHERE duration < 60
    AND year = 2021
    AND month = 7
    AND day = 28
)
AND
bakery_security_logs.license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021
        AND month = 7
        AND day = 28
        AND minute <= 25
        AND minute >= 15
        AND activity = "exit"
    ORDER BY minute
)
AND
atm_transactions.account_number IN (
    SELECT account_number
    FROM atm_transactions
    WHERE atm_location = "Leggett Street"
    AND year = 2021
    AND month = 7
    AND day = 28
);

--So it gave me a bunch of Bruces and Dianas so i'm going to request thier info.

SELECT *
FROM people
WHERE name = "Bruce"
OR name = "Diana";

/*
+--------+-------+----------------+-----------------+---------------+
|   id   | name  |  phone_number  | passport_number | license_plate |
+--------+-------+----------------+-----------------+---------------+
| 514354 | Diana | (770) 555-1861 | 3592750733      | 322W7JE       |
| 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
+--------+-------+----------------+-----------------+---------------+
*/

-- If the theif left the day after, there should be no logs at the bakery of their lisence palte after the 29th

SELECT *
FROM bakery_security_logs
WHERE license_plate = "322W7JE"
OR license_plate = "94KL13X";

-- They both only have it on that day, so I'm going to check the flight logs for the next day
SELECT passport_number
FROM passengers
JOIN flights
ON passengers.flight_id = flights.id
WHERE flights.year = 2021
    AND flights.month = 7
    AND flights.day = 29
ORDER BY passport_number;

-- THEY WERE ON THE SAME FLIGHT GAAAAA
-- Im going to check and see if the person they called had a purchase

SELECT *
FROM phone_calls
WHERE caller = "(770) 555-1861" OR caller = "(367) 555-5533"
AND year = 2021
AND month = 7
AND day = 28;

-- Bruce called (375) 555-8161, Diana called (725) 555-3243

SELECT *
FROM people
WHERE phone_number = "(375) 555-8161" OR phone_number = "(725) 555-3243";

/*
+--------+--------+----------------+-----------------+---------------+
|   id   |  name  |  phone_number  | passport_number | license_plate |
+--------+--------+----------------+-----------------+---------------+
| 847116 | Philip | (725) 555-3243 | 3391710505      | GW362R6       |
| 864400 | Robin  | (375) 555-8161 |                 | 4V16VO0       |
+--------+--------+----------------+-----------------+---------------+
*/

-- Lets look at their bank transactions

SELECT account_number, person_id
FROM bank_accounts
WHERE person_id = 847116
    OR person_id = 864400;

/*
+----------------+-----------+
| account_number | person_id |
+----------------+-----------+
| 47746428       | 847116    |
| 94751264       | 864400    |
+----------------+-----------+
*/

-- Look at their transactions on that day

SELECT *
FROM atm_transactions
WHERE

-- Abandoning htat because the acccomplice already has cash
-- I'm going to look at the "earliest flight out of fiftyville" the next day, which is 7-29-2021

SELECT *
FROM airports;

/*
+----+--------------+-----------------------------------------+---------------+
| id | abbreviation |                full_name                |     city      |
+----+--------------+-----------------------------------------+---------------+
| 1  | ORD          | O'Hare International Airport            | Chicago       |
| 2  | PEK          | Beijing Capital International Airport   | Beijing       |
| 3  | LAX          | Los Angeles International Airport       | Los Angeles   |
| 4  | LGA          | LaGuardia Airport                       | New York City |
| 5  | DFS          | Dallas/Fort Worth International Airport | Dallas        |
| 6  | BOS          | Logan International Airport             | Boston        |
| 7  | DXB          | Dubai International Airport             | Dubai         |
| 8  | CSF          | Fiftyville Regional Airport             | Fiftyville    |
| 9  | HND          | Tokyo International Airport             | Tokyo         |
| 10 | CDG          | Charles de Gaulle Airport               | Paris         |
| 11 | SFO          | San Francisco International Airport     | San Francisco |
| 12 | DEL          | Indira Gandhi International Airport     | Delhi         |
+----+--------------+-----------------------------------------+---------------+
*/
SELECT *
FROM flights
WHERE origin_airport_id = 8
    AND year = 2021
    AND month = 7
    AND day = 29
ORDER BY hour;

/*
+----+-------------------+------------------------+------+-------+-----+------+--------+
| id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
+----+-------------------+------------------------+------+-------+-----+------+--------+
| 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
| 43 | 8                 | 1                      | 2021 | 7     | 29  | 9    | 30     |
| 23 | 8                 | 11                     | 2021 | 7     | 29  | 12   | 15     |
| 53 | 8                 | 9                      | 2021 | 7     | 29  | 15   | 20     |
| 18 | 8                 | 6                      | 2021 | 7     | 29  | 16   | 0      |
+----+-------------------+------------------------+------+-------+-----+------+--------+
*/

-- So the theif would be taking flight 36, let's take a look at all the people on there

SELECT passport_number
FROM passengers
JOIN flights
ON passengers.flight_id = flights.id
WHERE flights.id = 36;

/*
+-----------------+
| passport_number |
+-----------------+
| 7214083635      |
| 1695452385      |
| 5773159633      |
| 1540955065      |
| 8294398571      |
| 1988161715      |
| 9878712108      |
| 8496433585      |
+-----------------+
*/
/*
+--------+-------+----------------+-----------------+---------------+
|   id   | name  |  phone_number  | passport_number | license_plate |
+--------+-------+----------------+-----------------+---------------+
| 514354 | Diana | (770) 555-1861 | 3592750733      | 322W7JE       |
| 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
+--------+-------+----------------+-----------------+---------------+
*/

--It looks like its bruce, which would make his accomplice robin.

--Getting the flight info
SELECT *
FROM flights
WHERE id = 36;

/*
+----+-------------------+------------------------+------+-------+-----+------+--------+
| id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
+----+-------------------+------------------------+------+-------+-----+------+--------+
| 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
+----+-------------------+------------------------+------+-------+-----+------+--------+
*/

--The destination is LaGuardia, so New York City