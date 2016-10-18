#include "statsd/statsd.h"

#include <QtTest/QtTest>

class TestStatsD : public QObject {
    Q_OBJECT
private Q_SLOTS:
    void testNoAction();
    void testQueue();
};

void TestStatsD::testNoAction()
{
    StatsDController statsd;
    QVERIFY(statsd.m_fd < 0);

    {
        statsd.time("time", 20);
        statsd.incr("ctr");
        statsd.timeFunction("timeFunc");
    }

    /* check that everything is discarded */
    QCOMPARE(statsd.m_output, std::string());
}

void TestStatsD::testQueue()
{
    StatsDController statsd;
    QVERIFY(statsd.m_fd < 0);

    statsd.connect("localhost", 8128);
    QVERIFY(statsd.m_fd >= 0);

    {
        statsd.time("time", 20);
        statsd.incr("ctr");
        statsd.timeFunction("timeFunc");
    }

    QCOMPARE(statsd.m_output, std::string("time:20|ms\nctr:1|c\ntimeFunc:0|ms"));
    statsd.flush();
    QCOMPARE(statsd.m_output, std::string());
}

QTEST_GUILESS_MAIN(TestStatsD)
#include "statsd_test.moc"
