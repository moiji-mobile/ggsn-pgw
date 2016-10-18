#include "utils/parsing.h"
#include "utils/logging.h"

#include <QtTest/QtTest>

class TestUtils : public QObject {
    Q_OBJECT
private Q_SLOTS:
    void testParseImsi();
    void testEncodeImsi();
    void testParseMsisdn();
    void testEncodeMsisdn();
    void testMsisdnLength();
    void testHexdump();
};

void TestUtils::testParseImsi()
{
    const uint8_t empty_data[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    const uint8_t imsi_data[] = { 0x03, 0x52, 0x00, 0x02, 0x10, 0x12, 0x02, 0xf1 };

    std::string imsi = parse_imsi(empty_data);
    QVERIFY(imsi.empty());

    imsi = parse_imsi(imsi_data);
    QCOMPARE(imsi, std::string("302500200121201"));
}

void TestUtils::testEncodeImsi()
{
    std::string imsi;
    uint8_t imsi_buf[8];
    const uint8_t empty_data[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    const uint8_t imsi_odd_data[] = { 0x03, 0x52, 0x00, 0x02, 0x10, 0x12, 0x02, 0xf1 };
    const uint8_t imsi_even_data[] = { 0x03, 0x52, 0x00, 0x02, 0x10, 0x12, 0x02, 0x81 };

    imsi = std::string();
    encode_imsi(imsi_buf, imsi);
    QVERIFY(memcmp(empty_data, imsi_buf, 8) == 0);

    imsi = "302500200121201";
    encode_imsi(imsi_buf, imsi);
    QVERIFY(memcmp(imsi_odd_data, imsi_buf, 8) == 0);

    imsi = "3025002001212018";
    encode_imsi(imsi_buf, imsi);
    QVERIFY(memcmp(imsi_even_data, imsi_buf, 8) == 0);
}

void TestUtils::testParseMsisdn()
{
    const uint8_t encoded_msisdn[] = { 0x91, 0x41, 0x83,  0x93, 0x49, 0x70, 0xf3 };

    std::string msisdn = parse_msisdn(encoded_msisdn, sizeof(encoded_msisdn));
    QCOMPARE(msisdn, std::string("14383994073"));

    msisdn = parse_msisdn(encoded_msisdn, sizeof(encoded_msisdn) - 1);
    QCOMPARE(msisdn, std::string("1438399407"));

    msisdn = parse_msisdn(encoded_msisdn, 1);
    QCOMPARE(msisdn, std::string());

    msisdn = parse_msisdn(encoded_msisdn, 0);
    QCOMPARE(msisdn, std::string());
}

void TestUtils::testEncodeMsisdn()
{
    int len;
    const uint8_t encoded_msisdn[] = { 0x91, 0x41, 0x83,  0x93, 0x49, 0x70, 0xf3 };
    uint8_t msisdn_buf[7];

    len = encode_msisdn(msisdn_buf, "14383994073");
    QCOMPARE(len, 7);
    QCOMPARE(len, (int) sizeof(encoded_msisdn));
    QVERIFY(memcmp(msisdn_buf, encoded_msisdn, 7) == 0);

    len = encode_msisdn(msisdn_buf, "1438399407");
    QCOMPARE(len, 6);
    QVERIFY(memcmp(msisdn_buf, encoded_msisdn, 6) == 0);

    len = encode_msisdn(msisdn_buf, "");
    QCOMPARE(len, 1);
    QVERIFY(memcmp(msisdn_buf, encoded_msisdn, 1) == 0);
}

void TestUtils::testMsisdnLength()
{
    QCOMPARE(msisdn_length(""), 0);
    QCOMPARE(msisdn_length("A"), 1);
    QCOMPARE(msisdn_length("AB"), 1);
    QCOMPARE(msisdn_length("ABC"), 2);
    QCOMPARE(msisdn_length("ABCD"), 2);
    QCOMPARE(msisdn_length("ABCDE"), 3);
    QCOMPARE(msisdn_length("ABCDEF"), 3);
}

void TestUtils::testHexdump()
{
    const uint8_t data[] = { 0xCA, 0xFE, 0x08 };
    QCOMPARE(hexstring(data, sizeof(data)), std::string("cafe08"));
    QCOMPARE(hexstring(data, sizeof(data)), std::string("cafe08"));
    QCOMPARE(hexstring(data, sizeof(data)), std::string("cafe08"));
    QCOMPARE(hexstring(data, sizeof(data)), std::string("cafe08"));
}

QTEST_GUILESS_MAIN(TestUtils)
#include "utils_test.moc"
