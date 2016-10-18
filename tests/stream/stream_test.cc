#include <QtTest/QtTest>
#include "utils/stream.h"

class TestStream : public QObject {
    Q_OBJECT
private Q_SLOTS:
    void testReadSkip();
    void testReadU8();
    void testReadU16();
    void testReadU32();
    void testWriteReserve();
    void testWriteU8();
    void testWriteU16();
    void testWriteU32();
};

void TestStream::testReadSkip()
{
    uint8_t data[] = { 0xFF };
    Reader rdr(data, sizeof(data));

    /* initial setting */
    QVERIFY(!rdr.error);
    QCOMPARE(rdr.remaining, (size_t) 1);

    rdr.skip(1);
    QVERIFY(!rdr.error);
    QCOMPARE(rdr.remaining, (size_t) 0);

    rdr.skip(1);
    QVERIFY(rdr.error);
    QCOMPARE(rdr.remaining, (size_t) 0);
}


void TestStream::testReadU8()
{
    uint8_t data[] = { 0xFF };
    Reader rdr(data, sizeof(data));

    /* initial setting */
    QVERIFY(!rdr.error);
    QCOMPARE(rdr.remaining, (size_t) 1);

    int val = rdr.get_u8();
    QCOMPARE(val, 0xFF);
    QVERIFY(!rdr.error);
    QCOMPARE(rdr.remaining, (size_t) 0);

    val = rdr.get_u8();
    QCOMPARE(val, 0x0);
    QVERIFY(rdr.error);
    QCOMPARE(rdr.remaining, (size_t) 0);
}

void TestStream::testReadU16()
{
    uint8_t data[] = { 0xFF, 0xFF };
    Reader rdr(data, sizeof(data));

    /* initial setting */
    QVERIFY(!rdr.error);
    QCOMPARE(rdr.remaining, (size_t) 2);

    int val = rdr.get_u16();
    QCOMPARE(val, 0xFFFF);
    QVERIFY(!rdr.error);
    QCOMPARE(rdr.remaining, (size_t) 0);

    val = rdr.get_u16();
    QCOMPARE(val, 0x0);
    QVERIFY(rdr.error);
    QCOMPARE(rdr.remaining, (size_t) 0);

    rdr = Reader(data, sizeof(data));
    rdr.skip(1);
    rdr.get_u16();
    QVERIFY(rdr.error);
    QCOMPARE(rdr.remaining, (size_t) 1);
}

void TestStream::testReadU32()
{
    uint8_t data[] = { 0xFF, 0xFF, 0xFF, 0xFF };
    Reader rdr(data, sizeof(data));

    /* initial setting */
    QVERIFY(!rdr.error);
    QCOMPARE(rdr.remaining, (size_t) 4);

    unsigned val = rdr.get_u32();
    QCOMPARE(val, 0xFFFFFFFFu);
    QVERIFY(!rdr.error);
    QCOMPARE(rdr.remaining, (size_t) 0);

    val = rdr.get_u32();
    QCOMPARE(val, 0x0u);
    QVERIFY(rdr.error);
    QCOMPARE(rdr.remaining, (size_t) 0);

    rdr = Reader(data, sizeof(data));
    rdr.skip(1);
    rdr.get_u32();
    QVERIFY(rdr.error);
    QCOMPARE(rdr.remaining, (size_t) 3);

    rdr = Reader(data, sizeof(data));
    rdr.skip(3);
    rdr.get_u32();
    QVERIFY(rdr.error);
    QCOMPARE(rdr.remaining, (size_t) 1);
}

void TestStream::testWriteReserve()
{
    uint8_t buf[2];
    Writer wrt(buf, sizeof(buf));
    QVERIFY(!wrt.error);
    QCOMPARE(wrt.remaining, (size_t) 2);
    QCOMPARE(wrt.len, 0);

    uint8_t* tmp = wrt.reserve(1);
    QVERIFY(!wrt.error);
    QCOMPARE(wrt.remaining, (size_t) 1);
    QCOMPARE(wrt.len, 1);
    QCOMPARE(tmp, &buf[0]);

    tmp = wrt.reserve(1);
    QVERIFY(!wrt.error);
    QCOMPARE(wrt.remaining, (size_t) 0);
    QCOMPARE(wrt.len, 2);
    QCOMPARE(tmp, &buf[1]);

    tmp = wrt.reserve(1);
    QVERIFY(wrt.error);
    QCOMPARE(tmp, (uint8_t *) nullptr);


    wrt = Writer(buf, sizeof(buf));
    tmp = wrt.reserve(100);
    QVERIFY(wrt.error);
    QCOMPARE(tmp, (uint8_t *) nullptr);
}

