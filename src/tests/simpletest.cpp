#include <QtTest/QtTest>

class SimpleTest: public QObject
{
	Q_OBJECT
private slots:
	void testOne();
};

void SimpleTest::testOne()
{
	QString str = "Hello";
	QVERIFY(str.toUpper() == "HELLO");
	QCOMPARE(str.toUpper(), QString("HELLO"));
}

QTEST_MAIN(SimpleTest)
#include "simpletest.moc"