void TestStream::testWriteU8()
{
    uint8_t buf[2] = { 0, };
    Writer wrt(buf, sizeof(buf));
    QVERIFY(!wrt.error);
    QCOMPARE(wrt.remaining, (size_t) 2);
    QCOMPARE(wrt.len, 0);

    wrt.put_u8(0xFE);
    QVERIFY(!wrt.error);
    QCOMPARE(wrt.data, &buf[1]);
    QCOMPARE(wrt.len, 1);
    QCOMPARE(wrt.remaining, (size_t) 1);
    QCOMPARE(buf[0], (uint8_t) 0xFE);

    wrt.put_u8(0xFD);
    QVERIFY(!wrt.error);
    QCOMPARE(wrt.data, &buf[2]);
    QCOMPARE(wrt.len, 2);
    QCOMPARE(wrt.remaining, (size_t) 0);
    QCOMPARE(buf[1], (uint8_t) 0xFD);

    wrt.put_u8(0xFF);
    QVERIFY(wrt.error);
}

void TestStream::testWriteU16()
{
    uint8_t buf[4] = { 0, };
    Writer wrt(buf, sizeof(buf));
    QVERIFY(!wrt.error);
    QCOMPARE(wrt.remaining, (size_t) 4);
    QCOMPARE(wrt.len, 0);

    wrt.put_u16(0xFEDD);
    QVERIFY(!wrt.error);
    QCOMPARE(wrt.data, &buf[2]);
    QCOMPARE(wrt.len, 2);
    QCOMPARE(wrt.remaining, (size_t) 2);
    QCOMPARE(buf[0], (uint8_t) 0xDD);
    QCOMPARE(buf[1], (uint8_t) 0xFE);

    wrt.put_u16(0xABEE);
    QVERIFY(!wrt.error);
    QCOMPARE(wrt.data, &buf[4]);
    QCOMPARE(wrt.len, 4);
    QCOMPARE(wrt.remaining, (size_t) 0);
    QCOMPARE(buf[2], (uint8_t) 0xEE);
    QCOMPARE(buf[3], (uint8_t) 0xAB);

    wrt.put_u16(0x0);
    QVERIFY(wrt.error);
}

void TestStream::testWriteU32()
{
    uint8_t buf[8] = { 0, };
    Writer wrt(buf, sizeof(buf));
    QVERIFY(!wrt.error);
    QCOMPARE(wrt.remaining, (size_t) 8);
    QCOMPARE(wrt.len, 0);

    wrt.put_u32(0xFEDDAABB);
    QVERIFY(!wrt.error);
    QCOMPARE(wrt.data, &buf[4]);
    QCOMPARE(wrt.len, 4);
    QCOMPARE(wrt.remaining, (size_t) 4);
    QCOMPARE(buf[0], (uint8_t) 0xBB);
    QCOMPARE(buf[1], (uint8_t) 0xAA);
    QCOMPARE(buf[2], (uint8_t) 0xDD);
    QCOMPARE(buf[3], (uint8_t) 0xFE);

    wrt.put_u32(0xAABBCCDD);
    QVERIFY(!wrt.error);
    QCOMPARE(wrt.data, &buf[8]);
    QCOMPARE(wrt.len, 8);
    QCOMPARE(wrt.remaining, (size_t) 0);
    QCOMPARE(buf[4], (uint8_t) 0xDD);
    QCOMPARE(buf[5], (uint8_t) 0xCC);
    QCOMPARE(buf[6], (uint8_t) 0xBB);
    QCOMPARE(buf[7], (uint8_t) 0xAA);

    wrt.put_u32(0x0);
    QVERIFY(wrt.error);
}

QTEST_GUILESS_MAIN(TestStream)
#include "stream_test.moc"
